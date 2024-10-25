#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../stack/include/const_define_struct.h"
#include "../../stack/include/hash.h"
#include "../../stack/include/operations_with_stack.h"
#include "../../stack/include/stk_error.h"

#include "../include/spu_error.h"
#include "../include/spu_run.h"
#include "../include/launch_spu.h"

static error_t check_header  (FILE* cmd_file, int* ptr_len_cmd);
static void    print_header  (header_t* hdr);
static void    creat_spu     (spu_t* ptr_spu, int len_cmd);
static void    destroy_spu    (spu_t* ptr_spu);
static void    read_cmd_file (spu_t* ptr_spu, FILE* cmd_file);

//---------------------------------------------------------------------------------------------

long launch_spu (FILE* cmd_file)
{
	assert (cmd_file);

	int len_cmd = 0;

	if (check_header (cmd_file, &len_cmd)) {fclose (cmd_file); return (long) ERROR_IN_HEADER;}

	spu_t spu = {};
	creat_spu (&spu, len_cmd);

	if (spu_error (&spu, __FILE__, __LINE__))
	{
		fclose     (cmd_file);
		destroy_spu (&spu);

		return spu.error_in_spu;
	}

	read_cmd_file (&spu, cmd_file);

	if (run_spu (&spu))
	{
		fclose     (cmd_file);
		destroy_spu (&spu);

		return spu.error_in_spu;
	}

	destroy_spu (&spu);
	fclose   (cmd_file);

	return spu.error_in_spu;
}

//---------------------------------------------------------------------------------------------

static error_t check_header (FILE* cmd_file, int* ptr_len_cmd)
{
	assert (cmd_file);

	error_t status_header = NOT_ERROR_IN_SPU;

	header_t hdr [SIZE_HEADER] = {};

	fread (hdr, sizeof (header_t), SIZE_HEADER, cmd_file);

	print_header (hdr);

	if (hdr[0] != SIGNATURE)
	{
		printf ("error in signature:\n");
		printf ("signature in spu  = %d\n", SIGNATURE);
		printf ("signature in file = %d\n\n", hdr[0]);
		status_header = ERROR_IN_HEADER;
	}

	if (hdr[1] != VERSION)
	{
		printf ("error in version:\n");
		printf ("version in spu  = %d\n", VERSION);
		printf ("version in file = %d\n\n", hdr[1]);
		status_header = ERROR_IN_HEADER;
	}

	*ptr_len_cmd = hdr[2];

	return status_header;
}

static void print_header (header_t* hdr)
{
	assert (hdr);

	printf ("hdr:\n");

	for (size_t index_hdr = 0; index_hdr < SIZE_HEADER; index_hdr++)
	{
		printf ("%6ld ", index_hdr);
	}

	printf ("\n");

	for (size_t index_hdr = 0; index_hdr < SIZE_HEADER; index_hdr++)
	{
		printf ("%6x ", hdr[index_hdr]);
	}

	printf("\n--------------------------------------------------------------------------------------------------------------\n\n");
}

static void creat_spu (spu_t* ptr_spu, int len_cmd)
{
	assert (ptr_spu);

	ptr_spu -> size_cmd = (size_t) len_cmd;

	ptr_spu -> cmd = (cmd_t*) calloc (len_cmd,  sizeof (cmd_t));
	ptr_spu -> reg = (reg_t*) calloc (SIZE_REG, sizeof (reg_t));
	ptr_spu -> ram = (ram_t*) calloc (SIZE_RAM, sizeof (ram_t));

	stk_ctor (&(ptr_spu -> stk),  DEFAULT_LEN_STK);
	stk_ctor (&(ptr_spu -> func), DEFAULT_LEN_STK);
}

static void destroy_spu (spu_t* ptr_spu)
{
	assert (ptr_spu);

	free (ptr_spu -> cmd);
	free (ptr_spu -> reg);
	free (ptr_spu -> ram);

	stk_dtor (&(ptr_spu -> stk));
	stk_dtor (&(ptr_spu -> func));
}

static void read_cmd_file (spu_t* ptr_spu, FILE* cmd_file)
{
	assert (ptr_spu);
	assert (cmd_file);

	fread (ptr_spu -> cmd, sizeof (cmd_t), ptr_spu -> size_cmd, cmd_file);
}
