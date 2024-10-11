#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../stack/include/const_define_struct.h"
#include "../../stack/include/hash.h"
#include "../../stack/include/operations_with_stack.h"
#include "../../stack/include/stk_error.h"

#include "../include/run.h"

const size_t MAX_LETTERS     = 30;
const size_t DEFAULT_LEN_STK = 32;

void run ()
{
	stk_t stk = {};
	if (stk_ctor (&stk, DEFAULT_LEN_STK) != 0) {abort ();}

	FILE* guide_file = fopen ("../guide.txt", "r");
	if (guide_file == NULL) {printf ("Can't open guide.txt\n"); abort ();}

	while (true)
	{
		int cmd = 0;

		scanf ("%d", &cmd);

		//------------------------------------------------------------------------------------------

		if (GUIDE == cmd)
		{
			char str[MAX_LETTERS] = "";
			while (fgets (str, MAX_LETTERS, guide_file) != NULL)
			{
				printf ("%s", str);
			}

			printf ("\n");

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (PUSH == cmd)
		{
			element_t arg = 0;
			scanf ("%d", &arg);

			stk_push (&stk, arg);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (ADD == cmd)
		{
			element_t a = 0, b = 0;
			stk_pop (&stk, &a);
			stk_pop (&stk, &b);

			stk_push (&stk, b + a);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (SUB == cmd)
		{
			element_t a = 0, b = 0;
			stk_pop (&stk, &a);
			stk_pop (&stk, &b);

			stk_push (&stk, b - a);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (MUL == cmd)
		{
			element_t a = 0, b = 0;
			stk_pop (&stk, &a);
			stk_pop (&stk, &b);

			stk_push (&stk, b * a);	

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (DIV == cmd)
		{
			element_t a = 0, b = 0;
			stk_pop (&stk, &a);
			stk_pop (&stk, &b);

			stk_push (&stk, b / a);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (OUT == cmd)
		{
			element_t arg = 0;
			stk_pop (&stk, &arg);

			printf ("%d\n", arg);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (IN == cmd)
		{
			element_t arg = 0;
			scanf ("%d", &arg);

			stk_push (&stk, arg);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (SQRT == cmd)
		{
			element_t arg = 0;
			stk_pop (&stk, &arg);

			stk_push (&stk, (int) sqrt ((double) arg));
			
			continue;
		}

		//------------------------------------------------------------------------------------------

		if (SIN == cmd)
		{
			element_t arg = 0;
			stk_pop (&stk, &arg);

			stk_push (&stk, (int) sin ((double) arg));

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (COS == cmd)
		{
			element_t arg = 0;
			stk_pop (&stk, &arg);

			stk_push (&stk, (int) cos ((double) arg));

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (DUMP == cmd)
		{
			stk_dump (&stk, __FILE__, __LINE__);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (HLT == cmd)
		{
			stk_dtor (&stk);

			break;
		}

		//------------------------------------------------------------------------------------------

		printf ("SNT_ERROR: '%d'\n", cmd);
	}

	 fclose (guide_file);
}