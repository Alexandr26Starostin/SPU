#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

enum work_dis_t
{
	BAD  = 0,
	NICE = 1
};

int  static find_file   (int argc, char** argv, FILE** ptr_code_file, FILE** ptr_cmd_file);
void static translation (FILE* code_file, FILE* cmd_file);

//-----------------------------------------------------------------------------

int main (int argc, char** argv)
{
	assert (argv);

	FILE* code_file = NULL;
	FILE* cmd_file  = NULL; 

	if (find_file (argc, argv, &code_file, &cmd_file) == BAD) {return BAD;}

	translation (code_file, cmd_file);

	fclose (code_file); 
	fclose (cmd_file); 

	return NICE;
}

//-----------------------------------------------------------------------------

int static find_file (int argc, char** argv, FILE** ptr_code_file, FILE** ptr_cmd_file)
{
	assert (argv);	
	assert (ptr_code_file);
	assert (ptr_cmd_file);

	if (argc == 1)
	{
		*ptr_code_file = fopen ("../code.txt", "r");
		*ptr_cmd_file  = fopen ("../commands.txt", "w");
	}

	else
	{
		*ptr_code_file = fopen (argv[1], "r");
		*ptr_cmd_file  = fopen (argv[2], "w");
	}

	if (*ptr_code_file == NULL || *ptr_cmd_file == NULL)
	{
		if (*ptr_code_file == NULL) {printf ("code_file == NULL\n");}
		if (*ptr_cmd_file  == NULL) {printf ("cmd_file  == NULL\n");}

		printf ("Not find files\n"); 
		printf ("Write in format: 1) exe  2) code 3) commands 4) other \n");

		return BAD;
	}

	return NICE;
}

//----------------------------------------------------------------------------------------------------

void static translation (FILE* code_file, FILE* cmd_file)
{
	assert (code_file);
	assert (cmd_file);

	int value = 0;

	fscanf (code_file, "%d", &value);
	
	while (fscanf (code_file, "%d", &value) != EOF)
	{
		if (value == 1)
		{
			fprintf (cmd_file, "push ");

			fscanf  (code_file, "%d", &value);
			fprintf (cmd_file, "%d\n", value);
			continue;
		}

		if (value == 2)
		{
			fprintf (cmd_file, "add\n");
			continue;
		}

		if (value == 3)
		{
			fprintf (cmd_file, "sub\n");
			continue;
		}

		if (value == 4)
		{
			fprintf (cmd_file, "mul\n");
			continue;
		}

		if (value == 5)
		{
			fprintf (cmd_file, "div\n");
			continue;
		}

		if (value == 6)
		{
			fprintf (cmd_file, "out\n");
			continue;
		}

		if (value == 7)
		{
			fprintf (cmd_file, "in\n");
			continue;
		}

		if (value == 8)
		{
			fprintf (cmd_file, "sqrt\n");
			continue;
		}

		if (value == 9)
		{
			fprintf (cmd_file, "sin\n");
			continue;
		}

		if (value == 10)
		{
			fprintf (cmd_file, "cos\n");
			continue;
		}

		if (value == 11)
		{
			fprintf (cmd_file, "dump\n");
			continue;
		}

		if (value == 0)
		{
			fprintf (cmd_file, "guide\n");
			continue;
		}

		if (value == -1)
		{
			fprintf (cmd_file, "hlt\n");
			continue;
		}

		printf ("SNT_ERROR:\n value: '%d' don't find\n", value);
		
		abort ();
	}
}
