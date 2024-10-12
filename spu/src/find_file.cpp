#include <stdio.h>
#include <assert.h>

#include "../include/const.h"
#include "../include/find_file.h"

//-----------------------------------------------------------------------

int find_file (int argc, char** argv, FILE** ptr_code_file)
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
		printf ("code_file == NULL\n");

		printf ("Not find file\n"); 
		printf ("Write in format: 1) exe  2) code  3) other \n");

		fclose (*ptr_code_file);

		return BAD;
	}

	return NICE;
}