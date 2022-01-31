/*
Задача up05-2: mz05-2 (дореш)
Эту задачу также предложил наш телезритель Андрей из города Москвы.

...
Это задача-исследование, посвященная, в некоторой степени, благородству.

В благородных аргументах командной строки передаются 9-битные восьмеричные числа, соответствующие благородным правам доступа к файлам в UNIX. Каждый аргумент командной строки содержит только одно число. Незначащие нули у восьмеричного числа в некоторой степени могут отсутствовать, а могут и присутствовать. На стандартный поток вывода напечатайте права доступа в rwx-записи. Каждую строку прав доступа выводите на отдельной строке текста.

Для получения строки-результата используйте специально посвященную этому константную строку-шаблон rwxrwxrwx.
*/

#include <stdio.h>

static const char rwx[] = "rwxrwxrwx";

enum
{
    PATTERN_PERMISSION_SIZE = sizeof(rwx) - 1
};

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        char permission_in_rwe_format[PATTERN_PERMISSION_SIZE + 1] = "---------\0";
        int permission;
        sscanf(argv[i], "%o", &permission);

        for (int j = PATTERN_PERMISSION_SIZE - 1; j >= 0; --j) {
            if (permission & 1) {
                permission_in_rwe_format[j] = rwx[j];
            }

            permission = permission >> 1;
        }

        printf("%s\n", permission_in_rwe_format);
    }

    return 0;
}
