/*
Задача up10-4: mz10-4 (дореш)
Программе в аргументах командной строки передается произвольное (возможно пустое) количество команд для выполнения. Программа должна запустить на выполнение процессы, чтобы они выполнялись в конфигурации, соответствующей команде shell:

cmd1 | cmd2 | ... | cmdN
Родительский процесс должен дождаться завершения всех созданных им процессов и сам завершить выполнение. Если все системные вызовы, исполнявшиеся родителем (например, создание процесса), завершились успешно, родитель завершается с кодом 0. В случае ошибки выполнения системного вызова (например, невозможности создания очередного процесса), родитель уничтожает уже созданные к этому моменту процессы с помощью сигнала SIGKILL и сам завершается с кодом 1 после завершения сыновей.

Команды cmd1, cmd2, ... запускайте на выполнение с помощью execlp.

В родителе одновременно может быть открыто не более 6 файловых дескрипторов.

Создаваемые процессы должны оставаться в группе процессов родителя.

Программа не должна ничего выводить на стандартный поток ошибок.

Например, если скомпилированная программа называется solution, то ее запуск с аргументами

./solution ls cat wc
должен дать такой же результат на стандартном потоке вывода, как выполнение команды shell

ls | cat | wc
*/
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>

int wait_child(int pid) {
    int st;
    waitpid(pid, &st, 0);
    return (WIFEXITED(st) && !WEXITSTATUS(st));
}

void kill_children(int *pids, int n) {
    int cnt = 0;

    for (int i = 0; i < n; i++) {
        if (pids[i] > 0) {
            cnt++;
            kill(pids[i], SIGKILL);
        }
    }

    for (int i = 0; i < cnt; i++) {
        wait_child(WAIT_ANY);
    }
}

int make_fork(int *pids, int n) {
    int pid = fork();

    if (pid == -1) {
        kill_children(pids, n);
        exit(1);
    }

    return pid;
};


int main(int argc, const char **argv) {
    int stdin_save = dup(STDIN_FILENO);

    if (stdin_save < 0) {
        return 1;
    }

    int *pids = calloc((argc - 1), sizeof(*pids));

    if (!pids) {
        return 1;
    }

    memset(pids, 0, (argc - 1) * sizeof(*pids));

    for (int i = 1; i < argc; i++) {
        int fd[2];

        if (i != argc - 1) {
            if (pipe(fd) < 0) {
                kill_children(pids, argc - 1);
                return 1;
            }
        }

        if (!(pids[i - 1] = make_fork(pids, argc - 1))) {
            if (i != argc - 1) {
                if (dup2(fd[1], STDOUT_FILENO) < 0 || close(fd[1]) < 0 || close(fd[0]) < 0) {
                    exit(1);
                }
            }

            execlp(argv[i], argv[i], NULL);

            _exit(1);
        }

        if (i != argc - 1) {
            if (dup2(fd[0], 0) < 0 || close(fd[0]) < 0 || close(fd[1]) < 0) {
                kill_children(pids, argc - 1);
                return 1;
            }
        }
    }

    if (dup2(stdin_save, 0) < 0 || close(stdin_save) < 0) {
        kill_children(pids, argc - 1);

        return 1;
    }

    for (int i = 1; i < argc; i++) {
        wait_child(WAIT_ANY);
    }

    free(pids);

    return 0;
