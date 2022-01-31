/*
Задача up05-5: mz05-5 (дореш)
Это последняя задача, предложенная на конкурс  Андреем из города Москвы.

...
В аргументе командной строки задается имя файла. Файл содержит 16-битные беззнаковые целые числа в представлении Big Endian в бинарном виде.

На стандартный поток вывода выведите минимальное число среди четных чисел в этом файле. Вывод завершайте символом перехода на новую строку. Если в файле отсутствуют четные числа, программа не должна выводить ничего.

Размер файла всегда кратен 2.

Для работы с файлом используйте интерфейс системных вызовов.

Если входной файл содержит:

1 2 3 4 5 6 7 8
На стандартный поток вывода должно быть напечатано:

2
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        return 1;
    }

    unsigned short min = USHRT_MAX;
    unsigned short num;
    unsigned char buf[sizeof(unsigned short )];
    
    int n;
    while ((n = read(fd, &buf, sizeof(buf))) > 0) {
        num = buf[0] << CHAR_BIT;
        num += buf[1];

        if (num < min && num % 2 == 0) {
            min = num;
        }
    }

    if (n == -1) {
        close(fd);
        return 1;
    }

    if (min != USHRT_MAX) {
        printf("%hu\n", min);
    }

    close(fd);
    return 0;
}
