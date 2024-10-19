#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../commands.h"
#include "../include/launch_asm.h"
#include "../include/verifier.h"

const size_t MAX_LEN_STR = 50;
const size_t SIZE_HEADER = 1;
const size_t SIZE_CMD    = 128;
const size_t SIZE_LABELS = 128;
const size_t SIZE_FIXUP  = 128;

static long read_file     (FILE*  asm_file, char** ptr_text);
static long len_file      (FILE*  file);  
static void close_files   (FILE*  asm_file, FILE* cmd_file);
static void creat_asm     (asm_t* ptr_assm);
static long verifier 	  (asm_t* ptr_assm, const char* file, int line);
static void print_error   (long   danger_bit);
static void detroy_asm    (asm_t* ptr_assm);
static void translate_asm (asm_t* ptr_assm, char* text);
static void print_asm     (asm_t* ptr_assm);
static long asm_error     (asm_t* ptr_assm, const char* file, int line);

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

		return assm.error_in_asm;
	}

	translate_asm (&assm, text);

	close_files (asm_file, cmd_file);

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
	ptr_assm -> fix_up = (fix_t*)   calloc (SIZE_FIXUP,  sizeof (fix_t));
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

	ptr_assm -> error_in_asm = NOT_ERROR;

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
		case CMD_NULL:    {printf ("pointer on cmd    == NULL; code_error == %ld\n", danger_bit);     break;}
		case FIX_UP_NULL: {printf ("pointer on fix_up == NULL; code_error == %ld\n", danger_bit);     break;}
		case LABELS_NULL: {printf ("pointer on labels == NULL; code_error == %ld\n", danger_bit);     break;}
		default:          {printf ("this error not find: %ld\n",                     danger_bit);     break;}
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
		printf ("%3ld ", (ptr_assm -> cmd)[index_cmd]);
	}
	printf ("\n\n");

	//----------------------------------------------------------------------------------------

	printf ("labels:\n");

	size_t len_labels = ptr_assm -> labels_count;
	for (size_t index_labels = 0; index_labels < len_labels; index_labels++)
	{
		printf ("%3ld ", index_labels);
		printf ("%3ld ", ((ptr_assm -> labels)[index_labels]).ip_label);
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
	printf ("----------------------------------------------------------------------------------------------------------------------------------------\n\n");

	getchar ();
}
















static void translate_asm (asm_t* ptr_assm, char* text)
{
	assert (ptr_assm);
	assert (text);

	char  word_in_text[MAX_LEN_STR] = "";
	int   index_text                = 0;
	char* ptr_text                  = text; 
	
	while (sscanf (ptr_text, "%s%n", word_in_text, &index_text) != EOF)
	{
		printf ("word from sscanf: %s\n", word_in_text);
		print_asm (ptr_assm);
		ptr_text += index_text;

		if (word_in_text[0] == '#') 
		{
			ptr_text = strchr(ptr_text, '\n') + 1;
			continue;
		}

		if (strcmp (word_in_text, "push") == 0)
		{		

		}
		
	}
}










































//translate_asm

