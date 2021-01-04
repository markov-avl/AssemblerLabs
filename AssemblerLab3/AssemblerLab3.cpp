#include <iostream>


/*
* ВАРИАНТ 7
*
* Найти слова, в которых больше трех повторяющихся символов
*/


int main() {
    const int N = 64;
    char line[N]{};
    char outline[N]{};
    int lineIndex;
    int outlineIndex;
    
    std::cout << "Input string: ";
    std::cin.getline(line, N);
    int lineLength = strlen(line);
    
    _asm {
        INIT:
            // уже получаем адрес выходной строки, т.к. он может пригодиться
            lea edi, outline
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
            // нам не нужно уменьшать длину слова на единицу, если это конец строки
            cmp ecx, 0
            je PASS_DEC_WORD_LENGTH
            dec edx

        PASS_DEC_WORD_LENGTH:
            // сначала проверяем, есть ли в слове вообще более 3 букв
            cmp edx, 3
            jnle CHECK_LETTERS
            jmp CHECK_MAIN_LOOP

        CHECK_LETTERS:
            // теперь будем двигаться справа налево
            std
            // надо сохранить куда-либо настоящий edi и ecx
            mov lineIndex, edi
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
            // если ecx не обнулится после трех поисков символа, тогда это слово нас устраивает
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

        // посимвольно переносим символы из исходной строки в выходную
        ADD_WORD:
            cld
            sub esi, edx
            add esi, 2
            mov edi, outlineIndex
            mov ecx, lineIndex
            cmp ebx, 0
            je PASS_DEC_CYCLES
            dec ecx

        PASS_DEC_CYCLES:
            sub ecx, esi
            rep movsb
            cmp ebx, 0
            // если слово является последним в строке, то уже точно не нужно вставлять пробел
            je SKIP_ADD_SPACE
            mov al, ' '
            stosb

        SKIP_ADD_SPACE:
            mov outlineIndex, edi

        // возвращаемся к исходным значениям, будто мы их и не меняли
        QUIT_CHECK_LETTERS_LOOP:
            mov edi, lineIndex
            mov esi, edi
            mov ecx, ebx

        // проверка, не дошли ли мы до конца строки, и если дошли - то задача выполнена
        CHECK_MAIN_LOOP:
            cmp ecx, 0
            jne MAIN_LOOP
    }

    std::cout << "Found words: " << outline << std::endl;
    
    return 0;
}