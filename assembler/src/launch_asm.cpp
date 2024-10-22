#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "../../commands.h"
#include "../include/launch_asm.h"
#include "../include/verifier.h"

const size_t MAX_LEN_STR = 50;
const size_t SIZE_HEADER = 1;
const size_t SIZE_CMD    = 128;
const size_t SIZE_LABELS = 128;
const size_t SIZE_FIX_UP  = 128;

static long   read_file       (FILE*  asm_file, char** ptr_text);
static long   len_file        (FILE*  file);  
static void   close_files     (FILE*  asm_file, FILE* cmd_file);
static void   creat_asm       (asm_t* ptr_assm);
static long   verifier 	      (asm_t* ptr_assm, const char* file, int line);
static void   print_error     (long   danger_bit);
static void   detroy_asm      (asm_t* ptr_assm);
static long   translate_asm   (asm_t* ptr_assm, char* text);
static void   print_asm       (asm_t* ptr_assm);
static long   asm_error       (asm_t* ptr_assm, const char* file, int line);
static void   write_in_file   (asm_t* ptr_assm, FILE* code_file);
static void   write_in_labels (asm_t* ptr_assm, char* label);
static long   find_label      (asm_t* ptr_assm, char* label);
static void   write_in_fix_up (asm_t* ptr_assm);
static void   read_fix_up     (asm_t* ptr_assm);

//-----------------------------------------------------------------------------------------------------------------------------------------------

