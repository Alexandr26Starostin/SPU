#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../include/const.h"

#include "../include/find_file.h"
#include "../include/translation.h"

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
