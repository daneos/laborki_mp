#!/bin/bash
./a.out dane.txt p.out
diff -y p.out output.txt
