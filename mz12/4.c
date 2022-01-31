/*
Задача up12-4: mz12-4 (дореш)
Программе в аргументах командной строки передаются имя выходного файла (OUT) и положительное 32-битное целое знаковое число (MOD).

В результате работы программы в выходном файле должен находиться исполняемый файл, который при запуске считывает со стандартного потока ввода одно положительное 32-битное целое знаковое число и выводит на стандартный поток вывода остаток от деления на MOD суммы чисел от 0 до введенного числа включительно.

Выходной файл должен иметь все права для владельца, право на чтение и выполнение для группы и право на выполнение для прочих пользователей.

Функции system и popen использовать запрещено.


*/
#include <stdio.h>
#include <dlfcn.h>
#include <link.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <limits.h>


const char beg_program[] = "#! /usr/bin/python\nn = int(input())\nsum = ((1 + n) * n) // 2\nprint(sum % ";
const char end_program[] = ")";

int main(int argc, char const *argv[])
{
    int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0751);

    int mod;
    sscanf(argv[2], "%d", &mod);

    dprintf(fd, "%s%d%s", beg_program, mod, end_program);
    close(fd);

    return 0;
}

