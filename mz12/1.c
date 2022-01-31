/*
Задача up12-1: mz12-1 (дореш)
Напишите функцию bit_reverse со следующим прототипом на Си:

STYPE bit_reverse(STYPE value);
Где STYPE — это некоторый целый знаковый тип. Кроме того, определен тип UTYPE — это некоторый целый беззнаковый тип того же размера, что и STYPE.

Функция меняет порядок бит в числе на противоположный. Например, рассмотрим 4-битное целое число 0101. Поменяв порядок бит на противоположный, получим число 1010. Однако, если то же число рассматривать как 8-битное, обратный порядок бит будет равен 10100000.

Для того, чтобы определить типы STYPE и UTYPE в своей программе для тестирования, можно использовать typedef следующим образом:

typedef int STYPE;
typedef unsigned int UTYPE;
Сдаваемый на проверку текст должен содержать только функцию bit_reverse. Определения типов STYPE и UTYPE и функция main в нем находиться не должны.
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>


enum
{
    BITS_IN_BYTE = 8
};

STYPE bit_reverse(STYPE value) {
    STYPE res = 0;

    UTYPE mask = 1;

    for (unsigned int i = 0; i < sizeof(STYPE) * BITS_IN_BYTE; ++i) {
        if (value & mask) {
            res = res | (((UTYPE) 1) << (sizeof(STYPE) * BITS_IN_BYTE - 1 - i));
        }

        mask = mask << 1;
    }

    return res;
}
