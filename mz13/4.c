/*
Задача mz13-4: ipc-4
В аргументах командной строки программе задается одно целое число count - число процессов-сыновей (1 ≤ count ≤ 100).

На стандартном потоке ввода подается последовательность 32-битных знаковых целых чисел в текстовом виде. Последовательность завершается признаком конца ввода.

Родитель создает count сыновей, которые нумеруются от 0 до count-1. Сыновья работают последовательно. В свою очередь каждый сын считывает одно число из входной последовательности, выводит на стандартный поток вывода свой номер и считанное число. Затем очередь работы передается сыну с номером, равным остатку (математическому) от деления считанного числа на количество процессов.

После создания всех процессов родитель передает очередь чтения процессу с номером 0. Затем родитель дожидается завершения работы сыновей и сам завершает работу.

Для синхронизации процессов используйте семафоры SysV IPC.

Идентификатор массива семафоров выбирайте самостоятельно. В конце работы программы все созданные объекты IPC должны быть уничтожены.

Стандартные потоки ввода и вывода являются каналами.

Ниже приведен пример работы при числе процессов 3.

Примеры
Входные данные
1 2 4 3 5 6 9 2
Результат работы
0 1
1 2
2 4
1 3
0 5
2 6
0 9
0 2
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
#include <time.h>
#include <fcntl.h>

enum ArgsPositions
{
    COUNT_POS = 1,
};

enum
{
    KEY = 123
};

int wait_child(pid_t pid) {
    int stat;

    waitpid(pid, &stat, 0);

    return (WIFEXITED(stat) && !WEXITSTATUS(stat));
}

void work(int count, int sem_id, int id) {
    while (1) {
        struct sembuf op = { .sem_num = id, .sem_op = -1, .sem_flg = SEM_UNDO };

        if (semop(sem_id, &op, 1) < 0) {
            exit(1);
        }

        int num;
        if (scanf("%d", &num) != 1) {
            exit(0);
        }

        printf("%d %d\n", id, num);
        fflush(stdout);

        op.sem_num = (num < 0) ? ((num % count + count) % count) : (num % count);
        op.sem_op = 1;
        op.sem_flg = SEM_UNDO;

        if (semop(sem_id, &op, 1) < 0) {
            exit(1);
        }
    }
}

void resources_destroyer(pid_t *pids, int n, int count, int sem_id) {
    for (int i = 0; i < n; ++i) {
        if (pids[i] != 0) {
            kill(pids[i], SIGKILL);
        }
    }

    for (int i = 0; i < n; ++i) {
        wait_child(WAIT_ANY);
    }

    semctl(sem_id, 0, IPC_RMID);
}

int main(int argc, char **argv)
{
    int count;
    sscanf(argv[COUNT_POS], "%d", &count);

    setbuf(stdin, NULL);
    setbuf(stdout   , NULL);

    pid_t *pids = calloc(count, sizeof(*pids));

    if (pids == NULL) {
        return 1;
    }

    memset(pids, 0, count * sizeof(*pids));

    int sem_id = semget(KEY, count, IPC_CREAT | 0666);

    if (sem_id < 0) {
        free(pids);

        return 1;
    }

    for (int i = 0; i < count; ++i) {
        pids[i] = fork();

        if (pids[i] < 0) {
            resources_destroyer(pids, i, count, sem_id);

            return 1;
        } else if (!pids[i]) {
            work(count, sem_id, i);
            _exit(1);
        }
    }

    struct sembuf op = {0, 1, SEM_UNDO };

    if (semop(sem_id, &op, 1) < 0) {
        resources_destroyer(pids, count, count, sem_id);

        return 1;
    }

    wait_child(WAIT_ANY);

    resources_destroyer(pids, count, count, sem_id);

    return 0;
}
