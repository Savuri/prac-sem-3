/*
Задача mz13-2: ipc-2
Программе в аргументах командной строки задаются:

Размер массива (count);
Ключ IPC (key);
Количество процессов (nproc);
Число итераций (iter_count);
Затравка ГПСЧ для каждого процесса (nproc аргументов).
Программа должна создать в разделяемой памяти массив из count элементов типа int и заполнить его значениями, считанными со стандартного потока ввода.

Далее программа должна создать nproc процессов, для каждого процесса проинициализировав ГПСЧ соответствующим значением из командной строки.

Все процессы должны выполнить iter_count итераций модификации массива по следующему алгоритму. На каждой итерации выбираются два случайных индекса в массиве (каждый индекс в диапазоне [0;count) ) и случайное значение в диапазоне [0; 10). Далее должна вызываться функция operation, которая выполняет следующие действия:

void
operation(int *data, int ind1, int ind2, int value)
{
    if (ind1 != ind2) {
        int tmp1 = data[ind1] - value;
        int tmp2 = data[ind2] + value;

        data[ind1] = tmp1;
        data[ind2] = tmp2;
    }
}
Считайте эту функцию написанной и доступной в вашей программе.

Родитель дожидается окончания всех сыновей и выводит на стандартный поток вывода значения массива.

Для синхронизации процессов используйте семафоры. Для пересылки значения используйте разделяемую память. Массив семафоров и разделяемая память должны уничтожаться в конце работы.

Должна быть обеспечена целостность массива.

Синхронизация должна позволять двум процессам одновременно модифицировать массив, если модифицируемые индексы массива не пересекаются.
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

enum Positions_in_args
{
    COUNT_POS = 1,
    KEY_POS = 2,
    N_PROC_POS = 3,
    ITER_COUNT_POS = 4,
    FIRST_SEED_POS = 5
};

enum
{
    MAX_VAL = 10
};

int wait_child(pid_t pid) {
    int stat;

    waitpid(pid, &stat, 0);

    return WIFEXITED(stat) && !WEXITSTATUS(stat);
}

void resource_destruction(pid_t *pids, int sons_cnt, int n_proc, int sem_id, int mem_id) {
    for (int i = 0; i < sons_cnt; ++i) {
        kill(pids[i], SIGKILL);
    }


    for (int i = 0; i < sons_cnt; ++i) {
        wait_child(WAIT_ANY);
    }

    semctl(sem_id, 0, IPC_RMID, 0);
    shmctl(mem_id, IPC_RMID, NULL);

    free(pids);
}

void print_array(int *arr, int n) {
    for (int i = 0; i < n; ++i) {
        if (i != n - 1) {
            printf("%d ", arr[i]);
        } else
            printf("%d\n", arr[i]);
    }
}

int main(int argc, char **argv) {
    int count, n_proc, iter_count;
    key_t key;

    sscanf(argv[COUNT_POS], "%d", &count);
    sscanf(argv[KEY_POS], "%d", &key);
    sscanf(argv[N_PROC_POS], "%d", &n_proc);
    sscanf(argv[ITER_COUNT_POS], "%d", &iter_count);

    int sem_id = semget(key, count, IPC_CREAT | 0666);

    if (sem_id < 0) {
        exit(1);
    }

    for (int i = 0; i < count; ++i) {
        struct sembuf op = {.sem_num = i, .sem_op = 1, .sem_flg = SEM_UNDO};

        if (semop(sem_id, &op, 1) < 0) {
            semctl(sem_id, n_proc, IPC_RMID, 0);
            exit(1);
        }
    }

    int mem_id = shmget(key, count * sizeof(mem_id), IPC_CREAT | 0666);

    if (mem_id < 0) {
        semctl(sem_id, n_proc, IPC_RMID, 0);
        exit(1);
    }

    int *mem = shmat(mem_id, NULL, 0);

    if (mem < 0) {
        semctl(sem_id, n_proc, IPC_RMID, 0);
        shmctl(mem_id, IPC_RMID, NULL);
        exit(1);
    }

    pid_t *pids = calloc(n_proc, sizeof(*pids));

    if (pids == NULL) {
        semctl(sem_id, n_proc, IPC_RMID, 0);
        shmctl(mem_id, IPC_RMID, NULL);
        exit(1);
    }

    memset(pids, 0, n_proc * sizeof(*pids));

    for (int i = 0; i < count; ++i) {
        scanf("%d", &mem[i]);
    }

    for (int i = 0; i < n_proc; ++i) {
        pids[i] = fork();

        if (pids < 0) {
            resource_destruction(pids, i, n_proc, sem_id, mem_id);
            exit(1);
        } else if (!pids[i]) {
            unsigned int seed;
            sscanf(argv[i + FIRST_SEED_POS], "%u", &seed);
            srand(seed);

            for (int j = 0; j < iter_count; ++j) {
                int ind1 = rand() / (RAND_MAX + 1.) * count;
                int ind2 = rand() / (RAND_MAX + 1.) * count;
                int val = rand() / (RAND_MAX + 1.) * MAX_VAL;

                if (ind1 != ind2) {
                    struct sembuf double_down[] = {
                            { .sem_num = ind1, .sem_op = -1, .sem_flg = SEM_UNDO },
                            { .sem_num = ind2, .sem_op = -1, .sem_flg = SEM_UNDO }
                    };

                    struct sembuf double_up[] = {
                            { .sem_num = ind1, .sem_op = 1, .sem_flg = SEM_UNDO },
                            { .sem_num = ind2, .sem_op = 1, .sem_flg = SEM_UNDO }
                    };

                    if (semop(sem_id, double_down, sizeof(double_down) / sizeof(*double_down)) < 0) {
                        exit(1);
                    }

                    operation(mem, ind1, ind2, val);

                    if (semop(sem_id, double_up, sizeof(double_up) / sizeof(*double_up)) < 0) {
                        exit(1);
                    }


                }
            }

            _exit(0);
        }
    }

    for (int i = 0; i < n_proc; ++i) {
        wait_child(WAIT_ANY);
    }

    print_array(mem, count);

    resource_destruction(pids, 0, n_proc, sem_id, mem_id);

    return 0;
}
