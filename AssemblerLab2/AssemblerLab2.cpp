#include <iostream>

int main()
{
    int a, x, y;

    std::cout << "Input x: ";
    std::cin >> x;
    std::cout << "Input a: ";
    std::cin >> a;

    _asm
    {
        // обнуляем y
        mov y, 0

        // сохраняем x и a в регистры ecx и edx соответственно
        mov ecx, x
        mov edx, a

        // вычисляем модуль x
        mov eax, ecx
        test eax, eax
        jns COMPARE_1
        neg eax

        // добавляем к y y1 и y2, используя ветвления
        COMPARE_1:
            cmp eax, 3
            jb LOWER
            add y, edx
            add y, ecx
            jmp COMPARE_2
            
            LOWER:
                add y, 4
                sub y, ecx

        COMPARE_2:
            test ecx, 1
            jz X_EVEN
            add y, edx
            
        X_EVEN:
            add y, 2
    }

    std::cout << "y = " << y << std::endl;

    return 0;
}