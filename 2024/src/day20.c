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
static position neightbors[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
static position pos_add(position a, position b) {
	position ret = {a.x + b.x, a.y + b.y};
	return ret;
}
static int in_bounds( position p, position size) {
	return p.x >= 0 && p.y >= 0 && p.y < size.y && p.x < size.x;
}
typedef char* string;
LIST(string)
LIST_CLEAN(string)

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
void reset_mem(int* mem, string_list lines, position size, int mem_default, int blocked) {
	int i, j;
	for (i = 0; i < size.y; i++) {
		for (j = 0; j < size.x; j++) {
			if (lines.data[i][j]=='#'){
				mem[i * size.x + j] = blocked;
			} else {
				mem[i * size.x + j] = mem_default;
			}
		}
	}
}
void day20part1(char* filename){
	string_list lines = string_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	int line_len;
	get_input(fp, &lines, &line_len);
	fclose(fp);
	DBG("starting solve input\n");
	position size = {line_len, lines.len};
	int i, j;
	position start, end;
	int memory[lines.len*line_len];
	for (i = 0; i < size.y; i++) {
		for (j = 0; j < size.x; j++) {
			if (lines.data[i][j]=='S'){
				start.x = j;
				start.y = i;
			}
			if (lines.data[i][j]=='E'){
				end.x = j;
				end.y = i;
			}
			if (lines.data[i][j]=='#'){
				memory[i * size.x + j] = 1;
			} else {
				memory[i * size.x + j] = 0;
			}
		}
	}
	position_list front = position_list_create();
	position_list tmp, next_front = position_list_create();
	position next, current;
	position_list_insert(&front, start);
	memory[0] = 1;
	int done = 0;
	int target_dist, dist = 0;
	size_t k;
	while (!done && front.len) {
		dist++;
		for (k = 0; k < front.len; k++) {
			current = front.data[k];
			for (size_t dir = 0; dir < 4; dir++) {
				next = pos_add(current, neightbors[dir]);
				if (next.x == end.x && next.y == end.y) {
					target_dist = dist;
					done = 1;
					break;
				}
				if (in_bounds(next, size) && !memory[next.x + next.y * size.x]) {
					memory[next.x + next.y * size.x] = dist;
					position_list_insert(&next_front, next);
				}
			}
			if (done) break;
		}
		tmp = front;
		front = next_front;
		next_front = tmp;
		next_front.len = 0;
	}
	int res = 0;
	for (i = 1; i < size.y-1; i++) {
		for (j = 1; j < size.x-1; j++) {
			if (lines.data[i][j] != '#') continue;
			reset_mem(memory, lines, size, 0, -1);
			memory[i * size.x + j] = 0;
			done = 0;
			front.len = 0;
			next_front.len = 0;
			position_list_insert(&front, start);
			memory[0] = 1;
			dist = 0;
			while (!done && front.len) {
				dist++;
				for (k = 0; k < front.len; k++) {
					current = front.data[k];
					for (size_t dir = 0; dir < 4; dir++) {
						next = pos_add(current, neightbors[dir]);
						if (next.x == end.x && next.y == end.y) {
							DBG("cheat saves %d ps\n", target_dist - dist);
							if (target_dist- dist >= 100) res++;
							done = 1;
							break;
						}
						if (in_bounds(next, size) && !memory[next.x + next.y * size.x]) {
							memory[next.x + next.y * size.x] = dist;
							position_list_insert(&next_front, next);
						}
					}
					if (done) break;
				}
				tmp = front;
				front = next_front;
				next_front = tmp;
				next_front.len = 0;
			}
		}
	}
	free(front.data);
	free(next_front.data);
	printf("result: %d\n", res);
	string_list_clean(&lines);
	free(lines.data);
}
void day20part2(char* filename){
	string_list lines = string_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	int line_len;
	get_input(fp, &lines, &line_len);
	fclose(fp);
	DBG("starting solve input\n");
	position size = {line_len, lines.len};
	int i, j;
	position start, end;
	int dist_from_fin[lines.len*line_len];
	for (i = 0; i < size.y; i++) {
		for (j = 0; j < size.x; j++) {
			if (lines.data[i][j]=='S'){
				start.x = j;
				start.y = i;
			}
			if (lines.data[i][j]=='E'){
				end.x = j;
				end.y = i;
			}
			if (lines.data[i][j]=='#'){
				dist_from_fin[i * size.x + j] = INT_MAX;
			} else {
				dist_from_fin[i * size.x + j] = -1;
			}
		}
	}
	position_list front = position_list_create();
	position_list tmp, next_front = position_list_create();
	position next, current;
	position_list_insert(&front, end);
	int done = 0;
	int dist = 0;
	dist_from_fin[end.x + end.y * size.x] = 0;
	size_t k;
	while (!done && front.len) {
		dist++;
		for (k = 0; k < front.len; k++) {
			current = front.data[k];
			for (size_t dir = 0; dir < 4; dir++) {
				next = pos_add(current, neightbors[dir]);
				if (in_bounds(next, size) &&  -1 == dist_from_fin[next.x + next.y * size.x]) {
					dist_from_fin[next.x + next.y * size.x] = dist;
					position_list_insert(&next_front, next);
				}
			}
			if (done) break;
		}
		tmp = front;
		front = next_front;
		next_front = tmp;
		next_front.len = 0;
	}
	for (i = 0; i < size.y; i++) {
		for (j = 0; j < size.x; j++) {
			if (dist_from_fin[i * size.x + j] == INT_MAX){
				DBG("#");
			} else {
				DBG(".");
			}
		}
		DBG("\n");
	}
	DBG("starting to test cheats\n");
	int memory[lines.len*line_len];
	reset_mem(memory, lines, size, -1, INT_MAX);
	int res = 0;
	int target_dist = dist_from_fin[start.x + start.y * size.x];
	front.len = 0;
	next_front.len = 0;
	position_list_insert(&front, start);
	memory[start.x + start.y * size.x] = 0;
	dist = 0;
	position cheat_end;
	int end_dist_to_fin;
	int min_cheat_gain = 100;
	DBG("target: %d\n", target_dist);
	while (front.len && dist < target_dist) {
		for (k = 0; k < front.len; k++) {
			current = front.data[k];
			DBG("current at %d, %d\n", current.x, current.y);
			for (i = -20; i <= 20; i++) {
				for (j = -(20-abs(i)); j <= 20-abs(i); j++) {
					DBG("cheating in dir %d,%d\n", j, i);
					cheat_end.x = current.x + j;
					cheat_end.y = current.y + i;
					if (!in_bounds(cheat_end, size)) continue;
					end_dist_to_fin = dist_from_fin[cheat_end.x + cheat_end.y * size.x];
					DBG("Cheating from %d, %d to %d, %d saving %d picoseconds\n", current.x, current.y, cheat_end.x, cheat_end.y, target_dist-abs(i)-abs(j)-dist-end_dist_to_fin)
					if (target_dist - abs(i) - abs(j) - dist - (min_cheat_gain-1) > end_dist_to_fin){
						DBG("Cheating from %d, %d to %d, %d saving %d picoseconds\n", current.x, current.y, cheat_end.x, cheat_end.y, target_dist-abs(i)-abs(j)-dist-end_dist_to_fin)
						res ++;
					}
				}
			}
			for (size_t dir = 0; dir < 4; dir++) {
				next = pos_add(current, neightbors[dir]);
				if (in_bounds(next, size) && -1 == memory[next.x + next.y * size.x]) {
					memory[next.x + next.y * size.x]= dist;
					position_list_insert(&next_front, next);
				}
			}
		}
		tmp = front;
		front = next_front;
		next_front = tmp;
		next_front.len = 0;
		dist++;
	}
	free(front.data);
	free(next_front.data);
	printf("result: %d\n", res);
	string_list_clean(&lines);
	free(lines.data);

}
