#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

typedef struct toople {
	int row;
	int col;
} position;
static position add(position a, position b) {
	position ret = {a.row + b.row, a.col + b.col};
	return ret;
}

static int position_equal(position a, position b) {
	if ((a.row == b.row) & (a.col == b.col)) return 1;
	return 0;
}

typedef char* string;
LIST(position)
LIST_CONTAINS(position)
LIST(string)
LIST_CLEAN(string)

static void get_input(FILE* fp, string_list* list, int* line_len) {
	if (line_len) *line_len = 0;
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	int chars_read;
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		string_list_insert(list, line);
		line = calloc(last_line_len, sizeof(char));
		if (!line_len) continue;
		if (*line_len){
			if (*line_len != chars_read) {
				printf("malformed input\n");
				exit(1);
			}
		} else {
			*line_len = chars_read;
		}
	}
	*line_len -= 1;
	free(line);
}
int test_pos(string_list* input, position pos, int height, int line_len) {
	if (pos.col < 0) return 0;
	if (pos.row < 0) return 0;
	if ((size_t) pos.row >= input->len) return 0;
	if (pos.col >= line_len) return 0;
	if (input->data[pos.row][pos.col]-'0' == height) return 1;
	return 0;
}
static position neighbours[4] = {{-1, 0}, {0, - 1}, {1, 0}, {0, 1}};
void day10part1(char* filename){
	size_t i, k, ni;
	int j;
	string_list lines = string_list_create();
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		perror("");
		exit(1);
	}
	int res = 0;
	DBG("getting input\n");
	int line_len;
	get_input(fp, &lines, &line_len);
	DBG("finished getting input\n");
	fclose(fp);
	position_list trail = position_list_create();
	position_list tmp, new_trail = position_list_create();
	position pos, current;
	int height;
	DBG("starting to iterate\n");
	for (i = 0; i < lines.len; i++) {
		for( j = 0; j < line_len; j++) {
			if (lines.data[i][j]-'0') continue;
			DBG("trying trailhead at %ld, %d\n", i, j);
			height = 1;
			pos.row = i;
			pos.col = j;
			position_list_insert(&trail, pos);
			while (height < 10 && trail.len) {
				for (k = 0; k < trail.len; k++) {
					current = trail.data[k];
					for (ni = 0; ni < 4; ni++) {
						pos = add(neighbours[ni], current);
						DBG("checking %d, %d next\n", pos.row, pos.col);
						if (test_pos(&lines, pos, height, line_len) && !position_list_contains(&new_trail, pos))
							position_list_insert(&new_trail, pos);
					}
				}
				DBG("at height %d are %ld remaining trails\n", height, new_trail.len);
				tmp = trail;
				trail = new_trail;
				new_trail = tmp;
				new_trail.len = 0;
				height++;
			}
			res += trail.len;
			trail.len = 0;
			new_trail.len = 0;
		}
	}
	printf("result: %d\n", res);
	free(trail.data);
	free(new_trail.data);
	string_list_clean(&lines);
	free(lines.data);
}
void day10part2(char* filename){
	size_t i, k, ni;
	int j, line_len;
	string_list lines = string_list_create();
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		perror("");
		exit(1);
	}
	int res = 0;
	DBG("getting input\n");
	get_input(fp, &lines, &line_len);
	DBG("finished getting input\n");
	fclose(fp);
	position_list trail = position_list_create();
	position_list tmp, new_trail = position_list_create();
	position pos, current;
	int height;
	DBG("starting to iterate\n");
	for (i = 0; i < lines.len; i++) {
		for( j = 0; j < line_len; j++) {
			if (lines.data[i][j]-'0') continue;
			DBG("trying trailhead at %ld, %d\n", i, j);
			height = 1;
			pos.row = i;
			pos.col = j;
			position_list_insert(&trail, pos);
			while (height < 10 && trail.len) {
				for (k = 0; k < trail.len; k++) {
					current = trail.data[k];
					for (ni = 0; ni < 4; ni++) {
						pos = add(neighbours[ni], current);
						if (test_pos(&lines, pos, height, line_len))
							position_list_insert(&new_trail, pos);
					}
				}
				tmp = trail;
				trail = new_trail;
				new_trail = tmp;
				new_trail.len = 0;
				height++;
			}
			res += trail.len;
			trail.len = 0;
			new_trail.len = 0;
		}
	}
	printf("result: %d\n", res);
	free(trail.data);
	free(new_trail.data);
	string_list_clean(&lines);
	free(lines.data);
}
