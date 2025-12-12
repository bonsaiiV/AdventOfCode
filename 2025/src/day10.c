#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/list.h"


typedef struct {
	size_t target_lights;
	size_t num_l;
	size_t * ops;
	size_t op_count;
} machine;

static list * get_input(char * filename) {
	FILE * fp = fopen(filename, "r");
	list * ret = create_list();
	size_t last_line_len = 8;
	char * line = calloc(last_line_len, sizeof(char));
	int chars_read;
	char * token, * t2;
	machine * m;
	size_t light_flag;
	size_t op_cap = 10;
	char * save_ptr = 0;
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		m = malloc(sizeof(machine));
		m->num_l = 0;
		token = strtok_r(line, " ", &save_ptr) + 1;
		//printf("%s\n", token);
		light_flag = 1;
		m->target_lights = 0;
		while (*token != ']') {
			//printf("%lb\n", light_flag);
			m->num_l += 1;
			if (*(token++) == '#') {
				m->target_lights += light_flag;
			}
			light_flag <<= 1;
		}
		//printf("%lb\n", m->target_lights);
		//printf("\n");
		m->ops = calloc(op_cap, sizeof(size_t));
		m->op_count = 0;
		while ((token = strtok_r(NULL, " ", &save_ptr))) {
			//printf("%s\n", token);
			if (*(token++) == '(') {
				if (m->op_count == op_cap) {
					op_cap = op_cap * 3 / 2 + 1;
					m->ops = reallocarray(m->ops, op_cap, sizeof(size_t));
				}
				m->ops[m->op_count] = 0;

				t2 = strtok(token, ",)");
				do {
					m->ops[m->op_count] += 1 << atoi(t2);
				} while ((t2 = strtok(NULL, ",)")));

				m->op_count++;
			}
		}
		list_push(ret, m);
	}
	free(line);
	return ret;
}
void day10part1(char* filename){
	size_t res = 0;
	list * input = get_input(filename);
	machine * m;
	LIST_FOR_EACH(m, input) {
		if (!m->target_lights) {
			continue;
		}
		size_t min_flips = m->op_count;
		for(size_t flips = 0; flips < (((size_t) 1) << m->op_count); flips++) {
			size_t lights = 0;
			size_t flip_count = 0;
			for (size_t o_i = 0; o_i < m->op_count; o_i++) {
				if ((((size_t) 1) << o_i) & flips) {
					flip_count++;
					lights ^= m->ops[o_i];
				}
			}
			if (lights == m->target_lights && flip_count < min_flips) {
				min_flips = flip_count;
			}
		}
		res += min_flips;
	}
	list_free_all(input);
	printf("result: %ld\n", res);
}
size_t solve_joltage(size_t levels_count, size_t * target, size_t num_ops, size_t ** ops) {
	//max_
}
void day10part2(char* filename){
	(void) filename;
}
