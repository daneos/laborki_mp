/*
 * Metody Programowania - Laboratorium 10 - Program 01
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <time.h>
#include "generator.h"

inline void toupper_str(char *str)
{
	while(*str = toupper(*str)) str++;
}

inline void print_help(char *argv0)
{
	printf("UZYCIE: %s <opcje> [plik_wyjsciowy]\n", argv0);
	printf("\t--generator\tgenerator liczb losowych (LCG, MLCG, ALFG, TEST)\n");
	printf("\t--from\t\tpoczatek zakresu generowanych liczb (domyslnie 0)\n");
	printf("\t--to\t\tkoniec zakresu (domyslnie 100)\n");
	printf("\t--seed\t\tziarno generatora (domyslnie czas systemu)\n");
	printf("\t-n\t\tilosc generowanych liczb (domyslnie 1)\n");
	printf("\t--help\t\twyswietla ten komunikat pomocy\n");
}

int main(int argc, char *argv[])
{
	int c;		// opcja
	int option_index = 0;	// index w tablicy long_opts
	struct option long_opts[] = {		// tablica długich opcji
		{ "generator",	required_argument,	0,	'g' },
		{ "from",		required_argument,	0,	'f' },
		{ "to",			required_argument,	0,	't' },
		{ "seed",		required_argument,	0,	's' },
		{ "help",		no_argument,		0,	'h' },
		{ 0,			0,					0,	0	}
	};
	
	opts options = { 1, 0, 100, time(NULL), NULL, stdout };	// wartosci domyslne opcji

	while(true)		// petla interpretujaca opcje
	{
		c = getopt_long(argc, argv, "n:g:f:t:s:h", long_opts, &option_index);	// getopt - interpretacja opcji, watpie zeby dzialalo pod windowsem
		if(c == -1) break;	// koniec opcji
		
		switch(c)
		{
			case 'n':
				options.n = atoll(optarg);
				break;

			case 'g':
				toupper_str(optarg);
				options.gen = optarg;
				break;

 			case 'f':
				options.from = atoll(optarg);
				break;

 			case 't':
				options.to = atoll(optarg);
				break;

 			case 's':
				options.seed = atoll(optarg);
				break;

 			case 'h':
				print_help(argv[0]);
				exit(0);
				break;

 			case '?':	// getopt sam wypisuje komunikat o bledzie
				break;

 			default:
				exit(1);	// blad nieobslugiwany przez getopt
				break;
		}
	}

	if(options.gen == NULL)
	{
		fprintf(stderr, "Musisz podac generator. Zobacz `%s --help`.\n", argv[0]);
		exit(1);
	}
	
	if(optind < argc)	// podano dodatkowy argument - nazwe pliku wyjsciowego
	{
		if((options.out = fopen(argv[optind], "w")) == NULL)	// otwarcie tylko do zapisu
		{
			perror("main()/fopen():out");
			exit(1);
		}
	}

	printf("n\t\t= %lld\n", options.n);
	printf("from\t\t= %lld\n", options.from);
	printf("to\t\t= %lld\n", options.to);
	printf("seed\t\t= %lld\n", options.seed);
	printf("generator\t= %s\n", options.gen);

	generator(&options);	// wywolanie generatora

	fclose(options.out);
	return 0;
}