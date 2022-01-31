/*
Задача up12-2: mz12-2 (дореш)
Программе передаются три аргумента командной строк:

count — 32-битное знаковое неотрицательное число;
modulo — 32-битное знаковое положительное число;
prog — имя программы для запуска.
Программа (главный процесс P0) должна создать сына (процесс P1), который должен создать своего сына (процесс P2), главный процесс должен создать второго сына (процесс P3), который должен создать своего сына (процесс P4). Процесс P1 связывается с процессом P2 неименованным каналом в направлении от P1 к P2, процесс P2 связывается с процессом P4 неименованным каналом в направлении от P2 к P4. Процесс P2 запускает с помощью exec*p на выполнение программу prog, соответствующим образом перенаправляя ей стандартные потоки ввода-вывода.

Процесс P1 генерирует count квадратов положительных целых чисел по модулю (то есть остаток от деления на) modulo и записывает их в канал. Например, последовательность первых 10 квадратов по модулю 10 равна { 1, 4, 9, 6, 5, 6, 9, 4, 1, 0 }.

Процесс P4 считывает данные, поступившие из своего канала и выводит их на стандартный поток вывода, заменяя символ пробела на символ '\n'.

Главный процесс ожидает завершения всей системы процессов, выводит на стандартный поток вывода число 0 и символ '\n' и завершает работу с кодом завершения 0.
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>


key_t make_fork(void ) {
    key_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "fork() error: %s\n", strerror(errno));
        exit(1);
    }

    return pid;
}

int wait_child(pid_t pid) {
    int stat;

    waitpid(pid, &stat, 0);

    return (WIFEXITED(stat) && !WEXITSTATUS(stat));
}


int main(int argc, char **argv) {
    int count, modulo;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &modulo);

    int fd1[2], fd2[2];

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        return 1;
    }

    pid_t pid1 = make_fork();

    if (!pid1) {
        pid_t pid2 = make_fork();

        if (!pid2) {
            close(fd1[1]);
            close(fd2[0]);

            if (dup2(fd1[0], STDIN_FILENO) < 0 || dup2(fd2[1], STDOUT_FILENO) < 0) {
                fprintf(stderr, "error: %s\n", strerror(errno));

                _exit(1);
            }

            execlp(argv[3], argv[3], NULL);

            fprintf(stderr, "error %s\n", strerror(errno));

            _exit(1);
        }

        //p1
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);

        FILE *out = fdopen(fd1[1], "w");

        for (int i = 1; i <= count; ++i) {
            int num = (long long)(i % modulo) * (i % modulo) % modulo;
            fprintf(out, "%d ", num);
            fflush(out);
        }

        fclose(out);

        wait_child(WAIT_ANY);

        _exit(0);
    }

    close(fd1[0]);
    close(fd1[1]);
    close(fd2[1]);

    pid_t pid3 = make_fork();

    if (!pid3) {
        pid_t pid4 = make_fork();

        if (!pid4) {
            char buf[PATH_MAX];
            int n;
            while ((n = read(fd2[0], &buf, sizeof(buf))) > 0) {
                for (int i = 0; i < n; ++i) {
                    if (buf[i] == ' ') {
                        buf[i] = '\n';
                    }
                }

                write(1, buf, n);
            }

            close(fd2[0]);
            _exit(0);
        }

        close(fd2[0]);
        wait_child(WAIT_ANY);
        _exit(0);
    }

    close(fd2[0]);

    wait_child(WAIT_ANY);
    wait_child(WAIT_ANY);

    printf("0\n");
    fflush(stdout);

    return 0;
}
