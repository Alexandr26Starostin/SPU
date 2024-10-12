#include <stdio.h>
#include <assert.h>

#include "../include/const.h"
#include "../include/find_file.h"

//------------------------------------------------------------------------------------------------

int find_file (int argc, char** argv, FILE** ptr_code_file, FILE** ptr_cmd_file)
{
	assert (argv);	
	assert (ptr_code_file);
	assert (ptr_cmd_file);

	if (argc == 1)
	{
		*ptr_code_file = fopen ("../code.txt", "r");
		*ptr_cmd_file  = fopen ("../commands.txt", "w");
	}

	else if (argc >= 3)
	{
		*ptr_code_file = fopen (argv[1], "r");
		*ptr_cmd_file  = fopen (argv[2], "w");
	}

	else 
	{
		printf ("disassembler wants to eat two files)) Don't be greedy\n");
		return BAD;
	}

	if (*ptr_code_file == NULL || *ptr_cmd_file == NULL)
	{
		if (*ptr_code_file == NULL) {printf ("code_file == NULL\n");}
		if (*ptr_cmd_file  == NULL) {printf ("cmd_file  == NULL\n");}

		printf ("Not find files\n"); 
		printf ("Write in format: 1) exe  2) code 3) commands 4) other \n");

		fclose (* ptr_code_file);
		fclose (*ptr_cmd_file);

		return BAD;
	}

	return NICE;
}
