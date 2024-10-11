#include <stdio.h>
#include <assert.h>

#include "../include/run.h"

enum work_spu_t
{
	BAD  = 0,
	NICE = 1
};

int static find_file (int argc, char** argv, FILE** ptr_code_file);

//---------------------------------------------------------------------------

int main (int argc, char** argv)
{
	assert (argv);

	FILE* code_file = NULL;
	if (find_file (argc, argv, &code_file) == BAD) {return BAD;}

	run (code_file);

	fclose (code_file);

	return NICE;
}

//------------------------------------------------------------------------------

int static find_file (int argc, char** argv, FILE** ptr_code_file)
{
	assert (argv);	
	assert (ptr_code_file);

	if (argc == 1)
	{
		*ptr_code_file = fopen ("../code.txt", "r");
	}

	else
	{
		*ptr_code_file = fopen (argv[1], "r");
	}

	if (*ptr_code_file == NULL)
	{
		if (*ptr_code_file == NULL) {printf ("code_file == NULL\n");}

		printf ("Not find file\n"); 
		printf ("Write in format: 1) exe  2) code  3) other \n");

		return BAD;
	}

	return NICE;
}
