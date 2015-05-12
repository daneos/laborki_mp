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

void wypisz_liste(FILE *fout, List::List<przedmiot> *L)
{
	fprintf(fout, "    id\tcena\tmasa\t\tadres\n    --------------------------------------------------\n");
	for(L->reset(); L->current(); L->next())
	{
		przedmiot *p = L->current()->getData();
		fprintf(fout, "    %d\t%d\t%d\t\t%p\n", p->i, p->cena, p->masa, p);
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

int k(Tree::Node::BinaryReturn<wezel> *W, int p, List::List<przedmiot> *L)
{
	int total = 0;
	int k;
	for(k=1; k <= L->getLen(); k++)
	{
		if(W->key->Depth < k || BITMAP_CHECK(W->key->Bitmap, k))
			total += (*L)[k]->getData()->masa;
		if(total >= p) break;
	}
	return total < p ? k+1 : k;
}

int totweight(Tree::Node::BinaryReturn<wezel> *W, int p, List::List<przedmiot> *L)
{
	int t = W->getData()->weight;
	int k_w = k(W, p, L);
	if(k_w > L->getLen()) return -1;
	for(int i = W->key->Depth+1; i <= k_w-1; i++)
		t += (*L)[i]->getData()->masa;
	return t;
}

int bound(Tree::Node::BinaryReturn<wezel> *W, int p, List::List<przedmiot> *L)
{
	int b = W->getData()->profit;
	int k_w = k(W, p, L);
	if(k_w > L->getLen()) return -1;		// jesli k jest wieksze od ilosci przedmiotow, nie da sie wyliczyc bound
	for(int i = W->key->Depth+1; i <= k_w-1; i++)
		b += (*L)[i]->getData()->cena;
	return b + (p - totweight(W, p, L)) * ((*L)[k_w]->getData()->cena/(*L)[k_w]->getData()->masa);
}

void wypisz_wezel_info(FILE *fout, Tree::Node::BinaryReturn<wezel> *W, int p, List::List<przedmiot> *L)
{
	wezel *w_data = W->getData();
	fprintf(fout, "[:] Wezel: (%d, %d)\t\tNode:%p\t\tData:%p\t\tP:%p\n", W->key->Bitmap, W->key->Depth, W, w_data, w_data->P);
	fprintf(fout, "    k         = %d\n", w_data->k);
	fprintf(fout, "    totweight = %d\n", totweight(W, p, L));
	fprintf(fout, "    bound     = %d\n", w_data->bound);
}

bool promising(Tree::Node::BinaryReturn<wezel> *W, int p, int max)
{
	if(W->getData()->weight >= p) return false;
	if(W->getData()->bound <= max) return false;
	return true;
}

void checknode_tree(FILE *fout, Tree::BinaryReturn<wezel> *T, int p, int *max, List::List<przedmiot> *L)
{
	wypisz_wezel_info(fout, T->Current(), p, L);
	if(T->Current()->getData()->profit > *max && T->Current()->getData()->weight <= p)
	{
		*max = T->Current()->getData()->profit;
		fprintf(fout, "[+] MAX = %d\n", *max);
	}
	else fprintf(fout, "[-] MAX niezmieniony: = %d\n", *max);

	if(promising(T->Current(), p, *max) && T->Current()->key->Depth < L->getLen())
	{
		fprintf(fout, "[+] Obiecujacy, sprawdzam dalej.\n\n");

		przedmiot *nastepny = (*L)[T->Current()->key->Depth+1]->getData();

		// wypelniam lewe dziecko
		wezel lewy_data = { 
			nastepny,												// przedmiot *P
			T->Current()->getData()->profit + nastepny->cena,		// int profit
			T->Current()->getData()->weight + nastepny->masa,		// int weight
			0,														// int bound
			0														// int k
		};
		Tree::Node::BinaryReturn<wezel> *lewy = new Tree::Node::BinaryReturn<wezel>(lewy_data);
		T->appendLeft(lewy);		// dolaczam do drzewa, aby wypelnic pola klucza (do obliczenia k i bound)
		lewy->getData()->bound = bound(lewy, p, L);
		lewy->getData()->k = k(lewy, p, L);

		// wypelniam prawe dziecko
		wezel prawy_data = { 
			nastepny,								// przedmiot *P
			T->Current()->getData()->profit,		// int profit
			T->Current()->getData()->weight,		// int weight
			0,										// int bound
			0										// int k
		};
		Tree::Node::BinaryReturn<wezel> *prawy = new Tree::Node::BinaryReturn<wezel>(prawy_data);
		T->appendRight(prawy);		// dolaczam do drzewa, aby wypelnic pola klucza (do obliczenia k i bound)
		prawy->getData()->bound = bound(prawy, p, L);
		prawy->getData()->k = k(prawy, p, L);

		T->Left();	checknode_tree(fout, T, p, max, L);		// sprawdzanie lewego dziecka
		T->Right();	checknode_tree(fout, T, p, max, L);		// sprawdzanie prawego dziecka
	}
	else fprintf(fout, "[-] Nieobiecujacy.\n\n");
	fprintf(fout, "[:] Wracam.\n\n");
	T->Parent();		// powrot do wyzszego wezla
}

void zapakuj(FILE *fout, int W, List::List<przedmiot> *L)
{
	fprintf(fout, "[:] Inicjalizacja drzewa...\n");
	wezel root = { NULL, 0, 0, 0, 0 };		// inicjalizacja drzewa
	Tree::BinaryReturn<wezel> *T = new Tree::BinaryReturn<wezel>;
	Tree::Node::BinaryReturn<wezel> *r = new Tree::Node::BinaryReturn<wezel>(root);
	T->setRoot(r);
	T->Reset();

	fprintf(fout, "[:] Inicjalizacja korzenia...\n");
	T->Current()->getData()->k = k(T->Current(), W, L);
	T->Current()->getData()->bound = bound(T->Current(), W, L);

	int maxprofit = 0;
	checknode_tree(fout, T, W, &maxprofit, L);

	// jakies rozwiazanie moze?

	delete T;
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
	fprintf(out, "[+] Wczytano plecak: %d\n", pojemnosc);
	wypisz_liste(out, L);
	printf("\n\n");
	zapakuj(out, pojemnosc, L);		// pakowanie plecaka

	delete L;
	fclose(in);		// zamkniecie plikow
	fclose(out);
	return 0;
}