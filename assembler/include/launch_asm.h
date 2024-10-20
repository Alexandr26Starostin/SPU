#ifndef TRANSLATION_H
#define TRANSLATION_H

struct fix_t
{
	size_t cmd_fix;
	size_t label_fix;
};

struct label_t 
{
	size_t ip_label;
	char*  name_label;
};

struct asm_t
{
	long     error_in_asm;
	long*    cmd;
	fix_t*   fix_up;
	label_t* labels;
	size_t   cmd_count;
	size_t   fix_up_count;
	size_t   labels_count;
};

long launch_asm (FILE* asm_file, FILE* cmd_file);

#endif