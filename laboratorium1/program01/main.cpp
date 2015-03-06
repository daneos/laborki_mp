/*
 * Metody Programowania - Laboratorium 1 - Program 01
 * Grzegorz Kowalski, 12i
 */

#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h> // memset

#define __DEBUG

typedef struct _suma {
	int D;			// suma dodatnich
	int U;			// suma ujemnych
	int i;			// indeks od
	int j;			// indeks do
	int s;			// suma przedzialu
	bool stop;		// flaga konca przedzialu (nastepne wartosci nie sa dodawane do sumy)
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
	do {
		scanf("%d", &z);	// liczba zestawow
	} while(z <= 0);
	
	#ifdef __DEBUG
		printf("DEBUG: Ilosc zestawow wczytana: %d\n", z);
	#endif
	
	zestaw *tz = (zestaw*)malloc(z*sizeof(zestaw)); // rezerwacja pamieci - tablica zestawow
	for(int i=0; i<z; i++)
	{
		int n;					// j.w.
		do {
			scanf("%d", &n);	// liczba elementow
		} while(n < 1 || n > 1000000);
		
		#ifdef __DEBUG
			printf("DEBUG: Ilosc elementow zestawu %d wczytana: %d\n", i, n);
		#endif
		
		tz[i].il_zest = z;
		tz[i].n = n;
		tz[i].dane = (int*)malloc(n*sizeof(int)); // rezerwacja pamieci - tablica danych w zestawie
		for(int ie=0; ie<n; ie++)
		{
			int element;					// j.w.
			do {
				scanf("%d", &element);		// element
			} while(element < -2000 || element > 2000);
			
			#ifdef __DEBUG
				printf("DEBUG: Element %d zestawu %d wczytany: %d\n", ie, i, element);
			#endif

			tz[i].dane[ie] = element;
		}
	}
	return tz;
}

inline int S(int D, int U)	// suma
{
	return (3*D)+(2*U);
}


suma max_podsuma(suma *ts, int is)
{
	suma max;
	max.s = 0;
	for(int i=0; i <= is; i++)
	{
		#ifdef __DEBUG
			printf("DEBUG: Suma: %d: i=%d, j=%d, s=%d\n", i, ts[i].i, ts[i].j, S(ts[i].D, ts[i].U));
		#endif

		if(ts[i].i <= ts[i].j)		// jesli i>j to znaczy ze podsuma zostala w calosci odcieta jako ujemna
		{
			ts[i].s = S(ts[i].D, ts[i].U);	// liczenie sumy
			if(ts[i].s > max.s)
				max = ts[i];
		}
	}

	return max;
}

