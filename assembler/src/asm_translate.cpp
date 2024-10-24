#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


#include "../../commands.h"
#include "../include/launch_asm.h"
#include "../include/asm_error.h"
#include "../include/asm_print.h"
#include "../include/asm_translate.h"
#include "../include/asm_labels.h"

const long   IMM_MASK    = 0x20;
const long   REG_MASK    = 0x40;
const long   MEM_MASK    = 0x80;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

long translate_asm (asm_t* ptr_assm, char* text)
{
	assert (ptr_assm);
	assert (text);

	char  word_in_text[MAX_LEN_STR] = "";
	int   index_text                = 0;
	char* ptr_text                  = text; 
	
	while (sscanf (ptr_text, "%s%n", word_in_text, &index_text) != EOF)
	{
		printf ("word from sscanf: %s\n", word_in_text);

		if (asm_error (ptr_assm, __FILE__, __LINE__))
		{
			return ptr_assm -> error_in_asm;
		}

		print_asm (ptr_assm);
		ptr_text += index_text;

		//-----------------------------------------------------------------------------------------------

		if (word_in_text[0] == '#') 
		{
			ptr_text = strchr(ptr_text, '\n') + 1;
			continue;
		}

		//-----------------------------------------------------------------------------------------------

		if (strcmp (word_in_text, "push") == 0)
		{		
			sscanf (ptr_text, "%s%n", word_in_text, &index_text);
			ptr_text += index_text;

			int put_push = PUSH;

			if ((word_in_text[0] == '[') && (strchr (word_in_text, ']') != NULL))
			{
				put_push |= MEM_MASK;
			}

			char* ptr_on_PLUS = strchr (word_in_text, '+');
			char* ptr_on_X    = strchr (word_in_text, 'X');

			if (ptr_on_PLUS != NULL)
			{
				put_push |= (REG_MASK | IMM_MASK);
				(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_push;
			}

			if (ptr_on_X != NULL)
			{
				if (ptr_on_PLUS == NULL)
				{
					put_push |= REG_MASK;
					(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_push;
				}

				(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = (long) *(ptr_on_X - 1) - 'A' + 1;
			}

			if (ptr_on_PLUS != NULL)
			{
				ptr_on_PLUS += 1;
				//while (ptr_on_PLUS[0] ==' ') {ptr_on_PLUS += 1;}

				int arg = 0;

				if (sscanf (ptr_on_PLUS, "%d", &arg) != 0)
				{
					(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;
				}
			}

			if ((put_push & (REG_MASK | IMM_MASK)) == 0)
			{
				size_t index_latter = 0;
				
				while (word_in_text [index_latter] != '\0')
				{
					if (isdigit(word_in_text [index_latter]))
					{
						put_push |= IMM_MASK;

						(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_push;

						int arg = 0;

						sscanf (word_in_text + index_latter, "%d", &arg);

						(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;
						
						break;
					}

					index_latter++;
				}
			}

			if ((put_push & (REG_MASK | IMM_MASK)) == 0)
			{
				ptr_assm -> error_in_asm |= PUSH_INCORRECT;
			}

			continue;
		}

		//-----------------------------------------------------------------------------------------------

		if (strcmp (word_in_text, "pop") == 0)
		{
			sscanf (ptr_text, "%s%n", word_in_text, &index_text);
			ptr_text += index_text;

			int put_pop = POP;

			if ((word_in_text[0] == '[') && (strchr (word_in_text, ']') != NULL))
			{
				put_pop |= MEM_MASK;
			}

			char* ptr_on_PLUS = strchr (word_in_text, '+');
			char* ptr_on_X    = strchr (word_in_text, 'X');

			if (ptr_on_PLUS != NULL && ((put_pop & MEM_MASK) == 0))
			{
				ptr_assm -> error_in_asm |= POP_INCORRECT;
				continue;
			}

			if (ptr_on_PLUS != NULL)
			{
				put_pop |= (REG_MASK | IMM_MASK);
				(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_pop;
			}

			if (ptr_on_X != NULL)
			{
				if (ptr_on_PLUS == NULL)
				{
					put_pop |= REG_MASK;
					(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_pop;
				}

				(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = (long) *(ptr_on_X - 1) - 'A' + 1;
			}

			if (ptr_on_PLUS != NULL)
			{
				ptr_on_PLUS += 1;
				//while (ptr_on_PLUS[0] ==' ') {ptr_on_PLUS += 1;}

				int arg = 0;

				if (sscanf (ptr_on_PLUS, "%d", &arg) != 0)
				{
					(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;
				}
			}

			if ((put_pop & (REG_MASK | IMM_MASK)) == 0 && (put_pop & MEM_MASK) != 0)
			{
				size_t index_latter = 0;
				
				while (word_in_text [index_latter] != '\0')
				{
					if (isdigit(word_in_text [index_latter]))
					{
						put_pop |= IMM_MASK;

						(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_pop;

						int arg = 0;

						sscanf (word_in_text + index_latter, "%d", &arg);

						(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;
						
						break;
					}

					index_latter++;
				}
			}

			if ((put_pop & (REG_MASK | IMM_MASK)) == 0)
			{
				ptr_assm -> error_in_asm |= POP_INCORRECT;
			}

			continue;
		}

		//-----------------------------------------------------------------------------------------------------------------------------------------------------------

		if (strcmp (word_in_text, "add") == 0)                                                   
		{                                                                                  
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = ADD;                                                                                                              
			continue;                                                                      
		}  

		if (strcmp (word_in_text, "out") == 0)                                                   
		{                                                                                  
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = OUT;                                                                                                              
			continue;                                                                      
		}  

		if (strcmp (word_in_text, "sub") == 0)                                                   
		{                                                                                  
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = SUB;                                                                                                              
			continue;                                                                      
		}  

		if (strcmp (word_in_text, "mul") == 0)                                                   
		{                                                                                  
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = MUL;                                                                                                              
			continue;                                                                      
		}  

		if (strcmp (word_in_text, "div") == 0)                                                   
		{                                                                                  
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = DIV;                                                                                                              
			continue;                                                                      
		}  

		if (strcmp (word_in_text, "in") == 0)                                                   
		{                                                                                  
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = IN;                                                                                                              
			continue;                                                                      
		}  

		if (strcmp (word_in_text, "sqrt") == 0)                                                   
		{                                                                                  
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = SQRT;                                                                                                              
			continue;                                                                      
		}  

		if (strcmp (word_in_text, "sin") == 0)                                                   
		{                                                                                  
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = SIN;                                                                                                              
			continue;                                                                      
		}  

		if (strcmp (word_in_text, "guide") == 0)                                                   
		{                                                                                  
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = GUIDE;                                                                                                              
			continue;                                                                      
		}  

		if (strcmp (word_in_text, "dump") == 0)                                                   
		{                                                                                  
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = DUMP;                                                                                                              
			continue;                                                                      
		}  

		if (strcmp (word_in_text, "hlt") == 0)                                                   
		{                                                                                  
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = HLT;                                                                                                              
			continue;                                                                      
		}  

		//----------------------------------------------------------------------------------------------------------------------------------
		
		if (strcmp (word_in_text, "jmp") == 0)                 
		{                                                
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = JMP;      

			sscanf (ptr_text, "%s%n", word_in_text, &index_text);

			ptr_text += index_text;

			if (strchr(word_in_text, ':') != NULL)
			{
				long ip_label = find_label (ptr_assm, word_in_text);
				(ptr_assm -> cmd) [ptr_assm -> cmd_count] = (int) ip_label;
				if (ip_label < 0)
				{
					write_in_fix_up (ptr_assm);
				}

				(ptr_assm -> cmd_count) += 1;
			}

			else 
			{
				ptr_assm -> error_in_asm |= LABEL_INCORRECT;
			}

			continue;                                   
		}

		if (strcmp (word_in_text, "ja") == 0)                 
		{                                                
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = JA;      

			sscanf (ptr_text, "%s%n", word_in_text, &index_text);

			ptr_text += index_text;

			if (strchr(word_in_text, ':') != NULL)
			{
				long label = find_label (ptr_assm, word_in_text);
				(ptr_assm -> cmd) [ptr_assm -> cmd_count] = (int) label;
				if (label < 0)
				{
					write_in_fix_up (ptr_assm);
				}

				(ptr_assm -> cmd_count) += 1;
			}

			else 
			{
				ptr_assm -> error_in_asm |= LABEL_INCORRECT;
			}

			continue;                                   
		}

		if (strcmp (word_in_text, "jb") == 0)                 
		{                                                
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = JB;      

			sscanf (ptr_text, "%s%n", word_in_text, &index_text);

			ptr_text += index_text;

			if (strchr(word_in_text, ':') != NULL)
			{
				long label = find_label (ptr_assm, word_in_text);
				(ptr_assm -> cmd) [ptr_assm -> cmd_count] = (int) label;
				if (label < 0)
				{
					write_in_fix_up (ptr_assm);
				}

				(ptr_assm -> cmd_count) += 1;
			}

			else 
			{
				ptr_assm -> error_in_asm |= LABEL_INCORRECT;
			}

			continue;                                   
		}


		//------------------------------------------------------------------------------------------------------------------------------------------

		if (strcmp (word_in_text, "call") == 0)
		{
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = CALL;

			sscanf (ptr_text, "%s%n", word_in_text, &index_text);

			ptr_text += index_text;

			if (strchr(word_in_text, ':') != NULL)
			{
				long label = find_label (ptr_assm, word_in_text);
				(ptr_assm -> cmd) [ptr_assm -> cmd_count] = (int) label;
				if (label < 0)
				{
					write_in_fix_up (ptr_assm);
				}

				(ptr_assm -> cmd_count) += 1;
			}

			else 
			{
				ptr_assm -> error_in_asm |= LABEL_INCORRECT;
			}

			continue;
		}

		if (strcmp (word_in_text, "ret") == 0)
		{
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = RET;                                                                                                              
			continue;
		}

		//-----------------------------------------------------------------------------------------------------------------------------------------------

		if (strchr(word_in_text, ':') != NULL)
		{
			write_in_labels (ptr_assm, word_in_text);
			continue;
		}

		//-----------------------------------------------------------------------------------------------------------------------------------------------

		if (strcmp (word_in_text, "draw") == 0)
		{
			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = DRAW;      

			int arg = 0;
			sscanf (ptr_text, "%d%n", &arg, &index_text);
			ptr_text += index_text;

			(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;

			continue;
		}

		//-----------------------------------------------------------------------------------------------------------------------------------------------

		printf ("SNT_ERROR:\n command: '%s' don't find\n", word_in_text);

		ptr_assm -> error_in_asm |= COMMAND_INCORRECT;
	}

	if (asm_error (ptr_assm, __FILE__, __LINE__))
	{
		return ptr_assm -> error_in_asm;
	}

	print_asm (ptr_assm);
	

	return ptr_assm -> error_in_asm;
}