/*
Задача up09-4: mz09-4 (дореш)
В аргументах командной строки передаются целые положительные числа произвольного размера. Число передаваемых аргументов больше или равно 1. На стандартный поток вывода напечатайте произведение этих чисел.

Для вычисления результата используйте скриптовый язык программирования, поддерживающий арифметику произвольной точности, например, python, python3.

На языке python3 умножение двух чисел выглядит следующим образом:

print(1*2)
Если записать эту программу в файл apb.py и запустить интерпретатор командой

python3 apb.py
То на стандартном потоке вывода будет получен результат:

2
Ваша программа должна создать скрипт, который не будет требовать никаких дополнительных входных данных и при запуске выведет на стандартный поток вывода произведение чисел. При запуске команды на выполнение из вашей программы не должно указываться явное имя интерпретатора, поэтому в скрипте используйте запись #!.

Не создавайте новый процесс. Ваша программа может запускаться в произвольном рабочем каталоге. Программа должна поддерживать возможность одновременной работы своих копий. Программа не должна допускать возможность изменения скрипта другими пользователями. Скрипт должен удаляться после выполнения.

Не используйте функции tmpnam, tmpnam_r, mktemp, mkstemp, tmpfile и аналогичные. В качестве пути к каталогу временных файлов используйте значение переменной окружения XDG_RUNTIME_DIR, если эта переменная не установлена, используйте значение переменной окружения TMPDIR, если и эта переменная не установлена, используйте путь /tmp.
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

static const char E_VAR_TMP_DIR1[] = "XDG_RUNTIME_DIR";
static const char E_VAR_TMP_DIR2[] = "TMPDIR";
static const char TMP_DIR[] = "/tmp";
static const char SCRIPT0[] = "#! /usr/bin/python3\nimport sys;\nimport os;\nprint(";
static const char SCRIPT1[] = ");\nos.remove(sys.argv[0])";

static const char PY[] = ".py";

void get_my_file_path(char full_file_name[]) {
    char *dir = getenv(E_VAR_TMP_DIR1);
    char file_name[sizeof(long long int) * CHAR_BIT + 1];

    {
        int rfd = open("/dev/urandom", O_RDONLY);

        if (rfd < 0) {
            fprintf(stderr, "Open error dev/urandom: %s", strerror(errno));
            _exit(1);
        }

        unsigned long long val;
        read(rfd, &val, sizeof(val));
        snprintf(file_name, sizeof(file_name), "%llu", val);
        close(rfd);
    }


    if (dir == NULL) {
        dir = getenv(E_VAR_TMP_DIR2);

        if (dir == NULL) {
            sprintf(full_file_name, "%s/%s%s", TMP_DIR, file_name, PY);
            return;
        }

        sprintf(full_file_name, "%s/%s%s", dir, file_name, PY);
        return;
    }

    sprintf(full_file_name, "%s/%s%s", dir, file_name, PY);
}

void write_script(int argc, char const *argv[], int fd) {
    if (write(fd, SCRIPT0, sizeof(SCRIPT0) - 1) < 0) {
        fprintf(stderr, "Write error: %s\n", strerror(errno));
        _exit(1);
    }

    if (write(fd, argv[1], strlen(argv[1])) < 0) {
        fprintf(stderr, "Write error: %s\n", strerror(errno));
        _exit(1);
    }

    char mul = '*';

    for (int i = 2; i < argc; ++i) {
        if (write(fd, &mul, sizeof(mul)) < 0 || write(fd, argv[i], strlen(argv[i])) < 0) {
            fprintf(stderr, "Write error: %s\n", strerror(errno));
            _exit(1);
        }
    }

    if (write(fd, SCRIPT1, sizeof(SCRIPT1) - 1) < 0) {
        fprintf(stderr, "Write error: %s\n", strerror(errno));
        _exit(1);
    }
}


int main(int argc, char const *argv[])
{
    if (argc < 2) {
        return 1;
    }


    char full_file_name[PATH_MAX];
    get_my_file_path(full_file_name);

    int fd = open(full_file_name, O_RDWR | O_CREAT | O_TRUNC, 0775);

    if (fd < 0) {
        fprintf(stderr, "Open error: %s", strerror(errno));
        return 1;
    }

    write_script(argc, argv, fd);

    if (close(fd) < 0) {
        fprintf(stderr, "Close error: %s", strerror(errno));
        return 1;
    }

    execl(full_file_name, full_file_name, NULL);
}
