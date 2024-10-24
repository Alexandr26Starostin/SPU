#ifndef RUN_SPU_H
#define RUN_SPU_H

#include "../../stack/include/const_define_struct.h"
#include "../../commands.h"

enum error_t
{
	BAD                = 0,
	NICE               = 1
};

const size_t MAX_LETTERS     = 30;
const size_t DEFAULT_LEN_STK = 32;
const size_t COUNT_REGISTERS = 4;
const size_t SIZE_RAM        = 32;

struct spu_t
{
	command_t*  commands;
	long*       ptr_reg;
	size_t      size_commands;
	stk_t       stk;
	long*       ptr_ram;
};

void run_spu (FILE* cmd_file);

#endif