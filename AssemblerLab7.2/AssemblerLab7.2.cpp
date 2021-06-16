#include <stdio.h>
#include <conio.h>
#include <iostream>

// ЧТЕНИЕ И ДОБАВЛЕНИЕ В ФАЙЛ

constexpr int BUFFER_SIZE = 256;

extern "C" char* FileIO(char* filename, char* text, int textLength);


int main() {
	char filename[] = "output.txt";
	char text[BUFFER_SIZE]{};

	std::cout << "Input the text:" << std::endl;
	std::cin.getline(text, BUFFER_SIZE);
	std::cout << FileIO(filename, text, strlen(text));

	return 0;
}