/*
Задача mz13-3: ipc-3
Программе задаются следующие аргументы командной строки:

Ключ IPC для очереди сообщений
Число процессов для запуска n
64-битное целое число value1
64-битное целое число value2
64-битное целое число maxval
Программа должна создать n процессов. Каждый процесс имеет свой номер от 0 до n - 1. Когда процесс получает очередь хода, с помощью очереди сообщений процесс должен получить два 64-битных целых числа x1, x2, вычислить их сумму x3 = x1 + x2, напечатать на стандартный поток вывода свой номер и число x3, затем переслать числа x2 и x3 процессу с номером x3 % n.

Гарантируется, что x3 всегда будет неотрицательным.

Если x3 по модулю больше значения maxval, то вместо пересылки следующему процессу все процессы должны завершиться.

В начале игры родительский процесс посылает процессу 0 числа value1 и value2.

Родительский процесс должен дождаться завершения работы всех процессов и сам завершиться.

Родительский процесс должен корректно обрабатывать ошибочное завершение fork(). При этом все уже созданные процессы должны быть уничтожены. Родитель должен завершиться с кодом 1.

Пример запуска программы:

../solution 1234 2 0 1 15
Результат работы:

0 1
1 2
0 3
1 5
1 8
0 13
1 21
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/msg.h>

enum ArgsPositions
{
    KEY_POS = 1,
    N_PROC_POS = 2,
    VAL1_POS = 3,
    VAL2_POS = 4,
    MAX_VAL_POS = 5
};


enum
{
    MODE = 0660
};

typedef struct Message
{
    long mtype;
    long long val1;
    long long val2;
} Message;

int wait_child(pid_t pid) {
    int stat;

    waitpid(pid, &stat, 0);

    return (WIFEXITED(stat) && !WEXITSTATUS(stat));
}


void resource_destruction(pid_t *pids, int sons_cnt, int msg_id) {
    for (int i = 0; i < sons_cnt; ++i) {
        kill(pids[i], SIGKILL);
    }

    for (int i = 0; i < sons_cnt; ++i) {
        wait_child(WAIT_ANY);
    }

    msgctl(msg_id, IPC_RMID, NULL);

    free(pids);
}

int main(int argc, char **argv) {
    int n_proc;
    key_t key;
    long long int val1, val2, maxval;

    sscanf(argv[KEY_POS], "%d", &key);
    sscanf(argv[N_PROC_POS], "%d", &n_proc);
    sscanf(argv[VAL1_POS], "%lld", &val1);
    sscanf(argv[VAL2_POS], "%lld", &val2);
    sscanf(argv[MAX_VAL_POS], "%lld", &maxval);


    int msg_id = msgget(key, IPC_CREAT | MODE);

    if (msg_id < 0) {
        fprintf(stderr, "msgget error: %s\n", strerror(errno));
        return 1;
    }

    Message msg;

    int message_size = sizeof(msg) - sizeof(msg.mtype);

    pid_t *pids = calloc(n_proc, sizeof(*pids));

    if (pids == NULL) {
        fprintf(stderr, "calloc error: %s\n", strerror(errno));
        return 1;
    }


    for (int i = 0; i < n_proc; ++i) {
        pids[i] = fork();

        if (pids[i] < 0) {
            fprintf(stderr, "msgget error: %s\n", strerror(errno));
            resource_destruction(pids, i, msg_id);
            return 1;
        } else if (!pids[i]) {
            while (1) {
                if (msgrcv(msg_id, &msg, message_size, i + 1, 0) < 0) {
                    _exit(0);
                }

                long long int val3 = msg.val1 + msg.val2;
                printf("%d %lld\n", i, val3);
                fflush(stdout);

                if (maxval < val3) {
                    _exit(0);
                }

                int next = val3 % n_proc + 1;

                msg.val1 = msg.val2;
                msg.val2 = val3;
                msg.mtype = next;

                if (msgsnd(msg_id, &msg, message_size, IPC_NOWAIT) < 0) {
                    _exit(0);
                }
            }
        }
    }

    msg.val1 = val1;
    msg.val2 = val2;
    msg.mtype = 1;

    if (msgsnd(msg_id, &msg, message_size, IPC_NOWAIT) < 0) {
        printf("FIRST =(\n");
        fflush(stdout);
        _exit(0);
    }

    wait_child(WAIT_ANY);
    resource_destruction(pids, 0, msg_id);

    for (int i = 0; i < n_proc; ++i) {
        wait_child(WAIT_ANY);
    }

    return 0;
}
