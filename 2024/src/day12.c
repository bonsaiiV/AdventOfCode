#include <stdio.h>
#include <stdlib.h>
#include "globals.h"


typedef char* string;
LIST(string)
LIST_CLEAN(string)

typedef struct triple {
	int pos;
	int start;
	int end;
}fence;
static fence merge(fence a, fence b) {
	fence ret = {a.pos, a.start, b.end};
	return ret;
}
LIST(fence)
static void fence_list_remove(fence_list* list, size_t i) {
	list->len -= 1;
	if (i == list->len) return;
	list->data[i] = list->data[list->len];
}
static void reduce_fence_list(fence_list* list) {
	size_t a, b;
	int work_to_do = 1;
	while (work_to_do) {
		work_to_do = 0;
		for (a = 0; a < list->len; a++) {
			for (b = 0; b < list->len; b++) {
				if (a == b) continue;
				if (list->data[a].pos == list->data[b].pos) {
					if (list->data[a].end == list->data[b].start-1) {
						list->data[a] = merge(list->data[a], list->data[b]);
						fence_list_remove(list, b);
						b--;
						work_to_do = 1;
						continue;
					}
					if (list->data[b].end == list->data[a].start-1) {
						list->data[a] = merge(list->data[b], list->data[a]);
						fence_list_remove(list, b);
						b--;
						work_to_do = 1;
						continue;
					}
				}
			}
		}
	}
}

typedef struct toople {
	int row;
	int col;
}point;
static point add(point a, point b) {
	point ret = {a.row + b.row, a.col + b.col};
	return ret;
}
LIST(point)
static int pop(point_list* list, point* ret) {
	if (!list->len) return 0;
	list->len -= 1;
	*ret = list->data[list->len];
	return 1;
}


static void get_input(FILE* fp, string_list* list, int* line_len) {
	if (line_len) *line_len = 0;
	int chars_read;
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		if (line_len) {
			if (*line_len) {
				if (*line_len != chars_read) {
					printf("malformed input\n");
					exit(1);
				}
			} else {
				*line_len = chars_read;
			}
		}
		string_list_insert(list, line);
		line = calloc(last_line_len, sizeof(char));
	}
	*line_len -= 1;
	free(line);
}
static int in_bounds( point p, size_t max_row, int max_col) {
	return p.row >= 0 && p.col >= 0 && (size_t) p.row < max_row && p.col < max_col;
}
static point neighbors[4] = {{-1, 0},{0, -1},{1, 0},{0, 1}};
void day12part1(char* filename){
	size_t i, k;
	int j;
	string_list lines = string_list_create();
	point_list front = point_list_create();
	FILE* fp = fopen(filename, "r");
	int res = 0;
	int line_len;
	DBG("getting input\n");
	get_input(fp, &lines, &line_len);
	fclose(fp);
	int* plot_mapped = calloc(lines.len * line_len, sizeof(int));
	int area, perimeter;
	for (i = 0; i < lines.len; i++) {
		for (j = 0; j < line_len; j++) {
			if (plot_mapped[i * line_len + j]) continue;
			area = 0;
			perimeter = 0;
			point current = {i, j};
			point_list_insert(&front, current);
			plot_mapped[i * line_len + j] = 1;
			while (pop(&front, &current)){
				DBG("considering %c at (%d,%d)\n", lines.data[current.row][current.col], current.row, current.col);
				area++;
				for (k = 0; k < 4; k++){
					point tmp = add(current, neighbors[k]);
					if (!in_bounds(tmp, lines.len, line_len) || lines.data[current.row][current.col] != lines.data[tmp.row][tmp.col]) {
						perimeter++;
						continue;
					}
					if (plot_mapped[tmp.row * line_len + tmp.col]) continue;
					plot_mapped[tmp.row * line_len + tmp.col] = 1;
					point_list_insert(&front, tmp);
				}
			}
			DBG("- A region of %c plants with price %d * %d = %d\n", lines.data[i][j], area, perimeter, area*perimeter);
			res += area*perimeter;
		}
	}
	free(plot_mapped);
	free(front.data);
	string_list_clean(&lines);
	free(lines.data);
	printf("result: %d\n", res);
}
void day12part2(char* filename){
	string_list lines = string_list_create();
	FILE* fp = fopen(filename, "r");
	int line_len;
	DBG("getting input\n");
	get_input(fp, &lines, &line_len);
	fclose(fp);

	int* plot_mapped = calloc(lines.len * line_len, sizeof(int));
	int area, perimeter;
	fence tmp_fence;
	point_list front = point_list_create();
	fence_list fences[4];
	size_t i, k, a;
	int j;
	for (k = 0; k < 4; k++){
		fences[k] = fence_list_create();
	}
	int res = 0;
	for (i = 0; i < lines.len; i++) {
		for (j = 0; j < line_len; j++) {
			if (plot_mapped[i * line_len + j]) continue;
			area = 0;
			perimeter = 0;
			point current = {i, j};
			point_list_insert(&front, current);
			plot_mapped[i * line_len + j] = 1;
			while (pop(&front, &current)){
				area++;
				for (k = 0; k < 4; k++){
					point tmp = add(current, neighbors[k]);
					if (!in_bounds(tmp, lines.len, line_len) || lines.data[current.row][current.col] != lines.data[tmp.row][tmp.col]) {
						if (k%2) {
							tmp_fence.pos = current.col;
							tmp_fence.start = current.row;
							tmp_fence.end = current.row;
						} else {
							tmp_fence.pos = current.row;
							tmp_fence.start = current.col;
							tmp_fence.end = current.col;
						}
						fence_list_insert(fences + k, tmp_fence);

						continue;
					}
					if (plot_mapped[tmp.row * line_len + tmp.col]) continue;
					plot_mapped[tmp.row * line_len + tmp.col] = 1;
					point_list_insert(&front, tmp);
				}
			}
			for (k = 0; k < 4; k++) {
				reduce_fence_list(fences + k);
				DBG("fences after in dir %ld\n", k);
				for (a = 0; a < fences[k].len; a++) {
					DBG("(%d, %d, %d)\n", fences[k].data[a].pos, fences[k].data[a].start, fences[k].data[a].end);
				}
				perimeter += fences[k].len;
				fences[k].len = 0;
			}
			DBG("- A region of %c plants with price %d * %d = %d\n", lines.data[i][j], area, perimeter, area*perimeter);
			res += area*perimeter;
		}
	}
	for (k = 0; k < 4; k++){
		free(fences[k].data);
	}
	free(plot_mapped);
	free(front.data);
	printf("result: %d\n", res);
	string_list_clean(&lines);
	free(lines.data);
}
