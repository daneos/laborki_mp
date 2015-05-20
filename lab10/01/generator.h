/*
 * Metody Programowania - Laboratorium 10
 * Generator liczb losowych
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

#include <string.h>

#ifndef __GENERATOR_H__
#define __GENERATOR_H__

typedef struct _opts {
	long long n;				// ilosc generowanych liczb
	long long from;				// poczatek przedzialu
	long long to;				// koniec przedzialu
	unsigned long long seed;	// ziarno generatora
	char *gen;					// generator
	FILE *out;					// plik wyjsciowy
} opts;

void generator_LCG(opts *o)
{

}

void generator_MLCG(opts *o)
{

}

void generator_ALFG(opts *o)
{

}

void generator_TEST(opts *o)
{
	int liczba_losowa = 4;	// wyznaczona na podstawie losowego rzutu kostka
	for(int i=0; i < o->n; i++)
		fprintf(o->out, "%d\n", liczba_losowa);
}

const static struct {
	const char *name;
	void (*gen)(opts*);
} generator_map [] = {			// mapa funkcji generujacych, pozwala na uruchamianie funkcji za pomoca ciagu znakow
	{ "LCG",	generator_LCG },
	{ "MLCG",	generator_MLCG },
	{ "ALFG",	generator_ALFG },
	{ "TEST",	generator_TEST }
};

bool generator(opts *o)		// funkcja wywolujaca odpowiedni generator na podstawie opcji
{
	for(int i = 0; i < (sizeof(generator_map) / sizeof(generator_map[0])); i++)
	{
		if(!strcmp(generator_map[i].name, o->gen) && generator_map[i].gen)
		{
			generator_map[i].gen(o);
			return true;
		}
	}
	return false;
}

#endif /* __GENERATOR_H__ */