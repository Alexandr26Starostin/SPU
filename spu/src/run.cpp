#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../stack/include/const_define_struct.h"
#include "../../stack/include/hash.h"
#include "../../stack/include/operations_with_stack.h"
#include "../../stack/include/stk_error.h"

#include "../include/run.h"

const size_t MAX_LETTERS     = 50;
const size_t DEFAULT_LEN_STK = 16;

void run ()
{
	stk_t stk = {};

	stk_ctor (&stk, DEFAULT_LEN_STK);

	while (true)
	{
		char cmd[MAX_LETTERS] = "";

		scanf ("%s", cmd);

		//------------------------------------------------------------------------------------------

		if (strcmp ("push", cmd) == 0)
		{
			element_t arg = 0;
			scanf ("%d", &arg);

			stk_push (&stk, arg);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (strcmp ("add", cmd) == 0)
		{
			element_t a = 0, b = 0;
			stk_pop (&stk, &a);
			stk_pop (&stk, &b);

			stk_push (&stk, b + a);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (strcmp ("sub", cmd) == 0)
		{
			element_t a = 0, b = 0;
			stk_pop (&stk, &a);
			stk_pop (&stk, &b);

			stk_push (&stk, b - a);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (strcmp ("mul", cmd) == 0)
		{
			element_t a = 0, b = 0;
			stk_pop (&stk, &a);
			stk_pop (&stk, &b);

			stk_push (&stk, b * a);	

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (strcmp ("div", cmd) == 0)
		{
			element_t a = 0, b = 0;
			stk_pop (&stk, &a);
			stk_pop (&stk, &b);

			stk_push (&stk, b / a);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (strcmp ("out", cmd) == 0)
		{
			element_t arg = 0;
			stk_pop (&stk, &arg);

			printf ("%d\n", arg);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (strcmp ("in", cmd) == 0)
		{
			element_t arg = 0;
			scanf ("%d", &arg);

			stk_push (&stk, arg);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (strcmp ("sqrt", cmd) == 0)
		{
			element_t arg = 0;
			stk_pop (&stk, &arg);

			stk_push (&stk, (int) sqrt ((double) arg));
			
			continue;
		}

		//------------------------------------------------------------------------------------------

		if (strcmp ("sin", cmd) == 0)
		{
			element_t arg = 0;
			stk_pop (&stk, &arg);

			stk_push (&stk, (int) sin ((double) arg));

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (strcmp ("cos", cmd) == 0)
		{
			element_t arg = 0;
			stk_pop (&stk, &arg);

			stk_push (&stk, (int) cos ((double) arg));

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (strcmp ("dump", cmd) == 0)
		{
			stk_dump (&stk, __FILE__, __LINE__);

			continue;
		}

		//------------------------------------------------------------------------------------------

		if (strcmp ("hlt", cmd) == 0)
		{
			stk_dtor (&stk);

			break;
		}

		//------------------------------------------------------------------------------------------

		printf ("SNT_ERROR: '%s'\n", cmd);
	}
}