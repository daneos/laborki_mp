/*
 * Metody Programowania - Laboratorium 3 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 1 | 03.2015
 */

#include <stdio.h>
#include <stdlib.h>	// malloc,free
#include <string.h> // memset

void wypisz(int *t, int n, FILE *f)
{
	for(int i=0; i < n; i++)
		fprintf(f, "%d", t[i]);
	fprintf(f, "\n");	// zakoncz wiersz
}

void oblicz(int n, FILE *fout)
{
	int *B = (int*)malloc(n*sizeof(int));	// rezerwacja pamieci

	//-------------------------------------------------------------------------
	memset(B, 0, n*sizeof(int));	// zerowanie, zamiast: for i:=1 to n do B[i]:=0;
	int p, i = 0;
	do
	{
		wypisz(B, n, fout);
		p=0;	// pozycja startowa na 0 zamiast 1, z racji ze numeracja indeksow jest od 0
		int j=++i;	// zamiast i:=i+1; j:=i;
		while(j%2 == 0)
		{
			j /= 2;		// zamiast j:=j/2;
			p++;	// zamiast p:=p+1;
		}
		if(p < n) B[p] = 1-B[p]; // zmiana warunku z <= na < : indeksy od 0
	}
	while(p < n);	// odwrocony warunek przy przejsciu z repeat-until na do-while
	//-------------------------------------------------------------------------

	free(B);	// zwolnienie pamieci
}

int main(int argc, char *argv[])
{
	if(argc < 2)	// nie podano n
	{
		printf("Uzycie: %s <n> [nazwa_pliku_wyj]\n", argv[0]);
		exit(1);
	}

	int n = atoi(argv[1]);

	FILE *out;
	if(argc < 3) out = stdout;	// jesli nie podano nazwy pliku wyjsciowego, wypisz na ekran
	else if((out = fopen(argv[2], "w")) == NULL)	// otwarcie tylko do zapisu
	{
		perror("main()/fopen():out");
		exit(1);
	}

	oblicz(n, out);		// obliczenia i wyjscie

	fclose(out);	// zamkniecie pliku
	return 0;
}