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
		list->data = reallocarray(list->data, list->cap, sizeof(char**));
	}
	list->data[list->len++] = elem;
}
static void list_destroy(struct list* list) {
	for (size_t i = 0; i < list->len; i++) {
		free(list->data[i]);
	}
	free(list->data);
}
const char* XMAS = "XMAS";
static int try_rel(struct list lines, size_t len, const char* str, size_t row, size_t col, int diff_row, int diff_col){
	int fw = 1, rev = 1;
	for (size_t i = 0; i < len; i++) {
		if (lines.data[row][col] != str[i]) fw = 0;
		if (lines.data[row][col] != str[len-1-i]) rev = 0;
		row += diff_row;
		col += diff_col;
	}
	return fw + rev;
}
static int try_x(struct list lines, size_t row, size_t col) {
	return try_rel(lines, 3, XMAS+1, row, col, 1, 1) && try_rel(lines, 3, XMAS+1, row+2, col, -1, 1);
}
static int try_down(struct list lines, size_t row, size_t col) {
	return try_rel(lines, 4, XMAS, row, col, 1, 0) && ((dbg && printf("found down at %ld, %ld\n", row, col)) || 1);
}
static int try_hor(struct list lines, size_t row, size_t col) {
	return try_rel(lines, 4, XMAS, row, col, 0, 1) && ((dbg && printf("found horizontal at %ld, %ld\n", row, col)) || 1);
}
static int try_diag_down(struct list lines, size_t row, size_t col) {
	return try_rel(lines, 4, XMAS, row, col, 1, 1) && ((dbg && printf("found diagonal down at %ld, %ld\n", row, col)) || 1);
}
static int try_diag_up(struct list lines, size_t row, size_t col) {
	return try_rel(lines, 4, XMAS, row+3, col, -1, 1) && ((dbg && printf("found diagonal up at %ld, %ld\n", row, col)) || 1);
}

static struct list get_input(char* filename) {
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	FILE* fp = fopen(filename, "r");
	struct list lines = list_create();
	while (getline(&line, &last_line_len, fp) != -1) {
		if (lines.line_len) {
			if (lines.line_len != last_line_len) {
				printf("malformed input: all lines should have the same length");
				exit(1);
			}
		} else {
			lines.line_len = last_line_len;
		}
		list_insert(&lines, line);
		line = calloc(last_line_len, sizeof(char));
	}
	free(line);
	return lines;
}
void day04part1(char* filename){
	int res = 0;
	size_t i, j;
	struct list lines = get_input(filename);
	dbg = 0;
	for (i = 0; i < lines.len; i++){
		for (j = 0; j < lines.line_len; j++){
			res += (i < lines.len-3 && try_down(lines, i, j));
			res += (j < lines.line_len && try_hor(lines, i, j));
			res += (j < lines.line_len-3 && i < lines.len-3 && try_diag_down(lines, i, j));
			res += (j < lines.line_len-3 && i < lines.len-3 && try_diag_up(lines, i, j));
		}
	}
	printf("result: %d\n", res);
	list_destroy(&lines);
}
void day04part2(char* filename){
	int res = 0;
	size_t i, j;
	struct list lines = get_input(filename);
	dbg = 0;
	for (i = 0; i < lines.len-2; i++){
		for (j = 0; j < lines.line_len-2; j++){
			if (try_x(lines, i, j)){
				res++;
				if (dbg) printf("found x at %ld, %ld\n", i, j);
			}
		}
	}
	printf("result: %d\n", res);
	list_destroy(&lines);
}
