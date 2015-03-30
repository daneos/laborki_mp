/*
 * Metody Programowania - Laboratorium 4 - Program 02
 * Implementacja algorytmu Quicksort
 * Grzegorz Kowalski, 12i
 * wersja 1 | 03.2015
 */

#ifndef __QUICKSORT_H__
#define __QUICKSORT_H__

void zamien(long *T, int i, int j)
{
	long tmp = T[i];
	T[i] = T[j];
	T[j] = tmp;
}

unsigned long dziel(long *T, int p, int r, int *q)
{
	unsigned long por = 0;
	int ep = T[p];					// wartosc elementu w punkcie podzialu

	int i = p-1;
	int j = r+1;
	for(;;)
	{
		while(T[--j] > ep) por++;
		while(T[++i] < ep) por++;
		if(i < j) zamien(T, i, j);	// przenoszenie elementow do odpowiednich czesci tablicy
		else
		{
			*q = j;			// ostateczny punkt podzialu
			return por;
		}
	}
}

unsigned long _quicksort(long *T, int p, int r)
{
	unsigned long por = 0;	// ilosc porownan
	if(p < r)
	{
		int q;
		por += dziel(T, p, r, &q);		// podziel tablice
		por += _quicksort(T, p, q);		
		por += _quicksort(T, q+1, r);	// wywolania rekurencyjne
	}
	return por;
}

#endif /* __QUICKSORT_H__ */