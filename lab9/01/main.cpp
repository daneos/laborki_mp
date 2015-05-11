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
	float czest;		// czestotliwosc/prawdopodobienstwo
} symbol;

void wypisz_liste(List::List< Tree::BinaryReturn<symbol> > *L)
{
	printf("wypisz\n");
	for(L->reset(); L->current(); L->next())
	{
		symbol *p = L->current()->getData()->getRoot()->getData();
		printf("%p, %s, %c, %f\n", p, p->zawiera_dane?"true":"false", p->zawiera_dane?p->litera:'~', p->czest);
	}
}

List::List< Tree::BinaryReturn<symbol> > *czytaj(FILE *fin)
{
	List::List< Tree::BinaryReturn<symbol> > *L = new List::List< Tree::BinaryReturn<symbol> >;		// lista drzew o elementach typu litera
	while(true)
	{
		symbol p;
		fscanf(fin, "%c %f\n", &p.litera, &p.czest);
		p.zawiera_dane = true;
		printf("IN: %c, %f\n", p.litera, p.czest);
		if(feof(fin)) break;	// dodaje litere do listy tylko jesli udalo sie ja odczytac w calosci
		
		Tree::Node::BinaryReturn<symbol> *korzen = new Tree::Node::BinaryReturn<symbol>(p);
		Tree::BinaryReturn<symbol> *drzewo = new Tree::BinaryReturn<symbol>;
		drzewo->setRoot(korzen);
		List::Element< Tree::BinaryReturn<symbol> > *li = new List::Element< Tree::BinaryReturn<symbol> >(*drzewo);

		int i=1;
		for(L->reset(); L->current(); L->next())	// petla automatycznie sortujaca
		{
			printf("\t\t%d\t|\t%f > %f\n", i, L->current()->getData()->getRoot()->getData()->czest, p.czest);
			if(L->current()->getData()->getRoot()->getData()->czest > p.czest)
			{
				L->prependBefore(li, i);
				printf("IN:\t\t@ List[%d]\n", i);
				break;
			}
			i++;
		}

		if(!L->current())
		{
			*L += li;
			printf("IN:\t\t@ List[END]\n");
		}
	}
	wypisz_liste(L);
	return L;
}

void huffman(FILE *fout, List::List< Tree::BinaryReturn<symbol> > *lista)
{
	while(lista->getLen() > 1)
	{
		symbol p;
		p.zawiera_dane = false;
		p.czest = (*lista)[1]->getData()->getRoot()->getData()->czest + (*lista)[2]->getData()->getRoot()->getData()->czest;	// suma czestotliwosci dwoch najmniejszych drzew

		Tree::Node::BinaryReturn<symbol> *korzen = new Tree::Node::BinaryReturn<symbol>(p);
		Tree::BinaryReturn<symbol> *drzewo = new Tree::BinaryReturn<symbol>;
		drzewo->setRoot(korzen);

		drzewo->appendLeft((*lista)[1]->getData());	// dodaje wezly nowego drzewa
		drzewo->appendRight((*lista)[2]->getData());

		lista->remove(1, false);
		lista->remove(2, false);

		int i=1;
		for(lista->reset(); lista->current(); lista->next())	// petla automatycznie sortujaca
		{
			if(lista->current()->getData()->getRoot()->getData()->czest > p.czest)
			{
				lista->prependBefore(new List::Element< Tree::BinaryReturn<symbol> >(*drzewo), i);
				break;
			}
			i++;
		}
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

	List::List< Tree::BinaryReturn<symbol> > *lista = czytaj(in);
	wypisz_liste(lista);
	huffman(out, lista);
	wypisz_liste(lista);
	
	delete lista;
	fclose(in);		// zamkniecie plikow
	fclose(out);
	return 0;
}