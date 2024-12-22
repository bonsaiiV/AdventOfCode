#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


typedef struct _pattern {
	char* str;
	int len;
} pattern;
typedef uint64_t ll;
typedef struct _targets {
	char* str;
	int remainder;
	ll* dyn_res;
} targets;
LIST(pattern)
LIST(targets)

static void get_input(FILE* fp, pattern_list* available, targets_list* target) {
	size_t last_line_len = 8;
	char* tok, * line = calloc(last_line_len, sizeof(char));
	int chars_read;
	pattern next;
	targets next_target;
	if (getline(&line, &last_line_len, fp) == -1) {
		printf("issue getting patterns\n");
	}
	tok = strtok(line, ", ");
	next.str = tok;
	next.len = strlen(tok);
	pattern_list_insert(available, next);
	while ((tok = strtok(0, ", "))) {
		next.str = tok;
		next.len = strlen(tok);
		if (next.str[next.len-1] == '\n') {
			next.str[next.len-1] = 0;
			next.len--;
		}
		DBG("pattern %s with %d chars available\n", next.str, next.len);
		pattern_list_insert(available, next);
	}
	line = calloc(last_line_len, sizeof(char));
	getline(&line, &last_line_len, fp);
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		next_target.str = line;
		next_target.remainder = chars_read-1;
		next_target.str[chars_read-1] = 0;
		next_target.dyn_res = calloc(chars_read, sizeof(ll));
		next_target.dyn_res[0] = 1;
		targets_list_insert(target, next_target);
		line = calloc(last_line_len, sizeof(char));
	}
	free(line);
}
static int check_target(pattern_list* available, char* target){
	if (*target == 0) return 1;
	pattern current;
	for (size_t i = 0; i < available->len; i++) {
		current = available->data[i];
		if (!strncmp(target, current.str, current.len)) {
			DBG("pattern: \"%s\" used to match target \"%s\"\n", current.str, target);
			DBG("this consumes %d chars\n", current.len);
			if (check_target(available, target + current.len)) {
				return 1;
			}
		}
	}
	return 0;
}
void day19part1(char* filename){
	size_t i;
	targets_list targets = targets_list_create();
	pattern_list available = pattern_list_create();
	FILE* fp = fopen(filename, "r");
	int res = 0;
	DBG("getting input\n");
	get_input(fp, &available, &targets);
	fclose(fp);
	DBG("starting run\n");
	for (i = 0; i < targets.len; i++) {
		DBG("target: %s\n", targets.data[i].str);
		res += check_target(&available, targets.data[i].str);
	}
	printf("result: %d\n", res);
	for (i = 0; i < targets.len; i++) {
		free(targets.data[i].str);
		free(targets.data[i].dyn_res);
	}
	free(available.data[0].str);
	free(targets.data);
	free(available.data);
}
static ll count_target(pattern_list* available, targets target){
	targets next;
	pattern current;
	if (!target.dyn_res[target.remainder]) {
		DBG("calculating dyn res for remainder: %d\n", target.remainder);
		for (size_t i = 0; i < available->len; i++) {
			current = available->data[i];
			if (!strncmp(target.str, current.str, current.len)) {
				next.str = target.str + current.len;
				next.remainder = target.remainder - current.len;
				next.dyn_res = target.dyn_res;
				target.dyn_res[target.remainder] += count_target(available, next);
			}
		}
	} else {
		DBG("dyn res for remainder %d is %ld\n", target.remainder, target.dyn_res[target.remainder]);
	}
	return target.dyn_res[target.remainder];
}
void day19part2(char* filename){
	printf("%ld\n", sizeof(uint64_t));
	size_t i;
	targets_list targets = targets_list_create();
	pattern_list available = pattern_list_create();
	FILE* fp = fopen(filename, "r");
	ll res = 0;
	DBG("getting input\n");
	get_input(fp, &available, &targets);
	fclose(fp);
	DBG("starting run\n");
	ll possibilities;
	for (i = 0; i < targets.len; i++) {
		DBG("target: %s\n", targets.data[i].str);
		possibilities = count_target(&available, targets.data[i]);
		DBG("matched using %ld possibilities\n", possibilities);
		res += possibilities;
	}
	printf("result: %ld\n", res);
	for (i = 0; i < targets.len; i++) {
		free(targets.data[i].str);
		free(targets.data[i].dyn_res);
	}
	free(available.data[0].str);
	free(targets.data);
	free(available.data);
}
