#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


typedef struct _input{
	int filled;
	char* source;
	int val;
} gate_input;
typedef struct __gate{
	gate_input a;
	gate_input b;
	char* output_name;
	int output_val;
	int fired;
	int (*operation) (int, int);
} gate;
typedef struct _constant{
	int val;
	char* name;
} constant;
typedef struct _swap {
	size_t index_a;
	size_t index_b;
} swap;
LIST(swap)
LIST(constant)
LIST(gate)
LIST(size_t)
typedef char* string;
LIST(string)
LIST_CLEAN(string)

static int or (int a, int b) {
	return a || b;
}
static int and (int a, int b) {
	return a && b;
}
static int xor (int a, int b) {
	return a != b;
}

static void get_input(FILE* fp, string_list* input, constant_list* constants, gate_list* gates) {
	size_t last_line_len = 8;
	char* tok, *line = calloc(last_line_len, sizeof(char));
	constant parsed_constant;
	while (getline(&line, &last_line_len, fp) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}
		tok = strtok(line, ": ");
		if (!tok) {
			printf("error parsing constant name\n");
			exit(1);
		}
		parsed_constant.name = tok;
		tok = strtok(0, "\n");
		if (!tok) {
			printf("error parsing constant val\n");
			exit(1);
		}
		parsed_constant.val = (*(tok+1)) - '0';
		constant_list_insert(constants, parsed_constant);
		string_list_insert(input, line);
		line = calloc(last_line_len, sizeof(char));
	}
	gate parsed_gate;
	parsed_gate.fired = 0;
	gate_input parsed_input;
	parsed_input.filled = 0;
	while (getline(&line, &last_line_len, fp) != -1) {
		parsed_input.source = strtok(line, " ");
		parsed_gate.a = parsed_input;
		tok = strtok(0, " ");
		if (!strcmp(tok, "OR")) {
			parsed_gate.operation = or;
		} else if (!strcmp(tok, "XOR")) {
			parsed_gate.operation = xor;
		} else if (!strcmp(tok, "AND")) {
			parsed_gate.operation = and;
		} else {
			printf("error parsing gate operation\n");
			exit(1);
		}

		parsed_input.source = strtok(0, " -> ");
		parsed_gate.b = parsed_input;
		parsed_gate.output_name = strtok(0, " -> ");
		*(parsed_gate.output_name + 3) = 0;
		gate_list_insert(gates, parsed_gate);
		string_list_insert(input, line);
		line = calloc(last_line_len, sizeof(char));
	}
	free(line);
}
void apply_logic(constant_list* constants, gate_list* gates) {
	size_t i, j;
	int work_to_do = 1;
	size_t last_constant_index = 0;
	constant current_const;
	gate* current_gate;
	DBG("number of gates: %ld\n", gates->len);
	while (work_to_do) {
		DBG("number of constants: %ld\n", constants->len);
		work_to_do = 0;
		for (i = last_constant_index; i < constants->len; i++) {
			current_const = constants->data[i];
			DBG("filling in %d for %s\n", current_const.val, current_const.name);
			for (j = 0; j < gates->len; j++) {
				current_gate = &gates->data[j];
				if (!strcmp(current_const.name, current_gate->a.source)) {
					current_gate->a.val = current_const.val;
					current_gate->a.filled = 1;
				}
				if (!strcmp(current_const.name, current_gate->b.source)) {
					current_gate->b.val = current_const.val;
					current_gate->b.filled = 1;
				}
			}
		}
		last_constant_index = constants->len;
		for (j = 0; j < gates->len; j++) {
			current_gate = &gates->data[j];
			if (current_gate->fired) {
				continue;
			}
			if (current_gate->a.filled && current_gate->b.filled) {
				work_to_do = 1;
				current_gate->output_val = current_gate->operation(current_gate->a.val, current_gate->b.val);
				current_const.val = current_gate->output_val;
				current_const.name = current_gate->output_name;
				constant_list_insert(constants, current_const);
				current_gate->fired = 1;
				DBG("result of firering with %d,%d from %s,%s: %s has value %d\n", current_gate->a.val, current_gate->b.val, current_gate->a.source, current_gate->b.source, current_const.name, current_const.val);
			}
		}
	}
}
void day24part1(char* filename){
	string_list input = string_list_create();
	constant_list constants = constant_list_create();
	gate_list gates = gate_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	get_input(fp, &input, &constants, &gates);
	fclose(fp);

	DBG("starting to solve\n");
	apply_logic(&constants, &gates);
	constant_list tmp = constant_list_create();
	size_t i;
	for (i = 0; i < constants.len; i++) {
		if ((*constants.data[i].name) == 'z') {
			constant_list_insert(&tmp, constants.data[i]);
		}
	}
	int outputs[tmp.len];
	for (i = 0; i < tmp.len; i++) {
		outputs[atoi(tmp.data[i].name + 1)] = tmp.data[i].val;
	}
	long res = 0;
	for (i = 0; i < tmp.len; i++) {
		printf("%d", outputs[i]);
		res += (long)outputs[i] << i;
	}
	putchar('\n');

	string_list_clean(&input);
	free(input.data);
	free(constants.data);
	free(gates.data);
	free(tmp.data);

	printf("result: %ld\n", res);
}
long const_to_l(constant_list* constants, char c) {
	constant_list tmp = constant_list_create();
	size_t i;
	for (i = 0; i < constants->len; i++) {
		if ((*constants->data[i].name) == c) {
			constant_list_insert(&tmp, constants->data[i]);
		}
	}
	long ret = 0;
	for (i = 0; i < tmp.len; i++) {
		ret += (long)tmp.data[i].val << i;
	}
	free(tmp.data);
	return ret;
}
int varcmp(const void* a, const void* b) {
	return strcmp(*(char**)a, *(char**)b);
}
int get_wrong_bits(long a, long b) {
	int wrong_bits = 0;
	long diff = a ^ b;
	while (diff) {
		wrong_bits += 1 & diff;
		diff = diff >> 1;
	}
	return wrong_bits;
}
void day24part2(char* filename){
	string_list input = string_list_create();
	constant_list constants = constant_list_create();
	gate_list gates = gate_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	get_input(fp, &input, &constants, &gates);
	fclose(fp);

	size_t number_of_consts = constants.len;
	long x = const_to_l(&constants, 'x');
	long y = const_to_l(&constants, 'y');
	long sum = x + y;

	DBG("starting to solve\n");
	int i;
	size_t j, k, l;
	char* tmp;
	int perm_valid = 1;
	int d = dbg;
	dbg = 0;
	apply_logic(&constants, &gates);
	dbg = d;
	long res = const_to_l(&constants, 'z');
	int base_wrong_bits = get_wrong_bits(res, sum);
	swap_list candidates = swap_list_create();
	for (j = 0; j < gates.len; j++) {
		for (k = j+1; k < gates.len; k++) {
			constants.len = number_of_consts;
			for (l = 0; l < gates.len; l++) {
				gates.data[l].fired = 0;
				gates.data[l].a.filled = 0;
				gates.data[l].b.filled = 0;
			}
			tmp = gates.data[j].output_name;
			gates.data[j].output_name = gates.data[k].output_name;
			gates.data[k].output_name = tmp;
			int d = dbg;
			dbg = 0;
			apply_logic(&constants, &gates);
			dbg = d;
			if (get_wrong_bits(const_to_l(&constants, 'z'), sum) < base_wrong_bits) {
				swap candidate = {j, k};
				swap_list_insert(&candidates, candidate);
			}
			tmp = gates.data[j].output_name;
			gates.data[j].output_name = gates.data[k].output_name;
			gates.data[k].output_name = tmp;
		}
	}
	for (j = 0; j < candidates.len; j++){
		printf("candidate: %ld,%ld\n", candidates.data[j].index_a, candidates.data[j].index_b);
	}
	size_t_list reset_points = size_t_list_create();
	size_t last_index_a = candidates.data[0].index_a;
	size_t_list_insert(&reset_points, 0);
	for (j = 0; j < candidates.len; j++) {
		if (candidates.data[j].index_a != last_index_a) {
			last_index_a = candidates.data[j].index_a;
			size_t_list_insert(&reset_points, j);
		}
	}
	size_t current_reset_points[4] = {0, 1, 2, 3};
	size_t swaps[4] = {0, reset_points.data[1], reset_points.data[2], reset_points.data[3]};
	while (res != sum) {
		perm_valid = 1;
		DBG("\ngate count: %ld\n", gates.len);
		DBG("swap indexes: ");
		for (i = 0; i < 4; i++) {
			swap candidate1 = {candidates.data[swaps[i]].index_a, candidates.data[swaps[i]].index_b};
			DBG("%ld, ", candidate1.index_a);
			DBG("%ld, ", candidate1.index_b);
			for ( j = i+1; j < 4; j++) {
				swap candidate2 = {candidates.data[swaps[j]].index_a, candidates.data[swaps[j]].index_b};
				if ( candidate1.index_a == candidate2.index_a || 
					candidate1.index_b == candidate2.index_b || 
					candidate1.index_a == candidate2.index_b || 
					candidate1.index_b == candidate2.index_a ) {
					perm_valid = 0;
				}
			}
		}
		DBG("\n");
		if (perm_valid) {
			//swap
			for (i = 0; i < 4; i++) {
				swap candidate = candidates.data[swaps[i]];
				tmp = gates.data[candidate.index_a].output_name;
				gates.data[candidate.index_a].output_name = gates.data[candidate.index_b].output_name;
				gates.data[candidate.index_b].output_name = tmp;
			}

			//calculate permutation without dbg
			int d = dbg;
			dbg = 0;
			apply_logic(&constants, &gates);
			dbg = d;
			//check permutation
			res = const_to_l(&constants, 'z');
			if (sum == res) {
				string_list swaped = string_list_create();
				for (i = 0; i < 4; i++) {
					string_list_insert(&swaped, gates.data[candidates.data[swaps[i]].index_a].output_name);
					string_list_insert(&swaped, gates.data[candidates.data[swaps[i]].index_b].output_name);
				}
				qsort(swaped.data, 8, sizeof(char*), varcmp);
				for (i = 0; i < 8; i++) {
					printf("%s,", swaped.data[i]); 
				}
				printf("\n");
				break;
			}

			//unswap
			for (i = 0; i < 4; i++) {
				swap candidate = candidates.data[swaps[i]];
				tmp = gates.data[candidate.index_a].output_name;
				gates.data[candidate.index_a].output_name = gates.data[candidate.index_b].output_name;
				gates.data[candidate.index_b].output_name = tmp;
			}

			//reset
			constants.len = number_of_consts;
			for (l = 0; l < gates.len; l++) {
				gates.data[l].fired = 0;
				gates.data[l].a.filled = 0;
				gates.data[l].b.filled = 0;
			}
		}
		//get next permutation
		i = 3;
		while (i >= 0) {
			DBG("%d\n", i);
			swaps[i]++;
			if (i != 3 && swaps[i] == reset_points.data[i+1]) {
				reset_points.data[i+1] += 1;
			}
			if (i == 3) {
				if (swaps[i] == candidates.len) {
					i--;
				} else {
					break;
				}
			} else if (swaps[i] == reset_points.data[reset_points.len - 3 + i]) {
				i--;
			} else {
				break;
			}
		}
		DBG("i: %d\n", i);
		if (i < 0) {
			printf("no solution found\n");
			break;
		}
		while (++i < 4) {
			swaps[i] = reset_points.data[current_reset_points[i]];
			current_reset_points[i+1] = current_reset_points[i+1];
		}
	}

	string_list_clean(&input);
	free(input.data);
	free(constants.data);
	free(gates.data);
}
