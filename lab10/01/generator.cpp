/*
 * Metody Programowania - Laboratorium 10
 * Implementacja: generatory liczb losowych
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

#include "generator.h"

// funkcje generatorow
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
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