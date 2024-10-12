#include <stdio.h>
#include <assert.h>

#include "../include/const.h"
#include "../include/find_file.h"
#include "../include/translation.h"

//-----------------------------------------------------------------------------

int main (int argc, char** argv)
{
	assert (argv);

	FILE* cmd_file  = NULL; 
	FILE* code_file = NULL;

	if (find_file (argc, argv, &cmd_file, &code_file) == BAD) {return BAD;}

	translation (cmd_file, code_file);

	fclose (cmd_file);

	return NICE;
}
