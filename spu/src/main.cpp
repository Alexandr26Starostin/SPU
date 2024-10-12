#include <stdio.h>
#include <assert.h>

#include "../include/const.h"
#include "../include/find_file.h"
#include "../include/run.h"

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
