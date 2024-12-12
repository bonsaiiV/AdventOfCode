#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


typedef char* string;
LIST(string)
LIST_CLEAN(string)

static void get_input(FILE* fp, string_list* list) {
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	while (getline(&line, &last_line_len, fp) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		string_list_insert(list, line);
		line = calloc(last_line_len, sizeof(char));
	}
	free(line);
}
void dayXXpart1(char* filename){
	size_t i;
	string_list lines = string_list_create();
	FILE* fp = fopen(filename, "r");
	int res = 0;
	DBG("getting input\n");
	get_input(fp, &lines);
	for (i = 0; i < lines.len; i++) {
	}
	fclose(fp);
	printf("result: %d\n", res);
	string_list_clean(&lines);
	free(lines.data);
}
void dayXXpart2(char* filename){
	(void) filename;
}
