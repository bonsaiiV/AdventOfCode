
#define DECLARE(DAY_NR)\
void day##DAY_NR##part1(char* filename);\
void day##DAY_NR##part2(char* filename);

#define DAY_STRUCT(DAY_NR) {day##DAY_NR##part1, day##DAY_NR##part2}

DECLARE(01)

struct __attribute__((packed)) day {
	void (*part1) (char input[]);
	void (*part2) (char input[]);
};

struct day daymap[] = {
	DAY_STRUCT(01), 
};
