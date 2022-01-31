/*
Задача up03-5: mz03-5 (дореш)
На стандартный поток ввода программе подается текстовый файл, который мог быть подготовлен как в Unix, так и в Windows. Первая строка текста этого файла содержит имя (путь) другого текстового файла, который требуется обработать. Последующие строки исходного текстового файла, если они присутствуют, должны игнорироваться.

Если файл существует и может быть прочитан, подсчитайте число вхождений каждого из символов десятичных цифр. Не существующий или нечитаемый файл полагайте пустым. Если стандартный поток ввода пуст, полагайте и обрабатываемый файл пустым.

Результат выведите в виде таблички:

<символ десятичной цифры> <число вхождений в файл этой цифры>
Не используйте глобальные переменные. От вашей программы требуется максимальная эффективность при обработке входных данных.

Пример ввода на стандартном потоке ввода:

input.txt
Это значит, что требуется обработать файл input.txt.

Пример вывода:

0 1
1 12312
2 3
3 0
4 0
5 0
6 0
7 0
8 0
9 0
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>


enum
{
    BUF_SIZE = 8192,
    COUNT_OF_DECIMAL_DIGITS = 10
};

void format_print(unsigned long long *arr)
{
    for (int i = 0; i < COUNT_OF_DECIMAL_DIGITS; ++i) {
        printf("%d %llu\n", i, arr[i]);
    }
}

int main(int argc, char **argv)
{
    unsigned long long arr[COUNT_OF_DECIMAL_DIGITS];

    for (int i = 0; i < COUNT_OF_DECIMAL_DIGITS; i++) {
        arr[i] = 0;
    }

    char file_name[PATH_MAX + 1];

    if (!fgets(file_name, sizeof(file_name), stdin)) {
        format_print(arr);

        return 0;
    }

    int len = strlen(file_name);

    for (int i = len - 1; i >= 0; --i) {
        if (file_name[i] == '\n') {
            if (i > 0 && file_name[i - 1] == '\r') {
                file_name[i - 1] = '\0';
            } else {
                file_name[i] = '\0';
            }
        }
    }

    int f = open(file_name, O_RDONLY | O_CREAT, 0666);

    if (f == -1) {
        format_print(arr);

        return 0;
    }

    int n;
    char buf[BUF_SIZE];

    while ((n = read(f, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < n; ++i) {
            if (isdigit(buf[i])) {
                arr[buf[i] - '0']++;
            }
        }
    }

    if (n == -1) {
        return 1;
    }


    for (int i = 0; i < COUNT_OF_DECIMAL_DIGITS; ++i) {
        printf("%d %llu\n", i, arr[i]);
    }

    if (close(f) == -1) {
        return 1;
    }

    return 0;
}
