/*
Задача up11-3: par-and-seq (дореш)
В аргументах командной строки процессу передается число N и имена текстовых файлов. Каждый текстовый файл должен содержать одну строку —имя исполняемого файла. Процесс запускает на параллельное исполнение не более чем первые N исполняемых файлов и после окончания параллельного исполнения на последовательное исполнение —оставшиеся исполняемые файлы. По окончанию работы процесс-родитель выводит на экран число — количество потомков, которые были успешно запущены и завершили свою работу с пользовательским кодом 0.

Единственная строка каждого текстового файла содержит символ '\n' в конце. В строке отсутствуют пробельные символы, кроме финального '\n'.
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
    BUF_SIZE = 10000,
    SHIFT_TO_FILES_IN_ARGV = 2
};

// 1 if all ok
int wait_child(int pid) {
    int status;

    waitpid(pid, &status, 0);

    return WIFEXITED(status) && !WEXITSTATUS(status);
}

int make_fork(void ) {
    int pid = fork();

    return pid;
}

void read_and_execute(const char file_name[]) {
    char exec_this[BUF_SIZE];

    FILE *f = fopen(file_name, "r");

    if (f == NULL) {
        _exit(1);
    }

    fscanf(f, "%s", exec_this);

    fclose(f);

    size_t len = strlen(exec_this);
    if (len == 0) {
        exit(1);
    }

    execlp(exec_this, exec_this, NULL);

    _exit(1);
}

int main(int argc, char const *argv[])
{
    int n;
    sscanf(argv[1], "%d", &n);

    int last_parallel;

    if (n < argc - SHIFT_TO_FILES_IN_ARGV) {
        last_parallel = n + 1;
    } else {
        last_parallel = argc - 1;
    }

    for (int i = SHIFT_TO_FILES_IN_ARGV; i <= last_parallel; ++i) {
        int pid = make_fork();

        if (!pid) {
            read_and_execute(argv[i]);
        }
    }

    unsigned int count_of_successes = 0;

    for (int i = SHIFT_TO_FILES_IN_ARGV; i <= last_parallel; ++i) {
        count_of_successes += wait_child(WAIT_ANY);
    }

    for (int i = last_parallel + 1; i < argc; i++) {
        int pid = make_fork();

        if (!pid) {
            read_and_execute(argv[i]);
        }

        count_of_successes += wait_child(WAIT_ANY);
    }

    printf("%u\n", count_of_successes);
    fflush(stdout);

    return 0;
}
