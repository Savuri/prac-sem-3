/*
Задача up06-2: mz06-2 (дореш)
Студент Николай из Германии баллотируется на выборах членов Могущественной тайной организации™ для работы в комитете по образованию.

image
Николай провел среди студентов опрос о качестве опросов. Респонденты прислали ему ответы, которые были сохранены в некоторый каталог в виде файлов с суффиксом .exe

В аргументах командной строки задается путь к каталогу.

На стандартный поток вывода напечатайте количество файлов в заданном каталоге (без подкаталогов), удовлетворяющих следующему условию:

Файл регулярный.
Файл доступен на выполнение текущему пользователю.
Его имя заканчивается на суффикс .exe.
Для получения идентификатора пользователя процесса используйте системный вызов getuid.

Символические ссылки прослеживайте до соответствующих файлов (используйте соответствующий системный вызов семейства *stat).

Не используйте системные вызовы, меняющие текущий каталог процесса.
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

static const char exe[] = ".exe";

enum
{
    LEN_OF_EXE_STRING = sizeof(exe) - 1
};

int main(int argc, char const *argv[])
{
    struct stat s;
    struct dirent *currfile;
    DIR *dir = opendir(argv[1]);

    if (!dir) {
        return 1;
    }

    char full_file_name[PATH_MAX];

    int cnt = 0;

    while ((currfile = readdir(dir)) > 0) {
        sprintf(full_file_name, "%s/%s", argv[1], currfile->d_name);

        if (stat(full_file_name, &s)) {
            return 1;
        }

        int len = strlen(currfile -> d_name);

        if (len >= LEN_OF_EXE_STRING && strcmp(&(currfile->d_name[len - LEN_OF_EXE_STRING]), exe) == 0
                && S_ISREG(s.st_mode) && !access(full_file_name, X_OK)) {
            cnt++;
        }
    }

    printf("%d\n", cnt);

    if (closedir(dir) == -1) {
        return 1;
    }

    return 0;