/*int  static read_file    (asm_t* ptr_assm, FILE* cmd_file);
int  static write_file   (asm_t* ptr_assm, FILE* code_file);
void static creat_asm    (asm_t* ptr_assm);
void static destroy_asm  (asm_t* ptr_assm);
// static check_labels (label_t* ptr_labels, size_t ip, char* str);


#define COMPARE_STR_CMD(str, name_cmd, command)                                        \
	if (strcmp (str, name_cmd) == 0)                                                   \
	{                                                                                  \
		stk_push (&(*ptr_assm).cmd, command);                                          \
		ip += 1;                                                                       \
		continue;                                                                      \
	}              

#define COMPARE_OM_JUMP(str, name_cmd, command)      \
	if (strcmp (str, name_cmd) == 0)                 \
	{                                                \
		stk_push (&((*ptr_assm).cmd), command);      \
                                                     \
		int arg = 0;                                 \
		fscanf  (cmd_file, "%x", &arg);              \
                                                     \
		stk_push (&((*ptr_assm).cmd), arg);          \
		continue;                                    \
	}

#define DEBUG

#ifdef DEBUG
	void static print_cmd (stk_t* ptr_cmd);  
#endif

//-------------------------------------------------------------------------------------------------------------

int translation (FILE* cmd_file, FILE* code_file) // TODO bad naming
{
	assert (cmd_file);
	assert (code_file);

	asm_t assm = {};

	creat_asm (&assm);
	
	for (size_t index = 0; index < SIZE_HEADER; index++)
	{
		stk_push (&(assm.cmd), 0);
	}
	
	// TODO macro for fclose, fclose and dtor

	if (read_file (&assm, cmd_file) == BAD)
	{
		fprintf     (code_file, "BAD_FILE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		fclose      (code_file);
		fclose      (cmd_file);
		destroy_asm (&assm);

		return BAD; // TODO more error
	}

	*((assm.cmd).data + 0) = (long) (assm.cmd.size - SIZE_HEADER);

		return BAD;
	}

	fclose (code_file);
	if (write_file (&assm, code_file) == BAD)
	{
		fclose   (code_file);
		fclose   (cmd_file);
		destroy_asm (&assm);

	fclose (cmd_file);
	destroy_asm (&assm);

	return NICE;  
}
//--------------------------------------------------------------------------------------------------------------

int static read_file  (asm_t* ptr_assm, FILE* cmd_file)
{
	assert (ptr_assm);
	assert (cmd_file);

	// TODO it's better to read file like in Onegin

	char str[MAX_LETTERS] = ""; // TODO what's str

	size_t ip = 0;

	while (fscanf (cmd_file, "%s", str) != EOF) // TODO check for strings
	{
		#ifdef DEBUG // TODO ifndef NDEBUG
			print_cmd (&(*ptr_assm).cmd);
		#endif

		if (str[0] == '#') 
		{
			fgets (str, MAX_LETTERS, cmd_file); 
			continue;
		}

		//----------------------------------------------------------------------------------------------

		if (strcmp (str, "push") == 0)
		{		
			int arg = 0;
			if (fscanf  (cmd_file, "%d", &arg) != 0) 
			{
				stk_push (&(*ptr_assm).cmd, PUSH | 0x10); // TODO magic number
				stk_push (&(*ptr_assm).cmd, arg);
				ip += 2;
				continue;
			}

			char reg = 0;
			if (fscanf  (cmd_file, "%cX", &reg) != 0)
			{
				stk_push (&(*ptr_assm).cmd, PUSH | 0x20); // TODO magic number

				for (int index_reg = 'A'; index_reg <= 'D'; index_reg++) // TODO MIN_REG and MAX_REG
				{
					if (reg == index_reg)
					{
						stk_push (&(*ptr_assm).cmd, index_reg - 'A' + 1);

						ip += 2;

						break;
					}
				}
				continue;
			}
			
			printf ("ERROR IN SNT PUSH\n");
			abort ();
		}

		if (strcmp (str, "pop") == 0)
		{
			stk_push (&(*ptr_assm).cmd, POP | 0x20);

			char reg[MAX_LETTERS] = "";
			fscanf (cmd_file, "%s", reg);

			for (int index_reg = 'A'; index_reg <= 'D'; index_reg++)
			{
				if (reg[0] == index_reg)
				{
					stk_push (&(*ptr_assm).cmd, index_reg - 'A' + 1);

					ip += 2;
					break;
				}
			}
			continue;
		}

		//-----------------------------------------------------------------------------------------------------

		COMPARE_OM_JUMP(str, "jmp", JMP);
		COMPARE_OM_JUMP(str, "ja",  JA);
		COMPARE_OM_JUMP(str, "jb",  JB);

		//------------------------------------------------------------------------------------------------------

		COMPARE_STR_CMD(str, "out",   OUT);
		COMPARE_STR_CMD(str, "add",   ADD);
		COMPARE_STR_CMD(str, "sub",   SUB);
		COMPARE_STR_CMD(str, "mul",   MUL);
		COMPARE_STR_CMD(str, "div",   DIV);
		COMPARE_STR_CMD(str, "in",    IN);
		COMPARE_STR_CMD(str, "sqrt",  SQRT);
		COMPARE_STR_CMD(str, "sin",   SIN);
		COMPARE_STR_CMD(str, "dump",  DUMP);
		COMPARE_STR_CMD(str, "guide", GUIDE);
		COMPARE_STR_CMD(str, "hlt",   HLT);

		// if (strchr (str, ':') != EOF)
		// {
		// 	check_labels ((*ptr_assm).labels, ip, str);
		// }

		printf ("SNT_ERROR:\n command: '%s' don't find\n", str);

		return BAD;
	}

	#ifdef DEBUG
		print_cmd (&(*ptr_assm).cmd);
	#endif

	return NICE;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

int static write_file (asm_t* ptr_assm, FILE* code_file)
{
	assert (ptr_assm);
	assert (code_file);

	for (size_t index = 0; index < SIZE_HEADER; index++)
	{
		
		fprintf (code_file, "%ld\n", *((*ptr_assm).cmd.data + index));
	}

	for (size_t index = SIZE_HEADER; index < (*ptr_assm).cmd.size; index++)
	{
		fprintf (code_file, "%lx ", *((*ptr_assm).cmd.data + index));
	}

	fprintf (code_file, "\n");

	return NICE;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------



void static destroy_asm (asm_t* ptr_assm)
{
	assert (ptr_assm);

	stk_dtor (&((*ptr_assm).cmd));
	free     ((*ptr_assm).labels);
	free     ((*ptr_assm).fix_up);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

#ifdef DEBUG
	void static print_cmd (stk_t* ptr_cmd)
	{
		assert (ptr_cmd);

		for (size_t ip = 0; ip < (*ptr_cmd).size; ip++)
		{
			printf ("%3ld ", ip);
		}
		printf ("\n");

		for (size_t ip = 0; ip < (*ptr_cmd).size; ip++)
		{
			printf ("%3lx ", (*ptr_cmd).data [ip]);
		}
		printf ("\n");

		getchar ();		
	}
#endif

// void static check_labels (label_t* ptr_labels, size_t ip, char* str)
// {
// 	printf (":)");
// }
*/