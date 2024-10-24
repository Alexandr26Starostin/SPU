#include <stdio.h>
#include <assert.h>

#include "../include/launch_asm.h"
#include "../include/asm_print.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

void print_asm (asm_t* ptr_assm)
{
	assert (ptr_assm);

	//----------------------------------------------------------------------------------------

	printf ("cmd:\n");

	size_t len_cmd = ptr_assm -> cmd_count;
	for (size_t index_cmd = 0; index_cmd < len_cmd; index_cmd++)
	{
		printf ("%3ld ", index_cmd);
	}
	printf ("\n");

	for (size_t index_cmd = 0; index_cmd < len_cmd; index_cmd++)
	{
		printf ("%3x ", (ptr_assm -> cmd)[index_cmd]);
	}
	printf ("\n\n");

	//----------------------------------------------------------------------------------------

	printf ("labels:\n");

	size_t len_labels = ptr_assm -> labels_count;
	for (size_t index_labels = 0; index_labels < len_labels; index_labels++)
	{
		printf ("%3ld ", index_labels);
		printf ("%3x ",  ((ptr_assm -> labels)[index_labels]).ip_label);
		printf ("%s\n",  ((ptr_assm -> labels)[index_labels]).name_label);
	}
	printf ("\n\n");

	//----------------------------------------------------------------------------------------

	printf ("fix_up:\n");

	size_t len_fix_up = ptr_assm -> fix_up_count;
	for (size_t index_fix_up = 0; index_fix_up < len_fix_up; index_fix_up++)
	{
		printf ("%3ld ", index_fix_up);
	}
	printf ("\n");

	for (size_t index_fix_up = 0; index_fix_up < len_fix_up; index_fix_up++)
	{
		printf ("%3ld ", ((ptr_assm -> fix_up)[index_fix_up]).cmd_fix);
	}
	printf ("\n");

	for (size_t index_fix_up = 0; index_fix_up < len_fix_up; index_fix_up++)
	{
		printf ("%3ld ", ((ptr_assm -> fix_up)[index_fix_up]).label_fix);
	}
	printf ("\n----------------------------------------------------------------------------------------------------------------------------------------\n\n");

	getchar ();
}