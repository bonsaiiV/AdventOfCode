#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "days.h"

int main(int argc, char **argv) {
	size_t part = 0, day = 0;
	int opt;
	while ((opt = getopt(argc, argv, "p:d:")) != -1) {
		switch (opt) {
		case 'd':
			day = atoi(optarg);
			break;
		case 'p':
			part = atoi(optarg);
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
	if (day >= sizeof(daymap)) {
		printf("day not implemented\n");
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
	char* input_file = "";
	if(optind < argc) {
		input_file = argv[optind];
	}
	void (**parts) (char[]) = ((void (**) (char[])) &daymap[day-1]) + part - 1;
	(*parts)(input_file);
	return 0;
}
