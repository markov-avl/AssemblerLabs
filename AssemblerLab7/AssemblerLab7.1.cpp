#include <windows.h>
#include <iostream>


extern "C" void masm();

extern "C" void* stdHandle(
	DWORD handler
) {
	return GetStdHandle(
		handler
	);
}

extern "C" BOOL readConsole(
	HANDLE                       hConsoleInput,
	LPVOID                       lpBuffer,
	DWORD                        nNumberOfCharsToRead,
	LPDWORD                      lpNumberOfCharsRead,
	PCONSOLE_READCONSOLE_CONTROL pInputControl
) {
	return ReadConsole(
		hConsoleInput,
		lpBuffer,
		nNumberOfCharsToRead,
		lpNumberOfCharsRead,
		pInputControl
	);
}

extern "C" HANDLE createFile(
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
) {
	return CreateFileA(
		lpFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile
	);
}

extern "C" BOOL writeFile(
	HANDLE       hFile,
	LPCVOID      lpBuffer,
	DWORD        nNumberOfBytesToWrite,
	LPDWORD      lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped
) {
	return WriteFile(
		hFile,
		lpBuffer,
		nNumberOfBytesToWrite * 2,
		lpNumberOfBytesWritten,
		lpOverlapped
	);
}

extern "C" BOOL closeHandle(
	HANDLE hObject
) {
	return CloseHandle(
		hObject
	);
}

extern "C" VOID exitProcess(
	UINT resp
) {
	return ExitProcess(
		resp
	);
}

int main() {
	masm();
	return 0;
}