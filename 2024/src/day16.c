#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "globals.h"


typedef struct _position {
	int x;
	int y;
}position;
static position neightbors[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
static position pos_add(position a, position b) {
	position ret = {a.x + b.x, a.y + b.y};
	return ret;
}
static position pos_mul(position a, int b) {
	position ret = {a.x * b, a.y * b};
	return ret;
}
typedef struct _heading {
	position pos;
	int dir;
	int score;
}heading;
static heading heading_move(heading a, int dir) {
	position pos = pos_add(a.pos, neightbors[dir]);
	int score = a.score+1;
	if (dir != a.dir) {
		score += 1000;
	}
	heading ret = {pos, dir, score};
	return ret;
}
static heading heading_unmove(heading a, int dir) {
	position pos = pos_add(a.pos, pos_mul(neightbors[a.dir], -1));
	int score = a.score-1;
	if (dir != a.dir) {
		score -= 1000;
	}
	heading ret = {pos, dir, score};
	return ret;
}
typedef char* string;
LIST(heading)
LIST(position)
static int position_list_contains(position_list* list, position a) {
	for (size_t i = 0; i < list->len; i++) {
		if (list->data[i].x == a.x && list->data[i].y == a.y)
		return 1;
	}
	return 0;
}
LIST(string)
LIST_CLEAN(string)

static void get_input(FILE* fp, string_list* list, size_t* line_len) {
	if (line_len) *line_len = 0;
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	int chars_read;
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		if (line_len){
			if (*line_len){
				if (*line_len != (size_t) chars_read) {
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
void day16part1(char* filename){
	size_t i, j;
	int dir;
	string_list lines = string_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	size_t line_len;
	get_input(fp, &lines, &line_len);
	fclose(fp);
	DBG("map size: %ld,%ld\n", line_len, lines.len);
	int min_score[lines.len][line_len][4];
	position end;
	heading start;
	start.score = 0;
	start.dir = 1;
	for (i = 0; i < lines.len; i++) {
		for (j = 0; j < line_len; j++) {
			for (dir = 0; dir < 4; dir++) {
				min_score[i][j][dir] = INT_MAX;
			}
			if (lines.data[i][j] == 'S'){
				start.pos.x = j;
				start.pos.y = i;
			}
			if (lines.data[i][j] == 'E'){
				end.x = j;
				end.y = i;
			}
		}
	}
	DBG("start: (%d,%d) end: (%d,%d)\n", start.pos.x, start.pos.y, end.x, end.y);
	min_score[start.pos.y][start.pos.x][1] = 0;
	heading_list front = heading_list_create();
	heading_list tmp, next_front = heading_list_create();
	heading_list_insert(&front, start);
	heading current, next;
	while (front.len) {
		DBG("len: %ld\n", front.len);
		for (i = 0; i < front.len; i++) {
			current = front.data[i];
			DBG("current: {(%d,%d), %d, %d}\n", current.pos.x, current.pos.y, current.dir, current.score);
			for (dir = 0; dir < 4; dir++) {
				next = heading_move(current, dir);
				if (lines.data[next.pos.y][next.pos.x] == '#') {
					continue;
				}
				DBG("critical pos: %d\n", min_score[13][13][1]);
				if (next.score < min_score[next.pos.y][next.pos.x][dir]) {
					min_score[next.pos.y][next.pos.x][dir] = next.score;
					if (lines.data[next.pos.y][next.pos.x] == 'E') {
						DBG("end reached\n");
						continue;
					}
					heading_list_insert(&next_front, next);
				}
			}
		}
		tmp = front;
		front = next_front;
		next_front = tmp;
		next_front.len = 0;
	}
	int res = INT_MAX;
	for (dir = 0; dir < 4; dir++) {
		if (min_score[end.y][end.x][dir] < res) {
			res = min_score[end.y][end.x][dir];
		}
	}
	free(front.data);
	free(next_front.data);
	printf("result: %d\n", res);
	string_list_clean(&lines);
	free(lines.data);
}
void day16part2(char* filename){
	size_t i, j;
	int dir;
	string_list lines = string_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	size_t line_len;
	position end;
	heading start;
	start.score = 0;
	start.dir = 1;
	get_input(fp, &lines, &line_len);
	fclose(fp);
	int min_score[lines.len][line_len][4];
	DBG("map size: %ld,%ld\n", line_len, lines.len);
	for (i = 0; i < lines.len; i++) {
		for (j = 0; j < line_len; j++) {
			for (dir = 0; dir < 4; dir++) {
				min_score[i][j][dir] = INT_MAX;
			}
			if (lines.data[i][j] == 'S'){
				start.pos.x = j;
				start.pos.y = i;
			}
			if (lines.data[i][j] == 'E'){
				end.x = j;
				end.y = i;
			}
		}
	}
	DBG("start: (%d,%d) end: (%d,%d)\n", start.pos.x, start.pos.y, end.x, end.y);
	min_score[start.pos.y][start.pos.x][1] = 0;
	heading_list front = heading_list_create();
	heading_list tmp, next_front = heading_list_create();
	heading_list_insert(&front, start);
	heading current, next;
	while (front.len) {
		for (i = 0; i < front.len; i++) {
			current = front.data[i];
			for (dir = 0; dir < 4; dir++) {
				next = heading_move(current, dir);
				if (lines.data[next.pos.y][next.pos.x] == '#') {
					continue;
				}
				if (next.score < min_score[next.pos.y][next.pos.x][dir]) {
					min_score[next.pos.y][next.pos.x][dir] = next.score;
					if (lines.data[next.pos.y][next.pos.x] == 'E') {
						continue;
					}
					heading_list_insert(&next_front, next);
				}
			}
		}
		tmp = front;
		front = next_front;
		next_front = tmp;
		next_front.len = 0;
	}
	int best = INT_MAX;
	for (dir = 0; dir < 4; dir++) {
		if (min_score[end.y][end.x][dir] < best) {
			best = min_score[end.y][end.x][dir];
		}
	}
	current.score = best;
	current.pos = end;
	for (current.dir = 0; current.dir < 4; current.dir++) {
		if (min_score[end.y][end.x][current.dir] == best) {
			heading_list_insert(&front, current);
		}
	}
	position_list on_path = position_list_create();
	position_list_insert(&on_path, end);
	while (front.len) {
		for (i = 0; i < front.len; i++) {
			current = front.data[i];
			DBG("current: {(%d,%d), %d, %d}\n", current.pos.x, current.pos.y, current.dir, current.score);
			for (dir = 0; dir < 4; dir++) {
				next = heading_unmove(current, dir);
				DBG("next: {(%d,%d), %d, %d}\n", next.pos.x, next.pos.y, next.dir, next.score);
				if (next.score == min_score[next.pos.y][next.pos.x][dir]) {
					heading_list_insert(&next_front, next);
					if (!position_list_contains(&on_path, next.pos)) {
						position_list_insert(&on_path, next.pos);
					}
				}
			}
		}
		tmp = front;
		front = next_front;
		next_front = tmp;
		next_front.len = 0;
	}
	free(on_path.data);
	free(front.data);
	free(next_front.data);
	printf("result: %ld\n", on_path.len);
	string_list_clean(&lines);
	free(lines.data);
}
