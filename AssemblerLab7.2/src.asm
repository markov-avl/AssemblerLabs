.686
.model flat, C
option casemap: none

include C:\masm32\include\windows.inc
include C:\masm32\include\kernel32.inc
includelib kernel32.lib

BUFFER_SIZE = 256

.data
fileHandle   DWORD ?
bytesWritten DWORD ?
bytesRead    DWORD ?
buffer       DWORD BUFFER_SIZE dup (?), 0

.code
FileIO proc C filename: DWORD, text: DWORD, textLength: DWORD

; запись

; HANDLE CreateFileA(
;   LPCSTR                lpFileName,
;   DWORD                 dwDesiredAccess,
;   DWORD                 dwShareMode,
;   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
;   DWORD                 dwCreationDisposition,
;   DWORD                 dwFlagsAndAttributes,
;   HANDLE                hTemplateFile
; )
push NULL
push FILE_ATTRIBUTE_NORMAL
push OPEN_EXISTING
push NULL
push NULL
push GENERIC_WRITE
push filename
call CreateFile

mov  fileHandle, eax

; DWORD SetFilePointer(
;   HANDLE hFile,
;   LONG   lDistanceToMove,
;   PLONG  lpDistanceToMoveHigh,
;   DWORD  dwMoveMethod
; )
push FILE_END
push NULL
push NULL
push fileHandle
call SetFilePointer

; BOOL WriteFile(
;   HANDLE       hFile,
;   LPCVOID      lpBuffer,
;   DWORD        nNumberOfBytesToWrite,
;   LPDWORD      lpNumberOfBytesWritten,
;   LPOVERLAPPED lpOverlapped
; )
push NULL
push offset bytesWritten
push textLength
push text
push fileHandle
call WriteFile

; BOOL CloseHandle(
;   HANDLE hObject
; )
push fileHandle
call CloseHandle

; чтение

; то же самое, что и сверху, в части записи
push NULL
push FILE_ATTRIBUTE_NORMAL
push OPEN_EXISTING
push NULL
push NULL
push GENERIC_READ
push filename
call CreateFile

mov  fileHandle, eax

; BOOL ReadFile(
;   HANDLE       hFile,
;   LPVOID       lpBuffer,
;   DWORD        nNumberOfBytesToRead,
;   LPDWORD      lpNumberOfBytesRead,
;   LPOVERLAPPED lpOverlapped
; )
push NULL
push offset bytesRead
push BUFFER_SIZE
push offset buffer
push fileHandle
call ReadFile

; то же самое, что и сверху, в части записи
push fileHandle
call CloseHandle

; возвращаем данные
mov  eax, offset buffer
ret

FileIO endp

end