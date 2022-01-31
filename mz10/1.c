/*
Задача up10-1: mz10-1 (дореш)
Напишите программу. Родитель создает сына, тот — внука, а тот — правнука. Правнук передает в канал текущее время, полученное с помощью системного вызова time, в бинарном виде (тип time_t).

Отец, сын и внук считывают время из канала. Процесс-отец выводит на экран строку "Y:????", где ???? заменяются на текущий год, сын — "M:??", где ?? заменяются на текущий месяц в году (от 1 до 12), число всегда выводится с двумя знаками, внук — "D:??", где ?? заменяются на номер дня в месяце, всегда выводящееся с двумя знаками.

Внук должен вывести число первым, сын — вторым, а отец — третьим. Записывать в канал разрешается только правнуку. Родитель должен дождаться завершения всех процессов и завершиться сам с кодом 0.
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

enum
{
    READS_CNT = 3,
    YEAR_SHIFT = 1900
};


int make_fork(void) {
    int pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Fork error: %s", strerror(errno));
        exit(1);
    }

    return pid;
};


// return 1 if all ok or 0 if not
int wait_child(void) {
    int st;
    wait(&st);

    return (WIFEXITED(st) && !WEXITSTATUS(st));
}

void read_and_print_time(int fd, char mode) {
    time_t t;
    if (read(fd, &t, sizeof(t)) < 0) {
        _exit(1);
    }

    struct tm timeinfo = { 0 };
    timeinfo.tm_isdst = -1;
    localtime_r(&t, &timeinfo);

    switch (mode) {
        case 'D':
            printf("D:%02d\n", timeinfo.tm_mday);
            break;
        case 'M':
            printf("M:%02d\n", timeinfo.tm_mon + 1);
            break;
        case 'H':
            printf("Y:%04d\n", timeinfo.tm_year + YEAR_SHIFT);
            break;
        default:
            break;
    }

    fflush(stdout);
}


int main (int argc, char *argv[]) {
    int fd[2];
    if (pipe(fd) == -1) {
        return 1;
    }

    int pid1;

    if (!(pid1 = make_fork())) {
        int pid2;

        if (!(pid2 = make_fork())) {
            int pid3;

            if (!(pid3 = make_fork())) {
                for (int i = 0; i < READS_CNT; i++) {
                    time_t t1 = time(NULL);
                    if (write(fd[1], &t1, sizeof(t1)) < 0) {
                        _exit(1);
                    }
                }

                _exit(0);
            }

            close(fd[1]);

            if (!wait_child()) {
                _exit(1);
            }

            read_and_print_time(fd[0], 'D');

            _exit(0);
        }

        close(fd[1]);

        if (!wait_child()) {
            _exit(1);
        }

        read_and_print_time(fd[0], 'M');

        _exit(0);
    }

    close(fd[1]);

    if (!wait_child()) {
        _exit(1);
    }

    read_and_print_time(fd[0], 'H');
    close(fd[0]);

    return 0;
}
