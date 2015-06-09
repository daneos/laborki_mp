/*
 * Metody Programowania - Laboratorium 11 - Program 02
 * Grzegorz Kowalski, 12i
 * wersja 1 | 06.2015
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "generator.h"

#define proper_div(x, y)	((long double)x / (long double)y)		// dzielenie z rzutowaniem

typedef struct _limit {
	long double min;
	long double max;
} limit;

typedef struct _wielomian {
	int n;		// stopien wielomianu
	int *a;		// wspolczynniki
	int prob;	// ilosc probek
	limit x;	// przedzial calkowania
	limit y;	// przedzial wartosci
} wielomian;

wielomian *wejscie(int argc, char *argv[])
{
	static wielomian w;
	w.n = argc-4;		// ilosc wspolczynnikow - stopien wielomianu
	w.a = (int*)malloc(w.n*sizeof(int));

	for(int i=w.n; i >= 1; i--)			// kolejne wspolczynniki
		w.a[w.n-i] = atoi(argv[i]);		// nie sprawdzam zakresu wspolczynnikow - sztuczne ograniczenie

	w.x.min = atof(argv[argc-3]);			// poczatek i koniec przedzialu
	w.x.max = atof(argv[argc-2]);			// j.w.
	if(w.x.max <= w.x.min) die("Poczatek przedzialu musi byc mniejszy od konca.");
	
	w.prob = atoi(argv[argc-1]);		// ilosc punktow
	if(w.prob <= 0) die("Ilosc probek musi byc wieksza od zera.");

	return &w;
}

long double wartosc_f(long double x, wielomian *W)
{
	long double wartosc = 0;
	for(int i=0; i < W->n; i++)
		wartosc += (long double)W->a[i] * pow(x, i);		// wyliczanie wartowsci wielomianu w punkcie x
	return wartosc;
}

inline bool pod_f(long double x, long double y, wielomian *W)
{
	return (y <= wartosc_f(x, W));		// sprawdzenie czy punkt znajduje sie pod wykresem funkcji
}

void ylimit(wielomian *W)
{
	W->y.max = INT_MIN;
	W->y.min = INT_MAX;

	for(int i=0; i < W->prob; i++)	// sprawdzam liniowo z dokladnoscia do ilosci punktow
	{
		long double wi = wartosc_f(W->x.min + proper_div(i, W->prob)*(W->x.max - W->x.min), W);
		if(wi < W->y.min) W->y.min = wi;
		if(wi > W->y.max) W->y.max = wi;
	}

	printf("ylimit.max = %Lf\nylimit.min = %Lf\n", W->y.max, W->y.min);
}

long double monte_carlo(wielomian *W)
{
	ylimit(W);		// wyznaczenie zakresu osi y

	opts genopts = { 
		W->prob,			// ilosc liczb
		W->x.min * W->prob,	// poczatek przedzialu
		W->x.max * W->prob,	// koniec przedzialu
		time(NULL),			// seed
		NULL,				// generator
		false,				// verbose
		NULL				// plik
	};

	long long *x = generator_ALFG(&genopts, 0);		// generacja wspolrzednych x
	genopts.from = W->y.min * W->prob;
	genopts.to = W->y.max * W->prob;
	genopts.seed = genopts.seed * x[0] + x[1];		// nowy seed
	long long *y = generator_ALFG(&genopts, 0);		// generacja wspolrzednych y

	int pod = 0;
	for(int i=0; i < W->prob; i++)
		if(pod_f(proper_div(x[i], W->prob), proper_div(y[i], W->prob), W)) pod++;
	free(x);
	free(y);

	long double P = (W->y.max - W->y.min) * (W->x.max - W->x.min);		// pole prostokata 
	return P * proper_div(pod, W->prob);
}

int main(int argc, char *argv[])
{
	if(argc < 5)
	{
		fprintf(stderr, "Uzycie: %s <wspolczynniki wielomianu> <poczatek> <koniec> <ilosc_punktow>\n", argv[0]);
		exit(-1);
	}

	wielomian *W = wejscie(argc, argv);		// wczytywanie argumentow

	printf("n = %d\nf(x) = ", W->n);
	for(int i=0; i < W->n; i++)
		printf("%d*x^%d + ", W->a[i], i);
	printf("\nprob = %d\nxlimit.max = %Lf\nxlimit.min = %Lf\n", W->prob, W->x.max, W->x.min);

	printf("\n%Lf\n", monte_carlo(W));			// obliczenia i wyjscie
	free(W->a);
	return 0;
}