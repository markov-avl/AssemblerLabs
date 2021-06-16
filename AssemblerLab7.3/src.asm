.686
.model flat, C
option casemap: none

include C:\masm32\include\windows.inc
include C:\masm32\include\kernel32.inc
includelib kernel32.lib

BUFFER_SIZE = 4096

.data
fileHandle   DWORD ?
bytesWritten DWORD ?
bytesRead    DWORD ?
buffer       DWORD BUFFER_SIZE dup (' '), 0

.code

DiskReading proc C diskname: DWORD, shift: DWORD

mov eax, diskname

push NULL
push FILE_ATTRIBUTE_NORMAL
push OPEN_EXISTING
push NULL
push FILE_SHARE_READ + FILE_SHARE_WRITE
push GENERIC_READ
push eax
call CreateFile

mov  fileHandle, eax

push FILE_BEGIN
push NULL
push shift
push fileHandle
call SetFilePointer

push NULL
push offset bytesRead
push BUFFER_SIZE
push offset buffer
push fileHandle
call ReadFile

push fileHandle
call CloseHandle

mov eax, offset buffer
ret

DiskReading endp

end