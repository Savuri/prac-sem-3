/*
Напишите функцию:

int mysys(const char *str);
Функция должна работать аналогично функции system, то есть создавать
процесс, запускать на выполнение строку str и дожидаться результата.

Строка str может содержать несколько аргументов, разделенных
пробелами.

Функция mysys должна возвращать -1 в случае, если новый процесс не
может быть создан. Если выполнение str завершилось с кодом завершения
0-127, функция должна возвращать это значение. Если выполнение str
завершилось из-за сигнала K, то функция должна возвращать значение 128

K.
Для интерпретации строки str должен использовать стандартный шелл
/bin/sh. (sh -c CMD).

Сама функция mysys не должна ничего выводить ни на стандартный поток
ошибок, ни на стандартный поток вывода.

Функция mysys должна корректно работать независимо от использования
основным процессом других функций создания процессов и функций
ввода-вывода.
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

int mysys(const char *str) {
    int pid = fork();

    if (pid < 0) {
        return -1;
    } else if (!pid) {
        execl("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    }

    int stat;
    waitpid(pid, &stat, 0);

    if (WIFSIGNALED(stat)) {
        return 128 + WTERMSIG(stat);
    }
    if (WIFEXITED(stat) && WEXITSTATUS(stat) >= 0 && WEXITSTATUS(stat) <= 127) {
        return WEXITSTATUS(stat);
    }
    return -1;
}
