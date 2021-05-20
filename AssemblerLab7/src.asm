PUBLIC readASM

.686
.model flat, C
option casemap : none

BUFFER_SIZE       = 128
STD_INPUT_HANDLE  = -10

.data

outputHandler dd 0
inputHandler  dd 0
fileHandler   dd 0
nRead         dd 0
inputBuffer   db BUFFER_SIZE DUP(?)
filename      db "output.txt", 0
bytesWritten  dd 0

.code

EXTERN stdHandle:PROC
EXTERN readConsole:PROC
EXTERN createFileA:PROC
EXTERN writeFile:PROC
EXTERN closeHandle:PROC
EXTERN exitProcess:PROC

readASM PROC

push ebp
mov  ebp, esp

; меняем дескриптор на считывание
push STD_INPUT_HANDLE
call stdHandle
mov  inputHandler, eax

; получаем ввод с консоли
push 0
push offset nRead
push BUFFER_SIZE
push offset inputBuffer
push inputHandler
call readConsole

; создаем файл output.txt, если он не создан
push 0
push 80h
push 2
push 0
push 2h
push 2
push offset filename
call createFileA
mov  fileHandler, eax

; записываем в файл введеную строку
push 0
push offset bytesWritten
push nRead
push offset inputBuffer
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
readASM ENDP

END
end Main