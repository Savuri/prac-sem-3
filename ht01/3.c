/*
Задача ht01-3: ДЗ 01-3
Программе в аргументах командной строки задаются имя выходного файла, число строк (rows) и число столбцов (cols).

Выходной файл - это бинарный файл, в котором записана матрица (двухмерный массив) размера rows строк на cols столбцов 32-битных целых чисел. Матрица хранится по строкам (как в Си). Запишите в выходной файл числа от 1 до rows*cols по спирали как показано на примере ниже.

Параметры rows и cols больше нуля. Матрица полностью помещается в память.

Используйте mmap для формирования выходного файла.

Например, если rows == 4, cols == 5, то в выходной файл должна быть записана матрица:

 1  2  3  4  5
14 15 16 17  6
13 20 19 18  7
12 11 10  9  8
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

void printMatrix(int **a, int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            printf("%4d", a[i][j]);
        }

        putchar('\n');
    }
}

int **getMatrix(int n, int m) {
    int **a = calloc(n, sizeof(*a));

    for (int i = 0; i < n; ++i) {
        a[i] = calloc(m, sizeof(**a));
    }

    int cnt = 1;

    int left = 0, right = m;
    int top = 0, bottom = n;

    while (top < bottom && left < right) {
        for (int i = left; i < right; ++i) {
            a[top][i] = cnt++;
        }
        top++;

        for (int i = top; i < bottom; ++i) {
            a[i][right - 1] = cnt++;
        }
        right--;

        if (top < bottom) {
            for (int i = right - 1; i >= left; --i) {
                a[bottom - 1][i] = cnt++;
            }
            bottom--;
        }

        if (left < right) {
            for (int i = bottom - 1; i >= top; --i) {
                a[i][left] = cnt++;
            }
            left++;
        }
    }

    return a;
}

void free_matrix(int **a, int n) {
    for (int i = 0; i < n; ++i) {
        free(a[i]);
    }

    free(a);
}


void print_spirall(int **a, int n, int m, int *out) {
    size_t out_offset = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            out[out_offset++] = a[i][j];
        }
    }
}


int main(int argc, char **argv) {
    if (argc != 4) {
        return 1;
    }

    int n, m;


    sscanf(argv[2], "%d", &n);
    sscanf(argv[3], "%d", &m);

    if (n < 1 || m < 1) {
        return 1;
    }

    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);

    if (fd < 0) {
        return 1;
    }


    if (ftruncate(fd, n * m * sizeof(int)) < 0) {
        close(fd);
        return 1;
    }

    int *out_mem = mmap(NULL, n * m * sizeof(*out_mem), PROT_WRITE, MAP_SHARED, fd, 0);

    if (out_mem == MAP_FAILED) {
        printf("Map Error");
        return 1;
    }

    close(fd);

    int **a = getMatrix(n, m);
    print_spirall(a, n, m, out_mem);

    free_matrix(a, n);

    if (munmap(out_mem, n * m * sizeof(*out_mem)) < 0) {
        return 1;
    }

    return 0;
}
