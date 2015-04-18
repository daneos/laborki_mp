/*
 * Metody Programowania - Laboratorium 6 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 1 | 04.2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

typedef struct _krawedz {
	int v;
	int w;
} krawedz;

void blad_danych(void)
{
	printf("Blad danych wejsciowych.\n");
	exit(1);
}

int czytaj_graf(FILE *fin, List::List<krawedz> *L)
{
	int n;
	fscanf(fin, "%d", &n);
	if(n < 1) blad_danych();

	int max=0;
	for(int i=0; i < n; i++)
	{
		krawedz e;
		fscanf(fin, "%d", &e.v);
		fscanf(fin, "%d", &e.w);
		if(e.v > max) max = e.v;
		if(e.w > max) max = e.w;
		*L += new List::Element<krawedz>(e);
	}
	return max;
}

void macierz(List::List<krawedz> *G, int iw, FILE *fout)
{
	int **M = (int**)malloc(iw*sizeof(int*));
	for(int i=0; i<iw; i++)						// alokacja macierzy
	{	
		M[i] = (int*)malloc(iw*sizeof(int));
		memset(M[i], 0, iw*sizeof(int));	// zerowanie
	}

	for(G->reset(); G->current(); G->next())
	{
		M[G->current()->getData()->v-1][G->current()->getData()->w-1]++;	// tworzenie macierzy (nad przekatna)
		M[G->current()->getData()->w-1][G->current()->getData()->v-1]++;	// tworzenie macierzy (pod przekatna)
	}

	fprintf(fout, "\t\t");
	for(int i=0; i<iw; i++) fprintf(fout, "%d\t", i+1);	// wypisanie nagłówka macierzy
	fprintf(fout, "\n\n");
	for(int i=0; i<iw; i++)
	{
		fprintf(fout, "%d\t\t", i+1);	// pierwsza kolumna macierzy
		for(int j=0; j<iw; j++)
			fprintf(fout, "%d\t", M[i][j]);	// elementy macierzy
		fprintf(fout, "\n");
		free(M[i]);		// zwolnienie kolumny macierzy
	}
	free(M);	// zwolnienie macierzy
}

void lista(List::List<krawedz> *G, int iw, FILE *fout)
{
	List::List<int> *L = (List::List<int>*)malloc(iw*sizeof(List::List<int>));

	for(G->reset(); G->current(); G->next())
	{
		L[G->current()->getData()->v-1] += new List::Element<int>(G->current()->getData()->w); // dodanie do listy v wierzcholka w
		L[G->current()->getData()->w-1] += new List::Element<int>(G->current()->getData()->v); // dodanie do listy w wierzcholka v
	}

	for(int i=0; i<iw; i++)
	{
		printf("%d:\t", i+1);
		for(L[i].reset(); L[i].current(); L[i].next())
			printf("%d%c", *L[i].current()->getData(), L[i].current()->next()?',':' ');
		printf("\n");
	}
	free(L);	// zwolnienie tablicy list
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

	List::List<krawedz> L;
	int iw = czytaj_graf(in, &L);	// odczyt pliku
	printf("Macierz sasiedztwa:\n");
	macierz(&L, iw, out);	// stworz i wypisz macierz sasiedztwa
	printf("\nLista sasiedztwa:\n");
	lista(&L, iw, out);		// stworz i wypisz liste sasiedztwa

	fclose(in);		// zamkniecie plikow
	fclose(out);
	return 0;
}