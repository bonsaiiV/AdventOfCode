#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


typedef char* string;
LIST(string)
LIST(int)
LIST_CLEAN(string)
typedef struct toople {
	long x;
	long y;
}position;
LIST(position)
static position add(position a, position b) {
	position ret = {a.x + b.x, a.y + b.y};
	return ret;
}
static position mul(position a, int b) {
	position ret = {a.x * b, a.y * b};
	return ret;
}

static void get_input(FILE* fp, string_list* list, int* line_len, int_list* moves) {
	if (line_len) *line_len = 0;
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	int num_chars;
	while ((num_chars = getline(&line, &last_line_len, fp)) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		if (*line_len) {
			if (*line_len != num_chars) {
				printf("malformed input\n");
				exit(1);
			}
		} else {
			*line_len = num_chars;
		}
		string_list_insert(list, line);
		DBG("%s", list->data[list->len-1]);
		line = calloc(last_line_len, sizeof(char));
	}
	*line_len -= 1;
	while ((num_chars = getline(&line, &last_line_len, fp)) != -1) {
		for (int i = 0; i < num_chars - 1; i++) {
			switch (line[i]) {
				case 'v':
					int_list_insert(moves, 3);
				break;
				case '^':
					int_list_insert(moves, 1);
				break;
				case '<':
					int_list_insert(moves, 0);
				break;
				case '>':
					int_list_insert(moves, 2);
				break;
			}
		}
	}
	free(line);
}
static position neighbors[4] = {{-1, 0},{0, -1},{1, 0},{0, 1}};
void day15part1(char* filename){
	string_list map = string_list_create();
	int_list moves = int_list_create();
	FILE* fp = fopen(filename, "r");
	int line_len;
	DBG("getting input\n");
	get_input(fp, &map, &line_len, &moves);
	fclose(fp);

	int do_break = 0;
	int res = 0;
	position robot, current;
	long i, j;
	DBG("getting robot pos\n");
	for (i = 0; (size_t) i < map.len && !do_break; i++) {
		for (j = 0; j < line_len && !do_break; j++) {
			DBG("%c", map.data[i][j]);
			if (map.data[i][j] == '@'){
				robot.x = j;
				robot.y = i;
				map.data[i][j] = '.';
				do_break = 1;
				DBG("\nrobot position found\n")
			}
		}
		DBG("\n");
	}
	DBG("iterating\n");
	for (i = 0; (size_t) i < moves.len; i++) {
		DBG("robot position: (%ld,%ld)\n", robot.x, robot.y);
		current = add(robot, neighbors[moves.data[i]]);
		if (map.data[current.y][current.x] == '.') {
			robot = current;
			continue;
		}
		while (map.data[current.y][current.x] == 'O') {
			current = add(current, neighbors[moves.data[i]]);
		}
		if (map.data[current.y][current.x] == '.') {
			robot = add(robot, neighbors[moves.data[i]]);
			map.data[current.y][current.x] = map.data[robot.y][robot.x];
			map.data[robot.y][robot.x] = '.';
		}
	}
	DBG("calculating result\n");
	for (i = 0; (size_t) i < map.len; i++) {
		for (j = 0; j < line_len; j++) {
			DBG("%c", map.data[i][j]);
			if (map.data[i][j] == 'O'){
				res += i * 100 + j;
			}
		}
		DBG("\n");
	}
	printf("result: %d\n", res);
	string_list_clean(&map);
	free(map.data);
	free(moves.data);
}
static void get_input_part2(FILE* fp, string_list* list, int* line_len, int_list* moves) {
	if (line_len) *line_len = 0;
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	char* doubled_line;
	int num_chars;
	while ((num_chars = getline(&line, &last_line_len, fp)) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		doubled_line = calloc((last_line_len-1)*2, sizeof(char));
		if (*line_len) {
			if (*line_len != num_chars) {
				printf("malformed input\n");
				exit(1);
			}
		} else {
			*line_len = num_chars;
		}
		for (int i = 0; i < *line_len-1; i++) {
			switch (line[i]) {
				case '#':
					doubled_line[i*2] = '#';
					doubled_line[i*2+1] = '#';
				break;
				case '.':
					doubled_line[i*2] = '.';
					doubled_line[i*2+1] = '.';
				break;
				case 'O':
					doubled_line[i*2] = '[';
					doubled_line[i*2+1] = ']';
				break;
				case '@':
					doubled_line[i*2] = '@';
					doubled_line[i*2+1] = '.';
				break;
			}
		}
		string_list_insert(list, doubled_line);
	}
	*line_len -= 1;
	*line_len *= 2;
	while ((num_chars = getline(&line, &last_line_len, fp)) != -1) {
		for (int i = 0; i < num_chars - 1; i++) {
			switch (line[i]) {
				case 'v':
					int_list_insert(moves, 3);
				break;
				case '^':
					int_list_insert(moves, 1);
				break;
				case '<':
					int_list_insert(moves, 0);
				break;
				case '>':
					int_list_insert(moves, 2);
				break;
			}
		}
	}
	free(line);
}
static char dir_char[4] = {'<','^','>','v'};
static void print_map(string_list map, int line_len, position robot, int dir) {
	for (int i = 0; (size_t) i < map.len; i++) {
		for (int j = 0; j < line_len; j++) {
			if (i == robot.y && j == robot.x) {
				//DBG("\033[0;31m");
				DBG("%c", dir_char[dir]);
				//DBG("\033[0m");
			} else {
				DBG("%c", map.data[i][j]);
			}
		}
		DBG("\n");
	}
}
static int position_list_contains(position_list* list, position a) {
	for (size_t i = 0; i < list->len; i++) {
		if (list->data[i].x == a.x && list->data[i].y == a.y) {
			return 1;
		}
	}
	return 0;
}
void day15part2(char* filename){
	DBG("getting input\n");
	string_list map = string_list_create();
	int_list moves = int_list_create();
	int line_len;
	FILE* fp = fopen(filename, "r");
	get_input_part2(fp, &map, &line_len, &moves);
	fclose(fp);

	int do_break = 0;
	int res = 0;
	position robot, current;
	DBG("getting robot pos\n");
	long i, j;
	for (i = 0; (size_t) i < map.len && !do_break; i++) {
		for (j = 0; j < line_len && !do_break; j++) {
			if (map.data[i][j] == '@'){
				robot.x = j;
				robot.y = i;
				map.data[i][j] = '.';
				do_break = 1;
				DBG("\nrobot position found\n")
			}
		}
	}
	position_list boxes = position_list_create();
	position next;
	DBG("iterating\n");
	for (i = 0; (size_t) i < moves.len; i++) {
		print_map(map, line_len, robot, moves.data[i]);
		/*for (size_t k = 0; (size_t) k < map.len; k++) {
			for (j = 0; j < line_len; j++) {
				DBG("%c", map.data[k][j]);
			}
			DBG("\n");
		}
		*/
		DBG("robot position: (%ld,%ld)\n", robot.x, robot.y);
		current = add(robot, neighbors[moves.data[i]]);
		DBG("moving in dir: %d\n", moves.data[i]);
		DBG("map size: (%d,%ld)\n", line_len, map.len);
		if (map.data[current.y][current.x] == '.') {
			robot = current;
			continue;
		}
		DBG("checking if hor or vert\n");
		if (moves.data[i] % 2 == 0){
			DBG("horizontal move\n");
			while (map.data[current.y][current.x] == '[' || map.data[current.y][current.x] == ']') {
				current = add(current, neighbors[moves.data[i]]);
			}
			DBG("checking if free\n");
			if (map.data[current.y][current.x] == '.') {
				robot = add(robot, neighbors[moves.data[i]]);
				while (current.x != robot.x) {
					DBG("current position: (%ld,%ld)\n", current.x, current.y);
					next = add(current, mul(neighbors[moves.data[i]], - 1));
					map.data[current.y][current.x] = map.data[next.y][next.x];
					current = next;
				}
				map.data[robot.y][robot.x] = '.';
			}
		} else {
			DBG("vertical move\n");
			int landed_in_wall = 0;
			int a;
			if (map.data[current.y][current.x] == '#') {
				continue;
			}
			if (map.data[current.y][current.x] == '[') {
				position_list_insert(&boxes, current);
			} else if (map.data[current.y][current.x] == ']') {
				position_list_insert(&boxes, add(current, neighbors[0]));
			}
			for (a = 0; (size_t) a < boxes.len; a++) {
				for (size_t b = 0; b < boxes.len; b++) {
					DBG("(%ld,%ld), ", boxes.data[b].x, boxes.data[b].y)
				}
				DBG("\n");
				current = add(boxes.data[a], neighbors[moves.data[i]]);
				if (map.data[current.y][current.x] == '#' || map.data[current.y][current.x+1] == '#') {
					landed_in_wall = 1;
					break;
				}
				if (map.data[current.y][current.x] == '[') {
					if (!position_list_contains(&boxes, current))
					position_list_insert(&boxes, current);
				} else {
					if (map.data[current.y][current.x] == ']'){
						next = add(current, neighbors[0]);
						if (!position_list_contains(&boxes, next))
						position_list_insert(&boxes, next);
					}
					if (map.data[current.y][current.x+1] == '['){
						next = add(current, neighbors[2]);
						if (!position_list_contains(&boxes, next))
						position_list_insert(&boxes, next);
					}
				}
			}
			DBG("landed_in_wall: %d\n", landed_in_wall);
			if (landed_in_wall) {
				boxes.len = 0;
				continue;
			}
			for (a = boxes.len-1; a >= 0; a--) {
				DBG("a: %d\n",a);
				current = boxes.data[a];
				next = add(current, neighbors[moves.data[i]]);
				map.data[next.y][next.x] = '[';
				map.data[next.y][next.x+1] = ']';
				map.data[current.y][current.x] = '.';
				map.data[current.y][current.x+1] = '.';
			}
			robot = add(robot, neighbors[moves.data[i]]);
		}
		boxes.len = 0;
	}
	DBG("calculating result\n");
	for (i = 0; (size_t) i < map.len; i++) {
		for (j = 0; j < line_len; j++) {
			if (i == robot.y && j == robot.x) {
				DBG("@");
			} else {
				DBG("%c", map.data[i][j]);
			}
			if (map.data[i][j] == '['){
				res += i * 100 + j;
			}
		}
		DBG("\n");
	}
	free(boxes.data);
	printf("result: %d\n", res);
	string_list_clean(&map);
	free(map.data);
	free(moves.data);
}
