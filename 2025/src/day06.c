#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data/list.h"

static list * get_input(char * filename) {
	FILE * fp = fopen(filename, "r");
	list * ret = create_list();
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	int chars_read;
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		list_push(ret, line);
		line = calloc(last_line_len, sizeof(char));
	}
	free(line);
	return ret;
}
void day06part1(char* filename){
	size_t res = 0, column_res = 0;
	list * input = get_input(filename);
	char ** sv_ptrs = calloc(input->size, sizeof(char *));
	char * op, * a;
	op = strtok_r(input->data[input->size-1], " ", &sv_ptrs[input->size-1]);
	if (*op == '+') {
		column_res = 0;
		for (size_t i = 0; i < input->size - 1; i++) {
			a = strtok_r(input->data[i], " ", &sv_ptrs[i]);
			column_res += atoi(a);
		}
	} else {
		column_res = 1;
		for (size_t i = 0; i < input->size - 1; i++) {
			a = strtok_r(input->data[i], " ", &sv_ptrs[i]);
			column_res *= atoi(a);
		}
	}
	res += column_res;
	while ((op = strtok_r(NULL, " ", &sv_ptrs[input->size-1]))) {
		if (*op == '\n') break;
		if (*op == '+') {
			column_res = 0;
			for (size_t i = 0; i < input->size - 1; i++) {
				a = strtok_r(NULL, " ", &sv_ptrs[i]);
				column_res += atoi(a);
			}
		} else {
			column_res = 1;
			for (size_t i = 0; i < input->size - 1; i++) {
				a = strtok_r(NULL, " ", &sv_ptrs[i]);
				column_res *= atoi(a);
			}
		}
		res += column_res;
	}

	list_free(input);
	free(sv_ptrs);
	printf("result: %ld\n", res);
}
void day06part2(char* filename){
	size_t res = 0, column_res;
	size_t nums[5], num_i;
	char op;
	list * input = get_input(filename);
	ssize_t cur_i = strlen(input->data[0]) - 2;
	while(cur_i > 0) {
		num_i = 0;
		do {
			nums[num_i] = 0;
			for (size_t i = 0; i < input->size - 1; i++) {
				char a = ((char *) input->data[i])[cur_i];
				if (a == ' ') continue;
				nums[num_i] *= 10;
				nums[num_i] += (a - '0');
			}
			num_i++;
		} while((op = ((char *) input->data[input->size-1])[cur_i--]) == ' ');

		cur_i--;
		if (op == '+') {
			column_res = 0;
			for (size_t i = 0; i < num_i; i++) {
				column_res += nums[i];
			}
		} else {
			column_res = 1;
			for (size_t i = 0; i < num_i; i++) {
				column_res *= nums[i];
			}
		}
		res += column_res;
	}
	list_free(input);
	printf("result: %ld\n", res);
}
