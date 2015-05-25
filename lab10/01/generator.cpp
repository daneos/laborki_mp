/*
 * Metody Programowania - Laboratorium 10
 * Implementacja: generatory liczb losowych
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

#include "generator.h"

// funkcje generatorow
//-----------------------------------------------------------------------------
long long *generator_LCG(opts *o, int multi)
{
	if(o->verbose) printf("Inicjalizacja generatora (M)LCG...\n");

	long long m = 101;		// modyfikacja pozwalajaca osiagnac zadany zakres
	long long x0 = o->seed % m;		// poniewaz x0 < m

	// rozklad m na czynniki pierwsze
	long long R = m;
	int k = 2;				// pierwsza liczba pierwsza
	int il_cz = 0;			// ilosc czynnikow
	int *czynniki = NULL;	// tablica czynnikow
	while(R > 1)
	{
		while(R % k == 0)
		{
			czynniki = (int*)realloc(czynniki, ++il_cz*sizeof(int));		// tablica dynamiczna, wersja C
			czynniki[il_cz] = k;
			R /= k;
		}
		k++;
	}

	int c;
	if(!multi)
	{
		// wyznaczanie przyrostu wzglednie pierwszego z m
		c = 2;
		while(true)
		{
			bool w_czynnikach = false;
			for(int i=0; i < il_cz; i++)
				if(c == czynniki[i]) w_czynnikach=true;
			if(!w_czynnikach) break;
			c++;
		}
	}
	else c = 0;		// generator MLCG

	// wyznaczanie mnoznika
	long long a;
	long long b = 1;
	for(int i=0; i < il_cz; i++) b *= czynniki[i];		// b jest wielokrotnoscia wszystkich dzielnikow m
	if(m % 4 == 0)
	{
		if(b % 4 == 0)
			a = b+1;		// jesli m oraz b jest podzielne przez 4 to a = b+1 (b = a-1)
	}
	else a = (time(NULL) * x0) ^ o->seed;		// w przeciwnym wypadku a ustawiane jest na wzglednie losowa wartosc
	a %= m;		// poniewz a < m 

	if(o->verbose)
	{
		printf("a\t= %lld\n", a);
		printf("x0\t= %lld\n", x0);
		printf("c\t= %d\n", c);
		printf("m\t= %lld\n", m);
	}

	if(m <= 0 || x0 < 0 || x0 >= m || c < 0 || c >= m || a < 0 || a >= m) die("Generator (M)LCG: Niepoprawne dane.");
	if(!multi && c == 0) die("Generator (M)LCG: Blad wyznaczania przyrostu.");

	long long *wyniki = (long long *)malloc(o->n*sizeof(long long));
	for(int i=0; i < o->n; i++)
	{
		x0 = (a * x0 + c) % m;
		wyniki[i] = o->from + ((double)x0/100.) * (o->to - o->from);	// w celu zapewnienia odpowiedniego zakresu liczb
	}
	return wyniki;
}

long long *generator_ALFG(opts *o, int not_used)
{

}

long long *generator_TEST(opts *o, int not_used)
{
	int liczba_losowa = 4;	// wyznaczona na podstawie losowego rzutu kostka

	long long *wyniki = (long long *)malloc(o->n*sizeof(long long));
	for(int i=0; i < o->n; i++)
		wyniki[i] = liczba_losowa;
	return wyniki;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool generator(opts *o)		// funkcja wywolujaca odpowiedni generator na podstawie opcji
{
	for(int i = 0; i < (sizeof(generator_map) / sizeof(generator_map[0])); i++)		// wyszukanie generatora za pomoca podanych opcji
	{
		if(!strcmp(generator_map[i].name, o->gen) && generator_map[i].gen)
		{
			long long *wyniki = generator_map[i].gen(o, generator_map[i].option);	// uruchomienie generatora
			for(int i=0; i < o->n; i++)
				fprintf(o->out, "%lld\n", wyniki[i]);	// wypisanie wynikow
			free(wyniki);	// zwolnienie wynikow
			return true;
		}
	}
	return false;	// brak generatora
}

void die(const char *message)
{
	fprintf(stderr, "%s\n", message);
	exit(-1);
}