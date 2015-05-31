/*
 * Metody Programowania - Laboratorium 10
 * Naglowek: generatory liczb losowych
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef __GENERATOR_H__
#define __GENERATOR_H__

typedef struct _opts {
	long long n;				// ilosc generowanych liczb
	long long from;				// poczatek przedzialu
	long long to;				// koniec przedzialu
	long long seed;				// ziarno generatora
	char *gen;					// generator
	bool verbose;				// tryb verbose
	FILE *out;					// plik wyjsciowy
} opts;

// funkcje generatorow
long long *generator_LCG(opts *o, int multi);
long long *generator_ALFG(opts *o, int not_used);
long long *generator_TEST(opts *o, int not_used);

bool generator(opts *o);		// funkcja wywolujaca odpowiedni generator na podstawie opcji

void die(const char *message);	// funkcja konczaca z bledem
int NWD(int a, int b);			// najwiekszy wspolny dzielnik

const struct {
	const char *name;				// nazwa generatora
	long long *(*gen)(opts*, int);	// funkcja generatora
	int option;						// alternatywna opcja
} generator_map[] = {				// mapa funkcji generujacych, pozwala na uruchamianie funkcji za pomoca ciagu znakow
	{ "LCG",	generator_LCG,	0 },
	{ "MLCG",	generator_LCG,	1 },
	{ "ALFG",	generator_ALFG,	0 },
	{ "TEST",	generator_TEST,	0 }
};

const struct {
	int r;
	int s;
} fibonacci[] = {		// najlepsze wartosci dla generatora fibonacciego
	{ 17, 5 },
	{ 31, 13 },
	{ 55, 24 },
	{ 68, 33 },
	{ 97, 33 },
	{ 607, 273 },
	{ 1279, 418 }
};

const int FIBONACCI_VALUES = sizeof(fibonacci)/sizeof(fibonacci[0]);	// ilosc wpisow w tablicy fibonacciego

#endif /* __GENERATOR_H__ */