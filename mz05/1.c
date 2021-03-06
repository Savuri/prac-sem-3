/*
Задача up05-1: mz05-1 (дореш)
Эту задачу предложил наш телезритель Андрей из города Москвы.

...
Это задача-рассуждение, обзор проблем образования и его распространенности.

Если рассудить, то программе в аргументах командной строки, безусловно, передаются пути к файлам.

Несмотря на все возникшие проблемы в образовании, на стандартный поток вывода напечатайте суммарный размер файлов, удовлетворяющих условиям, которые были, к глубочайшему удовлетворению, сформулированы ниже:

Вообще говоря, файл существует и его размер равен целому числу кибибайт (2^10).
Как бы это сказать, файл является регулярным, при этом не является символической ссылкой.
Распространено мнение, что файл имеет единственное имя в файловой системе.
*/
#include <stdio.h>
#include <sys/stat.h>

enum { MASK = 0x3FF };

int main(int argc, char **argv) {
    unsigned long long sizes_sum = 0;

    for (int i = 0; i < argc; ++i) {
        struct stat s;

        if (lstat(argv[i], &s) != -1 && S_ISREG(s.st_mode) && s.st_nlink == 1 && (s.st_size & MASK) == 0) {
            sizes_sum += s.st_size;
        }
    }


    printf("%llu\n", sizes_sum);

    return 0;
}
