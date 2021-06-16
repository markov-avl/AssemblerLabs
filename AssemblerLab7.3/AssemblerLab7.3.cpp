#include <stdio.h>
#include <conio.h>
#include <iostream>


constexpr int BUFFER_SIZE = 4096;

extern "C" char* DiskReading(char* diskname, int shift);


int main() {
	char diskname[] = "\\\\.\\PhysicalDrive0";
	char* mbr;
	char* gpt;

	mbr = DiskReading(diskname, 0);
	for (int i = 0; i < BUFFER_SIZE; ++i) {
		printf(" %08X ", *mbr++);
	}

	printf("\n\n");

	gpt = DiskReading(diskname, BUFFER_SIZE);
	for (int i = 0; i < BUFFER_SIZE; ++i) {
		printf("%c", *gpt++);
	}

	return 0;
}