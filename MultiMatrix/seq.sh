#! /bin/bash
gcc -o seq matrix-mul-seq.c
for j in {1..10}; do
for i in 10 50 100 200 300 400 500 1000
do
./seq $i >> seq.txt
done;
done;