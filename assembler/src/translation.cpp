#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

const size_t MAX_LETTERS     = 50;
const size_t DEFAULT_LEN_CMD = 64;

#include "../../commands.h"
#include "../include/translation.h"

/*void static read_file  (int* commands, size_t* ip, FILE* file);
void static write_file (int* commands, size_t* index_cmd, FILE* file);*/

//-------------------------------------------------------------------------------------------------------------

void translation (FILE* cmd_file, FILE* code_file)
{
	assert (cmd_file);
	assert (code_file);

	int* commands = (int*) calloc (DEFAULT_LEN_CMD, sizeof (int));
	if (commands == NULL) {fclose (cmd_file); fclose (code_file); abort ();}

	size_t index_cmd = 0;

	// (commands, &index_cmd, cmd_file);     index_cmd = 1;

	char str[MAX_LETTERS] = "";

	fprintf (code_file, "00000000\n");

	while (fscanf (cmd_file, "%s", str) != EOF)
	{
		if (str[0] == '#') 
		{
			fgets (str, MAX_LETTERS, cmd_file); 
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "push") == 0)
		{
			fprintf (code_file, "%d ", PUSH);

			fscanf  (cmd_file, "%s", str);
			fprintf (code_file, "%s\n", str);

			index_cmd += 2;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "add") == 0)
		{
			fprintf (code_file, "%d\n", ADD);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "sub") == 0)
		{
			fprintf (code_file, "%d\n", SUB);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "mul") == 0)
		{
			fprintf (code_file, "%d\n", MUL);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "div") == 0)
		{
			fprintf (code_file, "%d\n", DIV);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "out") == 0)
		{
			fprintf (code_file, "%d\n", OUT);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "in") == 0)
		{
			fprintf (code_file, "%d\n", IN);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "sqrt") == 0)
		{
			fprintf (code_file, "%d\n", SQRT);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "sin") == 0)
		{
			fprintf (code_file, "%d\n", SIN);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "cos") == 0)
		{
			fprintf (code_file, "%d\n", COS);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "dump") == 0)
		{
			fprintf (code_file, "%d\n", DUMP);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "guide") == 0)
		{
			fprintf (code_file, "%d\n", GUIDE);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "hlt") == 0)
		{
			fprintf (code_file, "%d\n", HLT);
			index_cmd += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		printf ("SNT_ERROR:\n command: '%s' don't find\n", str);
		fprintf (code_file, "BAD_FILE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

		fclose (code_file);
		fclose (cmd_file);
		
		abort ();
	}

	fclose (code_file);

	code_file = fopen ("../code.txt", "r+");

	fprintf (code_file, "%ld", index_cmd);

	size_t indent = 8 - ((index_cmd/10) + 1);

	for (size_t index = 0; index < indent; index++)
	{
		fprintf (code_file, " ");
	}

	fprintf (code_file, "\n");

	fclose (code_file);
}

/*
void static read_file  (int* commands, size_t* ip, FILE* file)
{
	assert (commands);
	assert (index_cmd);
	assert (file);

	char str[MAX_LETTERS] = "";

	while (fscanf (file, "%s", str) != EOF)
	{
		if (str[0] == '#') 
		{
			fgets (str, MAX_LETTERS, cmd_file); 
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "push") == 0)
		{
			commands[ip] = PUSH;
			ip += 1;

			fscanf  (file, "%d", commands + ip);
			ip += 1;
			
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "add") == 0)
		{
			commands[ip] = ADD;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "sub") == 0)
		{
			commands[ip] = SUB;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "mul") == 0)
		{
			commands[ip] = MUL;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "div") == 0)
		{
			commands[ip] = DIV;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "out") == 0)
		{
			commands[ip] = OUT;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "in") == 0)
		{
			commands[ip] = IN;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "sqrt") == 0)
		{
			commands[ip] = SQRT;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "sin") == 0)
		{
			commands[ip] = SIN;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "cos") == 0)
		{
			commands[ip] = COS;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "dump") == 0)
		{
			commands[ip] = DUMP;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "guide") == 0)
		{
			commands[ip] = GUIDE;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "hlt") == 0)
		{
			commands[ip] = HLT;
			ip += 1;
			continue;
		}

		//----------------------------------------------------------------------------------------------

		printf ("SNT_ERROR:\n command: '%s' don't find\n", str);
		fprintf (code_file, "BAD_FILE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

		fclose (code_file);
		fclose (cmd_file);
		
		abort ();
	}

	fclose (code_file);

	code_file = fopen ("../code.txt", "r+");

	fprintf (code_file, "%ld", index_cmd);

	size_t indent = 8 - ((index_cmd/10) + 1);

	for (size_t index = 0; index < indent; index++)
	{
		fprintf (code_file, " ");
	}

	fprintf (code_file, "\n");

	fclose (code_file);


}*/
