#!/bin/bash

# Metody Programowania - Laboratorium 4 - Program 02
# Skrypt generujacy i testujacy zestawy danych
# Grzegorz Kowalski, 12i
# wersja 1 | 03.2015

# Skrypt przyjmuje jeden argument - nazwe pliku wynikowego
[ -n "$1" ] || { echo "Nie podano nazwy pliku wyjsciowego!"; exit -1; }

IL_Z=4				# ilosc zestawow
IL_E=2500000		# ilosc elementow w zestawie
PROGRAM="./a.out"	# nazwa pliku wykonywalnego z programem
PLIK="zestawy.txt"	# nazwa pliku z generowanymi zestawami danych

# generacja testowych zestawow danych
echo $IL_Z > $PLIK

echo $IL_E >> $PLIK
for (( i=1; i <= $IL_E; i++ )); do		# liczby losowe
	echo -n "$RANDOM " >> $PLIK
done
echo >> $PLIK		# nowy wiersz

echo $IL_E >> $PLIK
for (( i=1; i <= $IL_E; i++ )); do		# liczby posortowane rosnaco
	echo -n "$i " >> $PLIK
done
echo >> $PLIK		# nowy wiersz

echo $IL_E >> $PLIK
for (( i=1; i <= $IL_E; i++ )); do		# liczby posortowane malejaco
	echo -n "$[$IL_E - $i] " >> $PLIK
done
echo >> $PLIK		# nowy wiersz

echo $IL_E >> $PLIK
for (( i=1; i <= $IL_E; i++ )); do		# ok. 10% liczb w zlych miejscach
	if [ $[$i % 10] -eq 0 ]; then
		echo -n "$RANDOM " >> $PLIK
	else
		echo -n "$i " >> $PLIK
	fi
done

# testowanie programu na wygenerowanych zestawach danych oraz generowanie tabelki
i=0
IFS=''				# zachowanie bialych znakow w danych wyjsciowych programu
$PROGRAM $PLIK /dev/null | {	# uruchomienie programu i interpretacja danych
	while read -r l; do
		WYJSCIE[$i]="$l"
		i=$[$i+1]
	done

	echo "+------------------+------------------+------------------+------------------+" > "$1"
	echo "|     tablica      |     dlugosc      |    porownania    |       czas       |" >> "$1"
	echo "+------------------+------------------+------------------+------------------+" >> "$1"
	echo -n "|  losowe liczby   | " >> "$1"
	echo ${WYJSCIE[0]} >> "$1"
	echo -n "|     rosnaco      | " >> "$1"
	echo ${WYJSCIE[1]} >> "$1"
	echo -n "|     malejaco     | " >> "$1"
	echo ${WYJSCIE[2]} >> "$1"
	echo -n "|     10% zle      | " >> "$1"
	echo ${WYJSCIE[3]} >> "$1"
	echo "+------------------+------------------+------------------+------------------+" >> "$1"
}

rm $PLIK		# usuniecie pliku z zestawami