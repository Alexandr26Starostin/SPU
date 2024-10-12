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

void static read_code_file (int* commands, size_t size_commands, FILE* code_file);

//----------------------------------------------------------------------------------------------------------------------

void run (FILE* code_file)
{
	assert (code_file);

	size_t size_commands = 0;
	fscanf (code_file, "%ld", &size_commands);

	int* commands = (int*) calloc (size_commands, sizeof (int));
	if (commands == NULL) {fclose (code_file); abort ();}

	read_code_file (commands, size_commands, code_file);

	stk_t stk = {};
	if (stk_ctor (&stk, DEFAULT_LEN_STK) != 0) {abort ();}

	FILE* guide_file = fopen ("../guide.txt", "r");	// FIXME
	if (guide_file == NULL) {printf ("Can't open guide.txt\n"); abort ();}

	for (size_t index = 0; index < size_commands; index++)
	{
		int cmd = commands[index];

		switch ((command_t) cmd)
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
				index        += 1;
				element_t arg = commands[index];

				stk_push (&stk, arg);
				break;
			}
			
			case ADD:
			{
				element_t a = 0, b = 0;
				stk_pop (&stk, &a);
				stk_pop (&stk, &b);

				stk_push (&stk, b + a);
				break;
			}
			
			case SUB:
			{
				element_t a = 0, b = 0;
				stk_pop (&stk, &a);
				stk_pop (&stk, &b);

				stk_push (&stk, b - a);
				break;
			}
			
			case MUL:
			{
				element_t a = 0, b = 0;
				stk_pop (&stk, &a);
				stk_pop (&stk, &b);

				stk_push (&stk, b * a);
				break;
			}
			
			case DIV:
			{
				element_t a = 0, b = 0;
				stk_pop (&stk, &a);
				stk_pop (&stk, &b);

				stk_push (&stk, b / a);
				break;
			}
			
			case OUT:
			{
				element_t arg = 0;
				stk_pop (&stk, &arg);

				printf ("%d\n", arg);
				break;
			}

			case IN:
			{
				element_t arg = 0;
				scanf ("%d", &arg);

				stk_push (&stk, arg);
				break;
			}
			
			case SQRT:
			{
				element_t arg = 0;
				stk_pop (&stk, &arg);

				stk_push (&stk, (int) sqrt ((double) arg));
				break;
			}
			
			case SIN:
			{
				element_t arg = 0;
				stk_pop (&stk, &arg);

				stk_push (&stk, (int) sin ((double) arg));
				break;
			}
			
			case COS:
			{
				element_t arg = 0;
				stk_pop (&stk, &arg);

				stk_push (&stk, (int) cos ((double) arg));
				break;
			}
			
			case DUMP:
			{
				stk_dump (&stk, __FILE__, __LINE__);
				break;
			}
			
			case HLT:
			{
				stk_dtor (&stk);
				break;
			}

			default:
			{
				printf ("SNT_ERROR: '%d'\n", cmd);

				fclose (code_file);
				fclose (guide_file);
				free (commands);

				abort ();
				break;
			}
		}
	}

	fclose (guide_file);
	free (commands);
}

//------------------------------------------------------------------------------------------------------------

void static read_code_file (int* commands, size_t size_commands, FILE* code_file)
{
	assert (commands);
	assert (code_file);

	for (size_t index = 0; index < size_commands; index++)
	{
		fscanf (code_file, "%d", commands + index);
	}
}
