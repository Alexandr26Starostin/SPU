#include <stdio.h>
#include <assert.h>

#include "../include/const.h"
#include "../include/find_file.h"

#define NAME_CODE_FILE "../code.txt"
//-----------------------------------------------------------------------

int find_file (int argc, char** argv, FILE** ptr_code_file)
{
	assert (argv);	
	assert (ptr_code_file);

	//TODO const char *CODE_FILE_NAME = "./hui.txt";

	if (argc == 1)
	{
		*ptr_code_file = fopen (NAME_CODE_FILE, "r");
	}

	else
	{
		*ptr_code_file = fopen (argv[1], "r");
	}

	if (*ptr_code_file == NULL)
	{
		printf ("code_file == NULL\n");

		printf ("Not find file\n"); 
		printf ("Write in format:\n1) exe\n 2) code\n3) other\n");

		fclose (*ptr_code_file);

		return BAD;
	}

	return NICE;
}