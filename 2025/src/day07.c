#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/list.h"

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
void swap(int ** p1, int ** p2) {
	int * tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}
void swap_s(size_t ** p1, size_t ** p2) {
	size_t * tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}
void day07part1(char* filename){
	size_t res = 0;
	list * input = get_input(filename);
	size_t line_len = strlen(input->data[0]) - 1;
	char * line;

	int * line_1 = calloc(line_len, sizeof(int));
	int * line_2 = calloc(line_len, sizeof(int));

	for (size_t c_i = 0; c_i < line_len; c_i++) {
		if (((char *) input->data[0])[c_i] == 'S') {
			line_1[c_i] = 1;
		}
	}

	for (size_t l_i = 1; l_i < input->size; swap(&line_1, &line_2), l_i++) {
		line = input->data[l_i];
		for (size_t c_i = 0; c_i < line_len; c_i++) {
			if (line_1[c_i]) {
				if (line[c_i] == '^') {
					if (c_i > 0) {
						line_2[c_i - 1] = 1;
					}
					if (c_i < line_len - 1) {
						line_2[c_i + 1] = 1;
					}
					res += 1;
				} else {
					line_2[c_i] = 1;
				}
				line_1[c_i] = 0;
			}
		}
	}


	list_free(input);
	printf("result: %ld\n", res);
}
void day07part2(char* filename){
	size_t res = 0;
	size_t last_bit = ((size_t) 1) << 63;
	printf("last_bit: %lu\n", last_bit);
	list * input = get_input(filename);
	size_t line_len = strlen(input->data[0]) - 1;
	char * line;

	size_t * line_1 = calloc(line_len, sizeof(size_t));
	size_t * line_2 = calloc(line_len, sizeof(size_t));

	for (size_t c_i = 0; c_i < line_len; c_i++) {
		if (((char *) input->data[0])[c_i] == 'S') {
			line_1[c_i] = 1;
			res += 1;
		}
	}

	size_t max_res = 1;
	for (size_t l_i = 1; l_i < input->size; swap_s(&line_1, &line_2), l_i++) {
		line = input->data[l_i];
		max_res *= 2;
		for (size_t c_i = 0; c_i < line_len; c_i++) {
			printf("%ld", line_1[c_i]);
			if (line[c_i] == '^') {
				if (c_i > 0) {
					line_2[c_i - 1] += line_1[c_i];
				}
				if (c_i < line_len - 1) {
					line_2[c_i + 1] += line_1[c_i];
				}
				res += line_1[c_i];
			} else {
				line_2[c_i] += line_1[c_i];
				max_res -= line_1[c_i];
			}
			line_1[c_i] = 0;
		}
	}
	size_t other_res = 0;
	for (size_t c_i = 0; c_i < line_len; c_i++) {
		other_res += line_1[c_i];
	}
	printf("other result: %ld\n", other_res);


	list_free(input);
	printf("max result: %lu\n", max_res);
	printf("result: %ld\n", res);
}
