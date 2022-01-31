/*
Задача up01-5: mz01-5 (дореш)
Напишите подпрограмму exchange. В регистре %eax подпрограмме передается адрес начала массива чисел типа uint32_t, в регистре %edx передается количество чисел (всегда больше 0). Пусть массив называется a, количество элементов в нем n. Для всех пар чисел, симметрично расположенных относительно середины массива, то есть для пар (a[0], a[n-1]), (a[1], a[n-2])... переставьте числа в парах таким образом, чтобы первое число в паре всегда было не больше второго числа в паре. В регистре %ecx верните количество выполненных перестановок.

Например, если дан массив {5, 1, 2, 3}, результатом его обработки должен быть массив {3, 1, 2, 5}, и в регистре ecx должно быть возвращено число 1.

Соблюдайте требования по сохранению/восстановлению регистров.

*/

.intel_syntax noprefix

.section .text
.global exchange
exchange:
    push eax
    push ebx
    push edx
    push 0
    
    xor ecx, ecx
    shr edx, 1
    push edx
.loop0:
    cmp ecx, [esp]
    jae .end_loop0
    mov edx, [esp + 8]
    dec edx
    sub edx, ecx
    mov ebx, [eax + 4 * edx]
    cmp [eax + 4 * ecx], ebx
    jb .end_if0
        push [eax + 4 * ecx]
        mov [eax + 4 * ecx], ebx
        pop ebx
        mov [eax + 4 * edx], ebx
        mov edx, [esp + 4]
        inc edx
        mov [esp + 4], edx
    .end_if0:
    inc ecx
    jmp .loop0
.end_loop0:

    add esp, 4
    pop ecx
    pop edx
    pop ebx
    pop eax
    ret
