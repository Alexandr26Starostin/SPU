#include <stdio.h>
#include <assert.h>

#include "../include/find_file.h"
#include "../include/launch_spu.h"

//---------------------------------------------------------------------------

int main (int argc, char** argv)
{
	assert (argv);

	FILE* cmd_file = NULL;

	result_find_file status_find_file = find_file (argc, argv, &cmd_file);
	if (status_find_file) {return status_find_file;}

	long status_launch_spu = launch_spu (cmd_file);
	if (status_launch_spu) {return (int) status_launch_spu;}

	return (int) status_launch_spu;
}
