#ifndef LAUNCH_SPU_H
#define LAUNCH_SPU_H

#include "../../stack/include/const_define_struct.h"
#include "../../commands.h"

const size_t MAX_LETTERS     = 30;
const size_t DEFAULT_LEN_STK = 32;

const size_t SIZE_REG = 8;
const size_t SIZE_RAM = 32;
const size_t MAX_BIT  = sizeof (long) * 8;

const int SIZE_HEADER   = 3;
const int SIGNATURE     = 0xC0FFEE;
const int VERSION       = 1;  

const long  IMM_MASK     = 0x20;
const long  REG_MASK     = 0x40;
const long  RAM_MASK     = 0x80;
const long  COMMAND_MACK = 0x1f;

enum error_t
{
	ERROR_IN_HEADER   = -3,
	NOT_ERROR_IN_SPU  = 0,
	CMD_NULL          = 1,
	REG_NULL          = 2,
	RAM_NULL          = 4,
	SIZE_REG_EXCEED   = 8,
	SIZE_RAM_EXCEED   = 16,
	NOT_FIND_GUIDE    = 32,
	INCORRECT_COMMAND = 64,

};

typedef int header_t;
typedef int cmd_t;
typedef int reg_t;
typedef int ram_t;

struct spu_t
{
	cmd_t*  cmd;
	reg_t*  reg;
	ram_t*  ram;
	stk_t   stk;
	size_t  size_cmd;
	long    error_in_spu;
};

long launch_spu (FILE* cmd_file);

#endif