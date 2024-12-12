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
void apply_rules(long a, long_list* list) {
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
	for (i = 0; i < 25; i++) {
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
size_t stone_hash(stone in, size_t max) {
	return in.val % max;
}
HASH_MAP(stone)
size_t stone_max_bucket( stone_set* set) { 
	size_t i; 
	size_t len, max = 0;
	for (i = 0; i < set->bucket_count; i++) {
		len = set->buckets[i].len;
		if (len > max) max = len;
	}
	return max;
}
void day11part2(char* filename){
	size_t i;
	stone_set stones = create_stone_set(101);
	stone_set tmp, next_stones = create_stone_set(101);
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
		add_to_stone_set(&stones, helper);
		line = calloc(last_line_len, sizeof(char));
	} while ((tok = strtok(0, " ")));
	free(line);
	fclose(fp);
	for (i = 0; i < 75; i++) {
		get_next_from_stone_set(&stones, 0);
		while (!get_next_from_stone_set(0, &a)) {
			DBG("{%ld, %ld}, ", a.val, a.count);
			if (a.val == 0) {
				a.val = 1;
				add_to_stone_set(&next_stones, a);
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
				add_to_stone_set(&next_stones, helper);
				add_to_stone_set(&next_stones, a);
			} else {
				a.val *= 2024;
				add_to_stone_set(&next_stones, a);
			}
		}
		DBG("\n");
		tmp = stones;
		stones = next_stones;
		next_stones = tmp;
		empty_stone_set(&next_stones);
	}
	printf("max bucket: %ld\n", stone_max_bucket(&stones));
	long long res = sum_stone_set(&stones);
	printf("result: %lld\n", res);
	clean_stone_set(&stones);
}
