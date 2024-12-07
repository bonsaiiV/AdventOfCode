#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

struct list {
	long long* data;
	size_t cap;
	size_t len;
};
static struct list list_create(void) {
	long long* data = calloc(8, sizeof(long long));
	struct list ret = {data, 8, 0};
	return ret;
}
static void list_insert(struct list* list, long long elem) {
	if (list->cap == list->len) {
		list->cap *= 2;
		list->data = reallocarray(list->data, list->cap, sizeof(long long));
	}
	list->data[list->len++] = elem;
}
/*static void get_input(FILE* fp, struct list* list) {
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	while (getline(&line, &last_line_len, fp) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		list_insert(list, line);
		line = calloc(last_line_len, sizeof(char));
	}
	free(line);
}*/
void day07part1(char* filename){
	size_t i;
	struct list values = list_create();
	struct list tmp, new_values = list_create();
	DBG("values: %p\n", (void*) values.data);
	DBG("new_values: %p\n", (void*) new_values.data);
	FILE* fp = fopen(filename, "r");
	long long val, sum, res = 0;
	int found;
	size_t last_line_len = 8;
	char* tok, *line = calloc(last_line_len, sizeof(char));
	while (getline(&line, &last_line_len, fp) != -1) {
		DBG("%s", line);
		tok = strtok(line, ":");
		sum = atoll(tok);
		tok = strtok(0, " ");
		list_insert(&values, atoll(tok));
		while ((tok = strtok(0, " "))) {
			val = atoi(tok);
			for (i = 0; i < values.len; i++) {
				list_insert(&new_values, val + values.data[i]); 
				list_insert(&new_values, val * values.data[i]); 
			}
			tmp = values;
			values = new_values;
			new_values = tmp;
			new_values.len = 0;
		}
		found = 0;
		for (i = 0; i < values.len; i++) {
			DBG("%lld, ", values.data[i]);
			if (values.data[i] == sum) {
				found = 1;
				break;
			}
		}
		DBG("\n");
		if (found) {
			res += sum;
			DBG(" added\n");
		} else {
			DBG(" ignored\n");
		}
		values.len = 0;
	}
	free(line);
	printf("result: %lld\n", res);
	DBG("free values: %p\n", (void*) values.data);
	free(values.data);
	DBG("free values: %p\n", (void*) new_values.data);
	free(new_values.data);
}
void day07part2(char* filename){
	size_t i;
	struct list values = list_create();
	struct list tmp_list, new_values = list_create();
	FILE* fp = fopen(filename, "r");
	long long val, sum, tmp, res = 0;
	int found;
	size_t last_line_len = 8;
	char* tok, *line = calloc(last_line_len, sizeof(char));
	while (getline(&line, &last_line_len, fp) != -1) {
		DBG("%s", line);
		tok = strtok(line, ":");
		sum = atoll(tok);
		tok = strtok(0, " ");
		list_insert(&values, atoll(tok));
		while ((tok = strtok(0, " "))) {
			val = atoi(tok);
			for (i = 0; i < values.len; i++) {
				tmp = 10;
				while (tmp <= val) {
					tmp *= 10;
				}
				list_insert(&new_values, val + values.data[i]); 
				list_insert(&new_values, val * values.data[i]); 
				list_insert(&new_values, val + values.data[i]*tmp); 
			}
			tmp_list = values;
			values = new_values;
			new_values = tmp_list;
			new_values.len = 0;
		}
		found = 0;
		for (i = 0; i < values.len; i++) {
			DBG("%lld, ", values.data[i]);
			if (values.data[i] == sum) {
				found = 1;
				break;
			}
		}
		DBG("\n");
		if (found) {
			res += sum;
			DBG("\tadded\n");
		} else {
			DBG("\tignored\n");
		}
		values.len = 0;
	}
	fclose(fp);
	free(line);
	printf("result: %lld\n", res);
	free(values.data);
	free(new_values.data);
}
