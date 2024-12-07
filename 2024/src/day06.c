#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

struct list {
	char** data;
	size_t cap;
	size_t len;
	size_t line_len;
};
static struct list list_create(void) {
	char** data = calloc(8, sizeof(char*));
	struct list ret = {data, 8, 0, 0};
	return ret;
}
static void list_insert(struct list* list, char* elem) {
	if (list->cap == list->len) {
		list->cap *= 2;
		list->data = reallocarray(list->data, list->cap, sizeof(char*));
	}
	list->data[list->len++] = elem;
}
static void list_clean(struct list* list) {
	for (size_t i = 0; i < list->len; i++) {
		free(list->data[i]);
	}
	list->len = 0;
}

static void get_input(FILE* fp, struct list* lines, size_t* guard_row, size_t* guard_col) {
	size_t last_line_len = 1;
	size_t i, line_nr = 0;
	char* line = calloc(last_line_len, sizeof(char));
	while (getline(&line, &last_line_len, fp) != -1) {
		if (lines->line_len) {
			if (lines->line_len != last_line_len){
				printf("malformed input\n");
				exit(1);
			}
		} else {
			lines->line_len = last_line_len;
		}
		for (i = 0; i < lines->line_len; i++) {
			if (line[i] == '^') {
				*guard_col = i;
				*guard_row = line_nr;
			}
		}
		list_insert(lines, line);
		line = calloc(last_line_len, sizeof(char));
		line_nr++;
	}
	lines->line_len -= 2;
	free(line);
}
void get_delta(int dir, int* d_row, int* d_col) {
	switch (dir) {
		case 0:
			*d_row = -1;
			*d_col = 0;
		break;
		case 1:
			*d_row = 0;
			*d_col = 1;
		break;
		case 2:
			*d_row = 1;
			*d_col = 0;
		break;
		case 3:
			*d_row = 0;
			*d_col = -1;
		break;
	}
}
void day06part1(char* filename){
	int dir = 0, done = 0;
	size_t guard_row, guard_col;
	int d_row, d_col;
	struct list lines = list_create();
	FILE* fp = fopen(filename, "r");
	int res = 0;
	get_input(fp, &lines, &guard_row, &guard_col);
	get_delta(dir, &d_row, &d_col);
	lines.data[guard_row][guard_col] = '1';
	while (!done) {
		DBG("guard pos: %ld, %ld\n", guard_col, guard_row);
		if (guard_row == 0 || guard_row == lines.len-1 || guard_col == 0 || guard_col == lines.line_len) {
			done = 1;
			break;
		} else if (lines.data[guard_row+d_row][guard_col + d_col] == '#') {
			dir = (dir+1) % 4;
			get_delta(dir, &d_row, &d_col);
		} else {
			guard_row += d_row;
			guard_col += d_col;
			lines.data[guard_row][guard_col] = '1';
		}
	}
	res = 0;
	for (size_t i = 0; i < lines.len; i++) {
		DBG("%s", lines.data[i]);
		for (size_t j = 0; j < lines.line_len; j++) {
			if (lines.data[i][j] == '1') res++;
		}
	}
	fclose(fp);
	printf("result: %d\n", res);
	list_clean(&lines);
	free(lines.data);
}
int test_loop(size_t guard_row, size_t guard_col, int dir, int* visited, struct list lines) {
	if (visited[guard_row * lines.line_len + guard_col] & (1 << dir)) {
		return 1;
	}
	return 0;
}
void day06part2(char* filename){
	int dir = 0, done = 0;
	size_t guard_row, guard_col;
	int d_row, d_col;
	struct list lines = list_create();
	FILE* fp = fopen(filename, "r");
	int res = 0;
	char tmp;
	get_input(fp, &lines, &guard_row, &guard_col);
	size_t const_row = guard_row, const_col = guard_col;
	get_delta(dir, &d_row, &d_col);
	while (!done) {
		DBG("guard pos: %ld, %ld\n", guard_col, guard_row);
		if (guard_row == 0 || guard_row == lines.len-1 || guard_col == 0 || guard_col == lines.line_len) {
			done = 1;
			break;
		} else if (lines.data[guard_row+d_row][guard_col + d_col] == '#') {
			dir = (dir+1) % 4;
			get_delta(dir, &d_row, &d_col);
		} else {
			guard_row += d_row;
			guard_col += d_col;
			lines.data[guard_row][guard_col] = '1';
		}
	}
	for (size_t i = 0; i < lines.len; i++) {
		for (size_t j = 0; j < lines.line_len; j++) {
			if (lines.data[i][j] != '1' || (i == const_row && j == const_col)) continue;
			guard_row = const_row;
			guard_col = const_col;
			dir = 0;
			get_delta(dir, &d_row, &d_col);
			done = 0;
			tmp = lines.data[i][j];
			lines.data[i][j] = '#';
			int* visited = calloc(lines.line_len * lines.len, sizeof(int));
			visited[guard_row * lines.line_len + guard_col] = 1;
			while (!done) {
				DBG("guard pos: %ld, %ld\n", guard_col, guard_row);
				DBG("guard dir: %d, %d\n", d_col, d_row);
				if (guard_row == 0 || guard_row == lines.len-1 || guard_col == 0 || guard_col == lines.line_len) {
					done = 1;
					break;
				} else if (lines.data[guard_row+d_row][guard_col + d_col] == '#') {
					dir = (dir+1) % 4;
					get_delta(dir, &d_row, &d_col);
				} else {
					guard_row += d_row;
					guard_col += d_col;
					if (test_loop(guard_row, guard_col, dir, visited, lines)){
						res += 1;
						DBG("loop by placing object at %ld, %ld\n", i, j);
						break;
					}
					visited[guard_row * lines.line_len + guard_col] += 1 << dir;
				}
			}
			free(visited);
			lines.data[i][j] = tmp;
			DBG("no loop when placing object at %ld, %ld\n", i, j);
		}
	}
	fclose(fp);
	printf("result: %d\n", res);
	list_clean(&lines);
	free(lines.data);
}
