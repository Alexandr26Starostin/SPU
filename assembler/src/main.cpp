#include <stdio.h>
#include <assert.h>

#include "../include/find_file.h"
#include "../include/launch_asm.h"
#include "../include/verifier.h"

//-----------------------------------------------------------------------------

int main (int argc, char** argv)
{
	assert (argv);

	FILE* asm_file = NULL; 
	FILE* cmd_file = NULL;

	long status_find_file = find_file (argc, argv, &asm_file, &cmd_file);
	if (status_find_file) {return (int) status_find_file;}

	long status_launch_asm = launch_asm (asm_file, cmd_file);
	if (status_launch_asm) {return (int) status_launch_asm;}

	return (int) status_launch_asm;
}
