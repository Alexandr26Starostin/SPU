#ifndef VERIFIER_H
#define VERIFIER_H

enum error_t
{
	PTR_TEXT_NULL  = -4,
	ERROR_IN_FREAD = -3,
	NOT_ERROR      = 0,
	CMD_NULL       = 1,
	FIX_UP_NULL    = 2,
	LABELS_NULL    = 4
};

const size_t MAX_BIT = sizeof (long) * 8;

#endif