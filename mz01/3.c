/*
Задача up01-3: mz01-3 (дореш)
Пусть дано некоторое простое число N (N > 1). Для любых двух чисел a и b (0 ≤ a, b < N) их произведение по модулю N c определяется как остаток от деления произведения a и b на N. Запишем это как c ≡ a * b (mod N). Пусть даны любые c (0 ≤ c < N) и a (0 < a < N). Тогда существует единственное b, такое что c ≡ a * b (mod N). Число b назовем результатом деления по модулю N числа c на число a. Запишем это как b ≡ c / a (mod N).

На стандартный поток вывода напечатайте таблицу деления для некоторого фиксированного простого числа N.

Формат входных данных
На стандартном потоке ввода задается единственное целое простое число N (N < 2000).

Формат результата
На стандартный поток вывода напечатайте таблицу деления, которая состоит из N строк и N - 1 столбцов, где строка соответствует делимому, а столбец - делителю. Столбцов выводится N - 1, так как столбец, соответствующий делителю 0 пропускается.

В примере ниже показана таблица для N = 5. В первой строке наблицы находятся результаты деления числа 0 на 1, 2, 3, 4 соответственно. В последней строке таблицы находятся результаты деления числа 4 на 1, 2, 3, 4 соответственно. В самом деле, 4 / 3 ≡ 3 (mod 5), так как 3 * 3 ≡ 4 (mod 5).

Примеры
Входные данные
5
Результат работы
0 0 0 0
1 3 2 4
2 1 4 3
3 4 1 2
4 2 3 1
*/

#include <stdio.h>
#include <stdlib.h>

enum { MAX_N = 2000 };

int main()
{
    int n;
    scanf("%d", &n);

    if (n <= 1 || n >= MAX_N) {
        return 1;
    }

    int **a = calloc(n, sizeof(*a));

    if (a == NULL) {
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        if ((a[i] = calloc(n - 1, sizeof(**a))) == NULL) {
            for (int j = 0; j < i; j++) {
                free(a[j]);
            }

            free(a);

            return 1;
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            a[i * j % n][j] = i;
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            printf("%d ", a[i][j]);
        }

        putchar('\n');
    }

    putchar('\n');

    for (int i = 0; i < n; ++i) {
        free(a[i]);
    }

    free(a);

    return 0;
}