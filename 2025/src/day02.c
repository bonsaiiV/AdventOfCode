#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/list.h"

static list * get_input(char * filename) {
	FILE * fp = fopen(filename, "r");
	list * ret = create_list();
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	int chars_read;
	while ((chars_read = getdelim(&line, &last_line_len, ',', fp)) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		list_push(ret, line);
		line = calloc(last_line_len, sizeof(char));
	}
	free(line);
	return ret;
}
void day02part1(char* filename){
	list * input = get_input(filename);
	char * range_str;
	long start, end;
	int buf_len = 80;
	char * buf = calloc(buf_len, sizeof(char));
	int num_len;
	long res = 0;
	int is_bad;
	LIST_FOR_EACH(range_str, input) {
		sscanf(range_str, "%ld-%ld", &start, &end);
		printf("%ld-%ld\n", start, end);
		for (long i = start; i <= end; i++) {
			if ((num_len = snprintf(buf, buf_len, "%ld", i)) >= buf_len) {
				printf("buffer too small");
				exit(1);
			}
			if (num_len % 2) continue;
			is_bad = 0;
			for (int j = 0; j < num_len/2; j++) {
				if (buf[j] != buf[j+num_len/2]) {
					is_bad = 1;
					break;
				}
			}
			if (!is_bad) {
				res += i;
			}

		}
	}
	printf("result: %ld\n", res);
}
void day02part2(char* filename){
	list * input = get_input(filename);
	char * range_str;
	long start, end;
	int buf_len = 80;
	char * buf = calloc(buf_len, sizeof(char));
	int num_len;
	long res = 0;
	int num_sequences, len_sequence, i_s;
	char expected_digit;
	LIST_FOR_EACH(range_str, input) {
		sscanf(range_str, "%ld-%ld", &start, &end);
		for (long i = start; i <= end; i++) {
			if ((num_len = snprintf(buf, buf_len, "%ld", i)) >= buf_len) {
				printf("buffer too small");
				exit(1);
			}
			for (num_sequences = 2; num_sequences <= num_len; num_sequences++) {
				int might_be_valid = 0;
				if (num_len % num_sequences) continue;
				len_sequence = num_len / num_sequences;
				for (int offset = 0; offset < len_sequence; offset++) {
					expected_digit = buf[offset];
					for (i_s = 1; i_s < num_sequences; i_s++) {
						int pos = offset+len_sequence * i_s;
						if (expected_digit != buf[pos]) {
							might_be_valid = 1;
							break;
						}
					}
					if (might_be_valid) break;
				}
				if (!might_be_valid) {
					res += i;
					break;
				}
			}
		}
	}
	printf("result: %ld\n", res);
}
