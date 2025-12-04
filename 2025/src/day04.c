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
void day04part1(char* filename){
	size_t res = 0;

	list * input = get_input(filename);
	size_t line_len = 0;
	for (char * c = input->data[0]; *c != '\n'; c++) line_len++;
	int * counts[3]; 
	for (int i = 0; i < 3; i++) {
		counts[i] = calloc(line_len, sizeof(int));
	}
	char * line;
	for (size_t c_i = 0; c_i < line_len; c_i++) {
		line = input->data[0];
		if(line[c_i] != '@') {
			counts[0][c_i] = 4;
			continue;
		}
		if (!(c_i == 0)) {
			counts[0][c_i-1]++;
			counts[1][c_i-1]++;
		}
		counts[1][c_i]++;
		if (!(c_i == line_len - 1)) {
			counts[0][c_i+1]++;
			counts[1][c_i+1]++;
		}
	}
	size_t l_i = 0, prev_i, next_i, cur_i;
	LIST_FOR_EACH(line, input){
		if (l_i == 0 || l_i == input->size - 1) {
			l_i++;
			continue;
		}
		prev_i = (l_i - 1) % 3;
		cur_i = (l_i) % 3;
		next_i = (l_i + 1) % 3;
		line = input->data[l_i];
		for (size_t c_i = 0; c_i < line_len; c_i++) {
			if(line[c_i] != '@') {
				counts[cur_i][c_i] = 4;
				continue;
			}
			if (!(c_i == 0)) {
				counts[prev_i][c_i-1]++;
				counts[cur_i][c_i-1]++;
				counts[next_i][c_i-1]++;
			}
			counts[prev_i][c_i]++;
			counts[next_i][c_i]++;
			if (!(c_i == line_len - 1)) {
				counts[prev_i][c_i+1]++;
				counts[cur_i][c_i+1]++;
				counts[next_i][c_i+1]++;
			}
		}
		for (size_t c_i = 0; c_i < line_len; c_i++) {
			if (counts[prev_i][c_i] < 4) {
				res++;
			}
			counts[prev_i][c_i] = 0;
		}
		l_i++;
	}
	l_i = input->size - 1;
	prev_i = (l_i - 1) % 3;
	cur_i = (l_i) % 3;
	next_i = (l_i + 1) % 3;
	line = input->data[l_i];
	for (size_t c_i = 0; c_i < line_len; c_i++) {
		if(line[c_i] != '@') {
			counts[cur_i][c_i] = 4;
			continue;
		}
		if (!(c_i == 0)) {
			counts[cur_i][c_i-1]++;
			counts[prev_i][c_i-1]++;
		}
		counts[prev_i][c_i]++;
		if (!(c_i == line_len - 1)) {
			counts[cur_i][c_i+1]++;
			counts[prev_i][c_i+1]++;
		}
	}
	for (size_t c_i = 0; c_i < line_len; c_i++) {
		if (counts[prev_i][c_i] < 4) {
			res++;
		}
		counts[prev_i][c_i] = 0;
	}
	for (size_t c_i = 0; c_i < line_len; c_i++) {
		if (counts[cur_i][c_i] < 4) {
			res++;
		}
		counts[prev_i][c_i] = 0;
	}

	for (int i = 0; i < 3; i++) {
		free(counts[i]);
	}
	list_free(input);
	printf("result: %ld\n", res);
}
void day04part2(char* filename){
	size_t res = 0;

	list * input = get_input(filename);
	size_t line_len = 0;
	for (char * c = input->data[0]; *c != '\n'; c++) line_len++;
	int * counts[3]; 
	int done = 0;
	for (int i = 0; i < 3; i++) {
		counts[i] = calloc(line_len, sizeof(int));
	}
	while (!done) {
		done = 1;
		char * line;
		for (size_t c_i = 0; c_i < line_len; c_i++) {
			line = input->data[0];
			if(line[c_i] != '@') {
				counts[0][c_i] = 4;
				continue;
			}
			if (!(c_i == 0)) {
				counts[0][c_i-1]++;
				counts[1][c_i-1]++;
			}
			counts[1][c_i]++;
			if (!(c_i == line_len - 1)) {
				counts[0][c_i+1]++;
				counts[1][c_i+1]++;
			}
		}
		size_t l_i = 0, prev_i, next_i, cur_i;
		LIST_FOR_EACH(line, input){
			if (l_i == 0 || l_i == input->size - 1) {
				l_i++;
				continue;
			}
			prev_i = (l_i - 1) % 3;
			cur_i = (l_i) % 3;
			next_i = (l_i + 1) % 3;
			line = input->data[l_i];
			for (size_t c_i = 0; c_i < line_len; c_i++) {
				if(line[c_i] != '@') {
					counts[cur_i][c_i] = 4;
					continue;
				}
				if (!(c_i == 0)) {
					counts[prev_i][c_i-1]++;
					counts[cur_i][c_i-1]++;
					counts[next_i][c_i-1]++;
				}
				counts[prev_i][c_i]++;
				counts[next_i][c_i]++;
				if (!(c_i == line_len - 1)) {
					counts[prev_i][c_i+1]++;
					counts[cur_i][c_i+1]++;
					counts[next_i][c_i+1]++;
				}
			}
			for (size_t c_i = 0; c_i < line_len; c_i++) {
				if (counts[prev_i][c_i] < 4) {
					((char *) input->data[l_i-1])[c_i] = '.';
					res++;
					done = 0;
				}
				counts[prev_i][c_i] = 0;
			}
			l_i++;
		}
		l_i = input->size - 1;
		prev_i = (l_i - 1) % 3;
		cur_i = (l_i) % 3;
		next_i = (l_i + 1) % 3;
		line = input->data[l_i];
		for (size_t c_i = 0; c_i < line_len; c_i++) {
			if(line[c_i] != '@') {
				counts[cur_i][c_i] = 4;
				continue;
			}
			if (!(c_i == 0)) {
				counts[cur_i][c_i-1]++;
				counts[prev_i][c_i-1]++;
			}
			counts[prev_i][c_i]++;
			if (!(c_i == line_len - 1)) {
				counts[cur_i][c_i+1]++;
				counts[prev_i][c_i+1]++;
			}
		}
		for (size_t c_i = 0; c_i < line_len; c_i++) {
			if (counts[prev_i][c_i] < 4) {
				((char *) input->data[l_i-1])[c_i] = '.';
				done = 0;
				res++;
			}
			counts[prev_i][c_i] = 0;
		}
		for (size_t c_i = 0; c_i < line_len; c_i++) {
			if (counts[cur_i][c_i] < 4) {
				((char *) input->data[l_i])[c_i] = '.';
				done = 0;
				res++;
			}
			counts[cur_i][c_i] = 0;
		}
	}
	for (int i = 0; i < 3; i++) {
		free(counts[i]);
	}
	list_free(input);
	printf("result: %ld\n", res);
}
