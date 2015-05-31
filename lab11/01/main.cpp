/*
 * Metody Programowania - Laboratorium 11 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

#include <stdio.h>
#include "generator.h"

inline bool w_kole(int x, int y, int r)
{
	return (x*x)+(y*y) <= (r*r);
}

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		fprintf(stderr, "Uzycie: %s <r> <n>\n", argv[0]);
		exit(-1);
	}

	int r = atoi(argv[1]);
	int n = atoi(argv[2]);

	opts genopts = { 
		n,				// ilosc liczb
		-r,				// poczatek przedzialu
		r,				// koniec przedzialu
		time(NULL),		// seed
		NULL,			// generator
		false,			// verbose
		NULL			// plik
	};

	long long *x = generator_ALFG(&genopts, 0);
	genopts.seed = genopts.seed * x[0] + x[1];	// nowy seed
	long long *y = generator_ALFG(&genopts, 0);

	int ilosc_w_kole = 0;
	for(int i=0; i < n; i++)
		if(w_kole(x[i], y[i], r)) ilosc_w_kole++;

	printf("pi = %f\n", 4.*((double)ilosc_w_kole/(double)n));
}