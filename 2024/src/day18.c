#include <stdio.h>
#include <stdlib.h>
#include "globals.h"


typedef struct __position{
	int x;
	int y;
} position;
LIST(position)

static void get_input(FILE* fp, position_list* list, position* size) {
	if (fscanf(fp, "size: %d,%d\n", &size->x, &size->y)!=2){
		printf("error while getting size\n");
		exit(1);
	}
	position next;
	while (fscanf(fp, "%d,%d\n", &next.x, &next.y) == 2) {
		position_list_insert(list, next);
	}
}
static position neightbors[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
static position pos_add(position a, position b) {
	position ret = {a.x + b.x, a.y + b.y};
	return ret;
}
static int in_bounds( position p, position size) {
	return p.x >= 0 && p.y >= 0 && p.y < size.y && p.x < size.x;
}
static void print_mem( int* memory, position size) {
	if (!dbg) return;
	size_t i, j;
	int val;
	for (i = 0; i < (size_t) size.y; i++) {
		for (j = 0; j < (size_t) size.x; j++) {
			val = memory[j + i * size.x];
			if (val == -1) {
				putchar('#');
			} else if (!val) {
				putchar('.');
			} else {
				putchar('O');
			}
		}
		putchar('\n');
	}
}
void day18part1(char* filename){
	size_t i;
	position_list bytes = position_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	position size;
	get_input(fp, &bytes, &size);
	fclose(fp);
	int* memory = calloc(size.x * size.y, sizeof(int));
	size_t max_time = 1024;
	if (size.x == 7) max_time = 12;
	position next_byte;
	for (i = 0; i < max_time; i++) {
		next_byte = bytes.data[i];
		memory[next_byte.x + next_byte.y * size.x] = -1;
	}
	position_list front = position_list_create();
	position_list tmp, next_front = position_list_create();
	position current = {0, 0};
	position_list_insert(&front, current);
	memory[0] = 1;
	int dist = 0;
	while (front.len) {
		dist++;
		DBG("\ndist: %d\n", dist);
		for (i = 0; i < front.len; i++) {
			current = front.data[i];
			DBG("current: (%d,%d)\n", current.x, current.y);
			for (size_t dir = 0; dir < 4; dir++) {
				next_byte = pos_add(current, neightbors[dir]);
				if (in_bounds(next_byte, size) && !memory[next_byte.x + next_byte.y * size.x]) {
					memory[next_byte.x + next_byte.y * size.x] = dist;
					position_list_insert(&next_front, next_byte);
				}
			}
		}
		tmp = front;
		front = next_front;
		next_front = tmp;
		next_front.len = 0;
	}
	print_mem(memory, size);
	printf("result: %d\n", memory[size.x * size.y - 1]);
	free(memory);
	free(bytes.data);
	free(front.data);
	free(next_front.data);
}
static void mem_reset(int* mem, position size) {
	for (int i = 0; i < size.x * size.y; i++) {
		mem[i] = 0;
	}
}
void day18part2(char* filename){
	position_list bytes = position_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	position size;
	get_input(fp, &bytes, &size);
	fclose(fp);
	int* memory = calloc(size.x * size.y, sizeof(int));
	position next_byte, next;
	position_list front = position_list_create();
	position_list tmp, next_front = position_list_create();
	position current;
	memory[0] = 1;
	int reachable;
	size_t i, j;
	for (i = 0; i < bytes.len; i++) {
		mem_reset(memory, size);
		for (j = 0; j <= i; j++) {
			next_byte = bytes.data[j];
			memory[next_byte.x + next_byte.y * size.x] = -1;
		}
		print_mem(memory, size);
		front.len = 0;
		current.x = 0;
		current.y = 0;
		position_list_insert(&front, current);
		reachable = 0;
		DBG("next_byte: (%d,%d)\n", next_byte.x, next_byte.y);
		while (front.len && !reachable) {
			for (j = 0; j < front.len; j++) {
				current = front.data[j];
				if (current.x == size.x - 1 && current.y == size.y - 1) {
					reachable = 1;
					break;
				}
				DBG("current: (%d,%d)\n", current.x, current.y);
				for (size_t dir = 0; dir < 4; dir++) {
					next = pos_add(current, neightbors[dir]);
					if (in_bounds(next, size) && !memory[next.x + next.y * size.x]) {
						memory[next.x + next.y * size.x] = 1;
						position_list_insert(&next_front, next);
					}
				}
			}
			tmp = front;
			front = next_front;
			next_front = tmp;
			next_front.len = 0;
		}
		if (!reachable) {
			break;
		}
	}
	printf("result: %d,%d\n", next_byte.x, next_byte.y);
	free(bytes.data);
	free(front.data);
	free(next_front.data);
	free(memory);
}
