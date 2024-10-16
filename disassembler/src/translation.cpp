#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../../commands.h"
#include "../include/translation.h"

//----------------------------------------------------------------------------

void translation (FILE* code_file, FILE* cmd_file)
{
	assert (code_file);
	assert (cmd_file);

	int value = 0;

	fscanf (code_file, "%d", &value);
	
	while (fscanf (code_file, "%d", &value) != EOF)
	{
		switch (value)
		{
			case (PUSH):
			{
				fprintf (cmd_file, "push ");

				fscanf  (code_file, "%d", &value);
				fprintf (cmd_file, "%d\n", value);
				break;
			}

			case (ADD):
			{
				fprintf (cmd_file, "add\n");
				break;
			}

			case (SUB):
			{
				fprintf (cmd_file, "sub\n");
				break;
			}

			case (MUL):
			{
				fprintf (cmd_file, "mul\n");
				break;
			}

			case (DIV):
			{
				fprintf (cmd_file, "div\n");
				break;
			}

			case (OUT):
			{
				fprintf (cmd_file, "out\n");
				break;
			}

			case (IN):
			{
				fprintf (cmd_file, "in\n");
				break;
			}

			case (SQRT):
			{
				fprintf (cmd_file, "sqrt\n");
				break;
			}

			case (SIN):
			{
				fprintf (cmd_file, "sin\n");
				break;
			}

			case (DUMP):
			{
				fprintf (cmd_file, "dump\n");
				break;
			}
			
			case (GUIDE):
			{
				fprintf (cmd_file, "guide\n");
				break;
			}

			case (HLT):
			{
				fprintf (cmd_file, "hlt\n");
				break;
			}

			default:
			{
				printf ("SNT_ERROR:\n value: '%d' don't find\n", value);
				fprintf (cmd_file, "BAD_FILE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

				fclose (code_file);
				fclose (cmd_file);
		
				abort ();
				break;
			}
		}
	}
}