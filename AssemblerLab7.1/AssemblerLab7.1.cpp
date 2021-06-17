#include <stdio.h>
#include <conio.h>
#include <iostream>

// ЗАПИСЬ В ФАЙЛ

constexpr int SIZE = 256;

extern "C" void FileInput(char* filename, char* text, int textLength);


int main() {
	char filename[] = "output.txt";
	char text[SIZE]{};
	
	std::cout << "Input the text:" << std::endl;
	std::cin.getline(text, SIZE);
	FileInput(filename, text, strlen(text));

	return 0;
}