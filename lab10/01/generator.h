/*
 * Metody Programowania - Laboratorium 10
 * Generator liczb losowych
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

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

void generator(opts *o)
{
	// do nothing...
}

#endif /* __GENERATOR_H__ */