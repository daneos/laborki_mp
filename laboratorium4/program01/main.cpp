/*
 * Metody Programowania - Laboratorium 4 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 1 | 03.2015
 */

#include <stdio.h>
#include <stdlib.h>

void move(char A, char B)
{
	printf("Przenieś z %c na %c.\n", A, B);
}

void hanoi(char A, char B, char C, int n)
{
	if(n == 1) move(A, C);
	else
	{
		hanoi(A, C, B, n-1);
		move(A, C);
		hanoi(B, A, C, n-1);
	}

}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Uzycie: %s <n>\n", argv[0]);
		exit(1);
	}

	hanoi('A', 'B', 'C', atoi(argv[1]));
	printf("Gotowe!\n");
	return 0;
}