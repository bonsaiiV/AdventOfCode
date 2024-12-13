#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

LIST(long)

static void get_input(FILE* fp, long_list* list) {
	size_t last_line_len = 8;
	char* tok;
	char* line = calloc(last_line_len, sizeof(char));
	if (getline(&line, &last_line_len, fp) == -1) {
		printf("error reading input\n");
	}
	tok = strtok(line, " ");
	do {
		long_list_insert(list, atol(tok));
		line = calloc(last_line_len, sizeof(char));
	} while ((tok = strtok(0, " ")));
	free(line);
}
static void apply_rules(long a, long_list* list) {
	if (a == 0) {
		long_list_insert(list, 1);
		return;
	}
	int digits = 0;
	long tmp = a;
	while (tmp) {
		digits++;
		tmp /= 10;
	}
	long div = 1;
	if (!(digits % 2)) {
		for (int i = 0; i < digits / 2; i++) {
			div *= 10;
		}
		tmp = a / div;
		long_list_insert(list, tmp);
		long_list_insert(list, a - tmp*div);
	} else {
		long_list_insert(list, a * 2024);
		return;
	}
}
void day11part1(char* filename){
	size_t i, j;
	long_list nums = long_list_create();
	long_list tmp, next_nums = long_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	get_input(fp, &nums);
	fclose(fp);
	for (i = 0; i < 75; i++) {
		for (j = 0; j < nums.len; j++) {
			DBG("%ld, ", nums.data[j]);
			apply_rules(nums.data[j], &next_nums);
		}
		DBG("\n");
		tmp = nums;
		nums = next_nums;
		next_nums = tmp;
		next_nums.len = 0;
	}
	printf("result: %ld\n", nums.len);
	free(nums.data);
	free(next_nums.data);
}
typedef struct tuple {
	long val;
	size_t count;
} stone;
LIST(stone)
void stone_list_add(stone_list* list, stone a) {
	size_t i;
	for (i = 0; i < list->len; i ++) {
		if (a.val == list->data[i].val) {
			list->data[i].count += a.count;
			return;
		}
	}
	stone_list_insert(list, a);
}
void day11part2(char* filename){
	size_t i, j;
	stone_list stones = stone_list_create();
	stone_list tmp, next_stones = stone_list_create();
	stone helper, a;
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	size_t last_line_len = 8;
	char* tok;
	char* line = calloc(last_line_len, sizeof(char));
	if (getline(&line, &last_line_len, fp) == -1) {
		printf("error reading input\n");
	}
	tok = strtok(line, " ");
	do {
		helper.val = atol(tok); helper.count = 1;
		stone_list_add(&stones, helper);
		line = calloc(last_line_len, sizeof(char));
	} while ((tok = strtok(0, " ")));
	free(line);
	fclose(fp);
	for (i = 0; i < 75; i++) {
		for (j = 0; j < stones.len; j++) {
			a = stones.data[j];
			DBG("{%ld, %ld}, ", a.val, a.count);
			if (a.val == 0) {
				a.val = 1;
				stone_list_add(&next_stones, a);
				continue;
			}
			int digits = 0;
			long tmp = a.val;
			while (tmp) {
				digits++;
				tmp /= 10;
			}
			long div = 1;
			if (!(digits % 2)) {
				for (int i = 0; i < digits / 2; i++) {
					div *= 10;
				}
				helper.val = a.val / div;
				helper.count = a.count;
				a.val -= helper.val * div;
				stone_list_add(&next_stones, helper);
				stone_list_insert(&next_stones, a);
			} else {
				a.val *= 2024;
				stone_list_insert(&next_stones, a);
			}
		}
		DBG("\n");
		tmp = stones;
		stones = next_stones;
		next_stones = tmp;
		next_stones.len = 0;
	}
	long long res = 0;
	for (i = 0; i < stones.len; i++) {
		res += stones.data[i].count;
	}
	printf("result: %lld\n", res);
	free(stones.data);
	free(next_stones.data);
}
