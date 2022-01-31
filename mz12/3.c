/*
Задача up12-3: mz12-3 (дореш)
Программа должна вывести на стандартный поток вывода свой PID и переключиться в режим ожидания поступления сигнала. Каждое поступление сигнала SIGUSR1 увеличивает значение счетчика на 5. Каждое поступление сигнала SIGUSR2 уменьшает значение счетчика на 4. Начальное значение счетчика - 32-битного целого числа - равно 0. При поступлении любого из этих сигналов на стандартный поток вывода выводится номер сигнала (значение константы SIG*) и значение счетчика после изменения. Если значение счетчика стало отрицательным программа завершает работу с кодом завершения 0 после вывода значений.

Стандартный ввод и стандартный вывод программы будут перенаправлены. Не забывайте выводить разделитель и сбрасывать буфер вывода. С другой стороны каналов находится другой процесс, который начнет свою работу как только получит pid процесса.

Например, если программе отправляются сигналы SIGUSR1 SIGUSR2 SIGUSR2, то программа должна вывести:

<PID>
10 5
12 1
12 -3
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

int cnt = 0;

void handler (int sigId) {
    if (sigId == SIGUSR1) {
        cnt += 5;

        printf("%d %d\n", sigId, cnt);
        fflush(stdout);
    } else if (sigId == SIGUSR2) {
        cnt -= 4;

        printf("%d %d\n", sigId, cnt);
        fflush(stdout);
        if (cnt < 0) {
            exit(0);
        }
    }
}


int main(int argc, char const *argv[])
{
    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGUSR2, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    printf("%d\n", getpid());
    fflush(stdout);


    while (1) {
        pause();
    }

    return 0;
}
