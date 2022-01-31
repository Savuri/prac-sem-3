/*
Задача up10-2: mz10-2 (дореш)
Процесс-родитель создает процесса-сына, а тот в свою очеред процесса-внука. Процесс-родитель и процесс-внук должны быть соединены анонимным каналом в направлении от родителя к внуку.

Процесс-родитель считывает 32-битные знаковые целые числа, подаваемые на стандартном потоке ввода в текстовом виде. Последовательность заканчивается признаком конца файла. Процесс-родитель передает считанные числа в канал в бинарном виде.

Процесс-внук считывает числа из канала и вычисляет их сумму. После чтения всех чисел процесс-внук выводит на стандартный поток вывода их сумму и завершает работу.

Процесс-родитель должен дождаться завершения всех порожденных им процессов и завершиться сам с кодом завершения 0.

Ошибка "Synchronization error" означает, что родитель завершился раньше какого-либо из порожденных им процессов.
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <math.h>
#include <dlfcn.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>

enum
{
    SEC_IN_MIN = 60,
    MIN_IN_HOUR = 60,
    SEC_IN_HOUR = MIN_IN_HOUR * SEC_IN_MIN,
    HOUR_IN_DAY = 24,
    SEC_IN_DAY = HOUR_IN_DAY * SEC_IN_HOUR
};

int make_fork(void ) {
    int pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Fork error: %s", strerror(errno));
        exit(1);
    }

    return pid;
};

int wait_child(int pid) {
    int st;
    waitpid(pid, &st, 0);
    // 1 if all ok
    return (WIFEXITED(st) && !WEXITSTATUS(st));
}

int main (int argc, char *argv[]) {
    int fd[2];
    if (pipe(fd) < 0) {
        fprintf(stderr, "Pipe error: %s\n", strerror(errno));
        return 1;
    }

    int pid1;

    if (!(pid1 = make_fork())) {
        int pid2;

        if (close(fd[1]) < 0) {
            _exit(1);
        }

        if (!(pid2 = make_fork())) {
            int64_t sum = 0;

            int num;
            int read_code;
            while ((read_code = read(fd[0], &num, sizeof(num))) > 0) {
                sum += num;
            }

            if (read_code < 0) {
                fprintf(stderr, "Read error: %s\n", strerror(errno));
                _exit(1);
            }
            printf("%" PRId64 "\n", sum);
            fflush(stdout);
            _exit(0);
        }

        if (close(fd[0]) < 0) {
            _exit(1);
        }

        if (wait_child(pid2)) {
            _exit(0);
        }

        fprintf(stderr, "Child error\n");
        _exit(1);
    }

    if (close(fd[0]) < 0) {
        _exit(1);
    }

    int num;
    while (scanf("%d", &num) >= 0) {
        if (write(fd[1], &num, sizeof(num)) < 0) {
            return 1;
        }
    }

    close(fd[1]);

    if (!wait_child(pid1)) {
        fprintf(stderr, "Child error\n");
        return 1;
    }

    return 0;
}
