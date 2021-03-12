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

	vector[0] = 3;
	for (int i = 1; i < N; ++i) {
		vector[i] = vector[0];
	}
	
	std::cout << "M1 matrix:" << std::endl << m1 << std::endl;
	std::cout << "M2 matrix:" << std::endl << m2 << std::endl;

	start = __rdtsc();
	_asm {
	INITIALISATION:
		mov ecx, N
		// пусть ebx будет номером k-го элемента матрицы
		xor ebx, ebx

	DECREASE_M1:
		// копируем в xmm0 i-ю строку матрицы m1SSE
		movups xmm0, [m1SSE + 4 * ebx]
		// копируем в xmm1 вектор, состоящий из N заданных чисел k
		movups xmm1, [vector]
		// уменьшаем в k раз i-ю строку матрицы
		divps xmm0, xmm1
		// вставляем обратно строку в матрицу, но уже уменьшенную в k раз
		movups [m1SSE + 4 * ebx], xmm0
		
	ROW_COMPOSITING:
		// пусть edx будет номером j-го столбца
		xor edx, edx
	ELEMENT_COMPOSITING:
		// сохраняем в column j-й столбец матрицы m2SSE
		fld[m2SSE + 4 * edx + 48]
		fld[m2SSE + 4 * edx + 32]
		fld[m2SSE + 4 * edx + 16]
		fld[m2SSE + 4 * edx]
		fstp[column]
		fstp[column + 4]
		fstp[column + 8]
		fstp[column + 12]
		// создаем копию i-й строки в xmm1
		movups xmm1, xmm0
		// перемножаем i-ю строку с j-м столбцом поэлементно
		mulps xmm1, [column]
		// переносим полученный вектор в column
		movups [column], xmm1
		// сохраняем сумму всех элементов полученного вектора
		fld[column]
		fld[column + 4]
		fadd
		fld[column + 8]
		fadd
		fld[column + 12]
		fadd
		fstp[eax]
		// сохраняем сумму eax в k-ый элемент матрицы-произведение
		mov compositionSSE[ebx], eax
		// увеличиваем ebx и edx на единицу
		inc ebx
		inc edx

		// повторяем всё это для каждого элемента строки
		cmp edx, N
		jne ELEMENT_COMPOSITING
		

	/*COMPOSITION_OF_M1_AND_M2:	
		mov ebx, 0
	
	GETTING_M2_COLUMN:
		mov edx, 0
		lea */


		/*mulps xmm1, xmm0
		movups[m2SSE + 4 * eax], xmm1

		addps xmm0, xmm1
		movups[res1 + 4 * eax], xmm0

		add eax, 4
		loop PROCESS*/
	}
	end = __rdtsc();
	std::cout << m1SSE << std::endl;
	std::cout << compositionSSE << std::endl;

	return 0;
}