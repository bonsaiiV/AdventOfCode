#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


typedef char* string;
LIST(string)
LIST_CLEAN(string)
LIST(string_list)
typedef int* int_arr;
LIST(int_arr)

static void get_input(FILE* fp, string_list_list* keys, string_list_list* locks, int* line_len) {
	if (line_len) *line_len = 0;
	size_t last_line_len = 5;
	char* line = calloc(last_line_len, sizeof(char));
	int chars_read;
	string_list next_input = string_list_create();
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		DBG("%s", line);
		if (!strcmp(line, "\n")){
			if (next_input.data[0][0] == '.') {
				string_list_list_insert(keys, next_input);
			} else {
				string_list_list_insert(locks, next_input);
			}
			next_input = string_list_create();
			continue;
		}
		if (line_len){
			if (*line_len){
				if (*line_len != chars_read) {
					printf("malformed input\n");
				}
			} else {
				*line_len = chars_read;
			}
		}
		string_list_insert(&next_input, line);
		line = calloc(last_line_len, sizeof(char));
	}
	if (next_input.data[0][0] == '.') {
		string_list_list_insert(keys, next_input);
	} else {
		string_list_list_insert(locks, next_input);
	}
	DBG("keys: %ld, locks: %ld\n", keys->len, locks->len);
	*line_len -= 1;
}
void day25part1(char* filename){
	string_list_list keys_str = string_list_list_create();
	string_list_list locks_str = string_list_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	int line_len;
	get_input(fp, &keys_str, &locks_str, &line_len);
	fclose(fp);

	size_t i, j;
	int k;
	int* heights;
	string_list current;

	DBG("starting with locks\n");
	int_arr_list locks = int_arr_list_create();
	for (i = 0; i < locks_str.len; i++) {
		heights = calloc(line_len, sizeof(int));
		current = locks_str.data[i];
		for (k = 0; k < line_len; k++) {
			for (j = 0; j < current.len; j++){
				if (current.data[j][k] == '#') {
					heights[k] = j;
				}
			}
		}
		int_arr_list_insert(&locks, heights);
		string_list_clean(&current);
		free(current.data);
	}
	free(locks_str.data);
	DBG("starting with keys\n");
	int_arr_list keys = int_arr_list_create();
	for (i = 0; i < keys_str.len; i++) {
		heights = calloc(line_len, sizeof(int));
		current = keys_str.data[i];
		for (k = 0; k < line_len; k++) {
			for (j = 0; j < current.len; j++){
				if (current.data[j][k] == '.') {
					heights[k] = j;
				}
			}
		}
		int_arr_list_insert(&keys, heights);
		string_list_clean(&current);
		free(current.data);
	}
	free(keys_str.data);
	int res = 0;
	int equal;
	DBG("testing combinations\n");
	for (i = 0; i < keys.len; i++) {
		for (k = 0; k < line_len; k++) {
			DBG("%d, ", keys.data[i][k]);
		}
		DBG("\n");

		for (j = 0; j < locks.len; j++) {
			DBG("\t");
			for (k = 0; k < line_len; k++) {
				DBG("%d, ", locks.data[j][k]);
			}
			DBG("\n");
			equal = 1;
			for (k = 0; k < line_len; k++) {
				if (locks.data[j][k] - keys.data[i][k] > 0){
					equal = 0;
					break;
				}
			}
			if (equal) {
				res++;
			}
		}
	}
	free(keys.data);
	free(locks.data);

	printf("result: %d\n", res);
}
void day25part2(char* filename){
	(void) filename;
}
