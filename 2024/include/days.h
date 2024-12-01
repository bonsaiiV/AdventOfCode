
void day01part1(char input[]);
void day01part2(char input[]);


struct __attribute__((packed)) day {
	void (*part1) (char input[]);
	void (*part2) (char input[]);
	char* name;
};

struct day daymap[] = {
	{day01part1, day01part2, "day01"},
};
