/*
Задача up05-4: mz05-4 (дореш)
Эту задачу снова предложил наш телезритель Андрей из города Москвы.

...
Напишите функцию

int
parse_rwx_permissions(const char *str);
На вход функции подается строка — права доступа к файлу в формате rwx (9 знаков). Указатель может принимать значение NULL.

Функция должна вернуть соответствующее значение прав доступа, либо -1, если строка не является корректной записью прав доступа. Корректная запись состоит только из символов '-', 'r', 'w', 'x', расположенных в правильном порядке.

Указание: используйте строку-образец "rwxrwxrwx" для разбора входной строки. В функции не должны использоваться константы 3, 8, 9, 10 (даже в виде enum или #define). Используйте битовые операции.

Поскольку строка-образец является константной, ее длину можно вычислить за константное время на этапе компиляции. В функции может использоваться не более одного вызова strlen.
*/
#include <string.h>

static const char rwx[] = "rwxrwxrwx";

int
parse_rwx_permissions(const char *str)
{
    if (str == NULL || strlen(str) != sizeof(rwx) - 1) {
        return -1;
    }

    int permission_number = 0;

    for (int i = 0; i < sizeof(rwx) - 1; i++) {
        permission_number = permission_number << 1;

        if (str[i] == rwx[i]) {
            permission_number++;
        } else if (str[i] != '-') {
            return -1;
        }
    }

    return permission_number;
}