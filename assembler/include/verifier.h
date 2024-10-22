#ifndef VERIFIER_H
#define VERIFIER_H

enum error_t
{
	PTR_TEXT_NULL      = -4,
	ERROR_IN_FREAD     = -3,
	NOT_ERROR          = 0,
	CMD_NULL           = 1,
	FIX_UP_NULL        = 2,
	LABELS_NULL        = 4,
	SIXE_CMD_EXCEED    = 8,
	SIXE_LABELS_EXCEED = 16,
	SIXE_FIX_UP_EXCEED = 32,
	PUSH_INCORRECT     = 64,
	POP_INCORRECT      = 128,
	COMMAND_INCORRECT  = 256,
	REPEATED_LABEL     = 512,
	LABEL_INCORRECT    = 1024
};

const size_t MAX_BIT = sizeof (long) * 8;

#endif