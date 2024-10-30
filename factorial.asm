in

pop  BX  
push BX
pop  AX

call FACTORIAL:

push BX
out
hlt

# Factorial main func
# Destroy AX, BX
#
# Ret reg -> BX
FACTORIAL:

	push AX
	push 1
	sub

	pop  AX
	push AX
	push BX
	mul
	pop  BX

	push AX
	push 2

	ja END:

	call FACTORIAL:

	END:

ret
