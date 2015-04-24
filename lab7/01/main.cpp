/*
 * Metody Programowania - Laboratorium 7 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 1 | 04.2015
 */

#include <stdio.h>
#include <stdlib.h>
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

void zapakuj(int p, List::List<przedmiot> *L)
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
	zapakuj(pojemnosc, L);		// pakowanie plecaka

	delete L;
	fclose(in);		// zamkniecie plikow
	fclose(out);
	return 0;
}