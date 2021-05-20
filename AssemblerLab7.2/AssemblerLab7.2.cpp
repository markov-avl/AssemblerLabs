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

extern "C" BOOL writeConsole(
	HANDLE      hConsoleOutput,
	CONST VOID* lpBuffer,
	DWORD       nNumberOfCharsToWrite,
	LPDWORD     lpNumberOfCharsWritten,
	LPVOID      lpReserved
) {
	return WriteConsole(
		hConsoleOutput,
		lpBuffer,
		nNumberOfCharsToWrite * 2,
		lpNumberOfCharsWritten,
		lpReserved
	);
}

extern "C" BOOL readFile(
	HANDLE       hFile,
	LPVOID       lpBuffer,
	DWORD        nNumberOfBytesToRead,
	LPDWORD      lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
) {
	return ReadFile(
		hFile,
		lpBuffer,
		nNumberOfBytesToRead,
		lpNumberOfBytesRead,
		lpOverlapped
	);
}


int main() {
	masm();
	return 0;
}