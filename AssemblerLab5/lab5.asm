; инициализация констант
NULL              EQU 0   ; просто 0 для удобства
BUFFER_SIZE       EQU 128 ; максимальная длина входной строки
STD_INPUT_HANDLE  EQU -10 ; дескриптор ввода
STD_OUTPUT_HANDLE EQU -11 ; дескриптор вывода
WHITE_COLOR       EQU 0xF ; значение для белого цвета (стандартный)
BLACK_COLOR       EQU 0x0 ; значение для черного цвета (который не будет использоваться)

; импорт необходимых функций из Win32API
extern _GetStdHandle@4            ; извлекает дескриптор ввода/вывода/ошибок
extern _ReadFile@20               ; сохраняет ввод из консоли
extern _WriteFile@20              ; выводит строку в консоль
extern _SetConsoleTextAttribute@8 ; устанавливает аттрибуты для вывода текста
extern _ExitProcess@4             ; завершает программу

; метка начала программы для линкера
global Start

; инициализация данных
section .data

	Buffer   	         db  BUFFER_SIZE dup (0xD)            ; буффер ввода, заполненный нуль-терминаторами
	MessageToInput       db  "Input message:", 0xA, 0xD       ; сообщение, побуждающее к вводу
	MessageToInputLength EQU $ - MessageToInput               ; длина сообщения MessageToInput
	MessageNoColor       db  "Not colored message:", 0xA, 0xD ; сообщение, обозначающее, что далее будет выводиться одноцветная строка
	MessageNoColorLength EQU $ - MessageNoColor               ; длина сообщения MessageNoColor
	MessageColor         db  "Colored message:", 0xA, 0xD     ; сообщение, обозначающее, что далее будет выводиться разноцветная строка
	MessageColorLength   EQU $ - MessageColor                 ; длина сообщения MessageColor
	Color                dd  WHITE_COLOR                      ; значение текущего цвета
	LoopCount            dd  BUFFER_SIZE                      ; счетчик кругов цикла

; инициализация данных без начальных значений
section .bss

	StandardHandle resd 1 ; контроллер потока ввода/вывода/ошибок
	Symbol         resd 1 ; символ, получаемый из введенной строки

section .text
	
	Start:

	; устанавливаем вывод
	SET_OUTPUT_HANDLE:
		push  STD_OUTPUT_HANDLE
		call  _GetStdHandle@4
		mov   dword [StandardHandle], eax 

	; выводим сообщение, побуждающее к вводу строки
	OUT_MESSAGE_TO_INPUT:
		push  NULL
		push  NULL
		push  MessageToInputLength
		push  MessageToInput
		push  dword [StandardHandle]
		call  _WriteFile@20

	; устанавливаем ввод
	SET_INPUT_HANDLE:
		push  STD_INPUT_HANDLE
		call  _GetStdHandle@4
		mov   dword [StandardHandle], eax

	; получаем сообщение в буффер и сохраняем
	GET_BUFFER:
		push  NULL
		push  NULL
		push  BUFFER_SIZE
		push  Buffer
		push  dword [StandardHandle]
		call  _ReadFile@20

	; снова устанавливаем вывод
	SET_OUTPUT_HANDLE_1:
		push  STD_OUTPUT_HANDLE
		call  _GetStdHandle@4
		mov   dword [StandardHandle], eax

	; сначала выводим введенное сообщение без изменений
	OUT_MESSAGE_NO_COLOR:
		; говорим, какой результат будем выводить
		push  NULL
		push  NULL
		push  MessageNoColorLength
		push  MessageNoColor
		push  dword [StandardHandle]
		call  _WriteFile@20
		; выводим сам результат
		push  NULL
		push  NULL
		push  BUFFER_SIZE
		push  Buffer
		push  dword [StandardHandle]
		call  _WriteFile@20

	; теперь выводим введенное сообщение разноцветным
	OUT_MESSAGE_COLOR:
		; говорим, какой результат будем выводить
		push  NULL
		push  NULL
		push  MessageColorLength
		push  MessageColor
		push  dword [StandardHandle]
		call  _WriteFile@20
		
		; сохраняем адрес строки, чтобы в дальнейшем выводить ее посимвольно
		lea   esi, Buffer
		; ставим максимальное количество кругов цикла
		mov   ecx, [LoopCount]
	
	; посимвольно выводим введенную строку, каждый раз меняя цвет символа
	OUT_BUFFER_COLOR:
		; сохраняем количество оставшихся кругов цикла
		mov   [LoopCount], ecx

		; каждый раз нам придется извлекать дескриптор вывода
		push  STD_OUTPUT_HANDLE
		call  _GetStdHandle@4
		mov   dword [StandardHandle], eax
		
		; ставим цвет символа
		mov   edx, [Color]
		push  edx
		push  eax
		call  _SetConsoleTextAttribute@8
		
		; загружаем символ из буффера
		lodsb
		mov   [Symbol], al
		cmp   al, 0xD
		je    EXIT

		; если загруженный символ не является нуль терминатором, то выводим его
		push  NULL
		push  NULL
		push  1
		push  Symbol
		push  dword [StandardHandle]
		call  _WriteFile@20

		; меняем цвет символа
		mov   edx, [Color]
		dec   edx
		cmp   edx, 0
		jne   DO_NOT_SET_WHITE_COLOR
		mov   edx, WHITE_COLOR

	DO_NOT_SET_WHITE_COLOR:
		mov   [Color], edx
		mov   ecx, [LoopCount]
		loop  OUT_BUFFER_COLOR

	; возвращаем цвет вывода и завершаем программу
	EXIT:
		push    STD_OUTPUT_HANDLE
		call    _GetStdHandle@4
		mov     dword [StandardHandle], eax
		push    WHITE_COLOR
		push    eax
		call    _SetConsoleTextAttribute@8

		push    NULL
		call    _ExitProcess@4