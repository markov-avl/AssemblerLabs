NULL EQU 0
STD_OUTPUT_HANDLE EQU -11
STD_INPUT_HANDLE EQU -10

extern _GetStdHandle@4
extern _WriteFile@20
extern _ReadFile@20
extern _ExitProcess@4

global Start

section .data

	message db "Hello", 0xD, 0xA
	buffer  db 10 dup (0)
	char    dd 0
	length  equ $-message

section .bss

	StandardHandle resd 1
	Written resd 1
	Read resd 1

section .text
	
	Start:
		push STD_INPUT_HANDLE
		call _GetStdHandle@4
		mov dword [StandardHandle], eax

		push NULL
		push Read
		push 10
		push buffer
		push dword [StandardHandle]
		call _ReadFile@20

		push STD_OUTPUT_HANDLE
		call _GetStdHandle@4
		mov dword [StandardHandle], eax

		push NULL
		push Written
		push 1
		push buffer
		push dword [StandardHandle]
		call _WriteFile@20

		push STD_OUTPUT_HANDLE
		call _GetStdHandle@4
		mov dword [StandardHandle], eax

		push NULL
		push Written
		push 1
		push buffer
		push dword [StandardHandle]
		call _WriteFile@20

		push NULL
		call _ExitProcess@4