#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

int get_input(char* filename, int** ret_a, int** ret_b){
	size_t len = 0, cap = 8;
	int* new_a, *a = calloc(cap, sizeof(int));
	int* new_b, *b = calloc(cap, sizeof(int));
	FILE* fp = fopen(filename, "r");
	while (fscanf(fp, "%d\t%d\n ", a + len, b + len) == 2) {
		if (++len == cap){
			cap *= 2;
			new_a = calloc(cap, sizeof(int));
			new_b = calloc(cap, sizeof(int));
			for (size_t i = 0; i < len; i++) {
				new_a[i] = a[i];
				new_b[i] = b[i];
			}
			free(a);
			free(b);
			a = new_a;
			b = new_b;
		}
	}
	fclose(fp);
	*ret_a = a;
	*ret_b = b;
	return len;
}
int comp(const void* a, const void* b){
	return (*(int *) a - *(int*) b);
}

void day01part1(char* input){
	int* a, *b;
	size_t len = get_input(input, &a, &b);
	qsort(a, len, sizeof(int), comp);
	qsort(b, len, sizeof(int), comp);
	int tmp, res = 0;
	for (size_t i = 0; i < len; i++) {
		tmp = a[i] - b[i];
		if (tmp < 0) res -= tmp;
		else res += tmp;
	}
	printf("%d\n", res);
	free(a);
	free(b);
}
void day01part2(char* input){
	int* a, *b;
	size_t len = get_input(input, &a, &b);
	int count;
	int res = 0;
	for (size_t i = 0; i < len; i++) {
		count = 0;
		for (size_t j = 0; j < len; j++) {
			if (a[i] == b[j]) count++;
		}
		res += count*a[i];
	}
	printf("%d\n", res);
}
