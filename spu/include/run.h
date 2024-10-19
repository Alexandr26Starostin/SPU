#ifndef RUN_H
#define RUN_H

#include "../../stack/include/const_define_struct.h"
#include "../../commands.h"

struct spu_t
{
	command_t*  commands;
	long*       ptr_reg;
	size_t      size_commands;
	stk_t       stk;
	long*       ptr_ram;
};

void run (FILE* code_file);

#endif