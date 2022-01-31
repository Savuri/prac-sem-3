/*
В аргумента командной строки задаются 64-битные целые числа. Нужно для каждого числа подсчитать сумму цифр этого числа, представленнго в 4-ричной системе счисления и вывести на stdout. Высчитывать сумму цифр нужно для всех чисел параллельно. Вывод суммы чисел на stdout должен быть строго в том же порядке, в каком эти числа идут в аргументах.
	
	Пример (с сайта):
	====== Test #1 =======
	-— Command line arguments —-
	[ 1]: > -26 <
	[ 2]: > 12345 <
	-— Input: size 0 —-

	-— Correct: size 4 —-
	5
	9

	-— Stderr: size 0 —-
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

enum
{
    BASE = 4,
    KEY = 12345
};

pid_t make_fork(void) {
    pid_t pid = fork();

    if (pid == -1) {
        _exit(1);
    }

    return pid;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        return 1;
    }

    key_t key = KEY;
    int shmid = shmget(key, sizeof(shmid) * (argc - 1), 0666 | IPC_CREAT | IPC_EXCL);
    int *buf = (int*) shmat(shmid, NULL, 0);


    pid_t *pids = calloc(argc - 1, sizeof(pids));

    for (int i = 0; i < argc - 1; ++i) {
        pids[i] = make_fork();

        if (!pids[i]) {
            long long int num;

            sscanf(argv[i + 1], "%lld", &num);

            if (num < 0) {
                num *= -1;
            }

            int sum = 0;

            while (num > 0) {
                sum += (int) (num % BASE);
                num /= BASE;
            }

            buf[i] = sum;

            _exit(0);
        }
    }

    for (int i = 0; i < argc - 1; ++i) {
        wait(NULL);
    }

    for (int i = 0; i < argc - 1; ++i) {
        printf("%d\n", buf[i]);
    }

    shmdt(buf);
    shmctl(shmid, IPC_RMID, NULL);
    free(pids);

    return 0;
}

// 140/200 Учесть ещё как минимум LLONG_MIN
