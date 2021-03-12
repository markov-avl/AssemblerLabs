﻿#include <iostream>
#include <intrin.h>
#include <random>
#include <ctime>
#define N 4
#define MAX_RANDOM 20

/*
* ВАРИАНТ 7
* 
* Размерность матриц 4х4. Уменьшить все элементы матрицы М1 в заданное число раз. Умножить М1 на матрицу М2.
* Сложить поэлементно М1 и матрицу-произведение.
*/

typedef float QuadraticMatrix[N][N];


std::ostream& operator<<(std::ostream& out, const QuadraticMatrix& matrix) {
	out.precision(7);
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			out << matrix[i][j] << '\t';
		}
		out << std::endl;
	}
	return out;
}


void generateQuadraticMatrices(QuadraticMatrix& matrix1, QuadraticMatrix& matrix2) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			matrix1[i][j] = rand() % MAX_RANDOM;
			matrix2[i][j] = matrix1[i][j];
		}
	}
}


int main() {
	unsigned __int64 start, end;
	QuadraticMatrix m1, m2, composition;
	QuadraticMatrix m1SSE, m2SSE, compositionSSE;
	float vector[N];
	float column[N];

	srand(time(nullptr));
	generateQuadraticMatrices(m1, m1SSE);
	generateQuadraticMatrices(m2, m2SSE);

	// НЕ ЗАБЫТЬ ДОБАВИТЬ ВВОД ПОЛЬЗОВАТЕЛЯ
	vector[0] = 2;
	for (int i = 1; i < N; ++i) {
		vector[i] = vector[0];
	}
	
	std::cout << "M1:" << std::endl << m1 << std::endl;
	std::cout << "M2:" << std::endl << m2 << std::endl;

	/*
	В результате SSE ассемблерной вставки будет:
	- в m1SSE будет храниться M1 / m;
	- в compositionSSE будет храниться (M1 / m) * M2.
	- в m2SSE будет храниться M1 + (M1 / m) * M2;
	*/

	start = __rdtsc();
	_asm {
	INITIALISATION:
		finit                                             // инициализация сопроцессора
		mov    ecx, N
		xor    ebx, ebx                                   // пусть ebx будет номером k-го элемента матрицы

	DECREASE:
		movups xmm0, [m1SSE + 4 * ebx]                    // копируем в xmm0 i-ю строку матрицы m1SSE
		movups xmm1, [vector]                             // копируем в xmm1 вектор, состоящий из N заданных чисел m
		divps  xmm0, xmm1                                 // уменьшаем в m раз i-ю строку матрицы
		movups [m1SSE + 4 * ebx], xmm0                    // вставляем обратно строку в матрицу, но уже уменьшенную в m раз
		
	ROW_COMPOSITION:
		xor    edx, edx                                   // пусть edx будет номером j-го столбца
	
	ELEMENT_COMPOSITION:
		// сохраняем в column j-й столбец матрицы m2SSE
		fld    [m2SSE + 4 * edx + 48]
		fld    [m2SSE + 4 * edx + 32]
		fld    [m2SSE + 4 * edx + 16]
		fld    [m2SSE + 4 * edx]
		fstp   [column]
		fstp   [column + 4]
		fstp   [column + 8]
		fstp   [column + 12]

		movups xmm1, xmm0                                 // создаем копию i-й строки в xmm1
		movups xmm2, [column]                             // копируем в xmm2 j-й столбец
		mulps  xmm1, xmm2                                 // перемножаем i-ю строку с j-м столбцом поэлементно
		movups [column], xmm1                             // переносим полученный вектор в column
		
		// сохраняем сумму всех элементов вектора
		fld    [column]
		fld    [column + 4]
		fld    [column + 8]
		fld    [column + 12]
		faddp  st(1), st(0)
		faddp  st(1), st(0)
		faddp  st(1), st(0)
		fstp   [compositionSSE + 4 * ebx]                 // сохраняем сумму в k-ый элемент матрицы-произведение

		// повторяем всё это для каждого элемента строки
		inc    ebx
		inc    edx
		cmp    edx, N
		jne    ELEMENT_COMPOSITION

		// повторяем всё это для каждой строки
		dec    ecx
		cmp    ecx, 0
		jne    DECREASE

	ADDITION:
		xor    ebx, ebx                                   // пусть ebx будет номером i-х строк матриц

	ROW_ADDITION:
		movups xmm0, [m1SSE + 4 * ebx]                    // сохраняем i-ю строку матрицы M1 в xmm0
		movups xmm1, [compositionSSE + 4 * ebx]           // сохраняем i-ю строку матрицы (M1 / m) * M2 в xmm1
		addps  xmm1, xmm0                                 // суммируем строки и сохраняем результат в xmm1
		movups [m2SSE + 4 * ebx], xmm1                    // сохраняем xmm1 в i-ю строку матрицы M2

		// повторяем всё это для каждой строки
		add    ebx, 4
		cmp    ebx, N * N
		jne    ROW_ADDITION
	}
	end = __rdtsc();
	
	// результаты теста SSE
	std::cout << std::endl << "SSE TEST RESULTS:" << std::endl << std::endl;
	std::cout << "M1 / " << vector[0] << ":" << std::endl << m1SSE << std::endl;
	std::cout << "M1 / " << vector[0] << " * M2:" << std::endl << compositionSSE << std::endl;
	std::cout << "M1 + (M1 / " << vector[0] << ") * M2:" << std::endl << m2SSE << std::endl;
	std::cout << "Time spent: " << end - start << " ticks" << std::endl;

	return 0;
}