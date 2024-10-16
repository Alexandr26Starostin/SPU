#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

const size_t MAX_LETTERS     = 50;
const size_t DEFAULT_LEN_STK = 64;
const size_t SIZE_HEADER     = 1;

#include "../../stack/include/const_define_struct.h"
#include "../../stack/include/hash.h"
#include "../../stack/include/operations_with_stack.h"
#include "../../stack/include/stk_error.h"

#include "../include/const.h"
#include "../../commands.h"
#include "../include/translation.h"

int static  read_file  (stk_t* ptr_cmd, FILE* cmd_file);
int static  write_file (stk_t* ptr_cmd, FILE* code_file);

#define COMPARE_STR_CMD(str, name_cmd, cmd)                                        \
	if (strcmp (str, name_cmd) == 0)                                               \
	{                                                                              \
		stk_push (ptr_cmd, cmd);                                                   \
		continue;                                                                  \
	}              

#define COMPARE_OM_JUMP(str, name_cmd, cmd)      \
	if (strcmp (str, name_cmd) == 0)             \
	{                                            \
		stk_push (ptr_cmd, cmd);                 \
                                                 \
		int arg = 0;                             \
		fscanf  (cmd_file, "%x", &arg);          \
                                                 \
		stk_push (ptr_cmd, arg);                 \
		continue;                                \
	}

#define DEBUG_NOT

#ifdef DEBUG
	void static print_cmd (stk_t* ptr_cmd);  
#endif

//-------------------------------------------------------------------------------------------------------------

int translation (FILE* cmd_file, FILE* code_file)
{
	assert (cmd_file);
	assert (code_file);

	stk_t cmd = {};
	if (stk_ctor (&cmd, DEFAULT_LEN_STK) != 0) {abort ();} //!

	for (size_t index = 0; index < SIZE_HEADER; index++)
	{
		stk_push (&cmd, 0);
	}

	if (read_file (&cmd, cmd_file) == BAD)
	{
		fprintf  (code_file, "BAD_FILE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		fclose   (code_file);
		fclose   (cmd_file);
		stk_dtor (&cmd);

		return BAD;
	}

	*(cmd.data + 0) = (int) (cmd.size - SIZE_HEADER);

	if (write_file (&cmd, code_file) ==  BAD)
	{
		fclose   (code_file);
		fclose   (cmd_file);
		stk_dtor (&cmd);

		return BAD;
	}

	fclose (code_file);
	fclose (cmd_file);
	stk_dtor (&cmd);

	return NICE;  
}
//--------------------------------------------------------------------------------------------------------------

int static read_file  (stk_t* ptr_cmd, FILE* cmd_file)
{
	assert (ptr_cmd);
	assert (cmd_file);

	char str[MAX_LETTERS] = "";

	while (fscanf (cmd_file, "%s", str) != EOF)
	{
		#ifdef DEBUG 
			print_cmd (ptr_cmd);
		#endif

		if (str[0] == '#') 
		{
			fgets (str, MAX_LETTERS, cmd_file); 
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "push") == 0)
		{		
			int arg = 0;
			if (fscanf  (cmd_file, "%d", &arg) != 0) 
			{
				stk_push (ptr_cmd, PUSH | 0x10);
				stk_push (ptr_cmd, arg);
				continue;
			}

			char reg = 0;
			if (fscanf  (cmd_file, "%cX", &reg) != 0)
			{
				stk_push (ptr_cmd, PUSH | 0x20);

				for (int index_reg = 'A'; index_reg <= 'D'; index_reg++)
				{
					if (reg == index_reg)
					{
						stk_push (ptr_cmd, index_reg - 'A' + 1);
						break;
					}
				}
				continue;
			}
			
			printf ("ERROR IN SNT PUSH\n");
			abort ();
		}

		if (strcmp (str, "pop") == 0)
		{
			stk_push (ptr_cmd, POP | 0x20);

			char reg[MAX_LETTERS] = "";
			fscanf (cmd_file, "%s", reg);

			for (int index_reg = 'A'; index_reg <= 'D'; index_reg++)
			{
				if (reg[0] == index_reg)
				{
					stk_push (ptr_cmd, index_reg - 'A' + 1);
					break;
				}
			}
			continue;
		}

		//-----------------------------------------------------------------------------------------------------

		COMPARE_OM_JUMP(str, "jmp", JMP);
		COMPARE_OM_JUMP(str, "ja",  JA);
		COMPARE_OM_JUMP(str, "jb",  JB);

		//------------------------------------------------------------------------------------------------------

		COMPARE_STR_CMD(str, "out",   OUT);
		COMPARE_STR_CMD(str, "add",   ADD);
		COMPARE_STR_CMD(str, "sub",   SUB);
		COMPARE_STR_CMD(str, "mul",   MUL);
		COMPARE_STR_CMD(str, "div",   DIV);
		COMPARE_STR_CMD(str, "in",    IN);
		COMPARE_STR_CMD(str, "sqrt",  SQRT);
		COMPARE_STR_CMD(str, "sin",   SIN);
		COMPARE_STR_CMD(str, "dump",  DUMP);
		COMPARE_STR_CMD(str, "guide", GUIDE);
		COMPARE_STR_CMD(str, "hlt",   HLT);

		printf ("SNT_ERROR:\n command: '%s' don't find\n", str);

		return BAD;
	}

	#ifdef DEBUG
		print_cmd (ptr_cmd);
	#endif

	return NICE;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

int static write_file (stk_t* ptr_cmd, FILE* code_file)
{
	assert (ptr_cmd);
	assert (code_file);

	for (size_t index = 0; index < SIZE_HEADER; index++)
	{
		
		fprintf (code_file, "%ld\n", *((*ptr_cmd).data + index));
	}

	for (size_t index = SIZE_HEADER; index < (*ptr_cmd).size; index++)
	{
		fprintf (code_file, "%lx ", *((*ptr_cmd).data + index));
	}

	fprintf (code_file, "\n");

	return NICE;
}

#ifdef DEBUG
	void static print_cmd (stk_t* ptr_cmd)
	{
		assert (ptr_cmd);

		for (size_t ip = 0; ip < (*ptr_cmd).size; ip++)
		{
			printf ("%3ld ", ip);
		}
		printf ("\n");

		for (size_t ip = 0; ip < (*ptr_cmd).size; ip++)
		{
			printf ("%3lx ", (*ptr_cmd).data [ip]);
		}
		printf ("\n");

		getchar ();

		
	}
#endif
