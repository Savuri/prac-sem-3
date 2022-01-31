/*
Задача ht01-1: ДЗ 01-1
Рассмотрим модельный 16-битный процессор с 16-битным виртуальным и физическим адресным пространством, адресуемым побайтно, в котором реализована одноуровневая виртуальная страничная адресация с размером страницы 512 байт. Процессор хранит целые числа в формате Little-Endian.

Первым аргументом командной строки передается бинарный файл размером 64KiB - содержимое физической памяти. Вторым аргументом командной строки передается физический адрес начала таблицы страниц в шестнадцатеричном виде.

Таблица страниц размещается в физической памяти, начиная с указанного адреса. Каждая запись таблицы страниц имеет размер 16 бит, в которой старшие биты хранят номер физической страницы, а младшие биты равны 0.

На стандартном потоке ввода задаются выровненные виртуальные адреса 16-битных ячеек памяти в шестнадцатеричной записи. На стандартный поток вывода напечатайте значения, хранящиеся в ячейках. Значения выводите в беззнаковом десятичном виде.

Для работы с файлом с содержимым физической памяти используйте mmap.
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

enum
{
    TABLE = 2,
    KB = 64,
    KB_SHIFT = 10,
};

enum
{
    NUMBER_SHIFT = 9
};

unsigned short get_virt_num(unsigned short virtual_address) {
    return virtual_address >> NUMBER_SHIFT;
}

unsigned short get_offset(unsigned short virtual_address) {
    return virtual_address - (get_virt_num(virtual_address) << NUMBER_SHIFT);
}

unsigned short get_phys_num(unsigned short virtual_address, unsigned short *page) {
    return (page[get_virt_num(virtual_address)] >> NUMBER_SHIFT) << NUMBER_SHIFT;
}

unsigned short get_number_by_virt_address(unsigned short *page, unsigned short table, unsigned short virtual_address) {
    page += table / sizeof(table);

    unsigned int offset = get_offset(virtual_address);
    unsigned int phys_num = get_phys_num(virtual_address, page);

    page -= table / sizeof(table);

    return *(page + (phys_num + offset) / sizeof(table));
}

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY | O_CREAT, 0666);
    if (fd < 0) {
        return 1;
    }

    unsigned short table;
    sscanf(argv[TABLE], "%hx", &table);

    unsigned short *page = mmap(NULL, KB << KB_SHIFT, PROT_READ, MAP_PRIVATE, fd, 0);
    unsigned short virtual_address;
    while ((scanf("%hx", &virtual_address)) != EOF) {
        printf("%hu\n", get_number_by_virt_address(page, table, virtual_address));
    }

    munmap(page, KB << KB_SHIFT);
    close(fd);
    exit(0);
}
