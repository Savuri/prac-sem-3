/*
Задача up04-2: mz04-2 (дореш)
Моргенштерн захотел уточнить свои траты за последний месяц. Особенно его интересуют не траты на самую дорогую покупку - Кадиллак, а на вторую по дороговизне покупку. Для этого Моргенштерн просматривает все чеки и находит в них вторую по дороговизне покупку в совокупности всех чеков. Несколько покупок по одной цене считаются как одна покупка.

Например, если Моргенштерн сделал 5 покупок, заплатив 100, 80, 100, 90, 100, то вторая по дороговизне покупка - 90.

В аргументах командной строки передаются имена входных файлов, содержащих информацию о чеках. Информация о каждом чеке представляет собой бинарный файл, состоящий из записей. Первые 16 байт каждой записи — это наименование товара (строка), а последующие 4 байта — целое число — уплаченная сумма в копейках. В одном чеке может быть произвольное количество записей, чек может быть пустым. Уплаченная сумма записывается как знаковое 32-битное число в формате Little-Endian. Уплаченная сумма может быть как положительной, так и отрицательной.

На стандартный поток вывода напечатайте искомую сумму в традиционном формате: рубли отделены от копеек символом “точка”, копейки записываются всегда двумя цифрами. Если по какой-либо причине второй по дороговизне покупки нет, ничего не выводите.

Можете предполагать, что компьютер, на котором выполняется программа, хранит целые числа в формате LE.

Для работы с файлами используйте POSIX API (open, read, write, etc). Для вывода результата можете использовать высокоуровневый вывод.
*/

#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>

enum
{
    BUF_SIZE = 16
};

int main(int argc, char **argv)
{
    int max1 = INT_MIN, max2 = INT_MIN;
    int max1_is_exists = 0, max2_is_exists = 0;

    int num;
    char buf[BUF_SIZE];

    for (int i = 1; i < argc; ++i) {
        int f = open(argv[i], O_RDONLY);

        if (f == -1) {
            continue;
        }

        int n1, n2;

        while ((n1 = read(f, &buf, sizeof(buf))) > 0 && (n2 = read(f, &num, sizeof(num))) > 0) {
            if (num >= max1) {
                if (num > max1 && max1_is_exists) {
                    max2 = max1;
                    max2_is_exists = 1;
                }

                max1_is_exists = 1;
                max1 = num;
            } else if (num >= max2) {
                max2 = num;
                max2_is_exists = 1;
            }
        }

        if (n1 == -1 || n2 == -1) {
            close(f);
            continue;
        }

        close(f);
    }

    if (max2_is_exists) {
        int ruble, kopek;

        if (max2 >= 0) {
            ruble = max2 / 100;
            kopek = max2 % 100;
            printf("%d.%02d\n", ruble, kopek);
        } else {
            ruble = -(max2 / 100);
            kopek = -(max2 % 100);
            printf("-%d.%02d\n", ruble, kopek);
        }
    }

    return 0;
}

/*
int main() {
    int n;
    scanf("%d", &n);

    int num;

    int max1 = INT_MIN, max2 = INT_MIN;
    int max2_is_exists = 0;
    int max1_was_updated = 0;

    for (int i = 0; i < n; ++i) {
        scanf("%d", &num);

        if (num >= max1) {
            if (num > max1 && max1_was_updated) {
                max2 = max1;
                max2_is_exists = 1;
            }

            max1_was_updated = 1;
            max1 = num;
        } else if (num >= max2) {
            max2 = num;
            max2_is_exists = 1;
        }
    }

    if (max2_is_exists) {
        printf("%d %d\n", max2, max1);
    }

    return 0;
}
*/
