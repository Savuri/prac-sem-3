#include <stdio.h>
#include <dlfcn.h>
#include <link.h>
#include <stdlib.h>
/*
Задача up11-2: signal-2 (дореш)
Программа должна напечатать на стандартный поток вывода свой pid.

Затем программе на стандартном потоке ввода подаются 32-битные знаковые целые числа. В зависимости от текущего режима работы введенное число либо прибавляется к текущему накопленному значению, либо умножается на текущее накопленное значение. После этого новое накопленное значение выводится на стандартный поток вывода. Переполнение при выполнении операций игнорируется. Программа начинает работу в режиме сложения. Начальное накопленное значение равно 0.

При получении сигнала SIGINT программа переключается в режим сложения. При получении сигнала SIGQUIT программа переключается в режим умножения.

Стандартный ввод и стандартный вывод программы будут перенаправлены. Не забывайте сбрасывать буфер вывода. С другой стороны каналов находится другой процесс, который начнет свою работу как только получит pid процесса. Можете расчитывать, что сигналы обрабатываются по модели BSD.

Пример дан в предположении, что программе не отправляются сигналы.
*/
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <limits.h>


enum
{
    ADD,
    MUL
};

int mod = ADD;

void handler (int sigId) {
    if (sigId == SIGINT) {
        mod = ADD;
    } else if (sigId == SIGQUIT) {
        mod = MUL;
    }
}


int main(int argc, char const *argv[])
{
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGQUIT, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    printf("%d\n", getpid());
    fflush(stdout);


    unsigned total = 0;
    int num;

    while (scanf("%d", &num) > 0) {
        if (mod == ADD) {
            total += num;
        } else {
            total *= num;
        }

        total = (int) total;

        printf("%d\n", (int) total);
        fflush(stdout);
    }

    return 0;
}
