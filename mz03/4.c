/*
Задача up03-4: mz03-4 (дореш)
На стандартном потоке ввода задается последовательность знаковых целых чисел. Целые числа записываются в десятичной форме со знаком + или - перед числом и разделяются одним или несколькими пробельными символами (то есть символами, для которых isspace возвращает истинное значение). Знак при положительном числе может опускаться.

На стандартный вывод напечатайте младшие 64 бита суммы этих чисел в виде знакового целого числа в десятичной записи.

Для чтения со стандартного ввода используйте системный вызов read с размером буфера для чтения 16 байт!

Не накапливайте в памяти считываемые символы.

Не забывайте выводить символ перехода на новую строку в конце вывода!

Глобальные (и static) переменные запрещены.
*/

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

enum { BUF_SIZE = 16 };

int main(int argc, char **argv) {
    unsigned long long sum = 0;

    unsigned char buf[BUF_SIZE];

    size_t n;

    unsigned long long num = 0;
    char sgn_of_num = '+';

    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < n; ++i) {
            if (isspace(buf[i])) {
                if (sgn_of_num == '+') {
                    sum += num;
                } else {
                    sum -= num;
                }

                num = 0;
                sgn_of_num = '+';
            } else if (buf[i] == '+' || buf[i] == '-') {
                sgn_of_num = buf[i];
            } else if (isdigit(buf[i])) {
                num = num * 10 + buf[i] - '0';
            } else {
                // non expected symbol
                continue;
            }
        }

    }

    if (n == -1) {
        return 1;
    }

    if (sgn_of_num == '+') {
        sum += num;
    } else {
        sum -= num;
    }

    printf("%lld\n", sum);

    return 0;
}
