#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/list.h"

static list * get_input(char * filename) {
	FILE * fp = fopen(filename, "r");
	list * ret = create_list();
	size_t last_line_len = 8;
	char * line = calloc(last_line_len, sizeof(char));
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
void dayXXpart1(char* filename){
	size_t res = 0;
	list * input = get_input(filename);

	list_free(input);
	printf("result: %ld\n", res);
}
void dayXXpart2(char* filename){
	(void) filename;
}
