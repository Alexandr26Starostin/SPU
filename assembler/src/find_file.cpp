#include <stdio.h>
#include <assert.h>

#include "../include/const.h"
#include "../include/find_file.h"

#define NAME_CMD_FILE  "../commands.txt"
#define NAME_CODE_FILE "../code.txt"

int find_file (int argc, char** argv, FILE** ptr_cmd_file, FILE** ptr_code_file)
{
	assert (argv);	
	assert (ptr_cmd_file);
	assert (ptr_code_file);

	if (argc == 1)
	{
		*ptr_cmd_file  = fopen (NAME_CMD_FILE, "r"); 
		*ptr_code_file = fopen (NAME_CODE_FILE, "w");
	}

	else if (argc >= 3)
	{
		*ptr_cmd_file  = fopen (argv[1], "r");
		*ptr_code_file = fopen (argv[2], "w");
	}

	else 
	{
		printf ("assembler wants to eat two files)) Don't be greedy\n");
		return BAD;
	}

	if (*ptr_cmd_file == NULL || *ptr_code_file == NULL)
	{
		if (*ptr_cmd_file  == NULL) {printf ("cmd_file  == NULL\n");}
		if (*ptr_code_file == NULL) {printf ("code_file == NULL\n");}

		printf ("Not find files\n"); 
		printf ("Write in format: 1) exe  2) commands  3) code  4) other \n");

		fclose (* ptr_code_file);
		fclose (*ptr_cmd_file);

		return BAD;
	}

	return NICE;
}