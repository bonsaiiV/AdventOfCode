
#define DECLARE(DAY_NR)\
void day##DAY_NR##part1(char* filename);\
void day##DAY_NR##part2(char* filename);

#define DAY_STRUCT(DAY_NR) {day##DAY_NR##part1, day##DAY_NR##part2}

DECLARE(01)
DECLARE(02)
DECLARE(03)
DECLARE(04)
DECLARE(05)
DECLARE(06)


struct __attribute__((packed)) day {
	void (*part1) (char input[]);
	void (*part2) (char input[]);
};

struct day daymap[] = {
	DAY_STRUCT(01), 
	DAY_STRUCT(02), 
	DAY_STRUCT(03), 
	DAY_STRUCT(04), 
	DAY_STRUCT(05), 
	DAY_STRUCT(06), 
};
