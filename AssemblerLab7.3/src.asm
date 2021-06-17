.686
.model flat, C
option casemap: none

include C:\masm32\include\windows.inc
include C:\masm32\include\kernel32.inc
includelib kernel32.lib

BUFFER_SIZE = 512

.data
fileHandle   DWORD ?
bytesWritten DWORD ?
bytesRead    DWORD ?
buffer       DWORD BUFFER_SIZE dup (' '), 0

.code

DiskReading proc C filename: DWORD, text: DWORD, textLength: DWORD, diskname: DWORD, shift: DWORD

push NULL
push FILE_ATTRIBUTE_NORMAL
push OPEN_EXISTING
push NULL
push FILE_SHARE_READ + FILE_SHARE_WRITE
push GENERIC_READ
push diskname
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

push fileHandle
call CloseHandle

mov  eax, offset buffer
ret

DiskReading endp

end