/*
Задача up12-5: mz12-5 (дореш)
Программе в аргументах командной строки передается имя произвольного файла.

Главный процесс создает двух сыновей. Второй сын пересылает первому сыну содержимое указанного файла, а первый сын выводит полученные данные на стандартный поток вывода.

Для пересылки файла используется механизм сигналов: файл пересылается побитно, для пересылки бита 0 второй сын отправляет первому сыну сигнал SIGUSR1, а для пересылки бита 1 - сигнал SIGUSR2. Первый процесс подтверждает получение каждого бита с помощью сигнала SIGALRM. После завершения пересылки всего файла второй процесс посылает первому процессу сигнал SIGIO.

Главный процесс должен дождаться завершения работы обоих процессов и завершиться сам.

Использовать другие средства межпроцессного взаимодействия запрещено.


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

char data = 0;
int bits_cnt = 0;
pid_t pid1, pid2;
int flag1 = 0, flag2 = 0;


void handler(int sig_id) {
    if (sig_id == SIGUSR1) {
        data = data << 1;
        data += 0;
        bits_cnt++;
    } else {
        data = data << 1;
        data += 1;
        bits_cnt++;
    }

    flag1 = 1;
}

void handler1(int sig_id) {
    if (sig_id == SIGALRM) {
        flag2 = 1;
    } else if (sig_id == SIGIO) {
        _exit(0);
    }
}

int main(int argc, char **argv) {
    sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGUSR2, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGALRM, &(struct sigaction){ .sa_handler = handler1, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGIO, &(struct sigaction){ .sa_handler = handler1, .sa_flags = SA_RESTART }, NULL);


    pid1 = make_fork();

    if (!pid1) {
        sigset_t mask, oldmask;

        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigaddset(&mask, SIGUSR2);
        sigaddset(&mask, SIGIO);

        while (1) {
            sigprocmask(SIG_BLOCK, &mask, &oldmask);
            while (!flag1)
                sigsuspend(&oldmask);
            flag1 = 0;
            sigprocmask(SIG_UNBLOCK, &mask, NULL);

            if (bits_cnt == 8) {
                putchar(data);
                fflush(stdout);
                data = 0;
                bits_cnt = 0;
            }

            kill(pid2, SIGALRM);
        }
    }

    pid2 = make_fork();

    if (!pid2) {
        sigset_t mask, oldmask;

        sigemptyset(&mask);
        sigaddset(&mask, SIGALRM);

        int fd = open(argv[1], O_RDONLY);

        char c;
        while (read(fd, &c, sizeof(c)) > 0) {
            for (int i = 7; i >= 0; --i) {
                char cur_bit = c >> i;
                cur_bit = cur_bit & 1;

                int sig;

                if (cur_bit) {
                    sig = SIGUSR2;
                } else {
                    sig = SIGUSR1;
                }

                kill(pid1, sig);

                sigprocmask(SIG_BLOCK, &mask, &oldmask);
                while (!flag2)
                    sigsuspend(&oldmask);
                flag2 = 0;
                sigprocmask(SIG_UNBLOCK, &mask, NULL);
            }
        }

        kill(pid1, SIGIO);

        _exit(0);
    }


    wait_child(WAIT_ANY);
    wait_child(WAIT_ANY);

    return 0;
}
