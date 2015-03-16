/*
 * Metody Programowania - Laboratorium 2 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 1 | 03.2015
 */

#include <stdio.h>
#include <stdlib.h>	// malloc,free
#include <string.h> // memset

typedef struct _zestaw {
	int n;			// ilosc elementow
	long *dane;		// tablica
} zestaw;

int ile(long sz, long *t)
{
	return 7;	// test
}

void blad_danych(void)
{
	printf("Blad danych wejsciowych.\n");
	exit(1);
}

void *czytaj_i_licz(FILE *fin, FILE *fout)
{
	zestaw *Z = (zestaw*)malloc(sizeof(zestaw));	// rezerwacja pamieci na zestaw
	
	int z = 0;
	fscanf(fin, "%d", &z);		// ilosc zestawow
	if(z < 0) blad_danych();

	for(int iz=0; iz < z; iz++)
	{
		fscanf(fin, "%d", &(Z->n));	// ilosc elementow
		if(Z->n < 1 || Z->n > 1000000) blad_danych();

		Z->dane = (long*)malloc(Z->n*sizeof(long));	// alokacja tablicy
		memset(Z->dane, 0, Z->n*sizeof(long));		// zerowanie tablicy

		for(int ie=0; ie < Z->n; ie++) fscanf(fin, "%ld", &(Z->dane[ie]));	// element

		int k = 0;
		fscanf(fin, "%d", &k);	// ilosc szukanych
		if(k < 1 || k > Z->n) blad_danych();

		for(int is=0; is < k; is++)
		{
			long szukana = 0;
			fscanf(fin, "%ld", &szukana);
			fprintf(fout, "%ld %d\n", szukana, ile(szukana, Z->dane));	// wyjscie i obliczenia
		}
	}
	free(Z->dane);
	free(Z);	// zwolnienie pamieci
}


int main(int argc, char *argv[])
{
	if(argc < 2)
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

	czytaj_i_licz(in, out);	// odczyt pliku, obliczenia i wyjscie

	fclose(in);		// zamkniecie plikow
	fclose(out);
	return 0;
}