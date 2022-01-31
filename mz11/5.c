/*
Задача up11-5: signal-4 (дореш)
Программе на стандартном потоке ввода задаются два целых числа low и high. Программа должна вывести на стандартный поток вывода свой PID, после вывода программа должна обрабатывать получение сигналов SIGINT и SIGTERM.

Программа должна вычислять простые числа в интервале [low,high). Если программа получила сигнал SIGINT, программа должна вывести на стандартный поток вывода последнее на текущей момент найденное простое число. При получении SIGINT в четвертый раз, программа должна закончить работу с кодом завершения 0. Таким образом, может быть выведено не более трех простых чисел.

При получении сигнала SIGTERM программа должна завершиться с кодом 0.

Если программа закончила вычисления, программа должна вывести на стандартный поток вывода число -1 и завершиться с кодом 0.

Если на момент прихода сигнала ни одно простое число найдено не было, выводите 0.

Учтите, что стандартный поток вывода программы будет перенаправлен в pipe.
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
#include <math.h>

enum
{
    LAST_SIGINT = 4
};

static int last_prime = 0;

void sigint_handler(int sig_id) {
    static int count = 0;

    count++;

    if (count == LAST_SIGINT) {
        _exit(0);
    }

    printf("%d\n", last_prime);
    fflush(stdout);
}


void sigterm_handler(int sig_id) {
    exit(0);
}

int is_prime(int num) {
    if (num <= 1) {
        return 0;
    }

    for (int i = 2; i <= (int) sqrt(num) + 1; i++) {
        if (num % i == 0) {
            return 0;
        }
    }

    return 1;
}


int main(int argc, char const *argv[])
{
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = sigint_handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGTERM, &(struct sigaction) { .sa_handler = sigterm_handler, .sa_flags = SA_RESTART }, NULL);

    printf("%d\n", getpid());
    fflush(stdout);

    int low, high;
    scanf("%d %d", &low, &high);

    for (int i = low; i < high; ++i) {
        if (is_prime(i)) {
            last_prime = i;
        }
    }

    printf("-1\n");
    fflush(stdout);

    return 0;
}
