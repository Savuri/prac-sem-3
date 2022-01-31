/*
Задача up04-4: mz04-4 (дореш)
В аргументах командной строки задается путь к бинарному файлу, содержащему целые знаковые числа типа long long, записанные в порядке байт хоста (компьютера, на котором исполняется программа). В файле найдите минимальное число и поменяйте его знак. Если минимальное число в файле встречается несколько раз, поменяйте знак только у первого по порядку минимального числа. При вычислениях предполагайте неограниченную битность, но в качестве результата оставьте младшие 64 бита.

Для работы с файлом используйте POSIX API. Допускается только один проход по файлу, но бинарный файл является файлом произвольного доступа.

На 32-битных системах компилируйте вашу программу с опцией -D_FILE_OFFSET_BITS=64. Не допускайте в программе undefined behavior.

Например, если дан входной бинарный файл input.bin, результатом обработки его должен быть файл распечатка содержимого которого показана ниже.

hexdump -C input.bin
00000000  03 00 00 00 00 00 00 00  fe ff ff ff ff ff ff ff  |................|
00000010  04 00 00 00 00 00 00 00                           |........|
00000018
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>


int main(int argc, char **argv)
{
    if (argc < 2) {
        return 1;
    }

    int fd = open(argv[1], O_RDWR);

    if (fd == -1) {
        return 1;
    }

    long long int min = LLONG_MAX;
    long long int pos_min = -1;
    int min_is_set = 0;

    long long int buf;
    int n;

    for (int i = 0; (n = read(fd, &buf, sizeof(buf))) > 0; i++) {
        if (buf < min || !min_is_set) {
            min = buf;
            pos_min = i;

            min_is_set = 1;
        }
    }

    if (!min_is_set) {
        close(fd);
        return 0;
    }

    if (n == -1) {
        close(fd);
        return 1;
    }

    if (min != LLONG_MIN) {
        min = -min;
    }

    if (lseek(fd, pos_min * sizeof(min), SEEK_SET) < 0) {
        close(fd);
        return 1;
    }

    if (write(fd, &min, sizeof(min)) < 0) {
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
