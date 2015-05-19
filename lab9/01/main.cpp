/*
 * Metody Programowania - Laboratorium 9 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Tree.h"

typedef struct _symbol {
	bool zawiera_dane;	// flaga danych
	char litera;		// litera
	double czest;		// czestotliwosc/prawdopodobienstwo
} symbol;

void wypisz_liste(List::List< Tree::BinaryReturn<symbol>* > *L)
{
	printf("wypisz\n");
	for(L->reset(); L->current(); L->next())
	{
		symbol *p = (*L->current()->getData())->getRoot()->getData();
		printf("%p, %s, %c, %f\n", p, p->zawiera_dane?"true":"false", p->zawiera_dane?p->litera:'~', p->czest);
	}
}

void czytaj(FILE *fin, List::List< Tree::BinaryReturn<symbol>* > *L)
{
	while(true)
	{
		if(feof(fin)) break;

		symbol s;
		s.zawiera_dane = true;
		fscanf(fin, "%c %lf\n", &s.litera, &s.czest);

		Tree::Node::BinaryReturn<symbol> *korzen = new Tree::Node::BinaryReturn<symbol>(s);
		Tree::BinaryReturn<symbol> *drzewo = new Tree::BinaryReturn<symbol>;
		drzewo->setRoot(korzen);
		List::Element< Tree::BinaryReturn<symbol>* > *li = new List::Element< Tree::BinaryReturn<symbol>* >(drzewo);

		int i=1;
		for(L->reset(); L->current(); L->next())	// petla automatycznie sortujaca
		{
			if((*L->current()->getData())->getRoot()->getData()->czest > s.czest)		// sortowanie wg. czestotliwosci
			{
				L->prependBefore(li, i);	// dodawanie w odpowiednie miejsce listy
				break;
			}
			i++;
		}
		if(!L->current()) *L += li;			// element nie zostal dodany wczesniej, dodaje na koniec
	}
}

void huffman(FILE *fout, List::List< Tree::BinaryReturn<symbol>* > *lista)
{
	while(lista->getLen() > 1)
	{
		Tree::BinaryReturn<symbol> *lewe = *(*lista)[1]->getData();
		Tree::BinaryReturn<symbol> *prawe = *(*lista)[2]->getData();
		lista->remove(1, false);	// usuwam wezly dodawane do drzewa
		lista->remove(1, false);	// tutaj tez 1, poniewaz po usunieciu pierwszego, drugi staje sie pierwszym elementem

		symbol p;
		p.zawiera_dane = false;
		p.czest = lewe->getRoot()->getData()->czest + prawe->getRoot()->getData()->czest;	// suma czestotliwosci dwoch najmniejszych drzew

		Tree::Node::BinaryReturn<symbol> *korzen = new Tree::Node::BinaryReturn<symbol>(p);
		Tree::BinaryReturn<symbol> *drzewo = new Tree::BinaryReturn<symbol>;
		drzewo->setRoot(korzen);

		drzewo->Reset();	drzewo->appendLeft(lewe);	// dodaje wezly nowego drzewa
		drzewo->Reset();	drzewo->appendRight(prawe);

		List::Element< Tree::BinaryReturn<symbol>* > *li = new List::Element< Tree::BinaryReturn<symbol>* >(drzewo);
		int i=1;
		for(lista->reset(); lista->current(); lista->next())	// petla automatycznie sortujaca
		{
			if((*lista->current()->getData())->getRoot()->getData()->czest > p.czest)
			{
				lista->prependBefore(li, i);
				break;
			}
			i++;
		}
		if(!lista->current()) *lista += li;			// element nie zostal dodany wczesniej, dodaje na koniec
	}
	printf("gotowe\n");
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

	List::List< Tree::BinaryReturn<symbol>* > *L = new List::List< Tree::BinaryReturn<symbol>* >;		// lista drzew o elementach typu symbol
	czytaj(in, L);
	wypisz_liste(L);
	huffman(out, L);
	wypisz_liste(L);
	
	delete L;
	fclose(in);		// zamkniecie plikow
	fclose(out);
	return 0;
}