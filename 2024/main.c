#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "days.h"

int dbg = 0;
int main(int argc, char **argv) {
	size_t part = 0, day = 0;
	int opt;
	char* input_file = 0;
	while ((opt = getopt(argc, argv, "p:d:tv")) != -1) {
		switch (opt) {
			case 'd':
				day = atoi(optarg);
				break;
			case 'p':
				part = atoi(optarg);
				break;
			case 't':
				if (!input_file) {
					input_file = "input/test";
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
	if (day > 25) {
		printf("there are only 25 days in Advent of Code\n\nyour day: %ld\n", day);
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
	if (!input_file){
		input_file = calloc(12, sizeof(char));
		int digits = snprintf(input_file, 0, "%ld", day);
		if (digits == 1) {
			snprintf(input_file, 12, "input/day0%ld", day);
		} else {
			snprintf(input_file, 12, "input/day%ld", day);
		}
	}
	if(optind < argc) {
		input_file = argv[optind];
	}
	void (**parts) (char[]) = ((void (**) (char[])) &daymap[day-1]) + part - 1;
	(*parts)(input_file);
	return 0;
}
