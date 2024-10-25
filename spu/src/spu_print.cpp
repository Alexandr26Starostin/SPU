#include <stdio.h>
#include <assert.h>

#include "../../commands.h"
#include "../include/launch_spu.h"
#include "../include/spu_print.h"

#define PRINT_CMD_
#define PRINT_REG_
#define PRINT_RAM_
#define PRINT_STK_
#define PRINT_FUNC_

#ifdef PRINT_CMD
	static void print_cmd (spu_t* ptr_spu, size_t position);
#endif

#ifdef PRINT_REG
	static void print_reg (spu_t* ptr_spu);
#endif 

#ifdef PRINT_RAM
	static void print_ram (spu_t* ptr_spu);
#endif

#ifdef PRINT_STK
	static void print_stk (spu_t* ptr_spu);
#endif

#ifdef PRINT_FUNC
	static void print_func (spu_t* ptr_spu);
#endif

//------------------------------------------------------------------------------------------------------------------------------------------

void print_spu (spu_t* ptr_spu, size_t position)
{
	assert (ptr_spu);

	#ifdef PRINT_CMD
		print_cmd (ptr_spu, position);
	#endif

	#ifdef PRINT_REG
		print_reg (ptr_spu);
	#endif
	
	#ifdef PRINT_RAM
		print_ram (ptr_spu);
	#endif

	#ifdef PRINT_STK
		print_stk (ptr_spu);
	#endif

	#ifdef PRINT_FUNC
		print_func (ptr_spu);
	#endif

	#if  defined (PRINT_CMD) || defined (PRINT_REG) || defined (PRINT_RAM) || defined (PRINT_STK) || defined (PRINT_FUNC)
		printf ("\n------------------------------------------------------------------------------------------------------------\n\n");
		getchar ();
	#else
		size_t position_false = 0;
		position_false       += position;
	#endif
}

//---------------------------------------------------------------------------------------------------------------------------------------------

#ifdef PRINT_CMD
	static void print_cmd (spu_t* ptr_spu, size_t position)
	{
		assert (ptr_spu);

		printf ("cmd:");

		for (size_t ip = 0; ip < ptr_spu -> size_cmd; ip++)
		{
			printf ("%3ld ", ip);
		}
		printf ("\n");

		for (size_t ip = 0; ip < ptr_spu -> size_cmd; ip++)
		{
			printf ("%3x ", (ptr_spu -> cmd)[ip]);
		}
		printf ("\n");

		for (size_t ip = 0; ip < position; ip++)
		{
			printf ("    ");
		}
		printf ("  !\n\n");
	}
#endif

#ifdef PRINT_REG
	static void print_reg (spu_t* ptr_spu)
	{
		assert (ptr_spu);

		printf ("reg:\n\n");

		char min_latter = 'A';
		char max_latter = 'A' - 1 + SIZE_REG;

		printf ("0X ");

		for (char latter = min_latter; latter < max_latter; latter++)
		{
			printf ("%cX ", latter);
		}

		printf ("\n");

		for (size_t index_reg = 0; index_reg < SIZE_REG; index_reg++)
		{
			printf ("%x  ", (ptr_spu -> reg)[index_reg]);
		}

		printf ("\n\n");
	}
#endif

#ifdef PRINT_RAM
	static void print_ram (spu_t* ptr_spu)
	{
		assert (ptr_spu);

		printf ("ram:\n\n");

		for (size_t index_ram = 0; index_ram < SIZE_RAM; index_ram++)
		{
			printf ("%3ld ", index_ram);
		}
		printf ("\n");

		for (size_t index_ram = 0; index_ram < SIZE_RAM; index_ram++)
		{
			printf ("%3x ", (ptr_spu -> ram)[index_ram]);
		}
		printf ("\n\n");
	}
#endif

#ifdef PRINT_STK
	static void print_stk (spu_t* ptr_spu)
	{
		assert (ptr_spu);

		printf ("stk:\n\n");

		stk_dump  (&(ptr_spu -> stk), __FILE__, __LINE__);

		printf ("\n");
	}
#endif

#ifdef PRINT_FUNC
	static void print_func (spu_t* ptr_spu)
	{
		assert (ptr_spu);

		printf ("func:\n\n");

		stk_dump (&(ptr_spu -> func), __FILE__, __LINE__);
	}
#endif
