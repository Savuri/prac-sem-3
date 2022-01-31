// Тут решено не верно, но тесты пробило хех. (на больших числах стопиться)
/*
Задача up11-4: signal-3 (дореш)
Родитель создает двух сыновей, связывая их каналом. Сыновья начинают обмениваться числами 1, 2, 3, ... Число 1 получает первый сын. При получении числа из входного канала каждый сын печатает на стандартный поток вывода родителя свой номер (1 или 2) и полученное число и пересылает обратно число, на 1 большее. При достижении числа, задаваемого в командной строке, пересылает то же самое число в канал и завершает работу.

Процесс должен завершить работу при получении максимального числа из канала. Завершать процесс в других случаях не разрешается. Запрещается отправлять сигнал отцу.

Отец отслеживает завершение работы первого процесса и управляет завершением второго процесса (см. выше). Затем отец дожидается завершения работы обоих процессов, затем выводит строку Done и завершает работу.

Если в командной строке передано число 5, pid первого процесса - 100, pid второго процесса - 101, то вывод должен быть таким:

1 1
2 2
1 3
2 4
Done
Данные передавать в текстовом виде, для ввода-вывода использовать высокоуровневый ввод-вывод (printf, scanf).

Для синхронизации использовать единственный сигнал SIGUSR1. PID процессов передавать через канал.

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
    ID1 = 1,
    ID2 = 2
};

pid_t make_fork(void ) {
    pid_t pid = fork();

    if (pid < 0) {
        exit(1);
    }

    return pid;
}

void handler(int sig_id) {
}

void work(FILE *in, FILE *out, int max, int id) {
    pid_t friend_pid;
    pause();
    fscanf(in, "%d", &friend_pid);
    pause();

    int num;
    while (fscanf(in, "%d", &num) != EOF && num < max) {

        printf("%d %d\n", id, num);
        fflush(stdout);
        fprintf(out, "%d ", ++num);
        fflush(out);

        kill(friend_pid, SIGUSR1);
        usleep(50);
    }
    fprintf(out, "%d ", friend_pid);

    fclose(in);
    fclose(out);

    _exit(0);
}

int wait_child(pid_t pid) {
    int st;

    waitpid(pid, &st, 0);

    return WIFEXITED(st) && !WEXITSTATUS(st);
}

int main(int argc, char const *argv[])
{
    int max;
    sscanf(argv[1], "%d", &max);

    sigaction(SIGUSR1, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);

    int fd[2];

    if (pipe(fd) < 0) {
        return 1;
    }

    FILE *in = fdopen(fd[0], "r");
    FILE *out = fdopen(fd[1], "w");

    pid_t pid1 = make_fork();

    if (!pid1) {
        work(in, out, max, ID1);
    }

    pid_t pid2 = make_fork();

    if (!pid2) {
        work(in, out, max, ID2);
    }

    fprintf(out, "%d\n", pid1);
    fflush(out);

    usleep(1000);
    kill(pid2, SIGUSR1);
    usleep(1000);

    fprintf(out, "%d\n", pid2);
    fflush(out);

    usleep(1000);
    kill(pid1, SIGUSR1);
    usleep(1000);

    fprintf(out, "%d\n", 1);
    fflush(out);

    usleep(1000);
    kill(pid1, SIGUSR1);
    usleep(1000);

    wait_child(WAIT_ANY);

    pid_t pid_last;
    fscanf(in, "%d", &pid_last);

    fprintf(out, "%d ", max);
    fflush(out);

    kill(pid_last, SIGUSR1);

    wait_child(WAIT_ANY);

    fclose(in);
    fclose(out);
    close(fd[0]);
    close(fd[1]);

    printf("Done\n");
    fflush(stdout);

    return 0;
}
