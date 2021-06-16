#include <stdio.h>
#include <conio.h>
#include <iostream>

// ЗАПИСЬ В ФАЙЛ

constexpr int BUFFER_SIZE = 256;

extern "C" void FileInput(char* filename, char* text, int textLength);


int main() {
	char filename[] = "output.txt";
	char text[BUFFER_SIZE]{};
	
	std::cout << "Input the text:" << std::endl;
	std::cin.getline(text, BUFFER_SIZE);
	FileInput(filename, text, strlen(text));

	return 0;
}