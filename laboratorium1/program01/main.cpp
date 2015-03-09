/*
 * Metody Programowania - Laboratorium 1 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 3 | 03.2015
 */

#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h> // memset

typedef struct _suma {
	int D;			// suma dodatnich
	int U;			// suma ujemnych
	int i;			// indeks od
	int j;			// indeks do
	int s;			// suma przedzialu
} suma;

typedef struct _zestaw {
	//--------- dane wejsciowe zestawu
	int il_zest;	// ilosc zestawow, taka sama w kazdym zestawie
	int n;			// ilosc elementow tablicy
	int *dane;		// tablica
	//--------- dane wyjsciowe zestawu
	suma max;		// maksymalna suma zestawu
} zestaw;

zestaw *wejscie(void)
{
	int z;					// bez inicjalizacji, nastepna instrukcja nadpisuje
	do scanf("%d", &z); while(z <= 0);		// liczba zestawow

	zestaw *tz = (zestaw*)malloc(z*sizeof(zestaw)); // rezerwacja pamieci - tablica zestawow
	for(int i=0; i<z; i++)
	{
		int n;					// j.w.
		do scanf("%d", &n); while(n < 1 || n > 1000000);	// liczba elementow

		tz[i].il_zest = z;
		tz[i].n = n;
		tz[i].dane = (int*)malloc(n*sizeof(int)); // rezerwacja pamieci - tablica danych w zestawie
		for(int ie=0; ie<n; ie++)
		{
			int element;					// j.w.
			do scanf("%d", &element); while(element < -2000 || element > 2000);		// element

			tz[i].dane[ie] = element;
		}
	}
	return tz;
}

inline int S(suma *s)	// suma
{
	return (3*s->D)+(2*s->U);
}

void max_zestawu(zestaw *z)
{
	suma akt, max;		// aktualna i maksymalna suma
	memset(&akt, 0, sizeof(suma));
	max = akt;			// zerowanie zmiennych
	
	for(; akt.j < z->n; akt.j++)
	{
		z->dane[akt.j] >= 0 ? akt.D += z->dane[akt.j] : akt.U += z->dane[akt.j];
		if(S(&akt) > S(&max)) max = akt;		// jesli aktualna suma jest wieksza od maksymalnej, nadpisz
		else if(S(&akt) < 0)
		{
			akt.i = akt.j+1;		// jesli aktualna suma < 0, zaktualizuj poczatek przedzialu
			akt.D = akt.U = 0;		// wyzeruj sume
		}
	}
	max.s = S(&max);		// zapisz sume ostateczna do struktury wyjsciowej
	z->max = max;
}

void wyjscie(zestaw *z)
{
	printf("%d %d %d\n", z->max.i, z->max.j, z->max.s);
}

int main(void)
{
	zestaw *Z = wejscie();			// wczytanie zestawow 
	for(int iz=0; iz < Z->il_zest; iz++)
	{
		max_zestawu(&Z[iz]);		// wyznaczanie maksymalnej sumy dla kazdego zestawu
		wyjscie(&Z[iz]);			// wypisanie rozwiazania zestawu
		free(Z[iz].dane);			// zwolnienie zawartosci zestawu
	}
	free(Z);						// zwolnienie tablicy zestawow

	#ifdef _WIN32
		getchar();					// zatrzymanie terminala (przydatne wlasciwie tylko pod windowsem)
	#endif

	return 0;
}