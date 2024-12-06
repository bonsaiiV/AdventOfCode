#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

#define DBG(...) if (dbg) printf(__VA_ARGS__);
struct list {
	char** data;
	size_t cap;
	size_t len;
};
static struct list list_create(void) {
	char** data = calloc(8, sizeof(char*));
	struct list ret = {data, 8, 0};
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
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	while (getline(&line, &last_line_len, fp) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		list_insert(list, line);
		line = calloc(last_line_len, sizeof(char));
	}
	free(line);
}
void dayXXpart1(char* filename){
	dbg = 1;
	size_t i;
	struct list lines = list_create();
	FILE* fp = fopen(filename, "r");
	int res = 0;
	DBG("getting input\n");
	get_input(fp, &lines);
	for (i = 0; i < lines.len; i++) {
	}
	printf("result: %d\n", res);
	list_clean(&lines);
	free(lines.data);
}
void dayXXpart2(char* filename){
	dbg = 1;
	size_t i;
	struct list lines = list_create();
	FILE* fp = fopen(filename, "r");
	int res = 0;
	DBG("getting rules\n");
	get_input(fp, &lines);
	for (i = 0; i < lines.len; i++) {
	}
	printf("result: %d\n", res);
	list_clean(&lines);
	free(lines.data);
}
