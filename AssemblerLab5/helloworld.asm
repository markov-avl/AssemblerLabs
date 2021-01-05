global Start

extern printf
extern scanf

section .data

	msg db "Prompt: ", 0
	fmt db "%s", 0

section .bss

	inpt resb 1000

section .text
	
	Start:
		push ebp
		mov ebp, esp

		push msg
		call printf
		add esp, 4

		push inpt
		push fmt
		call scanf
		add esp, 8

		push inpt
		call printf
		add esp, 4

		mov esp, ebp
		pop ebp

		ret