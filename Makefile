all: radix-sorting

radix-sorting: radix-sorting.c
	gcc -o radix-sorting -lm radix-sorting.c

