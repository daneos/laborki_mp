#!/bin/bash
./a.out < input.txt > p.out
diff -y p.out output.txt
