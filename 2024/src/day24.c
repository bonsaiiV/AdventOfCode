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
void print_gate(gate a) {
	if (a.operation == or) {
		printf("%s OR %s -> %s\n", a.a.source, a.b.source, a.output_name);
	} else if (a.operation == xor) {
		printf("%s XOR %s -> %s\n", a.a.source, a.b.source, a.output_name);
	} else if (a.operation == and) {
		printf("%s AND %s -> %s\n", a.a.source, a.b.source, a.output_name);
	} else {
		printf("bad gate\n");
	}
}
void day24part2(char* filename){
	string_list input = string_list_create();
	constant_list constants = constant_list_create();
	gate_list gates = gate_list_create();
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	get_input(fp, &input, &constants, &gates);
	fclose(fp);

	DBG("starting to solve\n");
	gate current_gate;
	char* var;
	size_t i, j;
	// actually solved it on paper but this helped for queries
	for (i = 0; i < gates.len; i++) {
		current_gate = gates.data[i];
		if (current_gate.operation == xor) {
			if (!(*current_gate.a.source == 'x' || *current_gate.b.source == 'x')) continue;
			var = current_gate.output_name;
			printf("%s\n", var);
			for (j = 0; j < gates.len; j++) {
				if (!strcmp(gates.data[j].a.source, var)) {
					print_gate(gates.data[j]);
				}
				if (!strcmp(gates.data[j].b.source, var)) {
					print_gate(gates.data[j]);
				}
			}
		}
	}

	string_list_clean(&input);
	free(input.data);
	free(constants.data);
	free(gates.data);
}
