#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "../../stack/include/const_define_struct.h"
#include "../../stack/include/hash.h"
#include "../../stack/include/operations_with_stack.h"
#include "../../stack/include/stk_error.h"

#include "../../commands.h"
#include "../include/spu_error.h"
#include "../include/spu_print.h"
#include "../include/spu_run.h"
#include "../include/spu_draw_picture.h"

const int   POISON          = -66666;
const char* NAME_GUIDE_FILE = "../guide.txt";

static int  get_arg_push (spu_t* ptr_spu, size_t* ip);
static int* get_arg_pop  (spu_t* ptr_spu, size_t* ip);

//--------------------------------------------------------------------------------------------------------------------------

long run_spu (spu_t* ptr_spu)
{
	assert (ptr_spu);

	FILE* guide_file = fopen (NAME_GUIDE_FILE, "r");
	if (guide_file == NULL) 
	{
		printf ("Can't open guide.txt\n"); 
		ptr_spu -> error_in_spu |= NOT_FIND_GUIDE;
	}

	size_t ip = 0;

	for (ip = 0; ip < ptr_spu -> size_cmd; ip++)
	{
		if (spu_error (ptr_spu, __FILE__, __LINE__))
		{
			fclose (guide_file);
			return ptr_spu -> error_in_spu;
		}
		
		print_spu (ptr_spu, ip);

		cmd_t command = (ptr_spu -> cmd)[ip];

		if ((command & COMMAND_MACK) == HLT)
		{
			fclose      (guide_file);
			break;
		}

		switch (command & COMMAND_MACK)
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
				int arg = get_arg_push (ptr_spu, &ip);
				stk_push (&(ptr_spu -> stk), (element_t) arg);

				break;
			}
			

			case POP:
			{
				long arg = 0;
				stk_pop (&(ptr_spu -> stk), &arg);

				*get_arg_pop (ptr_spu, &ip) = (int) arg;

				break;
			}
			
			case ADD:
			{
				element_t a = 0, b = 0;         
				stk_pop (&(ptr_spu -> stk), &a);         
				stk_pop (&(ptr_spu -> stk), &b);

				stk_push (&(ptr_spu -> stk), b + a);
				break;
			}

			case SUB:
			{
				element_t a = 0, b = 0;         
				stk_pop (&(ptr_spu -> stk), &a);         
				stk_pop (&(ptr_spu -> stk), &b);

				stk_push (&(ptr_spu -> stk), b - a);
				break;
			}
			
			case MUL:
			{
				element_t a = 0, b = 0;         
				stk_pop (&(ptr_spu -> stk), &a);         
				stk_pop (&(ptr_spu -> stk), &b);

				stk_push (&(ptr_spu -> stk), b * a);
				break;
			}
			
			case DIV:
			{
				element_t a = 0, b = 0;         
				stk_pop (&(ptr_spu -> stk), &a);         
				stk_pop (&(ptr_spu -> stk), &b);

				stk_push (&(ptr_spu -> stk), b / a);
				break;
			}

			case IDIV:
			{
				element_t a = 0, b = 0;         
				stk_pop (&(ptr_spu -> stk), &a);         
				stk_pop (&(ptr_spu -> stk), &b);

				stk_push (&(ptr_spu -> stk), (int) (b / a));
				break;
			}
			
			case OUT:
			{
				element_t arg = 0;      
				stk_pop (&(ptr_spu -> stk), &arg);

				printf ("%ld\n", arg);
				getchar ();
				break;
			}

			case IN:
			{
				element_t arg = 0;
				scanf ("%lx", &arg);
				stk_push (&(ptr_spu -> stk), arg);
				break;
			}
			
			case SQRT:
			{
				element_t arg = 0;      
				stk_pop (&(ptr_spu -> stk), &arg);

				stk_push (&(ptr_spu -> stk), (long) sqrt ((double) arg));
				break;
			}
			
			case SIN:
			{
				element_t arg = 0;      
				stk_pop (&(ptr_spu -> stk), &arg);

				stk_push (&(ptr_spu -> stk), (long) sin ((double) arg));
				break;
			}

			case COS:
			{
				element_t arg = 0;      
				stk_pop (&(ptr_spu -> stk), &arg);

				stk_push (&(ptr_spu -> stk), (long) cos ((double) arg));
				break;
			}
			
			case DUMP:
			{
				stk_dump (&(ptr_spu -> stk), __FILE__, __LINE__);
				break;
			}

			case JMP:
			{
				ip = (ptr_spu -> cmd)[ip + 1] - 1;
				break;
			}

			case JA:
			{
				element_t a = 0, b = 0;         
				stk_pop (&(ptr_spu -> stk), &a);         
				stk_pop (&(ptr_spu -> stk), &b);

				if (a > b)
				{
					ip = (ptr_spu -> cmd)[ip + 1] - 1;
				}
				else 
				{
					ip += 1;
				}

				break;
			}

			case JB:
			{
				element_t a = 0, b = 0;         
				stk_pop (&(ptr_spu -> stk), &a);         
				stk_pop (&(ptr_spu -> stk), &b);

				if (a < b)
				{
					ip = (ptr_spu -> cmd)[ip + 1] - 1;
				}
				else 
				{
					ip += 1;
				}

				break;
			}

			case CALL:
			{
				stk_push (&(ptr_spu -> func), (long) (ip + 1));
				ip = (ptr_spu -> cmd)[ip + 1] - 1;

				break;
			}

			case RET:
			{
				long ret_value = 0;
				stk_pop (&(ptr_spu -> func), &ret_value);

				ip = ret_value;

				break;
			}

			case DRAW:
			{
				size_t index_ram_for_draw = (size_t) (ptr_spu -> cmd)[ip + 1];
				ip += 1;
				draw_picture (ptr_spu, index_ram_for_draw);

				break;
			}
			

			default:
			{
				printf ("SNT_ERROR: '%x'\n", command);
				fclose      (guide_file);

				ptr_spu -> error_in_spu |= INCORRECT_COMMAND;

				spu_error (ptr_spu, __FILE__, __LINE__);
				
				return ptr_spu -> error_in_spu;

				break;
			}
		}
	}

	if (spu_error (ptr_spu, __FILE__, __LINE__))
	{
		fclose (guide_file);
		return ptr_spu -> error_in_spu;
	}
	
	printf ("in end:\n");
	print_spu (ptr_spu, ip);

	return ptr_spu -> error_in_spu;
}

