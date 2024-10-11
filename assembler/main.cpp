#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

const size_t MAX_LETTERS = 50;

enum work_t
{
	BAD  = 0,
	NICE = 1
};

int  static find_file   (int argc, char** argv, FILE** ptr_cmd_file, FILE** ptr_code_file);
void static translation (FILE* cmd_file, FILE* code_file);

//-----------------------------------------------------------------------------

int main (int argc, char** argv)
{
	assert (argv);

	FILE* cmd_file = NULL; 
	FILE* code_file = NULL;

	if (find_file (argc, argv, &cmd_file, &code_file) == BAD) {return BAD;}

	translation (cmd_file, code_file);

	return NICE;
}

//-----------------------------------------------------------------------------

int static find_file (int argc, char** argv, FILE** ptr_cmd_file, FILE** ptr_code_file)
{
	assert (argv);	
	assert (ptr_cmd_file);
	assert (ptr_code_file);

	if (argc == 1)
	{
		*ptr_cmd_file  = fopen ("../commands.txt", "r");
		*ptr_code_file = fopen ("../code.txt", "w");
	}

	else
	{
		*ptr_cmd_file  = fopen (argv[1], "r");
		*ptr_code_file = fopen (argv[2], "w");
	}

	if (*ptr_cmd_file == NULL || *ptr_code_file == NULL)
	{
		if (*ptr_cmd_file == NULL)  {printf ("cmd_file  == NULL\n");}
		if (*ptr_code_file == NULL) {printf ("code_file == NULL\n");}

		printf ("Not find files\n"); 
		printf ("Write in format: 1) exe  2) commands  3) code  4) other \n");

		return BAD;
	}

	return NICE;
}

//----------------------------------------------------------------------------------------------------

void static translation (FILE* cmd_file, FILE* code_file)
{
	assert (cmd_file);
	assert (code_file);

	char str[MAX_LETTERS] = "";

	while (fscanf (cmd_file, "%s", str) != EOF)
	{
		if (str[0] == '#') 
		{
			fgets (str, MAX_LETTERS, cmd_file); 
			continue;
		}

		if (strcmp (str, "push") == 0)
		{
			fprintf (code_file, "1 ");

			fscanf  (cmd_file, "%s", str);
			fprintf (code_file, "%s\n", str);
			continue;
		}

		if (strcmp (str, "add") == 0)
		{
			fprintf (code_file, "2\n");
			continue;
		}

		if (strcmp (str, "sub") == 0)
		{
			fprintf (code_file, "3\n");
			continue;
		}

		if (strcmp (str, "mul") == 0)
		{
			fprintf (code_file, "4\n");
			continue;
		}

		if (strcmp (str, "div") == 0)
		{
			fprintf (code_file, "5\n");
			continue;
		}

		if (strcmp (str, "out") == 0)
		{
			fprintf (code_file, "6\n");
			continue;
		}

		if (strcmp (str, "in") == 0)
		{
			fprintf (code_file, "7\n");
			continue;
		}

		if (strcmp (str, "sqrt") == 0)
		{
			fprintf (code_file, "8\n");
			continue;
		}

		if (strcmp (str, "sin") == 0)
		{
			fprintf (code_file, "9\n");
			continue;
		}

		if (strcmp (str, "cos") == 0)
		{
			fprintf (code_file, "10\n");
			continue;
		}

		if (strcmp (str, "dump") == 0)
		{
			fprintf (code_file, "11\n");
			continue;
		}

		if (strcmp (str, "guide") == 0)
		{
			fprintf (code_file, "0\n");
			continue;
		}

		if (strcmp (str, "hlt") == 0)
		{
			fprintf (code_file, "-1\n");
			continue;
		}

		printf ("SNT_ERROR:\n command: '%s' don't find\n", str);
		
		abort ();
	}
}