#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


typedef struct __instruction {
	int opcode;
	int operand;
} instruction;
typedef long long ll;
LIST(ll)
LIST(instruction)
LIST(int)
typedef struct __program {
	long long reg_a;
	long long reg_b;
	long long reg_c;
	instruction_list instructions;
} program;

static void get_input(FILE* fp, program* prog) {
	fscanf(fp, "Register A: %lld\n", &prog->reg_a);
	fscanf(fp, "Register B: %lld\n", &prog->reg_b);
	fscanf(fp, "Register C: %lld\n", &prog->reg_c);
	size_t last_line_len = 8;
	char* tok, *line = calloc(last_line_len, sizeof(char));
	if (getline(&line, &last_line_len, fp) == -1) {
		printf("error getting instruction line\n");
		exit(1);
	}
	DBG("%s\n", line);
	instruction next;
	prog->instructions = instruction_list_create();
	tok = strtok(line, " ");
	while ((tok = strtok(0, ","))){
		next.opcode = atoi(tok);
		if (!(tok = strtok(0, ","))) {
			printf("error splitting instructions\n");
		}
		next.operand = atoi(tok);
		instruction_list_insert(&prog->instructions, next);
	}
	free(line);
}
static long get_combo_operand(program* prog, int operand) {
	if (operand < 4) return operand;
	switch (operand) {
		case 4:
			return prog->reg_a;
		break;
		case 5:
			return prog->reg_b;
		break;
		case 6:
			return prog->reg_c;
		break;
	}
	printf("invalid operand\n");
	exit(1);
	return 0;
}
static void evaluate_instruction(program* prog, instruction next, int* ip, int_list* output) {
	long long next_output;
	*ip += 1;
	switch (next.opcode) {
		case 0:
			prog->reg_a = prog->reg_a >> get_combo_operand(prog, next.operand);
		break;
		case 1:
			prog->reg_b = prog->reg_b ^ next.operand;
		break;
		case 2:
			prog->reg_b = get_combo_operand(prog,next.operand) % 8;
		break;
		case 3:
			if (prog->reg_a) {
				*ip = next.operand;
			}
		break;
		case 4:
			prog->reg_b = prog->reg_b ^ prog->reg_c;
		break;
		case 5:
			next_output = get_combo_operand(prog, next.operand)%8;
			int_list_insert(output, next_output);
		break;
		case 6:
			prog->reg_b = prog->reg_a >> get_combo_operand(prog, next.operand);
		break;
		case 7:
			prog->reg_c = prog->reg_a >> get_combo_operand(prog, next.operand);
		break;
		default:
			printf("\ninvalid opcode\n");
			exit(1);
		break;
	}
}
void day17part1(char* filename){
	program prog;
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	get_input(fp, &prog);
	fclose(fp);
	int_list output = int_list_create();
	int ip = 0;
	while ((size_t) ip < prog.instructions.len) {
		DBG("ip: %d\n", ip);
		DBG("prog: %lld, %lld, %lld\n", prog.reg_a, prog.reg_b, prog.reg_c);
		DBG("op: %d %d\n\n", prog.instructions.data[ip].opcode, prog.instructions.data[ip].operand);
		evaluate_instruction(&prog, prog.instructions.data[ip], &ip, &output);
	}
	for (size_t i = 0; i<output.len; i++) {
		printf("%d,", output.data[i]);
	}
	printf("\n");
	free(output.data);
	free(prog.instructions.data);
}
static void solve_last_n(program* prog, int n, ll_list* start_a, int_list* output, ll_list* ret) {
	ret->len = 0;
	for (size_t j = 0; j < start_a->len; j++) {
		long long a = start_a->data[j] << 3;
		for (int i = 0; i < 8; i++) {
			prog->reg_a = a;
			prog->reg_b = 0;
			prog->reg_c = 0;
			output->len = 0;
			int ip = 0;
			while (!output->len) {
				DBG("ip: %d\n", ip);
				DBG("prog: %lld, %lld, %lld\n", prog->reg_a, prog->reg_b, prog->reg_c);
				DBG("op: %d %d\n\n", prog->instructions.data[ip].opcode, prog->instructions.data[ip].operand);
				evaluate_instruction(prog, prog->instructions.data[ip], &ip, output);
			}
			if (a && output->data[0] == ((int*) prog->instructions.data)[n]) {
				ll_list_insert(ret, a);
			}
			a++;
		}
	}
}
void day17part2(char* filename){
	program prog;
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	get_input(fp, &prog);
	fclose(fp);
	int_list output = int_list_create();
	size_t prog_len = prog.instructions.len * 2;
	ll_list a_candidates = ll_list_create();
	ll_list_insert(&a_candidates, 0);
	ll_list tmp, next_a_candidates = ll_list_create();
	for (size_t i = 0; i < prog_len; i++) {
		solve_last_n(&prog, prog_len - 1 - i, &a_candidates, &output, &next_a_candidates);
		if (!next_a_candidates.len) {
			printf("no solution found after position %ld\n", prog_len - 1 -i);
			exit(1);
		}
		tmp = a_candidates;
		a_candidates = next_a_candidates;
		next_a_candidates = tmp;
	}
	for (size_t i = 0; i<a_candidates.len; i++) {
		printf("%lld\n", a_candidates.data[i]);
	}
	printf("\n");
	free(prog.instructions.data);
	free(a_candidates.data);
	free(next_a_candidates.data);
	free(output.data);
}
