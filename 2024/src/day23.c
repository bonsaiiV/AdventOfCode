#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


typedef struct __edge{
	char* a;
	char* b;
} edge;
LIST(edge)
typedef char* string;
LIST(string)
LIST_CLEAN(string)

static void get_input(FILE* fp, string_list* list, edge_list* edges) {
	size_t last_line_len = 8;
	char* line = calloc(last_line_len, sizeof(char));
	int chars_read;
	edge next;
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		next.a = strtok(line, "-");
		next.b = strtok(0, "\n");
		edge_list_insert(edges, next);
		string_list_insert(list, line);
		line = calloc(last_line_len, sizeof(char));
	}
	free(line);
}
void day23part1(char* filename){
	string_list input = string_list_create();
	edge_list network = edge_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	get_input(fp, &input, &network);
	fclose(fp);

	DBG("starting solve input\n");
	int res = 0;
	size_t i, j, k;
	char* x, *y, *z;
	for (i = 0; i < network.len; i++) {
		x = network.data[i].a;
		y = network.data[i].b;
		for (j = 0; j < network.len; j++) {
			if (!strcmp(network.data[j].a, y)) {
				z = network.data[j].b;
			} else if (!strcmp(network.data[j].b, y)) {
				z = network.data[j].a;
			} else {
				continue;
			}
			for (k = 0; k < network.len; k++) {
				if (!strcmp(network.data[k].a, z)) {
					if (strcmp(network.data[k].b, x)) {
						continue;
					}
				} else if (!strcmp(network.data[k].b, z)) {
					if (strcmp(network.data[k].a, x)) {
						continue;
					}
				} else {
					continue;
				}
				if (*x == 't' || *y == 't' || *z == 't') {
					DBG("clique found: %s - %s - %s\n", x, y, z);
					res++;
				}
			}

		}
	}
	string_list_clean(&input);
	free(input.data);
	free(network.data);

	printf("result: %d\n", res/3);
}
void recurse_clique(string_list* addresses, edge_list* network, string_list* current_clique, string_list* best_clique, size_t min_add_index){
	DBG("depth: %ld\n", current_clique->len);
	int found;
	for (size_t i = min_add_index; i < addresses->len; i++) {
		char* candidate = addresses->data[i];
		for (size_t j = 0; j < current_clique->len; j++) {
			found = 0;
			for (size_t k = 0; k < network->len; k++) {
				if (!strcmp(network->data[k].a, candidate) && !strcmp(network->data[k].b, current_clique->data[j])) {
					found = 1;
					break;
				}
				if (!strcmp(network->data[k].b, candidate) && !strcmp(network->data[k].a, current_clique->data[j])) {
					found = 1;
					break;
				}
			}
			if (!found) break;
		}
		if (found) {
			string_list_insert(current_clique, candidate);
			if (best_clique->len < current_clique->len) {
				best_clique->len = 0;
				for (size_t k = 0; k < current_clique->len; k++) {
					string_list_insert(best_clique, current_clique->data[k]);
				}
			}
			recurse_clique(addresses, network, current_clique, best_clique, i+1); 
			current_clique->len--;
		}
	}
}
static int addcmp(const void* a, const void* b){
	return strcmp(*(char**)a, *(char**)b);
}
void day23part2(char* filename){
	string_list input = string_list_create();
	edge_list network = edge_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	get_input(fp, &input, &network);
	fclose(fp);

	DBG("starting solve input\n");
	size_t i, j;
	char* x, *y;
	int x_found, y_found;
	string_list addresses = string_list_create();
	for (i = 0; i < network.len; i++) {
		x = network.data[i].a;
		y = network.data[i].b;
		x_found = 0;
		y_found = 0;
		for (j = 0; j < addresses.len; j++) {
			if (!strcmp(addresses.data[j], x))
				x_found = 1;
			if (!strcmp(addresses.data[j], y))
				y_found = 1;
		}
		if (!x_found) {
			string_list_insert(&addresses, x);
		}
		if (!y_found) {
			string_list_insert(&addresses, y);
		}
	}
	string_list current_clique = string_list_create();
	string_list best_clique = string_list_create();
	recurse_clique(&addresses, &network, &current_clique, &best_clique, 0);
	qsort(best_clique.data, best_clique.len, sizeof(char*), addcmp);
	printf("best clique len: %ld\n", best_clique.len);
	for (i = 0; i < best_clique.len; i++) {
		printf("%s,", best_clique.data[i]);
	}
	printf("\n");


	string_list_clean(&input);
	free(input.data);
	free(network.data);
	free(best_clique.data);
	free(current_clique.data);

}
