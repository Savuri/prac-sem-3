/*
Задача up11-1: signal-1 (дореш)
Программа должна напечатать на стандартный поток вывода свой PID, после чего перейти в режим ожидания сигналов. Программа должна обрабатывать сигнал SIGHUP. На каждый приход сигнала программа должна напечатать номер поступления сигнала (0, 1, 2) — каждый раз на отдельной строке. На шестой приход сигнала программа не должна ничего выводить, а просто завершиться с кодом завершения 0.

Таким образом, программа всегда должна выводить:

СВОЙ-PID
0
1
2
3
4
Стандартный ввод и стандартный вывод программы будут перенаправлены. Не забывайте выводить разделитель и сбрасывать буфер вывода. С другой стороны каналов находится другой процесс, который начнет свою работу как только получит pid процесса.

Вывод на стандартный поток вывода разместите в обработчике сигнала.

Вариант семантики сигналов (SysV или BSD), используемый функцией signal, неизвестен.
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



enum
{
    SIG_CNT = '6'
};


void handler(int sigId) {
    static char count = '0';

    if (count == SIG_CNT - 1) {
        exit(0);
    }


    write(STDOUT_FILENO, &count, sizeof(count));
    write(STDOUT_FILENO, "\n", 1);

    count++;
}

int main(int argc, char const *argv[])
{
    sigaction(SIGHUP, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    printf("%d\n", getpid());
    fflush(stdout);


    while (1) {
        pause();
    }

    return 0;
}
