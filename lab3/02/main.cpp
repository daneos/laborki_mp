/*
 * Metody Programowania - Laboratorium 3 - Program 02
 * Grzegorz Kowalski, 12i
 * wersja 1 | 03.2015
 */

#include <stdio.h>
#include <stdlib.h>	// malloc,free

void wypisz(int *t, int n, FILE *f)
{
	for(int i=0; i < n; i++)
		fprintf(f, "%d", t[i]);
	fprintf(f, "\n");	// zakoncz wiersz
}

void oblicz(int n, int k, FILE *fout)
{
	int *A = (int*)malloc((k+1)*sizeof(int));	// rezerwacja pamieci

	//-------------------------------------------------------------------------
	for(int i=0; i <= k; i++) A[i] = i+1;
	int p=--k;		// wyrownanie indeksow (od 0)
	while(p >= 0)	// warunek zmieniony ze wzgledu na indeksy
	{
		wypisz(A, k+1, fout);	// liczba elementow dla wypisywania
		if(A[k] == n) p--;	// zamiast p:=p-1;
		else p=k;
		if(p >= 0)	// warunek zmieniony ze wzgledu na indeksy
			for(int i=k; i >= p; i--) A[i] = A[p]+i-p+1;
	}
	//-------------------------------------------------------------------------

	free(A);	// zwolnienie pamieci
}

int main(int argc, char *argv[])
{
	if(argc < 3)	// nie podano n lub k
	{
		printf("Uzycie: %s <n> <k> [nazwa_pliku_wyj]\n", argv[0]);
		exit(1);
	}

	int n = atoi(argv[1]);
	int k = atoi(argv[2]);
	if(k >= n)
	{
		printf("Blad danych wejsciowych: k musi byc mniejsze niz n\n");
		exit(0);
	}

	FILE *out;
	if(argc < 4) out = stdout;	// jesli nie podano nazwy pliku wyjsciowego, wypisz na ekran
	else if((out = fopen(argv[3], "w")) == NULL)	// otwarcie tylko do zapisu
	{
		perror("main()/fopen():out");
		exit(1);
	}

	oblicz(n, k, out);		// obliczenia i wyjscie

	fclose(out);	// zamkniecie pliku
	return 0;
}