long launch_asm (FILE* asm_file, FILE* cmd_file)
{
	assert (asm_file);
	assert (cmd_file);

	char* text             = NULL;
	long  status_read_file = NOT_ERROR;

	status_read_file = read_file (asm_file, &text);

	if (status_read_file)
	{
		printf ("%s:%d ERROR in read_file\n", __FILE__, __LINE__);
		close_files (asm_file, cmd_file);
		return status_read_file; 
	}

	asm_t assm = {};
	creat_asm (&assm);

	if (asm_error (&assm, __FILE__, __LINE__))
	{
		close_files (asm_file, cmd_file);
		detroy_asm  (&assm);
		free (text);

		return assm.error_in_asm;
	}

	if (translate_asm (&assm, text))
	{
		close_files (asm_file, cmd_file);
		detroy_asm  (&assm);
		free (text);

		return assm.error_in_asm;
	}

	read_fix_up (&assm);

	print_asm (&assm);

	write_in_file (&assm, cmd_file);

	close_files (asm_file, cmd_file);
	detroy_asm  (&assm);
	free (text);

	return NOT_ERROR;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

static long read_file (FILE* asm_file, char** ptr_text)
{
	assert (asm_file);
	assert (ptr_text);

    long count_memory = len_file (asm_file);

	*ptr_text = (char*) calloc (count_memory, sizeof (char));

	if (*ptr_text == NULL) 
	{
		printf ("%s:%d  *text == NULL\n", __FILE__, __LINE__);
		return (long) PTR_TEXT_NULL; 
	}

	long count_fread = (long) fread (*ptr_text, sizeof (char), count_memory, asm_file);

	if (count_fread != count_memory)
	{
		printf ("count_fread  != count_memory\n");
		printf ("count_fread  == %ld\n", count_fread);
		printf ("count_memory == %ld\n", count_memory);

		return (long) ERROR_IN_FREAD;
	}

	return (long) NOT_ERROR;
}

static long len_file (FILE* file)      
{ 
    assert (file);

    fseek (file, 0, SEEK_END);
    const long count_memory = ftell (file);
    fseek (file, 0, SEEK_SET);

    return count_memory;
}

static void close_files (FILE* cmd_file, FILE* code_file)
{
	assert (cmd_file);
	assert (code_file);

	fclose (cmd_file);
	fclose (code_file);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

static void creat_asm (asm_t* ptr_assm)
{
 	assert (ptr_assm);

	ptr_assm -> error_in_asm = NOT_ERROR;

	ptr_assm -> cmd_count    = 0;
	ptr_assm -> fix_up_count = 0;
	ptr_assm -> labels_count = 0;

	ptr_assm -> cmd    = (long*)    calloc (SIZE_CMD,    sizeof (long));
	ptr_assm -> fix_up = (fix_t*)   calloc (SIZE_FIX_UP,  sizeof (fix_t));
	ptr_assm -> labels = (label_t*) calloc (SIZE_LABELS, sizeof (label_t));
}

static void  detroy_asm  (asm_t* ptr_assm)
{
	assert (ptr_assm);

	free (ptr_assm -> cmd);
	free (ptr_assm -> fix_up);
	free (ptr_assm -> labels);
}

//-----------------------------------------------------------------------------------------------------------------------------

static long asm_error (asm_t* ptr_assm, const char* file, int line)
{
	assert (ptr_assm);

	if (ptr_assm -> cmd == NULL)
	{
		ptr_assm -> error_in_asm |= CMD_NULL;
	}

	if (ptr_assm -> fix_up == NULL)
	{
		ptr_assm -> error_in_asm |= FIX_UP_NULL;
	}

	if (ptr_assm -> labels == NULL)
	{
		ptr_assm -> error_in_asm |= LABELS_NULL;
	}

	if (ptr_assm -> cmd_count >= SIZE_CMD)
	{
		ptr_assm -> error_in_asm |= SIXE_CMD_EXCEED;
	}

	if (ptr_assm -> labels_count >= SIZE_LABELS)
	{
		ptr_assm -> error_in_asm |= SIXE_LABELS_EXCEED;
	}

	if (ptr_assm -> fix_up_count >= SIZE_FIX_UP)
	{
		ptr_assm -> error_in_asm |= SIXE_FIX_UP_EXCEED;
	}

	long status = ptr_assm -> error_in_asm;

	if (status)
	{
		verifier (ptr_assm, file, line);
	}

	return status;
}

static long verifier (asm_t* ptr_assm, const char* file, int line)
{
	assert (ptr_assm);
	assert (file);
	long status = ptr_assm -> error_in_asm;

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

static void print_error (long danger_bit)
{
	switch (danger_bit)
	{
		case CMD_NULL:           {printf ("pointer on cmd    == NULL;                                     code_error == %ld\n", danger_bit);     break;}
		case FIX_UP_NULL:        {printf ("pointer on fix_up == NULL;                                     code_error == %ld\n", danger_bit);     break;}
		case LABELS_NULL:        {printf ("pointer on labels == NULL;                                     code_error == %ld\n", danger_bit);     break;}
		case SIXE_CMD_EXCEED:    {printf ("exceed the size of cmd;    change the max size of cmd;         code_error == %ld\n", danger_bit);     break;}
		case SIXE_LABELS_EXCEED: {printf ("exceed the size of labels; change the max size of labels;      code_error == %ld\n", danger_bit);     break;}
		case SIXE_FIX_UP_EXCEED: {printf ("exceed the size of fix_up; change the max size of fix_up;      code_error == %ld\n", danger_bit);     break;}
		case PUSH_INCORRECT:     {printf ("push incorrect; use 'print_asm' to find, where error was born; code_error == %ld\n", danger_bit);     break;}
		case POP_INCORRECT:      {printf ("pop  incorrect; use 'print_asm' to find, where error was born; code_error == %ld\n", danger_bit);     break;}
		case COMMAND_INCORRECT:  {printf ("command incorrect; code_error == %ld\n",                                             danger_bit);     break;}
		case REPEATED_LABEL:     {printf ("label defined in two places;                                   code_error == %ld\n", danger_bit);     break;}
		case LABEL_INCORRECT:    {printf ("not find ':' in label;                                         code_error == %ld\n", danger_bit);     break;}
		default:                 {printf ("this error not find: %ld\n",                                                         danger_bit);     break;}
	}
}




//-------------------------------------------------------------------------------------------------------------------------------------------------------------


static void print_asm (asm_t* ptr_assm)
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
		printf ("%3lx ", (ptr_assm -> cmd)[index_cmd]);
	}
	printf ("\n\n");

	//----------------------------------------------------------------------------------------

	printf ("labels:\n");

	size_t len_labels = ptr_assm -> labels_count;
	for (size_t index_labels = 0; index_labels < len_labels; index_labels++)
	{
		printf ("%3ld ", index_labels);
		printf ("%3lx ", ((ptr_assm -> labels)[index_labels]).ip_label);
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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

static long translate_asm (asm_t* ptr_assm, char* text)
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

			long put_push = PUSH;

			if ((word_in_text[0] == '[') && (strchr (word_in_text, ']') != NULL))
			{
				put_push |= 0x40;
			}

			char* ptr_on_PLUS = strchr (word_in_text, '+');
			char* ptr_on_X    = strchr (word_in_text, 'X');

			if (ptr_on_PLUS != NULL)
			{
				put_push |= (0x20 | 0x10);
				(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_push;
			}

			if (ptr_on_X != NULL)
			{
				if (ptr_on_PLUS == NULL)
				{
					put_push |= 0x20;
					(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_push;
				}

				(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = (long) *(ptr_on_X - 1) - 'A' + 1;
			}

			if (ptr_on_PLUS != NULL)
			{
				ptr_on_PLUS += 1;
				//while (ptr_on_PLUS[0] ==' ') {ptr_on_PLUS += 1;}

				long arg = 0;

				if (sscanf (ptr_on_PLUS, "%ld", &arg) != 0)
				{
					(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;
				}
			}

			if ((put_push & (0x20 | 0x10)) == 0)
			{
				size_t index_latter = 0;
				
				while (word_in_text [index_latter] != '\0')
				{
					if (isdigit(word_in_text [index_latter]))
					{
						put_push |= 0x10;

						(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_push;

						long arg = 0;

						sscanf (word_in_text + index_latter, "%ld", &arg);

						(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;
						
						break;
					}

					index_latter++;
				}
			}

			if ((put_push & (0x20 | 0x10)) == 0)
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

			long put_pop = POP;

			if ((word_in_text[0] == '[') && (strchr (word_in_text, ']') != NULL))
			{
				put_pop |= 0x40;
			}

			char* ptr_on_PLUS = strchr (word_in_text, '+');
			char* ptr_on_X    = strchr (word_in_text, 'X');

			if (ptr_on_PLUS != NULL && ((put_pop & 0x40) == 0))
			{
				ptr_assm -> error_in_asm |= POP_INCORRECT;
				continue;
			}

			if (ptr_on_PLUS != NULL)
			{
				put_pop |= (0x20 | 0x10);
				(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_pop;
			}

			if (ptr_on_X != NULL)
			{
				if (ptr_on_PLUS == NULL)
				{
					put_pop |= 0x20;
					(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_pop;
				}

				(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = (long) *(ptr_on_X - 1) - 'A' + 1;
			}

			if (ptr_on_PLUS != NULL)
			{
				ptr_on_PLUS += 1;
				//while (ptr_on_PLUS[0] ==' ') {ptr_on_PLUS += 1;}

				long arg = 0;

				if (sscanf (ptr_on_PLUS, "%ld", &arg) != 0)
				{
					(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;
				}
			}

			if ((put_pop & (0x20 | 0x10)) == 0 && (put_pop & (0x40)) != 0)
			{
				size_t index_latter = 0;
				
				while (word_in_text [index_latter] != '\0')
				{
					if (isdigit(word_in_text [index_latter]))
					{
						put_pop |= 0x10;

						(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = put_pop;

						long arg = 0;

						sscanf (word_in_text + index_latter, "%ld", &arg);

						(ptr_assm -> cmd) [(ptr_assm -> cmd_count)++] = arg;
						
						break;
					}

					index_latter++;
				}
			}

			if ((put_pop & (0x20 | 0x10)) == 0)
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
				(ptr_assm -> cmd) [ptr_assm -> cmd_count] = ip_label;
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
				(ptr_assm -> cmd) [ptr_assm -> cmd_count] = label;
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
				(ptr_assm -> cmd) [ptr_assm -> cmd_count] = label;
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

		if (strchr(word_in_text, ':') != NULL)
		{
			write_in_labels (ptr_assm, word_in_text);
			continue;
		}

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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void write_in_file (asm_t* ptr_assm, FILE* code_file)
{
	assert (ptr_assm);
	assert (code_file);

	long hdr[SIZE_HEADER] = {};
	hdr[0] = ptr_assm -> cmd_count;

	fwrite  (hdr,             sizeof (long), SIZE_HEADER,           code_file);
	fwrite  (ptr_assm -> cmd, sizeof (long), ptr_assm -> cmd_count, code_file);
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void write_in_labels (asm_t* ptr_assm, char* label)
{
	assert (ptr_assm);
	assert (label);

	size_t index_label = 0;

	for (index_label = 0; index_label < (ptr_assm -> labels_count); index_label++)
	{
		if (strcmp (label, ((ptr_assm -> labels) [index_label]).name_label) == 0)
		{
			if (((ptr_assm -> labels) [index_label]).ip_label != -1)
			{
				ptr_assm -> error_in_asm |= REPEATED_LABEL;
			}

			break;
		}
	}

	((ptr_assm -> labels) [index_label]).ip_label   = ptr_assm -> cmd_count;
	strcpy(((ptr_assm -> labels) [index_label]).name_label, label);

	if (index_label == (ptr_assm -> labels_count))
	{
		(ptr_assm -> labels_count) += 1;
	}
}

static long find_label (asm_t* ptr_assm, char* label)
{
	assert (ptr_assm);
	assert (label);

	for (size_t index_label = 0; index_label < ptr_assm -> labels_count; index_label++)
	{
		if (strcmp (label, ((ptr_assm -> labels) [index_label]).name_label) == 0)
		{
			return ((ptr_assm -> labels) [index_label]).ip_label;
		}
	}

	((ptr_assm -> labels) [ptr_assm -> labels_count]).ip_label = -1;
	strcpy(((ptr_assm -> labels) [ptr_assm -> labels_count]).name_label, label);

	(ptr_assm -> labels_count) += 1;

	return -1;
}

static void write_in_fix_up (asm_t* ptr_assm)
{
	assert (ptr_assm);

	((ptr_assm -> fix_up) [ptr_assm -> fix_up_count]).cmd_fix   = ptr_assm -> cmd_count;
	((ptr_assm -> fix_up) [ptr_assm -> fix_up_count]).label_fix = (ptr_assm -> labels_count) - 1;

	ptr_assm -> fix_up_count += 1;
}

static void read_fix_up (asm_t* ptr_assm)
{
	assert (ptr_assm);

	for (size_t index_fix_up = 0; index_fix_up < ptr_assm -> fix_up_count; index_fix_up++)
	{
		size_t index_true_label = ((ptr_assm -> fix_up) [index_fix_up]).label_fix;
		size_t true_ip_label = ((ptr_assm -> labels) [index_true_label]).ip_label;

		size_t index_incorrect_cmd = ((ptr_assm -> fix_up) [index_fix_up]).cmd_fix;

		(ptr_assm -> cmd) [index_incorrect_cmd] = true_ip_label;
	}
}