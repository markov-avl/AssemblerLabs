#include <iostream>


constexpr int SIZE = 512;

extern "C" char* DiskReading(char* filename, char* diskname, int shift);


int main() {
	int* errorCode;
	char sector[] = "sector.txt";
	char diskName[] = "\\\\.\\PhysicalDrive0";
	
	_asm {
		mov errorCode, eax
	}
	std::cout << errorCode << std::endl;

	char* mbrSector = DiskReading(sector, diskName, 0);
	for (int i = 0; i < 512; i++) {
		if (i % 12 == 0 && i) {
			printf("\n");
		}
		printf("%08x ", mbrSector[i]);
	}

	printf("\n\n");

	char* gptSector = DiskReading(sector, diskName, 512);
	for (int i = 0; i < SIZE; i++) {
		if (i % 12 == 0 && i) {
			printf("\n");
		}
		printf("%08x ", gptSector[i]);
	}

	return 0;
}