void max_zestawu(zestaw *z)
{
	#ifdef __DEBUG
		printf("DEBUG: Zestaw: %d;%d;%p\n", z->il_zest, z->n, z->dane);
	#endif

	suma *tab_sum = (suma*)malloc((2*z->n+1)*sizeof(suma));	// moze byc max 2*n+1 podsum
	memset(tab_sum, 0, (2*z->n+1)*sizeof(suma));		// zerowanie pamieci, zeby nie musiec inicjalizowac D, U i stop w kazdej podsumie
	bool dodatnie = false;						// flaga okreslajaca wystapienie dodatnich wartosci
	int is = 0;					// indeks sum
	
	for(int i=0,j=0; j < z->n; j++)		// i - poczatek, j - koniec
	{
		#ifdef __DEBUG
			printf("DEBUG: i = %d;\tj = %d;\tis = %d;\telement = %d\n", i, j, is, z->dane[j]);
		#endif

		if(z->dane[j] >= 0 || dodatnie)			// element dodatni lub wczesniej wystapily wartosci dodatnie
		{
			if(z->dane[j] < 0)		// element ujemny (jedynie w przypadku wczesniejszych wartosci dodatnich)
			{
				is++;				// stworzenie kolejnej podsumy (aktualnie liczona, do wystapienia wartosci ujemnej)
				tab_sum[is].i = i;
				tab_sum[is].j = j;	// inicjalizacja indeksow nowej podsumy
				tab_sum[is].D = tab_sum[is-1].D;	// kopiowanie sumy elementow starej podsumy
				tab_sum[is].U = tab_sum[is-1].U;
				tab_sum[is].stop = true;	// blokada, koniec liczenia tej podsumy

				for(int tmp_is=is; tmp_is >= 0; tmp_is--)
				{
					#ifdef __DEBUG
						printf("DEBUG: Suma(D1): %d: D = %d;\tU = %d;\tS = %d\n", tmp_is, tab_sum[tmp_is].D, tab_sum[tmp_is].U, (3*tab_sum[tmp_is].D)+(2*tab_sum[tmp_is].U));
					#endif

					if(!tab_sum[tmp_is].stop)		// jesli nie zakonczylismy liczenia tej podsumy
					{
						tab_sum[tmp_is].U += z->dane[j];	// dodanie elementu do ujemnych w kazdej utworzonej podsumie
						tab_sum[tmp_is].j = j;				// aktualizacja indeksow
					}

					#ifdef __DEBUG
						printf("DEBUG: Suma(D2): %d: D = %d;\tU = %d;\tS = %d\n", tmp_is, tab_sum[tmp_is].D, tab_sum[tmp_is].U, (3*tab_sum[tmp_is].D)+(2*tab_sum[tmp_is].U));
					#endif
				}

				is++;				// stworzenie kolejnej podsumy (czystej)
				tab_sum[is].i = j+1;
				tab_sum[is].j = j+1;	// inicjalizacja indeksow nowej podsumy

				#ifdef __DEBUG
					printf("DEBUG: Wartosc ujemna: i = %d;\tj = %d;\tis = %d;\telement = %d;\n", i, j, is, z->dane[j]);
				#endif
			}
			else					// element dodatni
			{
				for(int tmp_is=is; tmp_is >= 0; tmp_is--)
				{
					if(!tab_sum[tmp_is].stop)		// j.w.
					{
						tab_sum[tmp_is].D += z->dane[j];	// j.w. dla elementow dodatnich
						tab_sum[tmp_is].j = j;				// aktualizacja indeksow
					}
				}
				dodatnie = true;		// ustawiam flage wystapienia dodatnich
			}
		}
		else i++;	// odciecie ujemnej wartosci z poczatku tablicy

		if(S(tab_sum[0].D, tab_sum[0].U) < 0)			// tab_sum[0] to zawsze suma calej dotychczas przetworzonej tablicy (pomijajac sumy ujemne na poczatku)
		{
			#ifdef __DEBUG
				printf("DEBUG: Ujemna podsuma @ %d\n", j);
			#endif
			
			i=j+1;		// odciecie ujemnej podsumy z poczatku tablicy
			dodatnie = false;		// po odcieciu mozemy uznac ze nie napotkalismy wartosci dodatnich
			for(int tmp_is=is; tmp_is >= 0; tmp_is--)
			{
				for(int tmp_i=i-1; tmp_i >= 0; tmp_i--)		// usuniecie ujemnej podsumy z kazdej podsumy ktora ja zawiera
				{
					if(tmp_i >= tab_sum[tmp_is].i && !tab_sum[tmp_is].stop)
					{
						#ifdef __DEBUG
							printf("DEBUG: Suma(O1): %d: D = %d;\tU = %d;\tS = %d\n", tmp_is, tab_sum[tmp_is].D, tab_sum[tmp_is].U, (3*tab_sum[tmp_is].D)+(2*tab_sum[tmp_is].U));
						#endif

						if(z->dane[tmp_i] >= 0) tab_sum[tmp_is].D -= z->dane[tmp_i];
						else tab_sum[tmp_is].U -= z->dane[tmp_i];

						#ifdef __DEBUG
							printf("DEBUG: Suma(O2): %d: D = %d;\tU = %d;\tS = %d\n", tmp_is, tab_sum[tmp_is].D, tab_sum[tmp_is].U, (3*tab_sum[tmp_is].D)+(2*tab_sum[tmp_is].U));
						#endif
					}
				}
				tab_sum[tmp_is].i = i;				// aktualizacja indeksow
			}
		}

	}

	z->max = max_podsuma(tab_sum, is);		// wybor maksymalnej podsumy
	free(tab_sum);					// zwolnienie tablicy podsum
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