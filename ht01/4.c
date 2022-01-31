/*
Задача ht01-4: ДЗ 01-4
Программе в аргументах командной строки передаются следующие параметры:

Имя динамической библиотеки для загрузки
Имя функции в этой библиотеке
Сигнатура функции
Далее идут аргументы функции (могут отсутствовать)
Ваша программа должна вызвать указанную функцию, передать ей указанные параметры в соответствии с сигнатурой и вывести на стандартный поток вывода результат, возвращенный этой функцией.

Сигнатура функции — это строка, задающая тип возвращаемого значения и типы аргументов. Первый символ строки задает тип возвращаемого значения: v — void, i — int, d — double, s — char * (строка). Последующие символы строки задают типы параметров: i — int, d — double, s — char * (строка).

Например, сигнатура vssid соответствует функции вида:

void func(char *, char *, int, double);
Общий размер параметров функции не превышает 64 байта.

Если функция возвращает значение типа double, то он должен быть напечатан с помощью форматного преобразования %.10g. Если функция возвращает void, ничего выводить не нужно. Не забывайте выводить \n в конце (кроме случая void).

Пример аргументов командной строки:

./solution libc.so.6 printf vssid 'Hello: %s, %d, %f' abc 10 12.4
Результат работы:

Hello: abc, 10, 12.400000
Ваше решение будет тестироваться на платформе Linux-i386. Кросс-платформенность не требуется.
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <math.h>
#include <dlfcn.h>


struct Params
{
    char arr[64];
};

enum
{
    FIRST_ARG_OF_DYN_FUN = 4,
    SIGNATURE = 3,
    LIBRARY_NAME = 2
};

enum
{
    VOID = 'v',
    INT = 'i',
    DOUBLE = 'd'
};

void load_params(struct Params *params, char *argv[]) {
    char *p = argv[SIGNATURE];
    p++;

    int cur_byte = 0;

    int cur_arg = FIRST_ARG_OF_DYN_FUN;

    while (*p != '\0') {
        if (*p == 'd') {
            double dNumber;
            sscanf(argv[cur_arg++], "%lf", &dNumber);
            double *pDouble = (double *) &(params->arr[cur_byte]);
            *pDouble = dNumber;
            cur_byte += sizeof(double );
        } else if (*p == 's') {
            int sAddress = (int ) argv[cur_arg++];
            int *pAdress = (int *) &(params->arr[cur_byte]);
            *pAdress = sAddress;
            cur_byte += sizeof(char *);
        } else {
            int intNumber;
            sscanf(argv[cur_arg++], "%d", &intNumber);

            int *pInt = (int *) &(params->arr[cur_byte]);
            *pInt = intNumber;

            cur_byte += sizeof(int );
        }

        p++;
    }
}
int main(int argc, char *argv[]) {
    void *handle = dlopen(argv[1], RTLD_LAZY);

    if (handle == NULL) {
        return 1;
    }


    void *fun = dlsym(handle, argv[LIBRARY_NAME]);

    struct Params fun_params = { 0 };
    load_params(&fun_params, argv);

    char return_type = *(argv[SIGNATURE]);

    if (return_type == VOID) {
        ((void (*)(struct Params)) fun)(fun_params);
    } else if (return_type == INT) {
        printf("%d\n", ((int (*)(struct Params)) fun)(fun_params));
    } else if (return_type == DOUBLE) {
        printf("%.10g\n", ((double (*)(struct Params)) fun)(fun_params));
    } else {
        char *s = ((char *(*)(struct Params)) fun)(fun_params);
        printf("%s\n", s);
        free(s);
    }

    dlclose(handle);

    return 0;
}
