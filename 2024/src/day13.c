#include <stdio.h>
#include <stdlib.h>
#include "globals.h"


typedef struct toople {
	long x;
	long y;
} position;
static position add(position a, position b) {
	position ret = {a.x + b.x, a.y + b.y};
	return ret;
}
static position mul(position a, long b) {
	position ret = {a.x * b, a.y * b};
	return ret;
}
typedef struct __machiene {
	position a;
	position b;
	position prize;
}machiene;
LIST(machiene)

static void get_input(FILE* fp, machiene_list* list) {
	long tmp;
	position a, b, prize;
	while (1){
		if (fscanf(fp, "Button A: X+%ld, Y+%ld\n", &a.x, &a.y) <= 0) {
			printf("no button a found in machiene %ld\n", list->len);
			return;
		}
		if (fscanf(fp, "Button B: X+%ld, Y+%ld\n", &b.x, &b.y) <= 0) {
			printf("no button b found in machiene %ld\n", list->len);
			return;
		}
		if (fscanf(fp, "Prize: X=%ld, Y=%ld\n", &prize.x, &prize.y) <= 0) {
			printf("prize found in machiene %ld\n", list->len);
			return;
		}
		if (b.x < b.y) {
			tmp = b.x;
			b.x = b.y;
			b.y = tmp;
			tmp = a.x;
			a.x = a.y;
			a.y = tmp;
			tmp = prize.x;
			prize.x = prize.y;
			prize.y = tmp;
		}
		machiene next = {a, b, prize};
		machiene_list_insert(list, next);
	}
}
void day13part1(char* filename){
	size_t i;
	machiene_list input = machiene_list_create();
	FILE* fp = fopen(filename, "r");
	int res = 0;
	position a, b, target;
	long count_b, count_a;
	DBG("getting input\n");
	get_input(fp, &input);
	for (i = 0; i < input.len; i++) {
		count_b = 100;
		count_a = 100;
		a = input.data[i].a;
		b = input.data[i].b;
		DBG("a: (%ld, %ld) b: (%ld,%ld)\n", a.x,a.y,b.x,b.y);
		target = input.data[i].prize;
		if (a.x > b.x*3 && a.y > b.y*3) printf("pressing a is cheaper\n");
		if(count_b > target.x / b.x){
			count_b = target.x / b.x;
		}
		if(count_b > target.y / b.y){
			count_b = target.y / b.y;
		}
		target = add(target, mul(b, -count_b));
		if(count_a > target.x / a.x){
			count_a = target.x / a.x;
		}
		if(count_a > target.y / a.y){
			count_a = target.y / a.y;
		}
		a = mul(a, -1);
		target = add(target, mul(a, count_a));
		while (count_b > 0 && count_a <= 100) {
			DBG("target: (%ld,%ld) count_a: %ld, count_b: %ld\n", target.x, target.y, count_a, count_b);
			if (target.x == 0 && target.y == 0) {
				res += count_a * 3 + count_b;
				break;
			}
			count_b--;
			target = add(target, b);
			while (target.x >= -a.x && target.y >= -a.y) {
				target = add(target, a);
				count_a++;
			}
		}
	}
	fclose(fp);
	printf("result: %d\n", res);
	free(input.data);
}
void day13part2(char* filename){
	size_t i;
	machiene_list input = machiene_list_create();
	FILE* fp = fopen(filename, "r");
	long res = 0;
	position a, b, target;
	//position offset = {10000000000000, 10000000000000};
	position offset = {0, 0};
	long tmp;
	long count_b, count_a;
	DBG("getting input\n");
	get_input(fp, &input);
	for (i = 0; i < input.len; i++) {
		a = input.data[i].a;
		b = input.data[i].b;
		DBG("a: (%ld, %ld) b: (%ld,%ld)\n", a.x,a.y,b.x,b.y);
		target = add(input.data[i].prize, offset);
		if (a.x > b.x*3 && a.y > b.y*3) printf("pressing a is cheaper\n");
		tmp = b.x;
		b.x *= b.y;
		a.x *= b.y;
		target.x *= b.y;
		b.y *= tmp;
		a.y *= tmp;
		target.y *= tmp;
		a.y -= a.x;
		target.y -= target.x;
		count_a = target.y / a.y;
		if (count_a * a.y != target.y) continue;
		target.x -= a.x * count_a;
		count_b = target.x / b.x;
		if (count_b * b.x != target.x) continue;
		if (count_b < 0 || count_a < 0) continue;
		DBG("count_a: %ld count_b: %ld\n", count_a, count_b);
		res += count_a * 3 + count_b;
	}
	fclose(fp);
	printf("result: %ld\n", res);
	free(input.data);

}
