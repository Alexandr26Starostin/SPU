#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "../../stack/include/const_define_struct.h"
#include "../../stack/include/hash.h"
#include "../../stack/include/operations_with_stack.h"
#include "../../stack/include/stk_error.h"

#include "../../commands.h"
#include "../include/run.h"

const size_t MAX_LETTERS     = 30;
const size_t DEFAULT_LEN_STK = 32;
const size_t COUNT_REGISTERS = 4;

void static read_code_file (command_t* commands, size_t size_commands, FILE* code_file);
void static creat_spu      (spu_t* ptr_spu, FILE* code_file);

#define NAME_GUIDE_FILE "../guide.txt"

#define POP_ONE_ELEMENT     \
	element_t arg = 0;      \
	stk_pop (&spu.stk, &arg);

#define POP_TWO_ELEMENT             \
	element_t a = 0, b = 0;         \
	stk_pop (&spu.stk, &a);         \
	stk_pop (&spu.stk, &b);

#define DEBUG_NOT

#ifdef DEBUG
	void static print_cmd (command_t* commands, size_t size_commands, size_t position);
#endif

//----------------------------------------------------------------------------------------------------------------------

void run (FILE* code_file)
{
	assert (code_file);

	spu_t spu = {};

	creat_spu (&spu, code_file);
	
	read_code_file (spu.commands, spu.size_commands, code_file);

	FILE* guide_file = fopen (NAME_GUIDE_FILE, "r");
	if (guide_file == NULL) {printf ("Can't open guide.txt\n"); abort ();}

	for (size_t ip = 0; ip < spu.size_commands; ip++)
	{
		#ifdef DEBUG
			print_cmd (spu.commands, spu.size_commands, ip);
			stk_dump (&spu.stk, __FILE__, __LINE__);
			getchar ();
		#endif

		command_t cmd = spu.commands[ip];

		switch (cmd & 0x0F)
		{
			case GUIDE:
			{
				char str[MAX_LETTERS] = "";
				while (fgets (str, MAX_LETTERS, guide_file) != NULL) {printf ("%s", str);}

				printf ("\n");
				break;
			}

			case PUSH:
			{
				if ((cmd & 0xF0) == 0x10)
				{
					ip        += 1;                              //TODO make new func
					element_t arg = spu.commands[ip];

					stk_push (&spu.stk, arg);
				}

				if ((cmd & 0xF0) == 0x20)
				{
					ip        += 1;
					element_t reg = spu.commands[ip];

					stk_push (&spu.stk, *(spu.ptr_reg + reg));
				}

				if ((cmd & 0xF0) == 0x30)
				{
					ip        += 1;
					element_t reg = spu.commands[ip];

					ip        += 1;
					element_t arg = spu.commands[ip];

					stk_push (&spu.stk, *(spu.ptr_reg + reg) + arg);
				}

				break;
			}
			
			case ADD:
			{
				POP_TWO_ELEMENT;

				stk_push (&spu.stk, b + a);
				break;
			}
			
			case SUB:
			{
				POP_TWO_ELEMENT;
				stk_push (&spu.stk, b - a);
				break;
			}
			
			case MUL:
			{
				POP_TWO_ELEMENT;
				stk_push (&spu.stk, b * a);
				break;
			}
			
			case DIV:
			{
				POP_TWO_ELEMENT;
				stk_push (&spu.stk, b / a);
				break;
			}

			case POP:
			{
				if ((cmd & 0xF0) == 0x20)
				{
					POP_ONE_ELEMENT;
					ip                    += 1;
					spu.ptr_reg[spu.commands[ip]] = arg;
				}

				break;
			}
			
			case OUT:
			{
				POP_ONE_ELEMENT;
				printf ("%ld\n", arg);
				getchar ();
				break;
			}

			case IN:
			{
				element_t arg = 0;
				scanf ("%lx", &arg);
				stk_push (&spu.stk, arg);
				break;
			}
			
			case SQRT:
			{
				POP_ONE_ELEMENT;
				stk_push (&spu.stk, (int) sqrt ((double) arg));
				break;
			}
			
			case SIN:
			{
				POP_ONE_ELEMENT;
				stk_push (&spu.stk, (int) sin ((double) arg));
				break;
			}
			
			case DUMP:
			{
				stk_dump (&spu.stk, __FILE__, __LINE__);
				break;
			}

			case JMP:
			{
				ip = spu.commands[ip + 1] - 1;
				break;
			}

			case JA:
			{
				POP_TWO_ELEMENT;

				if (a > b)
				{
					ip = spu.commands[ip + 1] - 1;
				}
				else 
				{
					ip += 1;
				}

				break;
			}

			case JB:
			{
				POP_TWO_ELEMENT;

				if (a < b)
				{
					ip = (size_t) spu.commands[ip + 1] - 1;
				}
				else 
				{
					ip += 1;
				}

				break;
			}
			
			case HLT:
			{
				stk_dtor (&spu.stk);
				fclose   (guide_file);
				fclose   (code_file);
				free     (spu.commands);
				free     (spu.ptr_reg);
				break;
			}

			default:
			{
				printf ("SNT_ERROR: '%lx'\n", cmd);

				stk_dtor (&spu.stk);
				fclose   (guide_file);
				fclose   (code_file);
				free     (spu.commands);
				free     (spu.ptr_reg);

				abort ();
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------

void static read_code_file (command_t* commands, size_t size_commands, FILE* code_file)
{
	assert (commands);
	assert (code_file);

	for (size_t ip = 0; ip < size_commands; ip++)
	{
		fscanf (code_file, "%lx", (long*) (commands + ip));
	}
}

void static creat_spu (spu_t* ptr_spu, FILE* code_file)
{
	assert (ptr_spu);
	assert (code_file);

	if (stk_ctor (&((*ptr_spu).stk), DEFAULT_LEN_STK) != 0) {abort ();}

	fscanf (code_file, "%ld", &((*ptr_spu).size_commands));

	(*ptr_spu).commands = (command_t*) calloc ((*ptr_spu).size_commands, sizeof (command_t));
	if ((*ptr_spu).commands == NULL) {fclose (code_file); abort ();}

	(*ptr_spu).ptr_reg = (long*) calloc (COUNT_REGISTERS + 1, sizeof (long)); 
}

#ifdef DEBUG
	void static print_cmd (command_t* commands, size_t size_commands, size_t position)
	{
		for (size_t ip = 0; ip < size_commands; ip++)
		{
			printf ("%3ld ", ip);
		}
		printf ("\n");

		for (size_t ip = 0; ip < size_commands; ip++)
		{
			printf ("%3lx ", commands [ip]);
		}
		printf ("\n");

		for (size_t ip = 0; ip < position; ip++)
		{
			printf ("    ");
		}
		printf ("  !\n");
	}
#endif
