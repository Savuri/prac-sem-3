/*
Задача up06-5: mz06-5 (дореш)
Студент Тимур из Франции баллотируется на выборах членов Могущественной тайной организации™ для работы в комитете по содействию предпринимательству.

image
Тимур хочет проверять, что только авторизованные клиенты имеют доступ к секретным рецептам бургеров.

Структура, хранящая информацию о процессе, описана следующим образом:

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};
Поле uid хранит идентификатор процесса. Поле gid_count хранит количество групп, которым принадлежит процесс. Массив gids хранит идентификаторы групп процесса.

Реализуйте функцию myaccess, каждый раз моделирующую поведение системного вызова access.

Для простоты будем предполагать, что пользователь с идентификатором 0 имеет право выполнить любую операцию.

int myaccess(const struct stat *stb, const struct Task *task, int access);
Параметр stb хранит информацию о файле. В структуре заполнены поля st_uid, st_gid и st_mode. Параметр task хранит информацию о процессе. Параметр access — это желаемые права доступа (три бита).

Функция должна вернуть 1, если данный процесс может выполнить данную операцию (или комбинацию операций) над данным файлом, и 0 в противном случае.

Для улучшения структуры функции продумайте ее разбиение на служебные функции. Служебные функции определяйте со статической областью видимости.

В сдаваемом на проверку файле должны присутствовать необходимые заголовочные файлы и определение структуры Task.
*/
#include <fcntl.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

enum
{
    USER_PERMISSION_SHIFT = 6,
    GROUP_PERMISSION_SHIFT = 3
};

static int is_owner(unsigned owner_id, unsigned process_uid)
{
    return owner_id == process_uid;
}

static int is_in_group(unsigned group_id, unsigned gids[], int gid_count)
{
    for (int i = 0; i < gid_count; ++i) {
        if (gids[i] == group_id) {
            return 1;
        }
    }

    return 0;
}

static int get_user_rights(__mode_t st_mode) {
    return (st_mode & S_IRWXU) >> USER_PERMISSION_SHIFT;
}

static int get_group_rights(__mode_t st_mode) {
    return (st_mode & S_IRWXG) >> GROUP_PERMISSION_SHIFT;
}

static int get_other_rights(__mode_t st_mode) {
    return st_mode & S_IRWXO;
}

int myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (!(task->uid)) {
        return 1;
    }

    if (is_owner(task->uid, stb->st_uid)) {
        return (get_user_rights(stb->st_mode) & access) == access;
    }

    if (is_in_group(stb->st_gid, task->gids, task->gid_count)) {
        return (get_group_rights(stb->st_mode) & access) == access;
    }

    return (get_other_rights(stb->st_mode) & access) == access;
}
