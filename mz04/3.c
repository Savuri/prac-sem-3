/*
Задача up04-3: mz04-3 (дореш)
В аргументах командной строки задается имя входного бинарного файла произвольного доступа. Бинарный файл рассматривается как массив структур:


struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};
Структура описывает вершину бинарного дерева поиска, где key – ключ поиска, а значения left_idx и right_idx – индексы в массиве соответственно левой и правой вершины. Корень дерева находится в элементе массива с индексом 0. Признаком отсутствия соответствующего поддерева в вершине является индекс, равный 0.

Числа в файле хранятся в естественном для x86 представлении (little-endian).

На стандартный поток вывода напечатайте ключи, размещенные в данном дереве, в порядке убывания.

Считывать содержимое всего файла в память целиком одним блоком запрещается. Для чтения содержимого файла необходимо пользоваться низкоуровневым вводом-выводом (open/read/etc). Корректная работа на big-endian архитектуре не требуется. Не используйте низкоуровневые манипуляции с памятью (union, приведение типов указателей). Не используйте библиотечные функции преобразования.

Для вывода ключей можно использовать высокоуровневые потоки (stdout).

Не забывайте выводить '\n' в конце вывода.

В программе должна быть реализована проверка корректности чтения из файла. Минимизируйте число системных вызовов, необходимых для чтения одной записи из файла.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void print_sub_tree(int fd, int32_t cur_idx) {
    if (cur_idx == 0) {
        return;
    }

    struct Node node;

    if (lseek(fd, cur_idx * sizeof(node), SEEK_SET) < 0) {
        close(fd);
        exit(1);
    }

    if (read(fd, &node, sizeof(node)) != sizeof(node)) {
        close(fd);
        exit(1);
    }

    print_sub_tree(fd, node.right_idx);
    printf("%d\n", node.key);
    print_sub_tree(fd, node.left_idx);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        return 1;
    }

    struct Node root;

    if (read(fd, &root, sizeof(root)) != sizeof(root)) {
        close(fd);
        return 1;
    }

    print_sub_tree(fd, root.right_idx);
    printf("%d\n", root.key);
    print_sub_tree(fd, root.left_idx);

    close(fd);

    return 0;
}
