/*
Задача up06-3: mz06-3 (дореш)
Студент Владимир из Италии баллотируется на выборах членов Могущественной тайной организации™ для работы в комитете по информационной политике.

image
В двух файлах в файловой системе находятся два пресс-релиза об успехах организации. Владимир хочет построить кратчайший относительный путь от каталога, в котором находится первый файл, ко второму файлу.

Напишите функцию:

char *relativize_path(const char *path1, const char *path2);
И путь path1, и путь path2 являются нормализованными, то есть в пути отсутствуют '/' подряд и последний символ не является '/', за исключением пути "/".

Результатом работы функции является новая строка, содержащая кратчайший по длине символов относительный путь от каталога, в котором содержится первый файл или каталог, ко второму файлу или каталогу.

Обрабатывайте пути как строки. Соответствующие файлы и каталоги не обязаны существовать в файловой системе.

Например, если аргументами функции являются строки /a/b/c/d и /a/e/f, результатом должна быть строка ../../e/f.

*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

char *my_real_path(const char *path) {
    char *result = calloc(PATH_MAX, sizeof(*result));

    char *p_result = result;
    const char *p = path;
    *p_result = '/';

    while (*p != '\0') {
        if (*p == '/') {
            if (strncmp(p, "/./", 3) == 0 || strncmp(p, "/.\0", 3) == 0) {
                p += 2;
            } else if (strncmp(p, "/../", 4) == 0 || strncmp(p, "/..\0", 4) == 0) {
                if (p_result != result) {
                    p_result--;
                }

                while (*p_result != '/' && p_result != result) {
                    p_result--;
                }

                p += 3;
            } else {
                *p_result++ = *p++;
            }
        } else {
            *p_result++ = *p++;
        }
    }

    if (p_result == result) {
        p_result++;
    }

    *p_result = '\0';

    return result;
}

char *cat_last(char *path) {
    char *p = path;
    char *tmp = path;

    while (*p != '\0') {
        if (*p == '/') {
            tmp = p;
        }

        p++;
    }

    if (tmp != path) {
        *tmp = '\0';
    } else {
        *(tmp + 1) = '\0';
    }

    return path;
}

char *relativize_path(const char *path1, const char *path2) {
    char *result = calloc(PATH_MAX, sizeof(*result));

    char *abs_path1 = cat_last(my_real_path(path1)), *abs_path2 = my_real_path(path2);

    char *p1 = abs_path1, *p2 = abs_path2;

    int p1_is_root = *(p1 + 1) == '\0';
    int p2_is_root = *(p2 + 1) == '\0';

    char *p_result_end = result;

    if (p1_is_root || p2_is_root) {
        if (p1_is_root && p2_is_root) {
            p_result_end += sprintf(result, ".");
        } else if (p1_is_root) {
            p_result_end += sprintf(result, "%s", abs_path2 + 1);
        } else {
            while (*p1 != '\0') {
                if (*p1 == '/') {
                    p_result_end += sprintf(p_result_end, "../");
                }

                p1++;
            }

            p_result_end--;
        }

        *p_result_end = '\0';

        free(abs_path1);
        free(abs_path2);

        return result;
    }

    while (*p1 == *p2 && *p1 != '\0') {
        p1++;
        p2++;
    }

    if (*p1 == *p2 && *p1 == '\0') {
        p_result_end += sprintf(result, "./");
    } else if (*p1 == '\0' && *p2 == '/') {
        p_result_end += sprintf(p_result_end, "%s/", p2 + 1);
    } else {
        if (!(*p1 == '/' && *p2 == '\0')) {
            while (*p1 != '/' || *p2 != '/') {
                p1--;
                p2--;
            }
        }

        // я на '/' конца их общих частей
        while (*p1 != '\0') {
            if (*p1 == '/') {
                p_result_end += sprintf(p_result_end, "../");
            }

            p1++;
        }

        if (p_result_end == result) {
            p_result_end += sprintf(p_result_end, "./");
        }

        p_result_end--;
        p_result_end += sprintf(p_result_end, "%s", p2);
        p_result_end++;
    }

    *--p_result_end = '\0';

    free(abs_path1);
    free(abs_path2);

    return result;
}
