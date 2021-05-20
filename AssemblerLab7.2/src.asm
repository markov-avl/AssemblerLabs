PUBLIC masm

.686
.model flat, C
option casemap : none

BUFFER_SIZE        = 128
STD_INPUT_HANDLE   = -10
STD_OUTPUT_HANDLER = -11

.data

outputHandler dd 0
inputHandler  dd 0
fileHandler   dd 0

nWritten      dd 0
nRead         dd 0
bytesCount    dd 0
bytesWritten  dd 0

fileBuffer    db BUFFER_SIZE DUP(?)
consoleBuffer db BUFFER_SIZE DUP(?)
filename      db "output.txt", 0

.code

EXTERN stdHandle:PROC
EXTERN writeConsole:PROC
EXTERN readConsole:PROC
EXTERN createFile:PROC
EXTERN writeFile:PROC
EXTERN readFile:PROC
EXTERN closeHandle:PROC
EXTERN exitProcess:PROC

masm PROC

push ebp
mov  ebp, esp

; меняем дескриптор на вывод
push STD_OUTPUT_HANDLER
call stdHandle
mov  outputHandler, eax

; создаем файл output.txt, если он не создан
push 0
push 80h
push 3
push 0
push 2h
push 3
push offset filename
call createFile
mov  fileHandler, eax

; читаем из файла
push 0
push offset bytesCount
push BUFFER_SIZE
push offset fileBuffer
push fileHandler
call readFile

; вывод файла в консоль
push 0
push offset nWritten
push bytesCount
push offset fileBuffer
push outputHandler
call writeConsole

; меняем дескриптор на считывание
push STD_INPUT_HANDLE
call stdHandle
mov  inputHandler, eax

; получаем ввод с консоли
push 0
push offset nRead
push BUFFER_SIZE
push offset consoleBuffer
push inputHandler
call readConsole

; записываем в файл введеную строку
push 0
push offset bytesWritten
push nRead
push offset consoleBuffer
push fileHandler
call writeFile

; закрываем файл
push fileHandler
call closeHandle

; выходим из процедуры
push 0
call exitProcess

mov  esp, ebp
pop  ebp
retn
masm ENDP

END
end Main