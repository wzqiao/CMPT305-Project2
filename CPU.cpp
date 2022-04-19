#include "CPU.h"
#include "all_global_values.h"

int integer_ALU;
int floating_point_unit;
int branch_execution_unit;
int is_reading;
int is_writing;

int circles = 0;
int type_count[5] = { 0,0,0,0,0 };

void use_integer_ALU()
{
	//0 is free, 1 is using
	integer_ALU = 1;
}

void use_floating_point_unit()
{
	//0 is free, 1 is using
	floating_point_unit = 1;
}

void use_branch_execution_unit()
{
	//0 is free, 1 is using
	branch_execution_unit = 1;
}

void read_port()
{
	//0 is free, 1 is using
	is_reading = 1;
}

void write_report()
{
	//0 is free, 1 is using
	is_writing = 1;
}
