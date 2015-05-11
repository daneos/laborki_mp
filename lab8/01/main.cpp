/*
 * Metody Programowania - Laboratorium 8 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Tree.h"

typedef struct _przedmiot {
	int i;
	int cena;
	int masa;
} przedmiot;

typedef struct _wezel {
	przedmiot *P;
	int profit;
	int weight;
	int bound;
	int k;
} wezel;


void wypisz_liste(List::List<przedmiot> *L)
{
	for(L->reset(); L->current(); L->next())
	{
		przedmiot *p = L->current()->getData();
		printf("%p, %d, %d, %d\n", p, p->i, p->cena, p->masa);
	}
}

int czytaj(FILE *fin, List::List<przedmiot> *L)
{
	int poj;
	fscanf(fin, "%d", &poj);
	while(true)
	{
		przedmiot p;
		fscanf(fin, "%d", &p.i);
		fscanf(fin, "%d", &p.cena);
		fscanf(fin, "%d", &p.masa);
		if(feof(fin)) break;	// dodaje przedmiot do listy tylko jesli udalo sie go odczytac w calosci
		
		List::Element<przedmiot> *li = new List::Element<przedmiot>(p);

		int i=1;
		for(L->reset(); L->current(); L->next())	// petla automatycznie sortujaca
		{
			przedmiot *current = L->current()->getData();
			if(current->cena/current->masa < p.cena/p.masa)		// sortowanie wg. wartosci cena/masa
			{
				L->prependBefore(li, i);	// dodawanie w odpowiednie miejsce listy
				break;
			}
			i++;
		}
		if(!L->current()) *L += li;			// element nie zostal dodany wczesniej, dodaje na koniec
	}
	return poj;
}

void zapakuj(FILE *fout, int W, List::List<przedmiot> *L)
{

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

	wypisz_liste(L);

	delete L;
	fclose(in);		// zamkniecie plikow
	fclose(out);
	return 0;
}