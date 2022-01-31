/*
Задача mz13-1: ipc-1
Программе в аргументах командной строки задаются:

Количество процессов (nproc) (1 <= nproc <= 400);
Ключ IPC (key);
Максимальное значение (maxval) (32-битное знаковое целое положительное значение).
Программа должна создать nproc процессов и запустить между ними пересылку числа. Процессы нумеруются от 1 до nproc. Процесс-родитель имеет номер 0. Каждый процесс получает число, печатает свой порядковый номер, значение числа, и номер процесса-отправителя как показано в примере. Затем процесс увеличивает его на 1 и пересылает число процессу с номером (val * val * val * val) % nproc + 1, где val – значение числа после увеличения. Если значение числа после увеличения стало больше maxval, процесс не пересылает число следующему, а (совместно с родителем) выполняет действия по завершению игры. Игра начинается с первого из созданных процессов со значения 0. Отец дожидается завершения всех процессов и сам завершает работу.

Обратите внимание, при достижении максимального значения оно печатается, но не пересылается дальше.

Для синхронизации процессов используйте семафоры. Для пересылки значения используйте разделяемую память. Массив семафоров и разделяемая память должны уничтожаться в конце работы. Сигналы использовать запрещено.

Тестирование завершается с вердиктом 'Synchronization error', если процесс-отец (то есть ваша программа, запускаемая на тестирование) заканчивает работу раньше какого-либо из своих потомков.

Тестирование завершается с вердиктом 'Security violation', если после завершения работы вашей программы остались неудаленные объекты IPC.
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>

enum
{
    RIGHTS = 0666
};

int wait_child(pid_t pid) {
    int stat;

    waitpid(pid, &stat, 0);

    return WIFEXITED(stat) && !WEXITSTATUS(stat);
}

void resource_destruction(pid_t *pids, int sem_id, int mem_id) {
    semctl(sem_id, 0, IPC_RMID, 0);
    shmctl(mem_id, IPC_RMID, NULL);

    free(pids);
}

typedef struct Message
{
    long long int val;
    int sender;
} Message;



int main(int argc, char **argv) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    Message *buf;
    int n, sem_id, mem_id;
    long long int maxval;
    key_t key;


    sscanf(argv[1], "%d", &n);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%lld", &maxval);

    sem_id = semget(key, n, RIGHTS | IPC_CREAT | IPC_EXCL);
    if (sem_id < 0) {
        exit(1);
    }

    mem_id = shmget(key, sizeof(Message), RIGHTS | IPC_CREAT | IPC_EXCL);
    if (mem_id < 0) {
        semctl(sem_id, 0, IPC_RMID, NULL);
        exit(1);
    }

    buf = (Message*)shmat(mem_id, NULL, 0);
    buf->val = 0;
    buf->sender = 0;

    pid_t *pids = calloc(sizeof(*pids), n);

    for (int i = 1; i <= n; i++) {
        pids[i - 1] = fork();
        if (pids[i - 1] < 0) {
            resource_destruction(pids, sem_id, mem_id);
            return 1;
        } else if (!pids[i - 1]) {
            free(pids);
            struct sembuf op;
            op.sem_flg = 0;

            while (1) {
                op.sem_num = (i - 1);
                op.sem_op = -1;
                if (semop(sem_id, &op, 1) == -1) {
                    shmdt(buf);
                    _exit(0);
                }

                printf("%d %lld %d\n", i, buf->val++, buf->sender);
                buf->sender = i;
                if (buf->val > maxval) {
                    semctl(sem_id, 0, IPC_RMID, NULL);
                    shmdt(buf);
                    _exit(0);
                }

                op.sem_num = ((buf->val % n) * (buf->val % n) * (buf->val % n) * (buf->val % n) % n);
                op.sem_op = 1;
                if (semop(sem_id, &op, 1) == -1) {
                    shmdt(buf);
                    _exit(0);
                }
            }
        }
    }

    semctl(sem_id, 0, SETVAL, 1);

    wait_child(WAIT_ANY);

    resource_destruction(pids, sem_id, mem_id);

    for (int i = 0; i < n - 1; i++) {
        wait_child(WAIT_ANY);
    }

    return 0;
}
