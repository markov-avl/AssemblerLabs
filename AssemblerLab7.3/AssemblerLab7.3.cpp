#include <stdio.h>
#include <conio.h>
#include <iostream>


extern "C" char* DiskReading(char* filename, char* text, int textLength, char* diskname, int shift);


int main() {
	char mbrFilename[] = "mbr.txt";
	char gptFilename[] = "gpt.txt";
	char diskname[] = "\\\\.\\PhysicalDrive0";
	char text[512 + 1]{};
	char* mbr;
	char* gpt;

	mbr = DiskReading(mbrFilename, text, strlen(text), diskname, 0);
	for (int i = 0; i < 512; ++i) {
		if (i % 8 == 0 && i) {
			printf("\n");
		}
		printf("%08x ", mbr[i]);
	}

	printf("\n\n");

	gpt = DiskReading(gptFilename, text, strlen(text), diskname, 512);
	for (int i = 0; i < 512; ++i) {
		printf("%c", gpt[i]);
	}

	return 0;
}