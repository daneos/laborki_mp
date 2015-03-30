/*
 * Metody Programowania - Laboratorium 4 - Program 02
 * Grzegorz Kowalski, 12i
 * wersja 1 | 03.2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "quicksort.h"

typedef struct _statystyka {
	int n;			// ilosc elementow
	unsigned long por;		// ilosc porownan
	double czas;	// czas obliczen
} statystyka;

statystyka quicksort(long *T, int n)
{
	statystyka S;
	S.n = n;
	time_t start = time(NULL);

	S.por = _quicksort(T, 0, n-1);		// rzeczywiste sortowanie

	time_t stop = time(NULL);
	S.czas = difftime(stop, start);	// przenosnosc, przenosnosc i jeszcze raz przenosnosc
	return S;
}

void wypisz(long *T, int n, FILE *fout)
{
	for(int i=0; i < n; i++)
		fprintf(fout, "%d ", T[i]);
	fprintf(fout, "\n");
}

void wypisz_stat(statystyka *S)
{
	printf("%16d | %16ld | %16.f |\n", S->n, S->por, S->czas);
}

void blad_danych(void)
{
	printf("Blad danych wejsciowych.\n");
	exit(1);
}

void sortuj(FILE *fin, FILE *fout)
{
	int z;
	fscanf(fin, "%d", &z);			// ilosc zestawow
	if(z < 1) blad_danych();

	for(int iz=0; iz < z; iz++)
	{
		int n;
		fscanf(fin, "%d", &n);		// ilosc elementow
		if(n < 1 || n > 5000000) blad_danych();

		long *T = (long*)malloc(n*sizeof(long));	// rezerwacja pamieci
		for(int ie=0; ie < n; ie++)
			fscanf(fin, "%ld", &T[ie]);			// element

		statystyka st = quicksort(T, n);		// sortowanie
		wypisz(T, n, fout);		// wyjscie
		wypisz_stat(&st);		// wyjscie danych statystycznych

		free(T);	// zwolnienie pamieci
	}
}

int main(int argc, char *argv[])
{
	if(argc < 2)	// nie podano nazwy pliku wejsciowego
	{
		printf("Uzycie: %s <nazwa_pliku_wej> [nazwa_pliku_wyj]\n", argv[0]);
		exit(1);
	}

	FILE *in = NULL, *out = NULL;
	if((in = fopen(argv[1], "r")) == NULL)	// otwarcie tylko do odczytu
	{
		perror("main()/fopen():in");
		exit(1);
	}

	if(argc < 3) out = stdout;	// jesli nie podano nazwy pliku wyjsciowego, wypisz na ekran
	else if((out = fopen(argv[2], "w")) == NULL)	// otwarcie tylko do zapisu
	{
		perror("main()/fopen():out");
		exit(1);
	}

	sortuj(in, out);	// odczyt pliku, obliczenia i wyjscie

	fclose(in);		// zamkniecie plikow
	fclose(out);
	return 0;
}