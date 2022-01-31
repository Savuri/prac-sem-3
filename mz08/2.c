*/
Задача mz08-2: fork-2
Процесс-отец (то есть процесс, который запускается из командной строки) создает процесс-сына, который в свою очеред создает своего сына (внука главного процесса).

Процесс-отец должен вывести на стандартный поток вывода число 1, сын - число 2, а внук - число 3.

Вывод должен быть таким, чтобы всегда на стандартном потоке вывода оказывалась последовательность 3 2 1, записанная в одну строку строго с одним пробелом между числами. Пробел в начале и конце строки не допускается. В конце вывода должен находиться символ \n. Для вывода использовать средства высокого уровня.

Процесс-родитель должен завершаться самым последним из всех процессов.
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, const char *argv[])
{
    int son = fork();

    if (son == -1) {
        return 0;
    } else if (!son) {
        // son
        int grandson = fork();

        if (grandson == -1) {
            exit(1);
        } else if (!grandson) {
            printf("3 ");
            exit(0);
            // grandson
        } else {
            // son
            int p_status1;
            wait(&p_status1);

            if (WIFEXITED(p_status1) && !WEXITSTATUS(p_status1)) {
                printf("2 ");
                exit(0);
            } else {
                exit(1);
            }
        }
    } else {
        // father
        int p_status2;
        wait(&p_status2);

        if (WIFEXITED(p_status2) && !WEXITSTATUS(p_status2)) {
            printf("1\n");
        } else {
            return 1;
        }
    }
}
