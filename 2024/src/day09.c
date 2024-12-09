#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

struct list {
	int* space;
	int* index;
	size_t cap;
	size_t len;
};
static struct list list_create(void) {
	int* space = calloc(8, sizeof(int));
	int* index = calloc(8, sizeof(int));
	struct list ret = {space, index, 8, 0};
	return ret;
}
static void list_insert(struct list* list, int free_space, int free_index) {
	if (list->cap == list->len) {
		list->cap *= 2;
		list->space = reallocarray(list->space, list->cap, sizeof(int));
		list->index = reallocarray(list->index, list->cap, sizeof(int));
	}
	list->space[list->len] = free_space;
	list->index[list->len] = free_index;
	list->len++;
}
void day09part1(char* filename){
	FILE* fp = fopen(filename, "r");
	long long index_free = 1, index_file = 0;
	int pos_in_file = 0, pos_in_free_space = 0;
	long long res = 0;
	DBG("getting input\n");
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	index_file = getline(&line, &last_line_len, fp) - 2;
	fclose(fp);
	if (index_file == -1) {
		printf("error getting input\n");
		exit(1);
	}
	long long pos_of_free = line[0]-'0';
	DBG("starting at: %lld\n",pos_of_free);
	while (index_free < index_file){
		if (pos_in_file >= line[index_file]-'0'){
			index_file -= 2;
			pos_in_file = 0;
			continue;
		}
		DBG("at pos %lld is file with id %lld\n",pos_of_free, index_file / 2);
		res += (index_file / 2) * pos_of_free;
		pos_in_file++;
		pos_of_free++;
		pos_in_free_space++;
		if (pos_in_free_space >= line[index_free]-'0') {
			if (index_free + 2 > index_file) {
				while (pos_in_file < line[index_file]-'0') {
					DBG("at pos %lld is file with id %lld\n",pos_of_free, index_file / 2);
					res += pos_of_free * (index_file / 2);
					pos_of_free++;
					pos_in_file++;
				}
				break;
			} else {
				do {
				index_free++;
				pos_in_free_space = 0;
				while (pos_in_free_space < line[index_free]-'0') {
					pos_in_free_space++;
					DBG("at pos %lld is file with id %lld\n",pos_of_free, index_free / 2);
					res += (index_free / 2) * pos_of_free;
					pos_of_free++;
				}
				index_free++;
				pos_in_free_space = 0;
				} while (!(line[index_free]-'0'));
			}
		}
	}
	free(line);
	printf("result: %lld\n", res);
}
void day09part2(char* filename){
	size_t i;
	int j;
	int index_file, index_free = 1;
	long long pos_in_fs;
	struct list free_spaces = list_create();
	FILE* fp = fopen(filename, "r");
	long long res = 0;

	DBG("getting input\n");
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	index_file = getline(&line, &last_line_len, fp) - 2;
	fclose(fp);
	if (index_file == -1) {
		printf("error getting input\n");
		exit(1);
	}

	pos_in_fs = line[0] - '0';
	DBG("index_file: %d\n", index_file);
	while (index_free < index_file) {
		DBG("inserting: %d, %lld\n",line[index_free]-'0', pos_in_fs);
		list_insert(&free_spaces, line[index_free]-'0', pos_in_fs);
		pos_in_fs += line[index_free]-'0';
		index_free++;
		pos_in_fs += line[index_free]-'0';
		index_free++;
	}
	int found;
	while (index_file > 0) {
		found = 0;
		int file_id = index_file / 2;
		int original_pos = (free_spaces.index[file_id-1] + free_spaces.space[file_id-1]);
		DBG("original_pos of file %d is: %d\n", file_id, original_pos);
		for (i = 0; i < free_spaces.len; i++) {
			if (free_spaces.index[i] > original_pos) break;
			if (free_spaces.space[i] >= line[index_file] - '0') {
				DBG("placing file with id %d at %d\n", file_id, free_spaces.index[i]);
				free_spaces.space[i] -= line[index_file] - '0';
				for (j = 0; j < line[index_file] - '0'; j++) {
					res += (file_id) * (free_spaces.index[i] + j);
				}
				free_spaces.index[i] += line[index_file] - '0';
				found = 1;
				break;
			}
		}
		if (!found) {
			for (j = 0; j < line[index_file] - '0'; j++) {
				res += (file_id) * (original_pos + j);
			}
		}
		index_file -= 2;
	}
	printf("result: %lld\n", res);
	free(line);
	free(free_spaces.space);
	free(free_spaces.index);
}
