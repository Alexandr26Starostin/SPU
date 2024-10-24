#ifndef COMMANDS_H
#define COMMANDS_H

enum command_t : long
{
	GUIDE = 0x00,
	PUSH  = 0x01,
	ADD   = 0x02,
	SUB   = 0x03,
	MUL   = 0x04,
	DIV   = 0x05,
	OUT   = 0x06,
	IN    = 0x07,
	SQRT  = 0x08,
	SIN   = 0x09,
	DUMP  = 0x0A,
	HLT   = 0x0B,
	POP   = 0x0C,
	JMP   = 0x0D,
	JA    = 0x0E,
	JB    = 0x0F,
	CALL  = 0x10,
	RET   = 0x11,
	DRAW  = 0x12
};

#endif