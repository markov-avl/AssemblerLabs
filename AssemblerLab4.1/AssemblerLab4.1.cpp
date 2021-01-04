#include <iostream>
#include <random>
#include <ctime>
#include <fstream>


/*
* ВАРИАНТ 7
*
* Алгоритм сортировки методом пузырька. Дан массив случайных чисел
* длины N. На первой итерации попарно упорядочиваются все соседние
* элементы; на второй – все элементы, кроме последнего элемента; на
* третьей – все элементы, кроме последнего элемента и предпоследнего
* элемента и т.п.
*
* РЕАЛИЗАЦИЯ С ПОМОЩЬЮ КОДА НА C++
*/


void fillTheArraysWithRandomNumbers(int* array, int N) {
    std::random_device rd;
    std::mt19937 mersenne(rd());
    for (int i = 0; i < N; ++i) {
        array[i] = (int)mersenne();
    }
}


bool isArraySorted(int* array, int N) {
    for (int i = 1; i < N; ++i) {
        if (array[i] < array[i - 1]) {
            return false;
        }
    }
    return true;
}


int main() {
    const char path[] = "output.txt";
    const char type[] = "C++ code";
    const int CYCLES = 15;
    const int N = 12000;
    int array[N];
    std::ofstream outfile;
    clock_t start;
    clock_t end;
    clock_t testStart;
    clock_t testEnd;
    double minTime = -1;
    double time;

    outfile.open(path);

    std::cout << "Test started" << std::endl;
    std::cout << "Type: " << type << std::endl;
    std::cout << "The number of cycles: " << CYCLES << std::endl;
    std::cout << "The number of array elements: " << N << std::endl;
    std::cout << "The path of the result file: " << path << std::endl;

    outfile << "Type: " << type << std::endl;
    outfile << "The number of cycles: " << CYCLES << std::endl;
    outfile << "The number of array elements: " << N << std::endl;
    outfile << std::endl;

    testStart = clock();

    for (int i = 0; i < CYCLES; ++i) {
        fillTheArraysWithRandomNumbers(array, N);

        start = clock();
        int temporary;
        for (int i = 0; i < N - 1; ++i) {
            for (int j = 0; j < N - i - 1; ++j) {
                if (array[j] > array[j + 1]) {
                    temporary = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temporary;
                }
            }
        }
        end = clock();

        time = ((double)end - (double)start) / CLOCKS_PER_SEC;
        if (time < minTime || minTime < 0) {
            minTime = time;
        }

        outfile << "Sorted in " << time << " sec" << std::endl;
        if (isArraySorted(array, N)) {
            outfile << "Array sorted correctly" << std::endl;
        }
        else {
            outfile << "Array sorted incorrectly" << std::endl;
        }
    }

    outfile << std::endl << "Fastest sorting time: " << minTime << " sec" << std::endl;

    testEnd = clock();
    time = ((double)testEnd - (double)testStart) / CLOCKS_PER_SEC;
    std::cout << "Test finished in " << time << " sec" << std::endl;

    return 0;
}