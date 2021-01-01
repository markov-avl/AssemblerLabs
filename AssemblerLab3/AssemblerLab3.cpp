#include <iostream>

int main()
{
    const int N = 64;
    char line[N] = "One44445 4445  44 wwwww 000000000000";
    char outline[N]{};
    int lineLength = strlen(line);
    int lineIndex, outlineIndex;
    
    _asm {
        lea edim outline
        mov outlineIndex, edi
        lea edi, line
        mov esi, edi
        mov ecx, lineLength

        MAIN_LOOP:
            // ищем знак пробела в строке, двигаясь слева направо
            cld
            mov al, ' '
            repne scasb

        CHECK_WORD:
            // пусть в edx хранится длина найденного слова
            mov edx, edi
            sub edx, esi
            mov esi, edi
            // нам не нужно уменьшать длину на единицу, если это конец строки
            cmp ecx, 0
            je PASS_DEC_WORD_LENGTH
            dec edx

            PASS_DEC_WORD_LENGTH:
                // сначала проверяем, есть ли в слове вообще более 3 букв
                cmp edx, 3
                jnle CHECK_LETTERS
                jmp CHECK_MAIN_LOOP

            // будем проверять слово так:
            // 1) сохраняем в ecx длину слова, то есть edx;
            // 2) ставим каретку на конец слова
            // 3) начинаем двигаться справа налево, пытаясь найти букву,
            //    на которой стояла каретка;
            // 4) если нам удается найти ещё 3 таких символа, не обнулив ecx, значит, это слово нам подходит,
            //    и мы копируем его из строки;
            // 5) иначе возвращаем каретку на место, на котором стояла раньше, но отодвигаем ее налево на 1,
            //    а также в ecx кладем edx - 1;
            // 6) если edx будет больше 3, начинаем выполнять алгоритм заново с пункта 3, иначе заканчиваем.
            CHECK_LETTERS:
                std
                // надо сохранить куда-либо настоящий индекс edi
                mov lineIndex, edi
                // надо сохранить куда-либо настоящей счетчик циклов ecx
                mov ebx, ecx
                // ставим каретку на последний символ слова
                dec esi
                cmp ecx, 0
                je CHECK_LETTERS_LOOP
                dec esi
                
                CHECK_LETTERS_LOOP:
                    // загружаем максимальное количество итераций
                    mov ecx, edx
                    // загружаем в al символ каретки
                    lodsb
                    mov edi, esi
                    repne scasb
                    repne scasb
                    repne scasb
                    cmp ecx, 0
                    jne ADD_WORD
                    dec edx
                    cmp edx, 3
                    je QUIT_CHECK_LETTERS_LOOP
                    jmp CHECK_LETTERS_LOOP

                ADD_WORD:

            QUIT_CHECK_LETTERS_LOOP:
                mov edi, index
                mov ecx, ebx

        CHECK_MAIN_LOOP:
            cmp ecx, 0
            jne MAIN_LOOP
    }

    std::cout << outline;
}