#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static void get_input(FILE* fp, struct list* list) {
	size_t  mem_size = 8;
	int last_line_len;
	char* line = calloc(mem_size, sizeof(char));
	while ((last_line_len = getline(&line, &mem_size, fp)) != -1) {
		if (list->line_len) {
			if ((size_t) last_line_len != list->line_len) {
				printf("malformed input\n");
				exit(1);
			}
		} else {
			list->line_len = (size_t) last_line_len;
		}
		list_insert(list, line);
		line = calloc(last_line_len, sizeof(char));
	}
	list->line_len -= 1;
	free(line);
}
void day08part1(char* filename){
	size_t i, j, k, l;
	struct list lines = list_create();
	FILE* fp = fopen(filename, "r");
	int res = 0;
	int dx, dy, ax, ay;
	DBG("getting input\n");
	get_input(fp, &lines);
	DBG("line len: %ld\n", lines.line_len);
	int* antinodes = calloc(lines.len * lines.line_len, sizeof(int));
	for (i = 0; i < lines.len; i++) {
		for (j = 0; j < lines.line_len; j++) {
			if (lines.data[i][j] == '.') continue;
			DBG("antenna \"%c\" at %ld, %ld found\n", lines.data[i][j], i, j);
			for (k = 0; k < lines.len; k++) {
				for (l = 0; l < lines.line_len; l++) {
					if (lines.data[i][j] != lines.data[k][l]) continue;
					if (k == i && l==j) continue;
					DBG("matching antenna at %ld, %ld found\n",  k, l);
					dy = k-i;
					dx = l-j;
					ay = k + dy;
					ax = l + dx;
					if (ax < 0 || ay < 0 || ax >= (int) lines.line_len || ay >= (int) lines.len) continue;
					DBG("success\n");
					antinodes[ay * lines.line_len + ax] = 1;
				}
			}
		}
	}
	for (i = 0; i < lines.len; i++) {
		for (j = 0; j < lines.line_len; j++) {
			DBG("%c", lines.data[i][j]);
			if (antinodes[i * lines.line_len + j]) res++;
		}
		DBG("\n");
	}
	fclose(fp);
	printf("result: %d\n", res);
	list_clean(&lines);
	free(lines.data);
}
void day08part2(char* filename){
	size_t i, j, k, l;
	struct list lines = list_create();
	FILE* fp = fopen(filename, "r");
	int res = 0;
	int dx, dy, ax, ay;
	DBG("getting input\n");
	get_input(fp, &lines);
	DBG("line len: %ld\n", lines.line_len);
	int* antinodes = calloc(lines.len * lines.line_len, sizeof(int));
	for (i = 0; i < lines.len; i++) {
		for (j = 0; j < lines.line_len; j++) {
			if (lines.data[i][j] == '.') continue;
			DBG("antenna \"%c\" at %ld, %ld found\n", lines.data[i][j], i, j);
			for (k = 0; k < lines.len; k++) {
				for (l = 0; l < lines.line_len; l++) {
					if (lines.data[i][j] != lines.data[k][l]) continue;
					if (k == i && l==j) continue;
					DBG("matching antenna at %ld, %ld found\n",  k, l);
					dy = k-i;
					dx = l-j;
					ay = k;
					ax = l;
					while (! (ax < 0 || ay < 0 || ax >= (int) lines.line_len || ay >= (int) lines.len)) {
						DBG("success at %d, %d\n", ay, ax);
						antinodes[ay * lines.line_len + ax] = 1;
						ax += dx;
						ay += dy;
					}
				}
			}
		}
	}
	for (i = 0; i < lines.len; i++) {
		for (j = 0; j < lines.line_len; j++) {
			DBG("%c", lines.data[i][j]);
			if (antinodes[i * lines.line_len + j]) res++;
		}
		DBG("\n");
	}
	fclose(fp);
	printf("result: %d\n", res);
	list_clean(&lines);
	free(lines.data);
}
