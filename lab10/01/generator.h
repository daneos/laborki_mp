/*
 * Metody Programowania - Laboratorium 10
 * Naglowek: generatory liczb losowych
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

#include <stdio.h>
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

// funkcje generatorow
void generator_LCG(opts *o);
void generator_MLCG(opts *o);
void generator_ALFG(opts *o);
void generator_TEST(opts *o);

bool generator(opts *o);		// funkcja wywolujaca odpowiedni generator na podstawie opcji

const static struct {
	const char *name;
	void (*gen)(opts*);
} generator_map [] = {			// mapa funkcji generujacych, pozwala na uruchamianie funkcji za pomoca ciagu znakow
	{ "LCG",	generator_LCG },
	{ "MLCG",	generator_MLCG },
	{ "ALFG",	generator_ALFG },
	{ "TEST",	generator_TEST }
};

#endif /* __GENERATOR_H__ */