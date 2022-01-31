/*
Задача mz09-3: mz09-3
Программе в аргументах командной строки передаются три команды для выполнения. Программа должна запустить на выполнение процессы, чтобы они выполнялись в конфигурации, соответствующей команде shell:

{ cmd1 || cmd2 } && cmd3
Родительский процесс должен дождаться завершения всех созданных им процессов и сам завершить выполнение с кодом возврата 0 или 1 в зависимости от успешности выполнения процессов.

Команды cmd1, cmd2, cmd3 можно запускать на выполнение с помощью execlp.

Запись a || b означает то же самое, что !a?b:true. Запись a && b означает то же самое, что a?b:false. Скобки { и } используются для группировки.

Для шелла процесс считается завершившимся успешно, если процесс завершился с помощью системного вызова exit (макрос WIFEXITED возвращает истинное значение), и код завершения процесса (макрос WEXITSTATUS) равен 0. В других случаях процесс считается завершившимся неуспешно.

Соответственно, ваша программа должна завершиться с кодом 0 в случае успешного выполнения заданной группы процессов и с кодом 1 в случае неуспешного их выполнения.

Организуйте вашу программу таким образом, чтобы избежать дублирование кода. В частности, запуск процесса и проверка статуса его завершения может быть вынесена в функцию.

Примечания по тестированию вашей программы:. Тестирование завершается с вердиктом 'Synchronization error', если процесс-отец (то есть ваша программа, запускаемая на тестирование) заканчивает работу раньше какого-либо из своих потомков.
*/
#include <stdio.h>
#include <dlfcn.h>
#include <link.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

int make_fork(void) {
    int pid = fork();

    if (pid < 0) {
        exit(1);
    }

    return pid;
}


// 0 - success
int execute(char const *argv) {
    if (!make_fork()) {
        execlp(argv, argv, NULL);

        _exit(1);
    }

    int status;
    wait(&status);

    return !(WIFEXITED(status) && !WEXITSTATUS(status));
}

int main(int argc, char const *argv[])
{
    return !((!execute(argv[1]) || !execute(argv[2])) && !execute(argv[3]));
}