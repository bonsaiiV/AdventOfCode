#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct charlist {
	char* data;
	size_t cap;
	size_t len;
};
struct charlist charlist_create(size_t cap, char* data) {
	struct charlist ret = {data, cap, cap};
	return ret;
}
void charlist_insert(struct charlist* list, char elem) {
	if (list->cap == list->len) {
		list->cap *= 2;
		list->data = reallocarray(list->data, list->cap, sizeof(char));
	}
	list->data[list->len++] = elem;
}
struct list {
	struct charlist* data;
	size_t cap;
	size_t len;
};
struct list list_create(void) {
	struct charlist* data = calloc(8, sizeof(struct charlist));
	struct list ret = {data, 8, 0};
	return ret;
}
void list_insert(struct list* list, struct charlist elem) {
	if (list->cap == list->len) {
		list->cap *= 2;
		list->data = reallocarray(list->data, list->cap, sizeof(struct charlist));
	}
	list->data[list->len++] = elem;
}
void list_destroy(struct list* list) {
	for (size_t i = 0; i < list->len; i++) {
		free(list->data[i].data);
	}
	free(list->data);
}
const char* XMAS = "XMAS";
int try_x(struct list lines, size_t row, size_t col) {
	const char* MAS = XMAS+1;
	int fw = 1, rev = 1;
	for (size_t i = 0; i < 3; i++) {
		if (lines.data[row + i].data[col+i] != MAS[i]) fw = 0;
		if (lines.data[row + i].data[col+i] != MAS[2-i]) rev = 0;
	}
	if (!(fw + rev)) return 0;
	fw = 1, rev = 1;
	for (size_t i = 0; i < 3; i++) {
		if (lines.data[row + 2 - i].data[col+i] != MAS[i]) fw = 0;
		if (lines.data[row + 2 - i].data[col+i] != MAS[2-i]) rev = 0;
	}
	return fw + rev;
}
int try_down(struct list lines, size_t row, size_t col) {
	int fw = 1, rev = 1;
	for (size_t i = 0; i < 4; i++) {
		if (lines.data[row + i].data[col] != XMAS[i]) fw = 0;
		if (lines.data[row + i].data[col] != XMAS[3-i]) rev = 0;
	}
	return fw + rev;
}
int try_hor(struct list lines, size_t row, size_t col) {
	int fw = 1, rev = 1;
	for (size_t i = 0; i < 4; i++) {
		if (lines.data[row].data[col+i] != XMAS[i]) fw = 0;
		if (lines.data[row].data[col+i] != XMAS[3-i]) rev = 0;
	}
	return fw + rev;
}
int try_diag(struct list lines, size_t row, size_t col) {
	int fw = 1, rev = 1;
	for (size_t i = 0; i < 4; i++) {
		if (lines.data[row + i].data[col+i] != XMAS[i]) fw = 0;
		if (lines.data[row + i].data[col+i] != XMAS[3-i]) rev = 0;
	}
	return fw + rev;
}
int try_diag_up(struct list lines, size_t row, size_t col) {
	int fw = 1, rev = 1;
	for (size_t i = 0; i < 4; i++) {
		if (lines.data[row - i].data[col+i] != XMAS[i]) fw = 0;
		if (lines.data[row - i].data[col+i] != XMAS[3-i]) rev = 0;
	}
	return fw + rev;
}

void day04part1(char* filename){
	int res = 0;
	char* line = calloc(80, sizeof(char));
	size_t i, j, line_len = 80;
	FILE* fp = fopen(filename, "r");
	struct list lines = list_create();
	int dbg = 0;
	while (getline(&line, &line_len, fp) != -1) {
		list_insert(&lines, charlist_create(line_len, line));
		line = calloc(line_len, sizeof(char));
	}
	free(line);
	for (i = 0; i < lines.len; i++){
		for (j = 0; j < lines.data[i].len; j++){
			if (i < lines.len-3 && try_down(lines, i, j)){
				res++;
				if (dbg) printf("found down at %ld, %ld\n", i, j);
			}
			if (j < lines.data[i].len && try_hor(lines, i, j)){
				res++;
				if (dbg) printf("found hor at %ld, %ld\n", i, j);
			}
			if (j < lines.data[i].len-3 && i < lines.len-3 && try_diag(lines, i, j)){
				res++;
				if (dbg) printf("found diag at %ld, %ld\n", i, j);
			}
			if (j < lines.data[i].len && i >= 3 && try_diag_up(lines, i, j)){
				res++;
				if (dbg) printf("found diag_up at %ld, %ld\n", i, j);
			}
		}
	}
	printf("result: %d\n", res);
	list_destroy(&lines);
}
void day04part2(char* filename){
	int res = 0;
	char* line = calloc(80, sizeof(char));
	size_t i, j, line_len = 80;
	FILE* fp = fopen(filename, "r");
	struct list lines = list_create();
	int dbg = 0;
	while (getline(&line, &line_len, fp) != -1) {
		list_insert(&lines, charlist_create(line_len, line));
		line = calloc(line_len, sizeof(char));
	}
	free(line);
	for (i = 0; i < lines.len-2; i++){
		for (j = 0; j < lines.data[i].len-2; j++){
			if (try_x(lines, i, j)){
				res++;
				if (dbg) printf("found x at %ld, %ld\n", i, j);
			}
		}
	}
	printf("result: %d\n", res);
	list_destroy(&lines);
}
