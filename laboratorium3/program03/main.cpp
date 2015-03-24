/*
 * Metody Programowania - Laboratorium 3 - Program 03
 * Grzegorz Kowalski, 12i
 * wersja 1 | 03.2015
 */

#include <stdio.h>
#include <stdlib.h>	// malloc,free
#include <string.h> // memset

void wypisz(int *s, int *r, int n, FILE *f)
{
	for(int i=0, suma=0; s[i] != 0; i++)		// malo to wszystko eleganckie, ale algorytm jest zgodny z pseudokodem
		for(int iw=0; iw < r[i] && suma < n; iw++, suma += s[i])	// algorytm zostawia "latajace" jedynki na koncu tablicy
			fprintf(f, "%s%d", i==0&&iw==0?"":"+", s[i]);			// z tego powodu wypisywanie musi sie zakonczyc po osiagnieciu zadanej sumy
	fprintf(f, "\n");	// zakoncz wiersz
}

void oblicz(int n, FILE *fout)
{
	int *S = (int*)malloc(n*sizeof(int));
	int *R = (int*)malloc(n*sizeof(int));		// rezerwacja pamieci
	memset(S, 0, n*sizeof(int));
	memset(R, 0, n*sizeof(int));				// zerowanie pamieci

	//-------------------------------------------------------------------------
	S[0]=n;		// indeksy
	R[0]=1;		// j.w.
	int d=0;	// j.w.
	wypisz(S, R, n, fout);
	while(S[0] > 1)	// j.w.
	{
		int sum=0;
		if(S[d] == 1) sum = sum + R[d--]; // zamiast d:=d-1;
		sum += S[d]; // zamiast sum:=sum+S[d];
		R[d] -= 1;	// zamiast R[d]:=R[d]-1;
		int l = S[d] - 1;
		if(R[d] > 0) d++;	// zamiast d:=d-1;
		S[d] = l;
		R[d] = sum / l;
		l = sum % l;
		if(l != 0)
		{
			d++;	// zamiast d:=d+1;
			S[d] = l;
			R[d] = 1;
		}
		wypisz(S, R, n, fout);
	}
	//-------------------------------------------------------------------------

	free(S);
	free(R);	// zwolnienie pamieci
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