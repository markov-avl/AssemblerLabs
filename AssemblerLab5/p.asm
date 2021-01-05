NULL              EQU 0
BUFFSIZE          EQU 64
STD_INPUT_HANDLE  EQU -10
STD_OUTPUT_HANDLE EQU -11
STD_COLOR         EQU 0x000F

extern _GetStdHandle@4
extern _ReadFile@20
extern _WriteFile@20
extern _SetConsoleTextAttribute@8
extern _ExitProcess@4

global Start


section .data

	buff     	   db BUFFSIZE dup (0)
	msg_clr        db 'Colored message: ', 0ah
	msg_no_clr     db 'Not colored message: ', 0ah
	msg_clr_len    EQU $-msg_clr
	msg_no_clr_len EQU $-msg_no_clr
	clr    	   	   EQU STD_COLOR


section .bss

	StandardHandle resd 1
	Write          resd 1
	Read           resd 1


section .text
	
	Start:
		; реализация ввода
		push    STD_INPUT_HANDLE
		call    _GetStdHandle@4
		mov     dword [StandardHandle], eax

		push 	NULL
		push 	Read
		push 	BUFFSIZE
		push 	buff
		push 	dword [StandardHandle]
		call 	_ReadFile@20

		; реализация вывода
		push    STD_OUTPUT_HANDLE
		call    _GetStdHandle@4
		mov     dword [StandardHandle], eax 

	PRINT:
		mov edx, clr
		dec edx
		push    edx
		push    eax
		call    _SetConsoleTextAttribute@8
		
		push    NULL
		push    Write
		push    BUFFSIZE
		push    buff
		push    dword [StandardHandle]
		call    _WriteFile@20

		push    STD_OUTPUT_HANDLE
		call    _GetStdHandle@4
		mov     dword [StandardHandle], eax

	; возвращаем цвет вывода и завершаем программу
	EXIT:
		push    STD_COLOR
		push    eax
		call    _SetConsoleTextAttribute@8

		push    NULL
		call    _ExitProcess@4