
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
DECLARE(07)
DECLARE(08)
DECLARE(09)
DECLARE(10)
DECLARE(11)
DECLARE(12)
DECLARE(13)
DECLARE(14)
DECLARE(15)
DECLARE(16)
DECLARE(17)
DECLARE(18)
DECLARE(19)


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
	DAY_STRUCT(07), 
	DAY_STRUCT(08), 
	DAY_STRUCT(09), 
	DAY_STRUCT(10), 
	DAY_STRUCT(11), 
	DAY_STRUCT(12), 
	DAY_STRUCT(13), 
	DAY_STRUCT(14), 
	DAY_STRUCT(15), 
	DAY_STRUCT(16), 
	DAY_STRUCT(17), 
	DAY_STRUCT(18), 
	DAY_STRUCT(19), 
};
