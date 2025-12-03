#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/list.h"

list * get_input(char * filename) {
	FILE * fp = fopen(filename, "r");
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
void day03part1(char* filename){
	list * input = get_input(filename);
	char * line, * cur, * max, * first, * second;
	int res = 0;
	LIST_FOR_EACH(line, input) {
		max = line;
		cur = line;
		while (*(++cur)!= '\n') {
			if (*cur > *max) {
				max = cur;
			}
		}
		if (max + 1 < cur) {
			first = max;
			max = first + 1;
			cur = first + 1;
			while (*(++cur) != '\n') {
				if (*cur > *max) {
					max = cur;
				}
			}
			second = max;
		} else {
			second = max;
			max = line;
			cur = line;
			while ((++cur) != second) {
				if (*cur > *max) {
					max = cur;
				}
			}
			first = max;
		}
		res += (*first - '0') * 10 + (*second - '0');
	}
	printf("result: %d\n", res);
}
void day03part2(char* filename){
	list * input = get_input(filename);
	char * line, * end, * cur, * max;
	long res = 0;
	int l, r;
	char * bats[12];
	LIST_FOR_EACH(line, input) {
		end = line;
		while (*(++end) != '\n');
		l = 0, r = 11;
		while (l <= r) {
			max = line;
			cur = line;
			while (++cur != end) {
				if (*cur > *max) {
					max = cur;
				}
			}
			if (max + r - l >= cur) {
				while (cur-- != max)
					bats[l++] = cur;
				end = max;
			} else {
				bats[r--] = max;
				line = max + 1;
			}
		}

		long mul = 1;
		for (int i = 0; i < 12; i++) {
			res += (*(bats[i]) - '0') * mul;
			mul *= 10;
		}
	}
	printf("result: %ld\n", res);
}
