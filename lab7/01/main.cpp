/*
 * Metody Programowania - Laboratorium 7 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 1 | 04.2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

typedef struct _przedmiot {
	char nazwa[30];		// koniecznie do poprawy
	int cena;
	int masa;
} przedmiot;

int czytaj(FILE *fin, List::List<przedmiot> *L)
{
	int poj;
	fscanf(fin, "%d", &poj);
	while(true)
	{
		przedmiot p;
		fscanf(fin, "%s", p.nazwa);
		fscanf(fin, "%d", &p.cena);
		fscanf(fin, "%d", &p.masa);
		if(feof(fin)) break;	// dodaje przedmiot do listy tylko jesli udalo sie go odczytac w calosci
		*L += new List::Element<przedmiot>(p);
	}
	return poj;
}

void wypisz(FILE *fout, int **tab, int x, int y)
{
	for(int i=0; i <= x; i++)
	{
		for(int j=0; j <= y; j++)
			fprintf(fout, "%d\t", tab[i][j]);
		fprintf(fout, "\n");
	}
}

void zapakuj(FILE *fout, int p, List::List<przedmiot> *L)
{
	int **P = (int**)malloc((L->getLen()+1)*sizeof(int*));		// alokacja i inicjalizacja P
	for(int i=0; i <= L->getLen(); i++)
	{
		P[i] = (int*)malloc((p+1)*sizeof(int));
		memset(P[i], 0, (p+1)*sizeof(int));
	}

	int **Q = (int**)malloc((L->getLen()+1)*sizeof(int*));		// alokacja i inicjalizacja Q
	for(int i=0; i <= L->getLen(); i++)
	{
		Q[i] = (int*)malloc((p+1)*sizeof(int));
		memset(Q[i], 0, (p+1)*sizeof(int));
	}

	L->reset();
	for(int i=1; L->current(); i++)
	{
		for(int j=1; j <= p; j++)
		{
			przedmiot *a = L->current()->getData(); 
			if((j >= a->masa) && (P[i-1][j] < P[i][j-a->masa] + a->cena))
			{
				P[i-1][j] = P[i][j-a->masa] + a->cena;	// dodanie aktualnego przedmiotu
				Q[i][j] = i;
			}
			else
			{
				P[i][j] = P[i-1][j];	// przepisanie wartosci poprzednich
				Q[i][j] = Q[i-1][j];
			}
		}
		L->next();
	}

	wypisz(fout, P, L->getLen(), p);
	fprintf(fout, "\n\n");
	wypisz(fout, Q, L->getLen(), p);

	for(int i=0; i < L->getLen(); i++)
		free(P[i]);
	free(P);		// zwolnienie P

	for(int i=0; i < L->getLen(); i++)
		free(Q[i]);
	free(Q);		// zwolnienie Q
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

	List::List<przedmiot> *L = new List::List<przedmiot>;
	int pojemnosc = czytaj(in, L);		// wczytywanie rzeczy
	zapakuj(out, pojemnosc, L);		// pakowanie plecaka

	delete L;
	fclose(in);		// zamkniecie plikow
	fclose(out);
	return 0;
}