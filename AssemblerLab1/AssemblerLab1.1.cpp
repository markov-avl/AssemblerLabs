#include <iostream>


/*
* ВАРИАНТ 7 ЗАДАНИЕ 1.1
*
*     (22 / 3 - 13 / 5 + 3) / 2
* y = ------------------------- - 12
*              5 + 6
*/


int main() {
    char a;

    _asm
    {
        // 1) -> bl = 22 div 3 = 7
        mov ax, 22
        mov bh, 3
        div bh
        mov bl, al

        // 2) -> bl = bl - 13 div 5 = 5
        mov ax, 13
        mov bh, 5
        div bh
        sub bl, al

        // 3) -> bl = bl + 3 = 8
        add bl, 3

        // 4) -> bl = bl div 2 = 4
        shr bl, 1

        // 5) -> bl = bl div (6 + 5) = 0
        mov ah, 0
        mov al, bl
        mov bh, 6
        add bh, 5
        div bh
        mov bl, al

        // 6) -> bl = bl - 12 = -12
        sub bl, 12

        mov a, bl
    }

    std::cout << (short)a << std::endl;

    return 0;
}