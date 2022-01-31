/*
Отец создаёт трёх сыновей, каждый из которых будет обрабатытвать класс ip-адресов A, B, C соответственно. 32-битные Ip-адреса в ВОСЬМЕРИЧНОМ виде считывать должен отец со стандартного потока ввода (считвывает до eof) и направляет соотвествующему сыну (Класс А - 1, Класс B - 2, Класс С - 3). Сын должен напечатать свой номер (Класс А - 1, Класс B - 2, Класс С - 3) и хост ip-адреса. Затем вывести символ перевода строки. Использовать семафоры.
	Пример:
	Ввод:
	416377444
	20213101636
	31014767240
	Вывод:
	1 39ff24
	2 839e
	3 a0
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
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/sem.h>

enum
{
    PROC_CNT = 3,
    KEY = 123
};

enum
{
    MASK_A = 0x80000000,
    MASK_B = 0x40000000,
    MASK_C = 0x20000000
};

enum
{
    CLASS_A_HOST_MASK = 0x00ffffff,
    CLASS_B_HOST_MASK = 0x0000ffff,
    CLASS_C_HOST_MASK = 0x000000ff
};

pid_t make_fork(void) {
    pid_t pid = fork();

    if (pid == -1) {
        _exit(1);
    }

    return pid;
}

char get_type(int num) {
    if ((num & MASK_A) == 0) {
        return 'a';
    } else if ((num & MASK_B) == 0) {
        return 'b';
    } else if ((num & MASK_C) == 0) {
        return 'c';
    }

    return 'u'; // unexpected
}

int main(int argc, char **argv) {
    int sem_id = semget(KEY, PROC_CNT + 1, 0666 | IPC_CREAT | IPC_EXCL);

    if (sem_id < 0) {
        perror("");

        return 1;
    }

    for (int i = 0; i < PROC_CNT; ++i) {
        if (semctl(sem_id, i, SETVAL, 0) < 0) {
            perror("");

            return 1;
        }
    }

    int shm_id = shmget(KEY, sizeof(int ), 0666 | IPC_CREAT | IPC_EXCL);

    if (shm_id < 0) {
        perror("");
        return 1;
    }

    int *buf = shmat(shm_id, NULL, 0);

    pid_t pids[PROC_CNT];

    for (int i = 0; i < PROC_CNT; ++i) {
        pids[i] = make_fork();

        if (!pids[i]) {
            while (1) {
                //i = 0 - a, i = 1 - b, i = 2 - c
                struct sembuf op = {.sem_num = i, .sem_op = -1, .sem_flg = 0};

                if (semop(sem_id, &op, 1) < 0) {
                    exit(1);
                }

                int host = 0;

                if (i == 0) {
                    host = (*buf) & CLASS_A_HOST_MASK;
                } else if (i == 1) {
                    host = (*buf) & CLASS_B_HOST_MASK;
                } else {
                    host = (*buf) & CLASS_C_HOST_MASK;
                }

                printf("%d %x\n", i + 1, host);
                fflush(stdout);

                op.sem_op = 1;
                op.sem_num = 3;

                if (semop(sem_id, &op, 1) < 0) {
                    exit(1);
                }
            }
        }
    }

    while (scanf("%o", buf) != EOF) {
        char type = get_type(*buf);

        if (type == 'u') {
            continue;
        }

        struct sembuf op = { .sem_num = type - 'a', .sem_op = 1, .sem_flg = 0 };

        if (semop(sem_id, &op, 1) < 0) {
            exit(1);
        }

        op.sem_op = -1;
        op.sem_num = 3;

        if (semop(sem_id, &op, 1) < 0) {
            exit(1);
        }
    }

    for (int i = 0; i < PROC_CNT; ++i) {
        kill(pids[i], SIGKILL);
    }

    for (int i = 0; i < PROC_CNT; ++i) {
        wait(NULL);
    }

    shmdt(buf);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID, NULL);

    return 0;
}

// 200/200
