#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>
#include <time.h>

enum
{
    START_YEAR = 1900,
    SIX_MONTHS = 6
};

enum
{
    SUNDAY = 0,
    SATURDAY = 6
};


int main(int argc, const char *argv[])
{
    struct tm t = { 0 };
    t.tm_isdst = -1;
    scanf("%d-%d-%d", &(t.tm_year), &(t.tm_mon), &(t.tm_mday));
    t.tm_mon -= 1;
    t.tm_year -= START_YEAR;
    timegm(&t);


    unsigned int duration;
    scanf("%u", &duration);

    for (unsigned int i = 0; i < 2 * duration; i++) {
        t.tm_mon += SIX_MONTHS;

        int shift_to_the_weekday = 0;

        timegm(&t);

        if (t.tm_wday == SUNDAY) {
            shift_to_the_weekday = 1;
        } else if (t.tm_wday == SATURDAY) {
            shift_to_the_weekday = 2;
        }

        t.tm_mday += shift_to_the_weekday;
        timegm(&t);
        printf("%04d-%02d-%02d\n", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
        t.tm_mday -= shift_to_the_weekday;
    }

    return 0;
}

// 25/100
