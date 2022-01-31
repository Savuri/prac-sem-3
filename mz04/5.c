/*
Задача up04-5: mz04-5 (дореш)
В аргументах командной строки задаются три параметра:

Путь к файлу path. Файл является файлом произвольного доступа.
Начальное значение h0. Это значение типа unsigned long long, задаваемое в шестнадцатеричном виде.
Количество значений для добавления count (count >= 0).
Во внешнем файле function.c дана функция

unsigned long long hash_function(unsigned long long);
Эта функция позволяет получить следующее значение последовательности h из предыдущего.

В конец файла path допишите count элементов последовательности h в обратном порядке, то есть hcount-1, hcount-2, ..., h0. Элементы последовательности должны записываться в бинарном виде в порядке байт, совпадающем с порядком байт в памяти выполняющейся программы (host byte order). Элементы последовательности не разрешается накапливать в памяти. В случае создания файл должен получать права на чтение и запись для владельца и группы. Другие права должны отсутствовать.

Для работы с выходным файлом используйте POSIX API. Для записи одного числа используйте один системный вызов write.

На 32-битных системах компилируйте вашу программу с опцией -D_FILE_OFFSET_BITS=64.

Например, если программа запущена следующим образом:

rm out.bin; ./solution out.bin 11223344 4
То должен быть создан файл out.bin, и команда hexdump -C out.bin выдаст следующий результат:

00000000  6a 86 b3 95 19 0d 1f c6  09 a7 0d 77 3c ab 64 dc  |j..........w<.d.|
00000010  8b 7d df 2a c6 26 14 6b  44 33 22 11 00 00 00 00  |.}.*.&.kD3".....|
00000020

*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

unsigned long long hash_function(unsigned long long);

int main(int argc, char **argv)
{
    if (argc < 4) {
        return 1;
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT, 0660);

    if (fd == -1) {
        return 1;
    }
    unsigned long long int hi;
    sscanf(argv[2],"%llX", &hi);

    unsigned long long int cnt;
    sscanf(argv[3], "%llu", &cnt);

    if (cnt == 0) {
        close(fd);
        return 0;
    }

    if (lseek(fd, (long ) ((cnt - 1) * sizeof(hi)), SEEK_END) < 0) {
        close(fd);
        return 1;
    }

    if (write(fd, &hi, sizeof(hi)) != sizeof(hi)) {
        close(fd);
        return 1;
    }

    while (--cnt) {
        if (lseek(fd, -2 * ((long ) sizeof(hi)), SEEK_CUR) < 0) {
            close(fd);
            return 1;
        }

        hi = hash_function(hi);

        if (write(fd, &hi, sizeof(hi)) != sizeof(hi)) {
            close(fd);
            return 1;
        }
    }

    close(fd);
    return 0;
}
