#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

#define DBG(...) if (dbg) printf(__VA_ARGS__);

struct rule {
	int first;
	int second;
};
struct rules_list {
	struct rule* data;
	size_t cap;
	size_t len;
};
static struct rules_list rules_create(void) {
	struct rule* data = calloc(8, sizeof(struct rule));
	struct rules_list ret = {data, 8, 0};
	return ret;
}
static void rules_insert(struct rules_list* list, struct rule elem) {
	if (list->cap == list->len) {
		list->cap *= 2;
		list->data = reallocarray(list->data, list->cap, sizeof(struct rule));
	}
	list->data[list->len++] = elem;
}
struct update {
	int* data;
	size_t cap;
	size_t len;
};
static struct update update_create(void) {
	int* data = calloc(8, sizeof(int));
	struct update ret = {data, 8, 0};
	return ret;
}
static void update_insert(struct update* list, int elem) {
	if (list->cap == list->len) {
		list->cap *= 2;
		list->data = reallocarray(list->data, list->cap, sizeof(int));
	}
	list->data[list->len++] = elem;
}
static int check_update(struct rules_list rules, struct update update, size_t* index_second, size_t* index_first) {
	size_t i, j;
	int second_found;
	for (i = 0; i < rules.len; i++) {
		second_found = 0;
		for (j = 0; j < update.len; j++) {
			if (second_found && update.data[j] == rules.data[i].first) {
				DBG("failed rule %d|%d\n", rules.data[i].first, rules.data[i].second);
				if (index_first)*index_first = j;
				return 0;
			}
			if (update.data[j] == rules.data[i].second) {
				DBG("found second of rule %d|%d at index %ld\n", rules.data[i].first, rules.data[i].second, i);
				if (index_second)*index_second = j;
				second_found = 1;
			}
		}
	}
	DBG("adding: %d\n", update.data[update.len / 2]);
	return update.data[update.len / 2];
}

static void get_rules(FILE* fp, struct rules_list* rules) {
	size_t last_line_len = 8;
	char* tok, *line = calloc(last_line_len, sizeof(char));
	struct rule rule;
	while (getline(&line, &last_line_len, fp) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		DBG("creating rule: %s\n", line);
		tok = strtok(line, "|");
		rule.first = atoi(tok);
		tok = strtok(0, "|");
		rule.second = atoi(tok);
		rules_insert(rules, rule);
	}
	free(line);
}
void day05part1(char* filename){
	dbg = 0;
	struct rules_list rules = rules_create();
	FILE* fp = fopen(filename, "r");
	int res = 0;
	DBG("getting rules\n");
	get_rules(fp, &rules);
	size_t last_line_len = 8;
	char* tok, *line = calloc(last_line_len, sizeof(char));
	struct update update = update_create();
	while (getline(&line, &last_line_len, fp) != -1) {
		DBG("line: %s\n", line);
		tok = strtok(line, ",");
		do {
			update_insert(&update, atoi(tok));
		} while ((tok = strtok(0, ",")));
		res += check_update(rules, update, 0, 0);
		update.len = 0;
	}
	printf("result: %d\n", res);
	free(rules.data);
}
void day05part2(char* filename){
	dbg = 0;
	struct rules_list rules = rules_create();
	FILE* fp = fopen(filename, "r");
	int mid, tmp, res = 0;
	get_rules(fp, &rules);
	int do_add;
	size_t i, j, last_line_len = 8;
	char* tok, *line = calloc(last_line_len, sizeof(char));
	struct update update = update_create();
	while (getline(&line, &last_line_len, fp) != -1) {
		do_add = 0;
		tok = strtok(line, ",");
		do {
			update_insert(&update, atoi(tok));
		} while ((tok = strtok(0, ",")));
		while (!(mid = check_update(rules, update, &i, &j))){
			do_add = 1;
			tmp = update.data[i];
			update.data[i] = update.data[j];
			update.data[j] = tmp;
		}
		res += mid * do_add;
		update.len = 0;
	}
	printf("result: %d\n", res);
	free(rules.data);
}