static int get_arg_push (spu_t* ptr_spu, size_t* ip)
{
	assert (ptr_spu);
	assert (ip);

	cmd_t command = (ptr_spu -> cmd) [(*ip)++];
	int   arg     = 0;

	if (command & IMM_MASK) 
	{
		arg = (ptr_spu -> cmd) [(*ip)++];
	}

	if (command & REG_MASK) 
	{
		int index_reg = (ptr_spu -> cmd) [(*ip)++];

		if (index_reg >= (int) SIZE_REG || index_reg < 0)
		{
			ptr_spu -> error_in_spu |= SIZE_REG_EXCEED;
			return POISON;
		}

		arg += (ptr_spu -> reg)[index_reg];
	}

	if (command & RAM_MASK)
	{
		if (arg >= (int) SIZE_RAM)
		{
			ptr_spu -> error_in_spu |= SIZE_RAM_EXCEED;
			return POISON;
		}

		arg = (ptr_spu -> ram)[arg];
	}

	(*ip) -= 1;

	return arg;
}

static int* get_arg_pop (spu_t* ptr_spu, size_t* ip)
{
	assert (ptr_spu);
	assert (ip);

	cmd_t command     = (ptr_spu -> cmd) [(*ip)++];
	int*  ptr_for_arg = 0;
	int   arg         = 0;

	if (command & IMM_MASK) 
	{
		arg = (ptr_spu -> cmd) [(*ip)++];
	}

	if (command & REG_MASK) 
	{
		int index_reg = (ptr_spu -> cmd) [(*ip)++];

		if (index_reg >= (int) SIZE_REG || index_reg < 0)
		{
			ptr_spu -> error_in_spu |= SIZE_REG_EXCEED;
			return NULL;
		}

		ptr_for_arg = ((ptr_spu -> reg) + index_reg);
	}

	if (command & RAM_MASK)
	{
		if (arg >= (int) SIZE_RAM)
		{
			ptr_spu -> error_in_spu |= SIZE_RAM_EXCEED;
			return NULL;
		}

		ptr_for_arg = ((ptr_spu -> ram) + *ptr_for_arg + arg);
	}

	(*ip) -= 1;

	return ptr_for_arg;
}