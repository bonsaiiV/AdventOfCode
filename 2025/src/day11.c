#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/list.h"

static list * get_input(char * filename) {
	FILE * fp = fopen(filename, "r");
	list * ret = create_list();
	size_t last_line_len = 8;
	char * line = calloc(last_line_len, sizeof(char));
	int chars_read;
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		list_push(ret, line);
		line = calloc(last_line_len, sizeof(char));
	}
	free(line);
	return ret;
}
typedef struct {
	list * n_ids;
	char * name;
	size_t in_count, path_count;
} node;
void day11part1(char* filename){
	size_t res = 0;
	list * input = get_input(filename);

	node all_nodes[input->size];
	#define OUT_NODE NULL
	char * line, * tok, * node_name;
	node * start, * cur;
	for (size_t n_id = 0; n_id < input->size; n_id++) {
		line = input->data[n_id];
		cur = all_nodes + n_id;
		cur->n_ids = create_list();
		if (!strncmp(line, "fft", 3)) {
			start = cur;
		}
		tok = strtok(line, " ");
		while ((tok = strtok(NULL, " "))) {
			if (!strncmp(tok, "dac", 3)) {
				list_push(cur->n_ids, OUT_NODE);
			}
			LIST_FOR_EACH(node_name, input) {
				if (!strncmp(node_name, tok, 3)) {
					list_push(cur->n_ids, all_nodes + node_name_vi);
				}
			}
		}
	}
	list * front = create_list();
	list_push(front, start);
	node * n_node;
	while (front->size) {
		cur = list_pop(front);
		LIST_FOR_EACH(n_node, cur->n_ids) {
			if (n_node == OUT_NODE) {
				//printf("outnode\n");
				res++;
				continue;
			}
			list_push(front, n_node);
		}
	}


	list_free(input);
	printf("result: %ld\n", res);
}
typedef struct {
	node * node;
	int has_fft, has_dac;
} search_path;
void day11part2(char* filename){
	size_t res = 0;
	list * input = get_input(filename);

	node all_nodes[input->size];
	#define OUT_NODE NULL
	char * line, * tok, * node_name;
	node * start, * cur;
	for (size_t n_id = 0; n_id < input->size; n_id++) {
		line = input->data[n_id];
		cur = all_nodes + n_id;
		cur->n_ids = create_list();
		cur->name = line;
		cur->in_count = 0;
		cur->path_count = 0;
		if (!strncmp(line, "svr", 3)) {
			start = cur;
		}
		tok = strtok(line, " ");
		while ((tok = strtok(NULL, " "))) {
			if (!strncmp(tok, "fft", 3)) {
				list_push(cur->n_ids, OUT_NODE);
			}
			LIST_FOR_EACH(node_name, input) {
				if (!strncmp(node_name, tok, 3)) {
					list_push(cur->n_ids, all_nodes + node_name_vi);
				}
			}
		}
	}
	list * front = create_list();
	list_push(front, start);
	start->in_count = 1;
	start->path_count = 1;
	node * n_node;

	while (front->size) {
		cur = list_pop(front);
		LIST_FOR_EACH(n_node, cur->n_ids) {
			if (n_node == OUT_NODE) {
				continue;
			}
			if (!n_node->in_count){
				list_push(front, n_node);
			}
			n_node->in_count += 1;
		}
	}
	list_push(front, start);
	while (front->size) {
		cur = list_pop(front);
		LIST_FOR_EACH(n_node, cur->n_ids) {
			if (n_node == OUT_NODE) {
				res += cur->path_count;
				continue;
			}
			n_node->path_count += cur->path_count;
			n_node->in_count -= 1;
			if (!n_node->in_count){
				list_push(front, n_node);
			}
		}
	}
	list_free(input);
	printf("result: %ld\n", res);
}
