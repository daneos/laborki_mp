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
	char *nazwa;
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
		fscanf(fin, "%ms", &p.nazwa);	// POSIX.1-2008 dynamiczna alokacja (prawdopodobnie nie zadziala pod Windowsem)
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

void wypisz_plecak(FILE *fout, int **P, int **Q, List::List<przedmiot> *L, int poj)
{
	int i = L->getLen();
	int j = poj;
	int wyk = 0;	// wykorzystane miejsce
	fprintf(fout, "przedmiot\tcena\tmasa\n----------------------------\n");
	while(j > 0)
	{
		przedmiot *p = (*L)[Q[i][j]]->getData();
		fprintf(fout, "%-10s\t%d\t%d\n", p->nazwa, p->cena, p->masa);
		j -= p->masa;
		wyk += p->masa;
	}
	fprintf(fout, "\nwartosc: %d, wykorzystane miejsce: %d / %d\n", P[i][poj], wyk, poj); 
}

int **tablica_init(int x, int y)
{
	int **tab = (int**)malloc(x*sizeof(int*));
	for(int i=0; i < x; i++)
	{
		tab[i] = (int*)malloc(y*sizeof(int));
		memset(tab[i], 0, y*sizeof(int));
	}
	return tab;
}

void tablica_free(int **tab, int x)
{
	for(int i=0; i < x; i++)
		free(tab[i]);
	free(tab);
}

void zapakuj(FILE *fout, int p, List::List<przedmiot> *L)
{
	int **P = tablica_init(L->getLen()+1, p+1);
	int **Q = tablica_init(L->getLen()+1, p+1);

	L->reset();
	for(int i=1; L->current(); i++)
	{
		for(int j=1; j <= p; j++)
		{
			przedmiot *a = L->current()->getData(); 
			if((j >= a->masa) && (P[i-1][j] < P[i][j-a->masa] + a->cena))
			{
				P[i][j] = P[i-1][j-a->masa] + a->cena;	// dodanie aktualnego przedmiotu
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

	fprintf(fout, "P:\n");
	wypisz(fout, P, L->getLen(), p);
	fprintf(fout, "\n\nQ:\n");
	wypisz(fout, Q, L->getLen(), p);				// wypisywanie wyniku
	fprintf(fout, "\n\nNajlepsze upakowanie:\n");
	wypisz_plecak(fout, P, Q, L, p);

	tablica_free(P, L->getLen()+1);
	tablica_free(Q, L->getLen()+1);
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