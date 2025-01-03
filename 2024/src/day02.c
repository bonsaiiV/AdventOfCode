#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


void day02part1(char* filename){
	int res = 0;
	char* tok, *line, *buf = calloc(80, sizeof(char));
	size_t line_len = 80;
	int last_num, next_num, is_ascending, failed;
	FILE* fp = fopen(filename, "r");
	while (getline(&buf, &line_len, fp) != -1) {
		failed = 0;
		line = buf;
		tok = strtok(line, " ");
		last_num = atoi(tok);
		tok = strtok(0, " ");
		next_num = atoi(tok);
		is_ascending = 1;
		if (next_num < last_num) is_ascending = 0;
		if (next_num == last_num) continue;
		//printf("last num: %d, next_num: %d\n", last_num, next_num);
		if (abs(next_num - last_num) > 3) {
			continue;
		}
		while ((tok = strtok(0, " "))){
			last_num = next_num;
			next_num = atoi(tok);
			//printf("last num: %d, next_num: %d\n", last_num, next_num);
			int diff = next_num - last_num;
			if (!is_ascending) {
				diff = -diff;
			}
			if (diff < 1 || diff > 3) {
				printf("failed\n");
				failed = 1;
				break;
			}
		}
		//printf("finished\n");
		if (!failed) res++;
	}
	fclose(fp);
	printf("%d\n", res);
	free(buf);
}
int try_skip(int* a, size_t len, size_t index, int is_ascending) {
	//printf("try_skip %d, %d\n", (int)index, is_ascending);
	int last_num, next_num;
	next_num = a[0];
	size_t i = 1;
	if (index == 0){
		next_num = a[1];
		i = 2;
	}
	for (; i < len; i++){
		if (i == index) continue;
		last_num = next_num;
		next_num = a[i];
		//printf("last num: %d, next_num: %d\n", last_num, next_num);
		int diff = next_num - last_num;
		diff = diff*is_ascending;
		if (diff < 1 || diff > 3) {
			return 0;
		}
	}
	//printf("successfully skiped index %d\n", (int)index);
	return 1;
}
int get_input_line(FILE** fp, int** ret_a){
	size_t len = 0, cap = 8;
	int* new_a, *a = calloc(cap, sizeof(int));
	char* tok, *buf = calloc(80, sizeof(char));
	size_t line_len = 80;
	if (getline(&buf, &line_len, *fp) == -1)
		return -1;

	a[0] = atoi(strtok(buf, " "));
	len = 1;
	while ((tok = strtok(0, " "))){
		a[len] = atoi(tok);
		if (++len == cap){
			cap *= 2;
			new_a = calloc(cap, sizeof(int));
			for (size_t i = 0; i < len; i++) {
				new_a[i] = a[i];
			}
			free(a);
			a = new_a;
		}
	}
	*ret_a = a;
	return len;
}
void print_arr(int* a, size_t len) {
	for (size_t i = 0; i < len; i++) {
		printf("%d, ", a[i]);
	}
	puts("");
}
void day02part2(char* filename){
	int res = 0;
	int len;
	int success;
	FILE* fp = fopen(filename, "r");
	int* a;
	while ((len = get_input_line(&fp, &a)) != -1) {
		success = 0;
		for (int i = 0; i <= len && !success; i++) {
			success += try_skip(a, len, i, -1);
			success += try_skip(a, len, i, 1);
		}
		if (success) res++;
		free(a);
		//printf("res: %d\n", res);
	}
	fclose(fp);
	printf("%d\n", res);
}
