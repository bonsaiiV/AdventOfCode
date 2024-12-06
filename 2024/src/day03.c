#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

static char* digits = "0123456789";
int get_digit(int* ret, char in){
	for (int i = 0; i < 10; i++) {
		if (digits[i] == in) {
			*ret = i;
			return 1;
		}
	}
	return 0;
}
void day03part1(char* filename){
	int res = 0;
	int digit, a, b;
	char* line = calloc(80, sizeof(char));
	size_t i, line_len = 80;
	FILE* fp = fopen(filename, "r");
	while (getline(&line, &line_len, fp) != -1) {
		i = 0;
		while (line[i] != 0){
			a = 0;
			b = 0;
			if (!strncmp("mul(", line + i, 4)) {
				i += 4;
				while(get_digit(&digit, line[i])){ 
					a = a * 10 + digit;
					i++;
				}
				if (a == 0 || line[i] != ','){
					continue;
				}
				i++;
				while(get_digit(&digit, line[i])){ 
					b = b * 10 + digit;
					i++;
				}
				if (b == 0 || line[i] != ')'){
					continue;
				}
				res += a*b;
			}
			i++;
		}
	}
	free(line);
	printf("result: %d\n", res);
}
void day03part2(char* filename){
	int res = 0;
	int digit, a, b;
	int enabled = 1;
	char* line = calloc(80, sizeof(char));
	size_t i, line_len = 80;
	FILE* fp = fopen(filename, "r");
	while (getline(&line, &line_len, fp) != -1) {
		i = 0;
		while (line[i] != 0){
			a = 0;
			b = 0;
			if (!strncmp("do()", line + i, 4)) {
				i += 4;
				enabled = 1;
			}
			if (!strncmp("don't()", line + i, 6)) {
				i += 4;
				enabled = 0;
			}
			if (enabled && !strncmp("mul(", line + i, 4)) {
				i += 4;
				while(get_digit(&digit, line[i])){ 
					a = a * 10 + digit;
					i++;
				}
				if (a == 0 || line[i] != ','){
					continue;
				}
				i++;
				while(get_digit(&digit, line[i])){ 
					b = b * 10 + digit;
					i++;
				}
				if (b == 0 || line[i] != ')'){
					continue;
				}
				res += a*b;
			}
			i++;
		}
	}
	free(line);
	printf("result: %d\n", res);
}
