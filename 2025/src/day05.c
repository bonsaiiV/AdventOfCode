#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data/list.h"

static list * get_input(char * filename) {
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
	list_push(ret, 0);
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

typedef struct {
	size_t start;
	size_t end;
} range;

void day05part1(char* filename){
	size_t res = 0;
	list * input = get_input(filename);
	size_t range_size = 0, range_cap = 5;
	range * ranges = calloc(range_cap, sizeof(range));

	char * line;
	int do_ranges = 1;
	range next_range;
	LIST_FOR_EACH(line, input) {
		if (do_ranges) {
			if (!line) {
				do_ranges = 0;
				for (size_t i = 0; i < range_size; i++) {
					next_range = ranges[i];
					printf("%lu-%lu\n", next_range.start, next_range.end);
				}
				continue;
			}
			sscanf(line, "%lu-%lu", &next_range.start, &next_range.end);
			if (range_size == range_cap) {
				range_cap = range_cap * 3 / 2 + 1;
				ranges = reallocarray(ranges, range_cap, sizeof(range));
			}
			ranges[range_size++] = next_range;
		} else {
			int is_bad = 1;
			size_t id = atol(line);
			for (size_t i = 0; i < range_size; i++) {
				next_range = ranges[i];
				if (id >= next_range.start && id <= next_range.end) {
					is_bad = 0;
					break;
				}
			}
			if (!is_bad) {
				res++;
			}
		}
	}

	list_free(input);
	printf("result: %ld\n", res);
}
void day05part2(char* filename){
	ssize_t res = 0;
	list * input = get_input(filename);
	list * ranges = create_list();

	char * line;
	range * new_r;
	LIST_FOR_EACH(line, input) {
		if (!line) {
			break;
		}
		new_r = malloc(sizeof(range));
		sscanf(line, "%lu-%lu", &new_r->start, &new_r->end);
		range * a;
		for (size_t i = 0; i < ranges->size; i++) {
			a = ranges->data[i];
			if (a->end >= new_r->start && new_r->end >= a->start) {
				new_r->end = new_r->end > a->end ? new_r->end : a->end;
				new_r->start = new_r->start < a->start ? new_r->start : a->start;
				free(a);
				ranges->data[i--] = ranges->data[--ranges->size];
			}
		}
		list_push(ranges, new_r);
	}
	LIST_FOR_EACH(new_r, ranges) {
		res += new_r->end + 1 - new_r->start;
	}
	printf("result: %ld\n", res);
	list_free_all(ranges);
	list_free(input);
}
