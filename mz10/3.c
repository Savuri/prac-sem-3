/*
Задача up10-3: mz10-3 (дореш)
Программе в аргументах командной строки передаются три команды для выполнения, затем два имени файла (всего 5 аргументов командной строки). Программа должна запустить на выполнение процессы, чтобы они выполнялись в конфигурации, соответствующей команде shell:

( cmd1 < file1 && cmd2 ) | cmd3 >> file2
Родительский процесс должен дождаться завершения всех созданных им процессов и сам завершить выполнение.

Считайте, что команды cmd1, cmd2, cmd3 можно запускать на выполнение с помощью execlp.

Запись CMD1 && CMD2 означает, что процесс CMD2 должен быть запущен только после успешного завершения процесса CMD1.

Запись ( CMD ) используется для группировки, при этом вся командная строка CMD выполняется в отдельном процессе, порожденном основным процессом.
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

enum
{
    CMD1 = 1,
    CMD2 = 2,
    CMD3 = 3,
    INPUT_FILE_FOR_CMD1 = 4,
    OUTPUT_FILE_FOR_CMD3 = 5
};


int make_fork(void) {
    int pid = fork();

    if (pid < 0) {
        exit(1);
    }

    return pid;
}

// 1 - success
int wait_child(int pid) {
    int status;
    wait(&status);

    return WIFEXITED(status) && !WEXITSTATUS(status);
}

int main(int argc, char const *argv[]) {
    int pfd[2];
    if (pipe(pfd) == -1) {
        fprintf(stderr, "Pipe error: %s\n", strerror(errno));
        return 1;
    }


    int manage_pid = make_fork();
    if (!manage_pid) {
        dup2(pfd[1], STDOUT_FILENO);

        close(pfd[0]);
        close(pfd[1]);


        int pid1;
        if (!(pid1 = make_fork())) {

            int fd_in = open(argv[INPUT_FILE_FOR_CMD1], O_RDONLY);

            if (fd_in < 0 || dup2(fd_in, STDIN_FILENO) < 0) {
                _exit(1);
            }

            close(fd_in);

            execlp(argv[CMD1], argv[CMD1], NULL);

            _exit(1);
        }

        int pid2;

        int first_child_result;

        if ((first_child_result = wait_child(pid1)) && !(pid2 = make_fork())) {
            execlp(argv[CMD2], argv[CMD2], NULL);
            _exit(1);
        } else {
            wait(NULL);
        }

        exit(0);
    }

    close(pfd[1]);

    int pid3;
    if (!(pid3 = make_fork())) {
        int fd_out = open(argv[OUTPUT_FILE_FOR_CMD3], O_WRONLY | O_APPEND | O_CREAT, 0666);

        if (fd_out < 0 || dup2(fd_out, STDOUT_FILENO) < 0 || dup2(pfd[0], STDIN_FILENO) < 0) {
            _exit(1);
        }

        close(pfd[0]);

        execlp(argv[CMD3], argv[CMD3], NULL);

        _exit(1);
    }

    close(pfd[0]);

    wait(NULL);
    wait(NULL);

    return 0;
}
