#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data/list.h"


typedef struct {
	size_t * affected;
	size_t n_affected;
} op;

typedef struct {
	size_t target_lights;
	size_t num_l;
	size_t * ops;
	op * s_ops;
	size_t op_count;
	size_t * target_joltage;
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
		m->s_ops = calloc(op_cap, sizeof(op));
		m->op_count = 0;
		while ((token = strtok_r(NULL, " ", &save_ptr))) {
			//printf("%s\n", token);
			if (*(token++) == '(') {
				if (m->op_count == op_cap) {
					op_cap = op_cap * 3 / 2 + 1;
					m->ops = reallocarray(m->ops, op_cap, sizeof(size_t));
					m->s_ops = reallocarray(m->s_ops, op_cap, sizeof(op));
				}
				size_t n_affected = 0;
				m->ops[m->op_count] = 0;
				m->s_ops[m->op_count].affected = calloc(m->num_l, sizeof(size_t));

				t2 = strtok(token, ",)");
				do {
					size_t l_index = atoi(t2);
					m->ops[m->op_count] += 1 << l_index;
					m->s_ops[m->op_count].affected[n_affected++] = l_index;
				} while ((t2 = strtok(NULL, ",)")));

				m->s_ops[m->op_count].n_affected = n_affected;

				m->op_count++;
			} else {
				size_t i_j = 0;
				m->target_joltage = calloc(m->num_l, sizeof(size_t));

				t2 = strtok(token, ",}");
				do {
					m->target_joltage[i_j] = atoi(t2);
					i_j++;
				} while ((t2 = strtok(NULL, ",)")));
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
size_t solve_joltage(const size_t levels_count, const size_t * target, const size_t num_ops, const op * ops, const size_t budget) {
	size_t i, j;
	size_t sum_joltage = 0;
	for(i = 0; i < levels_count; i++) {
		sum_joltage += target[i];
	}
	// end if there are no more ops to test
	if (num_ops == 0 ) {
		if (sum_joltage == 0) {
			return 0;
		} else {
			return budget;
		}
	}
	const op c_op = ops[0];
	if (budget * c_op.n_affected <= sum_joltage) {
		return budget;
	}
	size_t cur_joltage, min_relevant_joltage = sum_joltage;
	for (i = 0; i < c_op.n_affected; i++) {
		cur_joltage = target[c_op.affected[i]];
		if (cur_joltage < min_relevant_joltage) {
			min_relevant_joltage = cur_joltage;
		}
	}
	size_t new_target[levels_count];
	size_t relevant_op_count[levels_count];
	for (i = 0; i < levels_count; i++) {
		new_target[i] = target[i];
		relevant_op_count[i] = 0;
	}
	op relevant_op[levels_count];
	size_t af_i, af_joltage;
	for (i = 1; i < num_ops; i++) {
		for (af_i = 0; af_i < ops[i].n_affected; af_i++) {
			af_joltage = ops[i].affected[af_i];
			relevant_op_count[af_joltage]++;
			relevant_op[af_joltage] = ops[i];
		}
	}
	int iterate = 1;
	op rel_op;
	for (i = 0; i < c_op.n_affected; i++) {
		af_joltage = c_op.affected[i];
		new_target[af_joltage] -= min_relevant_joltage;
		if (relevant_op_count[af_joltage] == 0) {
			iterate = 0;
			if (new_target[af_joltage]) {
				return budget;
			}
		}
		if (relevant_op_count[af_joltage] == 1) {
			rel_op = relevant_op[af_joltage];
			for (af_i = 0; af_i < rel_op.n_affected; af_i++) {
				if (new_target[rel_op.affected[af_i]] < new_target[af_joltage]) {
					return budget;
				}
			}
		}
	}
	size_t new_budget = budget - min_relevant_joltage;
	size_t ret_budget = budget;
	size_t sub_res;
	if (!iterate) {
		sub_res = solve_joltage(levels_count, new_target, num_ops - 1, ops + 1, new_budget);
		return min_relevant_joltage + sub_res;
	}
	//printf("bound of op: %lu\n\n", min_relevant_joltage);
	for (i = 0; i <= min_relevant_joltage; i++) {
		sub_res = solve_joltage(levels_count, new_target, num_ops - 1, ops + 1, new_budget);
		if (sub_res < new_budget) {
			ret_budget -= (new_budget - sub_res);
			new_budget = sub_res;
		}
		for (j = 0; j < c_op.n_affected; j++) {
			new_target[c_op.affected[j]] += 1;
		}
		new_budget += 1;

	}
	return ret_budget;
}
int cmp_op (op * a, op * b) {
	return b->n_affected - a->n_affected;
}
void day10part2(char* filename){
	size_t m_res, res = 0;
	list * input = get_input(filename);

	machine * m;
	size_t i;

	size_t skip_to = 0;
	LIST_FOR_EACH(m, input) {
		if (loop.i < skip_to) {
			loop.i = skip_to;
			m = input->data[loop.i];
		}//*/
		size_t sum_joltage = 0;
		for(i = 0; i < m->num_l; i++) {
			sum_joltage += m->target_joltage[i];
		}
		qsort(m->s_ops, m->op_count, sizeof(op), (int (*) (const void *, const void *)) cmp_op);
		sum_joltage /= m->s_ops[m->op_count - 1].n_affected;
		m_res = solve_joltage(m->num_l, m->target_joltage, m->op_count, m->s_ops, sum_joltage + 1);
		printf("m_res[%lu]: %lu\n", loop.i, m_res);
		res += m_res;
	}

	list_free_all(input);
	printf("result: %ld\n", res);
}
