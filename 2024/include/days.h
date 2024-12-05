
void day01part1(char input[]);
void day01part2(char input[]);
void day02part1(char input[]);
void day02part2(char input[]);
void day03part1(char input[]);
void day03part2(char input[]);
void day04part1(char input[]);
void day04part2(char input[]);
void day05part1(char input[]);
void day05part2(char input[]);


struct __attribute__((packed)) day {
	void (*part1) (char input[]);
	void (*part2) (char input[]);
};

struct day daymap[] = {
	{day01part1, day01part2},
	{day02part1, day02part2},
	{day03part1, day03part2},
	{day04part1, day04part2},
	{day05part1, day05part2},
};
