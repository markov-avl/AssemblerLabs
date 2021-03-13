#include <iostream>
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

typedef float Vector[N];
typedef Vector QuadraticMatrix[N];


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

void generateQuadraticMatrix(QuadraticMatrix& matrix) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			matrix[i][j] = rand() % MAX_RANDOM;
		}
	}
}

void printResults(bool isSSE, QuadraticMatrix& result1, QuadraticMatrix& result2, QuadraticMatrix& result3, float m, __int64 ticks) {
	std::cout << (isSSE ? "SSE TEST RESULTS:" : "NO SSE TEST RESULTS:") << std::endl << std::endl;
	std::cout << "M1 / " << m << ":" << std::endl << result1 << std::endl;
	std::cout << "M1 * M2:" << std::endl << result2 << std::endl;
	std::cout << "M1 + M1 * M2:" << std::endl << result3 << std::endl;
	std::cout << "Time spent: " << ticks << " ticks" << std::endl;
}


int main() {
	unsigned __int64 start;
	unsigned __int64 end;
	QuadraticMatrix m1;
	QuadraticMatrix	m2;
	QuadraticMatrix result1;
	QuadraticMatrix result2;
	QuadraticMatrix result3;
	QuadraticMatrix resultSSE1;
	QuadraticMatrix resultSSE2;
	QuadraticMatrix	resultSSE3;
	Vector vector;
	Vector column;
	float m;

	srand(time(nullptr));
	generateQuadraticMatrix(m1);
	generateQuadraticMatrix(m2);

	// НЕ ЗАБЫТЬ ДОБАВИТЬ ВВОД ПОЛЬЗОВАТЕЛЯ
	// НЕ ЗАБЫТЬ ПЕРЕНЕСТИ ЗАПОЛНЕНИЕ ВЕКТОРА В SSE АССЕМБЛЕРНУЮ ВСТАВКУ
	m = 2;
	for (int i = 0; i < N; ++i) {
		vector[i] = m;
	}
	
	std::cout << "M1:" << std::endl << m1 << std::endl;
	std::cout << "M2:" << std::endl << m2 << std::endl;

	/*
	* В результате NO SSE ассемблерной вставки будет:
	* - в result1 будет храниться M1 / m;
	* - в result2 будет храниться M1 * M2 (результат легко проверить на https://planetcalc.ru);
	* - в result3 будет храниться M1 + M1 * M2.
	*/
	{
		start = __rdtsc();
		_asm {
		INITIALIZATION:
			finit                                             // инициализация сопроцессора

		}
		end = __rdtsc();

		// результаты теста NO SSE
		std::cout << std::endl;
		printResults(false, result1, result2, result3, m, end - start);
	}

	/*
	* В результате SSE ассемблерной вставки будет:
	* - в resultSSE1 будет храниться M1 / m;
	* - в resultSSE2 будет храниться M1 * M2 (результат легко проверить на https://planetcalc.ru);
	* - в resultSSE3 будет храниться M1 + M1 * M2.
	*/
	{
		start = __rdtsc();
		_asm {
		INITIALIZATION:
			finit                                             // инициализация сопроцессора
			mov    ecx, N
			xor    ebx, ebx                                   // пусть ebx будет номером k-го элемента матрицы

		REDUCING:
			movups xmm0, [m1 + 4 * ebx]                       // копируем в xmm0 i-ю строку матрицы m1
			movups xmm1, xmm0                                 // копируем в xmm1 i-ю строку матрицы m1
			movups xmm2, [vector]                             // копируем в xmm1 вектор, состоящий из N заданных чисел m
			divps  xmm1, xmm2                                 // уменьшаем в m раз i-ю строку матрицы
			movups [resultSSE1 + 4 * ebx], xmm1               // вставляем уже уменьшенную в m раз строку в матрицу resultSSE1

		ROW_COMPOSITION:
			xor    edx, edx                                   // пусть edx будет номером j-го столбца

		ELEMENT_COMPOSITION:
			// сохраняем в column j-й столбец матрицы m2SSE
			fld    [m2 + 4 * edx + 48]
			fld    [m2 + 4 * edx + 32]
			fld    [m2 + 4 * edx + 16]
			fld    [m2 + 4 * edx]
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
			fstp   [resultSSE2 + 4 * ebx]                     // сохраняем сумму в k-ый элемент матрицы-произведение resultSSE2

			// повторяем всё это для каждого элемента строки
			inc    ebx
			inc    edx
			cmp    edx, N
			jne    ELEMENT_COMPOSITION

			// повторяем всё это для каждой строки
			dec    ecx
			cmp    ecx, 0
			jne    REDUCING

		ADDITION:
			xor    ebx, ebx                                   // пусть ebx будет номером i-х строк матриц

		ROW_ADDITION:
			movups xmm0, [m1 + 4 * ebx]                       // сохраняем i-ю строку матрицы M1 в xmm0
			movups xmm1, [resultSSE2 + 4 * ebx]               // сохраняем i-ю строку матрицы-произведение resultSSE2 в xmm1
			addps  xmm1, xmm0                                 // суммируем строки и сохраняем результат в xmm1
			movups [resultSSE3 + 4 * ebx], xmm1               // сохраняем xmm1 в i-ю строку матрицы resultSSE3

			// повторяем всё это для каждой строки
			add    ebx, 4
			cmp    ebx, N* N
			jne    ROW_ADDITION
		}
		end = __rdtsc();

		// результаты теста SSE
		std::cout << std::endl << std::endl;
		printResults(true, resultSSE1, resultSSE2, resultSSE3, m, end - start);
	}

	return 0;
}