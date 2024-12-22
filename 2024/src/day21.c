#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "globals.h"


typedef struct __position{
	int x;
	int y;
} position;
LIST(position)
static int pos_equal(position a, position b) {
	return a.x == b.x && a.y == b.y;
}
typedef char* string;
LIST(string)
LIST_CLEAN(string)
LIST(position_list)
static void position_list_list_empty(position_list_list* a){
	for (size_t i = 0; i < a->len; i++) {
		free(a->data[i].data);
	}
	a->len = 0;
}

static void get_input(FILE* fp, string_list* list, int* line_len) {
	if (line_len) *line_len = 0;
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	int chars_read;
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		if (line_len){
			if (*line_len){
				if (*line_len != chars_read) {
					printf("malformed input\n");
				}
			} else {
				*line_len = chars_read;
			}
		}
		if (!strcmp(line, "\n")){
			break;
		}
		string_list_insert(list, line);
		line = calloc(last_line_len, sizeof(char));
	}
	*line_len -= 1;
	free(line);
}
static const position keypad_right = {0, -1};
static const position keypad_left = {-2, -1};
static const position keypad_down = {-1, -1};
static const position keypad_up = {-1, 0};
static const position keypad_a = {0, 0};
static position_list position_list_clone(position_list* a) {
	position_list ret = position_list_create();
	for (size_t i = 0; i < a->len; i++) {
		position_list_insert(&ret, a->data[i]);
	}
	return ret;
}
static position_list_list position_list_list_clone(position_list_list* a) {
	position_list_list ret = position_list_list_create();
	for (size_t i = 0; i < a->len; i++) {
		position_list_list_insert(&ret, position_list_clone(&a->data[i]));
	}
	return ret;
}
static void get_paths_recurse(position a, position b, position_list_list* ret){
	if (a.x == b.x){
		while(a.y < b.y) {
			for (size_t i = 0; i < ret->len; i++){
				position_list_insert(&ret->data[i], keypad_up);
			}
			a.y += 1; 
		}
		while(a.y > b.y) {
			for (size_t i = 0; i < ret->len; i++){
				position_list_insert(&ret->data[i], keypad_down);
			}
			a.y -= 1; 
		}
	} else if (a.y == b.y){
		while(a.x < b.x) {
			for (size_t i = 0; i < ret->len; i++){
				position_list_insert(&ret->data[i], keypad_right);
			}
			a.x += 1; 
		}
		while(a.x > b.x) {
			for (size_t i = 0; i < ret->len; i++){
				position_list_insert(&ret->data[i], keypad_left);
			}
			a.x -= 1; 
		}
	} else {
		position_list_list dx;
		position next = a;
		if (a.x < b.x) {
			dx = position_list_list_clone(ret);
			for (size_t i = 0; i < dx.len; i++){
				position_list_insert(&dx.data[i], keypad_right);
			}
			next.x = next.x + 1;
		} else {
			if (a.y == 0 && a.x == -1) {
				dx.cap = 0;
			} else {
				dx = position_list_list_clone(ret);
				for (size_t i = 0; i < dx.len; i++){
					position_list_insert(&dx.data[i], keypad_left);
				}
				next.x = next.x - 1;
			}
		}
		if (dx.cap) get_paths_recurse(next, b, &dx);
		next.x = a.x;
		if (a.y < b.y) {
			if (a.y == -1 && a.x == -2) {
				position_list_list_empty(ret);
			} else {
				for (size_t i = 0; i < ret->len; i++){
					position_list_insert(&ret->data[i], keypad_up);
				}
				next.y = next.y + 1;
			}
		} else {
			if (a.y == 1 && a.x == -2) {
				position_list_list_empty(ret);
			} else {
				for (size_t i = 0; i < ret->len; i++){
					position_list_insert(&ret->data[i], keypad_down);
				}
				next.y = next.y - 1;
			}
		}
		if (ret->len) get_paths_recurse(next, b, ret);
		if (dx.cap){
			for (size_t i = 0; i < dx.len; i++) {
				position_list_list_insert(ret, dx.data[i]);
			}
			free(dx.data);
		}
	}
}
static void get_paths(position a, position b, position_list_list* ret){
	position_list_list_empty(ret);
	position_list_list_insert(ret, position_list_create());
	get_paths_recurse(a, b, ret);
	for (size_t i = 0; i < ret->len; i++){
		position_list_insert(&ret->data[i], keypad_a);
	}
}
static long* cache = 0;
static int max_level = 0;
static size_t get_cache_addr(position a, position b, int level) {
	size_t ret;
	ret = -a.x;
	ret *= 2;
	ret += -a.y;
	ret *= 3;
	ret += -b.x;
	ret *= 2;
	ret += -b.y;
	ret *= max_level;
	ret += level;
	return ret;

}
static long get_cache(position a, position b, int level) {
	if (!cache) {
		cache = calloc(3 * 2 * 3 * 2 * (max_level), sizeof(long));
	}
	return cache[get_cache_addr(a, b, level)];
}
static void set_cache(position a, position b, int level, long val) {
	cache[get_cache_addr(a, b, level)] = val;
}
static long get_shortest_sequence(position current, position target, int level){
	DBG("LEVEL %d: finding sequence from %d,%d to %d,%d\n", level, current.x, current.y, target.x, target.y);
	if (!level) return 1;
	if (pos_equal(current, target)) {
		//in this case all just press a
		return 1;
	}
	long cache_val;
	if (level != max_level && (cache_val = get_cache(current, target, level))){
		return cache_val;
	}
	position_list_list paths = position_list_list_create();
	get_paths(current, target, &paths);
	DBG("found %ld paths\n", paths.len);
	long sum, min = LONG_MAX;
	position last_pos, next_pos;
	size_t i;
	for (i = 0; i < paths.len; i++) {
		last_pos.x = 0;
		last_pos.y = 0;
		sum = 0;
		for (size_t j = 0; j < paths.data[i].len; j++) {
			next_pos = paths.data[i].data[j];
			sum += get_shortest_sequence(last_pos, next_pos, level-1);
			last_pos = next_pos;
		}
		if (sum < min) min = sum;
		free(paths.data[i].data);
	}
	free(paths.data);
	if (level) DBG("length of minimal sequence from %d,%d to %d,%d is %ld\n", current.x, current.y, target.x, target.y, min);
	if (level != max_level) set_cache(current, target, level, min);
	return min;
}
static const position keypad_num_positions[10] = {{-1, 0}, {-2, 1}, {-1, 1}, {0, 1}, {-2, 2}, {-1, 2}, {0, 2}, {-2, 3}, {-1, 3}, {0, 3}};
static position get_pos_from_char(char c){
	position ret;
	switch (c) {
		case 'A':
			ret.x = 0;
			ret.y = 0;
		break;
		default:
			ret = keypad_num_positions[c - '0'];
		break;
	}
	return ret;
}
void day21part1(char* filename){
	string_list lines = string_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	int line_len;
	get_input(fp, &lines, &line_len);
	fclose(fp);
	DBG("starting solve input\n");
	size_t i;
	int j;
	long sum;
	long res = 0;
	int numeric_part;
	position last_pos, next_pos;
	max_level = 3;
	for (i = 0; i < lines.len; i++) {
		DBG("code: %s", lines.data[i]);
		sum = 0;
		last_pos = get_pos_from_char('A');
		for (j = 0; j < line_len; j++) {
			next_pos = get_pos_from_char(lines.data[i][j]);
			sum += get_shortest_sequence(last_pos, next_pos, 3);
			last_pos = next_pos;

		}
		lines.data[i][line_len-1] = 0;
		numeric_part = atoi(lines.data[i]);
		DBG("shortest sequence len: %ld\n", sum);
		DBG("numeric part: %d\n", numeric_part);
		res += sum * numeric_part;
	}
	printf("result: %ld\n", res);
	string_list_clean(&lines);
	free(lines.data);
	free(cache);
}
void day21part2(char* filename){
	string_list lines = string_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	int line_len;
	get_input(fp, &lines, &line_len);
	fclose(fp);
	DBG("starting solve input\n");
	size_t i;
	int j;
	long sum;
	long res = 0;
	int numeric_part;
	position last_pos, next_pos;
	max_level = 26;
	for (i = 0; i < lines.len; i++) {
		DBG("code: %s", lines.data[i]);
		sum = 0;
		last_pos = get_pos_from_char('A');
		for (j = 0; j < line_len; j++) {
			next_pos = get_pos_from_char(lines.data[i][j]);
			sum += get_shortest_sequence(last_pos, next_pos, 26);
			last_pos = next_pos;

		}
		lines.data[i][line_len-1] = 0;
		numeric_part = atoi(lines.data[i]);
		DBG("shortest sequence len: %ld\n", sum);
		DBG("numeric part: %d\n", numeric_part);
		res += sum * numeric_part;
	}
	printf("result: %ld\n", res);
	string_list_clean(&lines);
	free(lines.data);
	free(cache);
}
