#! /bin/bash
gcc -o pthread matrix-mul-pthread.c -l pthread
for j in {1..10}; do
for i in 10 50 100 200 300 400 500 1000
do
./pthread $i $i>> pthread.txt
done;
done;