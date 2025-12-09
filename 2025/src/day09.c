#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/list.h"

typedef struct {
	ssize_t x, y;
} p2;
static list * get_input(char * filename) {
	FILE * fp = fopen(filename, "r");
	list * ret = create_list();
	size_t last_line_len = 8;
	char * line = calloc(last_line_len, sizeof(char));
	p2 * point = malloc(sizeof(p2));
	int chars_read;
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		sscanf(line, "%ld,%ld", &point->x, &point->y);
		list_push(ret, point);
		point = malloc(sizeof(p2));
	}
	free(line);
	return ret;
}
void day09part1(char* filename){
	size_t res = 0;
	list * input = get_input(filename);

	p2 * a, * b;
	ssize_t max = 0, dx, dy, area;
	LIST_FOR_EACH(a, input) {
		LIST_FOR_EACH(b, input) {
			if (a->x > b->x) {
				dx = a->x + 1 - b->x;
			} else {
				dx = b->x + 1 - a->x;
			}
			if (a->y > b->y) {
				dy = a->y + 1 - b->y;
			} else {
				dy = b->y + 1 - a->y;
			}
			area = dx * dy;
			if (area > max) {
				max = area;
			}
		}
	}
	res = max;

	list_free_all(input);
	printf("result: %ld\n", res);
}
void day09part2(char* filename){
	size_t res = 0;
	list * input = get_input(filename);

	p2 * a, * b;
	ssize_t max = 0, min_x, min_y, max_x, max_y, area;
	p2 * c, * d;
	int bad;
	LIST_FOR_EACH(a, input) {
		LIST_FOR_EACH(b, input) {
			if (a->x > b->x) {
				min_x = b->x;
				max_x = a->x;
			} else {
				min_x = a->x;
				max_x = b->x;
			}
			if (a->y > b->y) {
				min_y = b->y;
				max_y = a->y;
			} else {
				min_y = a->y;
				max_y = b->y;
			}
			area = (max_x + 1 - min_x) * (max_y + 1 - min_y);
			if (area < max) {
				continue;
			}
			//printf("(%ld, %ld, %ld, %ld): %ld\n", min_x, min_y, max_x, max_y, area);
			bad = 0;
			for (size_t i = 0; i < input->size; i++) {
				c = input->data[i];
				d = input->data[(i+1) % input->size];
				//printf("\t(%ld,%ld)\n", c->x, c->y);
				//printf("\t\t%d, %d, %d, %d\n", c->x > min_x, c->x < max_x, c->y > min_y, c->y < max_y);
				if (c->x > min_x && c->x < max_x && c->y > min_y && c->y < max_y) {
					bad = 1;
					break;
				}
				if (c->x == d->x) {
					if (c->x > min_x && c->x < max_x) {
						if (c->y <= min_y && d->y >= max_y) {
							bad = 1;
							break;
						}
						if (d->y <= min_y && c->y >= max_y) {
							bad = 1;
							break;
						}
					}
				} else {
					if (c->y > min_y && c->y < max_y) {
						if (c->x <= min_x && d->x >= max_x) {
							bad = 1;
							break;
						}
						if (d->x <= min_x && c->x >= max_x) {
							bad = 1;
							break;
						}
					}
				}
			}
			if (bad) {
				continue;
			}
			max = area;
		}
	}
	res = max;

	list_free_all(input);
	printf("result: %ld\n", res);
}
