#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>


int main(int argc, const char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        if (*(argv[i]) == '\0') {
            continue;
        }


        int len = strlen(argv[i]);
        int is_correct = 1;

        if (!islower(argv[i][len / 2])) {
            is_correct = 0;
        }

        for (int j = 0; j < len / 2 && is_correct; j++) {
            if (argv[i][j] != argv[i][len - 1 - j] || !islower(argv[i][j]) || ! islower(argv[i][len - 1 - j])) {
                is_correct = 0;
            }
        }

        if (is_correct) {
            printf("%s\n", argv[i]);
        }
    }

    return 0;
}

// 100/100
