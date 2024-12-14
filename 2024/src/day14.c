#include <stdio.h>
#include <stdlib.h>
#include "globals.h"


typedef struct toople {
	long x;
	long y;
}position;
LIST(position)

static position add(position a, position b) {
	position ret = {a.x + b.x, a.y + b.y};
	return ret;
}
static position mul(position a, long b) {
	position ret = {a.x * b, a.y * b};
	return ret;
}
static void get_input(FILE* fp, position_list* currents, position_list* vels) {
	position current, vel;
	while (fscanf(fp, "p=%ld,%ld v=%ld,%ld\n", &current.x, &current.y, &vel.x, &vel.y) > 0) {
		DBG("current: (%ld,%ld) vel: (%ld,%ld)\n", current.x, current.y, vel.x, vel.y);
		position_list_insert(currents, current);
		position_list_insert(vels, vel);
	}
}
void day14part1(char* filename){
	size_t quad_1 = 0, quad_2 = 0, quad_3 = 0, quad_4 = 0;
	FILE* fp = fopen(filename, "r");
	int res = 0;
	position current, vel, end;
	position board = {101,103};
	DBG("getting input\n");
	while (fscanf(fp, "p=%ld,%ld v=%ld,%ld\n", &current.x, &current.y, &vel.x, &vel.y) > 0) {
		DBG("current: (%ld,%ld) vel: (%ld,%ld)\n", current.x, current.y, vel.x, vel.y);
		end = add(current, mul(vel, 100));
		DBG("end: (%ld,%ld)\n", end.x, end.y);
		end.x %= board.x;
		end.y %= board.y;
		if (end.x < 0) end.x += board.x;
		if (end.y < 0) end.y += board.y;
		DBG("end: (%ld,%ld)\n", end.x, end.y);
		if (end.x < (board.x)/2) {
			if (end.y < (board.y)/2) {
				quad_1++;
			} else if (end.y > (board.y)/2) {
				quad_2++;
			}
		} else if (end.x > (board.x)/2) {
			if (end.y < (board.y)/2) {
				quad_3++;
			} else if (end.y > (board.y)/2) {
				quad_4++;
			}
		}
	}
	fclose(fp);
	DBG("quadrants: %ld, %ld, %ld, %ld\n", quad_1, quad_2, quad_3, quad_4);
	res = quad_1 * quad_2 * quad_3 * quad_4;
	printf("result: %d\n", res);
}
void day14part2(char* filename){
	size_t i;
	int j, k;
	FILE* fp = fopen(filename, "r");
	position_list currents = position_list_create(), vels = position_list_create();
	get_input(fp, &currents, &vels);
	position board = {101,103};
	int count, it = 0;
	while (it < 6439) {
		for (i = 0; i < currents.len; i++) {
			currents.data[i] = add(currents.data[i], mul(vels.data[i], 6439));
			currents.data[i].x %= board.x;
			currents.data[i].y %= board.y;
			if (currents.data[i].x < 0) currents.data[i].x += board.x;
			if (currents.data[i].y < 0) currents.data[i].y += board.y;
		}
		it += 6439;
	}
	do {
		for (j = 0; j < board.y; j++) {
			for (k = 0; k < board.x; k++) {
				count = 0;
				for (i = 0; i < currents.len; i++) {
					if (currents.data[i].x == k && currents.data[i].y == j) count++;
				}
				if (!count) putchar('.');
				else if (count<10) printf("%d", count);
				else putchar('#');
			}
			putchar('\n');
		}
		putchar('\n');
		printf("iteration %d\n", it);
		it += 103;
		for (i = 0; i < currents.len; i++) {
			currents.data[i] = add(currents.data[i], mul(vels.data[i], 103));
			currents.data[i].x %= board.x;
			currents.data[i].y %= board.y;
			if (currents.data[i].x < 0) currents.data[i].x += board.x;
			if (currents.data[i].x > board.x) currents.data[i].x -= board.x;
			if (currents.data[i].y < 0) currents.data[i].y += board.y;
			if (currents.data[i].y > board.y) currents.data[i].y -= board.y;
		}

	} while (getchar());
}
