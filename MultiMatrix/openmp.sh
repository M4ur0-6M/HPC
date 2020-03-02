#! /bin/bash

gcc -o openmp matrix-mul-openmp.c -fopenmp
for j in {1..10}; do
for i in 10 50 100 200 300 400 500 1000
do
./openmp $i $i>> openmp.txt
done;
done;