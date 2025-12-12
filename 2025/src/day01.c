#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data/list.h"


static list * get_input(char * filename) {
	FILE * fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "could not open file %s\n", filename);
		perror("error");
	}
	list * ret = create_list();
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
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

void day01part1(char* filename){
	list * input = get_input(filename);
	char * line;
	int res = 0;
	int dial = 50;
	int rot;
	int sign;
	LIST_FOR_EACH(line, input) {
		if (*line == 'R') sign = 1;
		else sign = -1;
		rot = atoi(line+1);
		dial += sign * rot;
		if ((dial % 100) == 0) res++;
	}
	printf("result: %d\n", res);
	list_free_all(input);
}
void day01part2(char* filename){
	list * input = get_input(filename);
	char * line;
	int res = 0;
	int old_dial, dial = 50;
	int rot;
	int sign;
	LIST_FOR_EACH(line, input) {
		if (*line == 'R') sign = 1;
		else sign = -1;
		rot = atoi(line+1);
		if (rot > 0)
			res += rot/100;
		else
			res -= rot/100;
		rot = rot%100;
		if (!rot) continue;
		old_dial = dial;
		dial += sign * rot;
		if (!old_dial) {
			if (dial < 0) dial += 100;
			printf("\n");
			continue;
		}
		if (dial == 0) {
			res ++;
		} else if (dial < 0) {
			res++;
			dial += 100;
		} else if (dial >= 100) {
			res++;
			dial -= 100;
		}
	}
	printf("result: %d\n", res);
	list_free_all(input);
}
