#include <stdio.h>
#include <assert.h>

#include "../include/launch_spu.h"
#include "../include/spu_error.h"

//------------------------------------------------------------------------------------------------------------------------------------------

long spu_error (spu_t* ptr_spu, const char* file, int line)
{
	assert (ptr_spu);

	if (ptr_spu -> cmd == NULL)
	{
		ptr_spu -> error_in_spu |= CMD_NULL;
	}

	if (ptr_spu -> reg == NULL)
	{
		ptr_spu -> error_in_spu |= REG_NULL;
	}

	if (ptr_spu -> ram == NULL)
	{
		ptr_spu -> error_in_spu |= RAM_NULL;
	}

	long status = ptr_spu -> error_in_spu;

	if (status)
	{
		verifier (ptr_spu, file, line);
	}

	return status;
}

long verifier (spu_t* ptr_spu, const char* file, int line)
{
	assert (ptr_spu);
	assert (file);

	long status = ptr_spu -> error_in_spu;

	if (status)
	{
		printf ("Find error in %s:%d\n", file, line);

		for (size_t index_bit = 0; index_bit < MAX_BIT; index_bit++)
		{
			long danger_bit = (1 << index_bit) & status;
			if (danger_bit)
			{
				print_error (danger_bit);
			}
		}
	}
	return status;
}

void print_error (long danger_bit)
{
	switch (danger_bit)
	{
		case CMD_NULL:           	 {printf ("pointer on cmd == NULL;                                        code_error == %ld\n", danger_bit);     break;}
		case REG_NULL:        	     {printf ("pointer on reg == NULL;                                        code_error == %ld\n", danger_bit);     break;}
		case RAM_NULL:        	     {printf ("pointer on ram == NULL;                                        code_error == %ld\n", danger_bit);     break;}
		case NOT_FIND_GUIDE:         {printf ("not find 'guide file';                                         code_error == %ld\n", danger_bit);     break;}
		case INCORRECT_COMMAND:		 {printf ("incorrect command;                                             code_error == %ld\n", danger_bit);     break;}
		default:                 	 {printf ("this error not find: %ld\n",                                                         danger_bit);     break;}
	}
}
