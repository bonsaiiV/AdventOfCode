#include <stdio.h>
#include <stdlib.h>
#include "globals.h"


typedef struct _op{
	long (*fun) (long, long);
	long arg;
} op;
static long mul(long a, long b) {
	return a*b;
}
static long divide(long a, long b) {
	return a/b;
}
static op sequence[3] = {{mul, 64}, {divide, 32}, {mul, 2048}};
LIST(long)

static void get_input(FILE* fp, long_list* list) {
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	int chars_read;
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		long_list_insert(list, atol(line));
	}
	free(line);
}
void day22part1(char* filename){
	long_list input = long_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	get_input(fp, &input);
	fclose(fp);
	DBG("starting solve input\n");
	size_t i, j, k;
	long current, tmp;
	long res = 0;
	for (i = 0; i < input.len; i++) {
		current = input.data[i];
		for (j = 0; j < 2000; j++) {
			for (k = 0; k < 3; k++) {
				tmp = sequence[k].fun(current, sequence[k].arg);
				tmp = current ^ tmp;
				current = tmp % 16777216;
			}
		}
		res += current;
	}
	printf("result: %ld\n", res);
	free(input.data);
}
static int get_index(size_t index, int delta[4]) {
	int ret = index;
	for (int i = 0; i < 4; i++) {
		ret *= 20;
		ret += (9 + delta[i]);
	}
	return ret;
}
void day22part2(char* filename){
	long_list input = long_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	get_input(fp, &input);
	fclose(fp);
	DBG("starting solve input\n");
	size_t i, j, k;
	long current, tmp, last;
	long delta;
	int* scores = calloc(input.len * 20 * 20 * 20 * 20, sizeof(int));
	int delta_hist[4];
	for (i = 0; i < input.len; i++) {
		current = input.data[i];
		last = current % 10;
		for (j = 0; j < 2000; j++) {
			for (k = 0; k < 3; k++) {
				tmp = sequence[k].fun(current, sequence[k].arg);
				tmp = current ^ tmp;
				current = tmp % 16777216;
			}
			tmp = current % 10;
			delta = tmp - last;
			if (j < 4){
				delta_hist[j] = delta;
			} else {
				delta_hist[0] = delta_hist[1];
				delta_hist[1] = delta_hist[2];
				delta_hist[2] = delta_hist[3];
				delta_hist[3] = delta;
			}
			if (j >= 3 && !scores[get_index(i, delta_hist)]) {
				scores[get_index(i, delta_hist)] = tmp;
			}
			last = tmp;
		}
	}
	DBG("summing\n");
	long res = 0;
	int a, b, c, d;
	int sum;
	for (a = -9; a < 10; a++) {
		delta_hist[0] = a;
		for (b = -9; b < 10; b++) {
			delta_hist[1] = b;
			for (c = -9; c < 10; c++) {
				delta_hist[2] = c;
				for (d = -9; d < 10; d++) {
					delta_hist[3] = d;
					sum = 0;
					for (i = 0; i < input.len; i++) {
						sum += scores[get_index(i, delta_hist)];
						if (sum)
						DBG("%ld: score: %d\n", i, scores[get_index(i, delta_hist)]);
					}
					if (sum > res){
						res = sum;
						DBG("%d is new best with %d, %d, %d, %d\n", sum, a, b, c, d);
					}
				}
			}
		}
	}
	free(scores);
	free(input.data);
	printf("result: %ld\n", res);
}
