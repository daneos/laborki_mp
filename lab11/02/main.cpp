/*
 * Metody Programowania - Laboratorium 11 - Program 02
 * Grzegorz Kowalski, 12i
 * wersja 1 | 06.2015
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "generator.h"

#define proper_div(x, y)	((double)x / (double)y)		// dzielenie z rzutowaniem

typedef struct _wielomian {
	int n;		// stopien wielomianu
	int *a;		// wspolczynniki
	int p;		// poczatek przedzialu
	int k;		// koniec przedzialu
	int prob;	// ilosc probek
} wielomian;

wielomian *wejscie(int argc, char *argv[])
{
	static wielomian w;
	w.n = argc-4;		// ilosc wspolczynnikow - stopien wielomianu
	w.a = (int*)malloc(w.n*sizeof(int));

	for(int i=w.n; i >= 1; i--)			// kolejne wspolczynniki
		w.a[w.n-i] = atoi(argv[i]);		// nie sprawdzam zakresu wspolczynnikow - sztuczne ograniczenie

	w.p = atoi(argv[argc-3]);			// poczatek i koniec przedzialu
	w.k = atoi(argv[argc-2]);			// j.w.
	if(w.k <= w.p) die("Poczatek przedzialu musi byc mniejszy od konca.");
	
	w.prob = atoi(argv[argc-1]);		// ilosc punktow
	if(w.prob <= 0) die("Ilosc probek musi byc wieksza od zera.");

	return &w;
}

double wartosc_f(double x, wielomian *W)
{
	double wartosc = 0;
	for(int i=0; i < W->n; i++)
		wartosc += (double)W->a[i] * pow(x, i);		// wyliczanie wartowsci wielomianu w punkcie x
	return wartosc;
}

inline bool pod_f(double x, double y, wielomian *W)
{
	return (y <= wartosc_f(x, W));		// sprawdzenie czy punkt znajduje sie pod wykresem funkcji
}

double monte_carlo(wielomian *W)
{
	int ylimit_top = 162;		// TODO: Limity wartosci y
	int ylimit_bottom = 6;		// TODO: Limity wartosci y

	opts genopts = { 
		W->prob,		// ilosc liczb
		W->p*W->prob,	// poczatek przedzialu
		W->k*W->prob,	// koniec przedzialu
		time(NULL),		// seed
		NULL,			// generator
		false,			// verbose
		NULL			// plik
	};

	long long *x = generator_ALFG(&genopts, 0);		// generacja wspolrzednych x
	genopts.from = ylimit_bottom * W->prob;
	genopts.to = ylimit_top * W->prob;
	long long *y = generator_ALFG(&genopts, 0);		// generacja wspolrzednych y

	int pod = 0;
	for(int i=0; i < W->prob; i++)
		if(pod_f(proper_div(x[i], W->prob), proper_div(y[i], W->prob), W)) pod++;

	int P = (ylimit_top - ylimit_bottom) * (W->k - W->p);		// pole prostokata 
	return (double)P * proper_div(pod, W->prob);
}

int main(int argc, char *argv[])
{
	if(argc < 5)
	{
		fprintf(stderr, "Uzycie: %s <wspolczynniki wielomianu> <poczatek> <koniec> <ilosc_punktow>\n", argv[0]);
		exit(-1);
	}

	wielomian *W = wejscie(argc, argv);		// wczytywanie argumentow
	printf("%f\n", monte_carlo(W));			// obliczenia i wyjscie
	return 0;
}