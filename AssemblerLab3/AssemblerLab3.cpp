#include <iostream>

int main()
{
    const int N = 64;
    char line[N] = "One w w 000000000000";
    char outline[N]{};
    int lineLength = strlen(line);
    
    _asm {
        lea edi, line
        mov esi, edi
        mov ecx, lineLength

        MAIN_LOOP:
            cld
            mov al, ' '
            repne scasb

        CHECK_WORD:
            mov edx, edi
            sub edx, esi
            cmp ecx, 0
            // нам не нужно уменьшать длину на единицу, если это конец строки
            je PASS_DEC_WORD_LENGTH
            dec edx

            PASS_DEC_WORD_LENGTH:
                cmp edx, 3
                jnle CHECK_LETTERS
                jmp CHECK_MAIN_LOOP

            CHECK_LETTERS:
                mov ebx, lineLength
                sub ebx, ecx
                cmp ecx, 0
                mov eax, ecx
                je SKIP_DEC_LINE_INDEX
                dec ebx

                SKIP_DEC_LINE_INDEX:
                    mov al, line[edx]
                    lea edi line[ebx]

        */
        
    }

    std::cout << outline;
}