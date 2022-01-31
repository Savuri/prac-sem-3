/*
Задача up10-5: mz10-5 (дореш)
Родитель создает двух сыновей, связывая их двумя каналами. Сыновья начинают обмениваться числами 1, 2, 3, ... Число 1 получает первый сын. При получении числа из входного канала каждый сын печатает на стандартный поток вывода родителя свой номер (1 или 2) и полученное число и пересылает обратно число, на 1 большее. При достижении числа, задаваемого в командной строке, сын не пересылает число в канал, а завершает работу.

Процесс может завершить работу либо при получении максимального числа из канала, либо при наступлении конца файла в канале. Завершать процесс в других случаях не разрешается.

Отец дожидается завершения работы обоих процессов, затем выводит строку Done и завершает работу.

Если в командной строке передано число 5, pid первого процесса - 100, pid второго процесса - 101, то вывод должен быть таким:

1 1
2 2
1 3
2 4
Done
Данные передавать в pipe в текстовом виде, для ввода-вывода использовать высокоуровневый ввод-вывод (printf, scanf и т. д.).

Привязывать файловые дескрипторы к дескрипторам потока можно либо с помощью функции fdopen, либо с помощью перенаправления стандартных потоков ввода-вывода.
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
    FIRST_SON = 1,
    SECOND_SON = 2
};

pid_t make_fork(void ) {
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "fork error: %s\n", strerror(errno));
        _exit(1);
    }

    return pid;
}

int wait_child(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);

    return WIFEXITED(status) && !WEXITSTATUS(status);
}


void work(FILE *in, FILE *out, int son_i, int max) {
    int num;

    while (fscanf(in, "%d", &num) != EOF && num < max) {
        printf("%d %d\n", son_i, num);
        fflush(stdout);

        fprintf(out, "%d\n", num + 1);
        fflush(out);
    }
}

int main(int argc, char const *argv[])
{
    int fd1[2], fd2[2];

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        fprintf(stderr, "pipe error: %s\n", strerror(errno));
        _exit(1);
    }

    int max;
    sscanf(argv[1], "%d", &max);

    pid_t pid1 = make_fork();

    if (!pid1) {
        // need fd1[0], fd2[1]

        close(fd1[1]);
        close(fd2[0]);

        int son_i = FIRST_SON;

        FILE *in = fdopen(fd1[0], "r");
        FILE *out = fdopen(fd2[1], "w");

        if (in == NULL || out == NULL) {
            fprintf(stderr, "fdopen error: %s\n", strerror(errno));
            _exit(1);
        }

        work(in, out, son_i, max);

        fclose(in);
        fclose(out);

        close(fd1[0]);
        close(fd2[1]);

        _exit(0);
    }

    pid_t pid2 = make_fork();

    if (!pid2) {
        // need fd1[1], fd2[0]
        close(fd1[0]);
        close(fd2[1]);

        int son_i = SECOND_SON;

        FILE *in = fdopen(fd2[0], "r");
        FILE *out = fdopen(fd1[1], "w");

        if (in == NULL || out == NULL) {
            fprintf(stderr, "fdopen error: %s\n", strerror(errno));
            _exit(1);
        }

        work(in, out, son_i, max);

        fclose(in);
        fclose(out);

        close(fd2[0]);
        close(fd1[1]);

        _exit(0);
    }

    FILE *out = fdopen(fd1[1], "w");

    fprintf(out,"1\n");
    fflush(out);

    fclose(out);

    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);

    if (!wait_child(pid1) || !wait_child(pid2)) {
        fprintf(stderr, "Child error: %s\n", strerror(errno));
        _exit(1);
    }

    printf("Done\n");
    fflush(stdout);

    return 0;
}
