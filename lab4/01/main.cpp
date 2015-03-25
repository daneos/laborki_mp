/*
 * Metody Programowania - Laboratorium 4 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 1 | 03.2015
 */

#include <stdio.h>
#include <stdlib.h>	// exit,atoi

void move(char A, char B, int *i)
{
	printf("%d: Przenies z %c na %c.\n", ++(*i), A, B);
}

void hanoi(char A, char B, char C, int n, int *i)
{
	//-------------------------------------------------------------------------
	if(n == 1) move(A, C, i);
	else
	{
		hanoi(A, C, B, n-1, i);
		move(A, C, i);
		hanoi(B, A, C, n-1, i);
	}
	//-------------------------------------------------------------------------
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Uzycie: %s <n>\n", argv[0]);
		exit(1);
	}

	int krok = 0;
	hanoi('A', 'B', 'C', atoi(argv[1]), &krok);
	printf("Gotowe w %d krokach!\n", krok);

	return 0;
}