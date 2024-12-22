#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


typedef struct __position{
	int x;
	int y;
} position;
LIST(position)
static position neightbors[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
static position pos_add(position a, position b) {
	position ret = {a.x + b.x, a.y + b.y};
	return ret;
}
typedef char* string;
LIST(string)
LIST_CLEAN(string)

static void get_input(FILE* fp, string_list* list, int* line_len) {
	if (line_len) *line_len = 0;
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	int chars_read;
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		if (line_len){
			if (*line_len){
				if (*line_len != chars_read) {
					printf("malformed input\n");
				}
			} else {
				*line_len = chars_read;
			}
		}
		if (!strcmp(line, "\n")){
			break;
		}
		string_list_insert(list, line);
		line = calloc(last_line_len, sizeof(char));
	}
	*line_len -= 1;
	free(line);
}
void dayXXpart1(char* filename){
	string_list input = string_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	int line_len;
	get_input(fp, &input, &line_len);
	fclose(fp);

	DBG("starting solve input\n");
	int res = 0;
	size_t i, j;
	for (i = 0; i < input.len; i++) {
		for (j = 0; j < input.len; j++) {
		}
	}
	string_list_clean(&input);
	free(input.data);

	printf("result: %d\n", res);
}
void dayXXpart2(char* filename){
	(void) filename;
}
