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
#define bucket_count 101
size_t stone_hash(stone in, size_t max) {
	return in.val % max;
}
struct hash_map {
	stone_list buckets[bucket_count];
};
struct hash_map stone_map_create(void){
	struct hash_map ret;
	size_t i;
	for (i = 0; i < bucket_count; i++) {
		ret.buckets[i] = stone_list_create();
	}
	return ret;
}
void stone_map_add(struct hash_map* map, stone in) {
	stone_list_add(map->buckets + stone_hash(in, bucket_count), in);
}
size_t stone_max_bucket(struct hash_map* map) {
	size_t i;
	size_t len, max = 0;
	for (i = 0; i < bucket_count; i++) {
		len = map->buckets[i].len;
		if (len > max) max = len;
	}
	return max;
}
size_t stone_map_count(struct hash_map* map) {
	size_t i;
	size_t count = 0;
	for (i = 0; i < bucket_count; i++) {
		count += map->buckets[i].len;
	}
	return count;
}
size_t stone_map_sum(struct hash_map* map) {
	size_t i, j;
	size_t sum = 0;
	for (i = 0; i < bucket_count; i++) {
		for (j = 0; j < map->buckets[i].len; j++) {
			sum += map->buckets[i].data[j].count;
		}
	}
	return sum;
}
int map_get_next(struct hash_map* in_map, stone* ret) {
	static struct hash_map* map = 0;
	if (in_map) {
		map = in_map;
		return 1;
	}
	if (!map) return -1;
	if (!ret) return -1;
	static size_t next_bucket = 0;
	static size_t next_index = 0;
	while (1) {
		if (next_bucket >= bucket_count) {
			next_bucket = 0;
			next_index = 0;
			return 2;
		}
		if (next_index < map->buckets[next_bucket].len) {
			*ret = map->buckets[next_bucket].data[next_index++];
			return 0;
		} else {
			next_index = 0;
			next_bucket++;
		}
	} 
}
void map_empty(struct hash_map* map) {
	size_t i;
	for (i = 0; i < bucket_count; i++) {
		map->buckets[i].len = 0;
	}
}
void map_clean(struct hash_map* map) {
	size_t i;
	for (i = 0; i < bucket_count; i++) {
		free(map->buckets[i].data);
	}
}
void day11part2(char* filename){
	size_t i;
	struct hash_map stones = stone_map_create();
	struct hash_map tmp, next_stones = stone_map_create();
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
		stone_map_add(&stones, helper);
		line = calloc(last_line_len, sizeof(char));
	} while ((tok = strtok(0, " ")));
	free(line);
	fclose(fp);
	for (i = 0; i < 75; i++) {
		map_get_next(&stones, 0);
		while (!map_get_next(0, &a)) {
			DBG("{%ld, %ld}, ", a.val, a.count);
			if (a.val == 0) {
				a.val = 1;
				stone_map_add(&next_stones, a);
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
				stone_map_add(&next_stones, helper);
				stone_map_add(&next_stones, a);
			} else {
				a.val *= 2024;
				stone_map_add(&next_stones, a);
			}
		}
		DBG("\n");
		tmp = stones;
		stones = next_stones;
		next_stones = tmp;
		map_empty(&next_stones);
	}
	printf("max bucket: %ld\n", stone_max_bucket(&stones));
	long long res = stone_map_sum(&stones);
	printf("result: %lld\n", res);
	map_clean(&stones);
}
