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
buffer       DWORD BUFFER_SIZE dup (' '), 0

.code
FileInput proc C filename: DWORD, text: DWORD, textLength: DWORD

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
push CREATE_ALWAYS
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

mov  eax, text

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

ret

FileInput endp

end