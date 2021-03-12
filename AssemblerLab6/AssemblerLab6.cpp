#include <iostream>
#include <intrin.h>
#include <random>
#include <ctime>

#define N 4
#define FP 3400000000
#define MAX_RANDOM 20

typedef float QuadraticMatrix[N][N];


std::ostream& operator<<(std::ostream& out, const QuadraticMatrix& matrix) {
	out.precision(3);
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

	vector[0] = 1;
	for (int i = 1; i < N; ++i) {
		vector[i] = vector[0];
	}
	
	std::cout << "M1 matrix:" << std::endl << m1 << std::endl;
	std::cout << "M2 matrix:" << std::endl << m2 << std::endl;

	start = __rdtsc();
	_asm {
	INITIALISATION:
		finit                                             // инициализация сопроцессора
		mov    ecx, N
		xor    ebx, ebx                                   // пусть ebx будет номером k-го элемента матрицы

	DECREASE_M1:
		movups xmm0, [m1SSE + 4 * ebx]                    // копируем в xmm0 i-ю строку матрицы m1SSE
		movups xmm1, [vector]                             // копируем в xmm1 вектор, состоящий из N заданных чисел k
		divps  xmm0, xmm1                                 // уменьшаем в k раз i-ю строку матрицы
		movups [m1SSE + 4 * ebx], xmm0                    // вставляем обратно строку в матрицу, но уже уменьшенную в k раз
		
	ROW_COMPOSITING:
		xor edx, edx                                      // пусть edx будет номером j-го столбца
	
	ELEMENT_COMPOSITING:
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
		mulps  xmm1, [column]                             // перемножаем i-ю строку с j-м столбцом поэлементно
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
		jne    ELEMENT_COMPOSITING

		dec    ecx
		cmp    ecx, N
		jne    DECREASE_M1
	}
	end = __rdtsc();
	
	std::cout << end - start << " ticks" << std::endl;
	std::cout << m1SSE << std::endl;
	std::cout << compositionSSE << std::endl;

	return 0;
}