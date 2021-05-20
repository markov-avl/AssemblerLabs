#include <windows.h>
#include <iostream>
using namespace std;

extern "C" void readASM();

extern "C"
void* stdHandle(DWORD h)
{
	return GetStdHandle(h);
}

extern "C"
BOOL readConsole(HANDLE hConsoleInput, LPVOID  lpBuffer, DWORD nNumberOfCharsToRead, LPDWORD lpNumberOfCharsRead, PCONSOLE_READCONSOLE_CONTROL pInputControl)
{
	return ReadConsole(hConsoleInput, lpBuffer, nNumberOfCharsToRead, lpNumberOfCharsRead, pInputControl);
}

extern "C"
HANDLE createFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	return CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

extern "C"
BOOL writeFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	return WriteFile(hFile, lpBuffer, 2 * nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

extern "C"
BOOL closeHandle(HANDLE hObject)
{
	return CloseHandle(hObject);
}

extern "C"
VOID exitProcess(UINT r)
{
	return ExitProcess(r);
}


int main()
{
	readASM();
	return 0;
}