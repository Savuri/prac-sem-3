/*
Задача ht01-5: ДЗ 01-5
В аргументах командной строки передается путь к каталогу. Этот каталог является корнем поддерева файловой системы.

Обойдите рекурсивно все подкаталоги и на стандартный поток вывода напечатайте команды 'cd', с помощью которых можно обойти все подкаталоги и вернуться в исходный каталог. Каждая команда cd может переходить на один каталог вверх/вниз по иерархии.

В одном каталоге подкаталоги должны просматриваться в лексикографическом порядке без учета регистра букв. Сравнение без учета регистра букв выполняется функцией strcasecmp. Каталог, который нельзя открыть на чтение с помощью opendir, должен быть пропущен, то есть в него не нужно входить и из него не нужно выходить.

Никакого экранирования спецсимволов не требуется. Гарантируется, что в одном каталоге не присутствуют каталоги, отличающиеся только регистром букв. Обрабатывать пути длиннее чем PATH_MAX-1 не требуется.

Не используйте scandir, ftw и аналогичные функции. Не используйте chdir, fchdir, telldir, seekdir, а также getpwd и аналогичные. Можно использовать opendir, readdir, closedir и *stat.

Например.

cd a
cd b
cd c
cd ..
cd d
cd ..
cd ..
cd ..
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


struct Dirs
{
    char **dirs_names;
    int max_dirs_cnt;
    int cur_dirs_cnt;
};

enum
{
    STANDART_DIRS_CNT = 32,
    REALLOC_COEF = 3
};

struct Dirs *create_dirs_arr(void) {
    struct Dirs *d = malloc(sizeof(struct Dirs));

    d->max_dirs_cnt = STANDART_DIRS_CNT;
    d->cur_dirs_cnt = 0;
    d->dirs_names = calloc(d->max_dirs_cnt, sizeof(*(d->dirs_names)));

    return d;
}

int cmp(const void **s1, const void **s2) {
    return strcasecmp(*s1, *s2);
}

void recursive_directory_traversal(DIR *cur_dir, char path[]) {
    struct dirent *dd;
    char full_file_path[PATH_MAX];

    struct Dirs *dirs_arr = create_dirs_arr();

    while ((dd = readdir(cur_dir))) {
        if (!strcmp(dd->d_name, "..") || !strcmp(dd->d_name, ".")) {
            continue;
        }

        unsigned int len = snprintf(full_file_path, sizeof(full_file_path), "%s/%s", path, dd->d_name);

        struct stat s;
        if (!access(full_file_path, R_OK) && !lstat(full_file_path, &s) && S_ISDIR(s.st_mode) &&
                len < sizeof(full_file_path)) {
            if (dirs_arr->cur_dirs_cnt == dirs_arr->max_dirs_cnt) {
                dirs_arr->max_dirs_cnt *= REALLOC_COEF;
                dirs_arr->dirs_names = realloc(dirs_arr->dirs_names, dirs_arr->max_dirs_cnt *
                        sizeof(*(dirs_arr->dirs_names)));
            }

            dirs_arr->dirs_names[dirs_arr->cur_dirs_cnt++] = strdup(dd->d_name);
        }
    }
    closedir(cur_dir);

    qsort(dirs_arr->dirs_names, dirs_arr->cur_dirs_cnt, sizeof(*(dirs_arr->dirs_names)),
            (int(*)(const void *, const void *))cmp);

    for (int i = 0; i < dirs_arr->cur_dirs_cnt; ++i) {
        snprintf(full_file_path, sizeof(full_file_path), "%s/%s", path, dirs_arr->dirs_names[i]);

        DIR *child_dir = opendir(full_file_path);

        if (child_dir != NULL) {
            printf("cd %s\n", dirs_arr->dirs_names[i]);
            recursive_directory_traversal(child_dir, full_file_path);
            printf("cd ..\n");
        }

        free(dirs_arr->dirs_names[i]);
    }

    free(dirs_arr->dirs_names);
    free(dirs_arr);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        return 1;
    }

    if (strlen(argv[1]) > PATH_MAX - 1 || strlen(argv[1]) == 0) {
        return 0;
    }

    DIR *d = opendir(argv[1]);

    if (d != NULL) {
        recursive_directory_traversal(d, argv[1]);
    }
    return 0;
}
