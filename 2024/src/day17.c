#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


typedef struct __instruction {
	int opcode;
	int operand;
} instruction;
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
static int evaluate_instruction(program* prog, instruction next, int* ip, int_list* output) {
	long long next_output;
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
				return 0;
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
	*ip += 1;
	return 0;
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
	free(prog.instructions.data);
}
void day17part2(char* filename){
	program prog;
	FILE* fp = fopen(filename, "r");
	DBG("getting input\n");
	get_input(fp, &prog);
	fclose(fp);
	int_list output = int_list_create();
	int ip = 0;
	long long a = 1;
	size_t i;
	for (i = 0; i < (2 * prog.instructions.len)-1; i++) {
		a = a << 3;
	}
	DBG("a max: %lld\n", a*3);
	prog.reg_a = a;
	while (1) {
		while ((size_t) ip < prog.instructions.len) {
			DBG("a: %lld\n", a);
			DBG("\tip: %d\n", ip);
			DBG("\tprog: %lld, %lld, %lld\n", prog.reg_a, prog.reg_b, prog.reg_c);
			DBG("\top: %d %d\n\n", prog.instructions.data[ip].opcode, prog.instructions.data[ip].operand);
			evaluate_instruction(&prog, prog.instructions.data[ip], &ip, &output);
		}
		if (output.len == prog.instructions.len*2) {
			int success = 1;
			for (i = 0; i < output.len; i++) {
				long long next_output = output.data[i];
				long long next_expected_code = ((int*) prog.instructions.data)[i];
				if ( next_expected_code != next_output){
					success = 0;
					DBG("failed attempt as: %lld is not equal to %lld\n", next_output, next_expected_code);
				}
			}
			if (success) {
				break;
			}
		}
		++a;
		output.len = 0;
		prog.reg_a = a;
		prog.reg_b = 0;
		prog.reg_c = 0;
		ip = 0;
	}
	printf("result: %lld\n",a);
	for (size_t i = 0; i<output.len; i++) {
		printf("%d,", output.data[i]);
	}
	printf("\n");
	free(prog.instructions.data);
}
