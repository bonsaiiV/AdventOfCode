#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "days.h"

int dbg = 0;
int main(int argc, char **argv) {
	size_t part = 0, day = 0;
	int opt;
	char* input_data = 0;
	while ((opt = getopt(argc, argv, "p:d:i:vt")) != -1) {
		switch (opt) {
			case 'd':
				day = atoi(optarg);
			break;
			case 'p':
				part = atoi(optarg);
			break;
			case 'i':
				if (!input_data) {
					input_data = strdup(optarg);
				}
				break;
			case 't':
				if (!input_data) {
					input_data = strdup("test");
				}
				break;
			case 'v':
				dbg = 1;
			break;
			case '?':
				printf("unknown option\n");
				exit(1);
		}
	}
	int bad_arg = 0;
	if (day == 0) {
		printf("please specify the day to run\n");
		bad_arg = 1;
	}
	if (day > 12) {
		printf("there are only 12 days in Advent of Code\n\nyour day: %ld\n", day);
		bad_arg = 1;
	}
	if (day >= sizeof(daymap)) {
		printf("day not implemented, maybe you forgot to declare it\n");
		bad_arg = 1;
	}
	if (part == 0) {
		printf("please specify the part to run\n");
		bad_arg = 1;
	}
	if (part > 2) {
		printf("day must be 1 or 2\n");
		bad_arg = 1;
	}
	if (bad_arg) {
		exit(1);
	}
	if (!input_data){
		input_data = strdup("my_input");
	}
	int data_file_name_len = strlen(input_data);
	char* input_file = calloc(13+data_file_name_len, sizeof(char));
	int digits = snprintf(input_file, 0, "%ld", day);
	if (digits == 1) {
		snprintf(input_file, 12, "input/day0%ld", day);
	} else {
		snprintf(input_file, 12, "input/day%ld", day);
	}
	snprintf(input_file+11, 2+data_file_name_len, "/%s", input_data);
	free(input_data);

	void (**parts) (char[]) = ( (void (**) (char[]) ) &daymap[day-1]) + part - 1;

	(*parts)(input_file);
	free(input_file);
	return 0;
}
