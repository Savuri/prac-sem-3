/*
Задача up06-4: mz06-4 (дореш)
Студент Дмитрий из Италии баллотируется на выборах членов Могущественной тайной организации™ для работы в стипендиальном комитете.

image
Для оптимизации выплат стипендии он предлагает перейти на новый календарь, в котором летоисчисление начинается с 7 октября 1925 года по Григорианскому календарю. Этот день считается первым днем первого месяца первого года нового календаря. Для скрепности в новом календаре каждый месяц состоит из 30 дней, а каждый год — из 12 месяцев.

На стандартном потоке ввода задаются тройки чисел: Y M D — даты по Григорианскому календарю: Y — год, M — месяц (от 1 до 12), D — день месяца (от 1 до 31). Гарантируется, что задаваемая таким образом дата больше даты начала эпохи и представима 32-битным типом time_t.

Для каждой введенной даты в Григорианском календаре на стандартный поток вывода напечатайте соответствующую ей дату в новом календаре в виде Yn Mn Dn, где Yn - год, Mn - месяц (от 1 до 12), Dn - день (от 1 до 30).


*/
#include <stdio.h>
#include <time.h>
#include <string.h>

enum
{
    NEW_START_YEAR = 1925,
    NEW_START_MONTH = 10,
    NEW_START_DAY = 7,
    SECONDS_IN_DAY = 86400
};

enum
{
    SCANF_ARGS = 3
};

enum NewCalendar
{
    MONTHS_IN_YEAR = 12,
    DAYS_IN_MONTH = 30,
    DAYS_IN_YEAR = DAYS_IN_MONTH * MONTHS_IN_YEAR
};

int main(int argc, char **argv)
{
    struct tm new_start = { 0 };
    new_start.tm_isdst = -1;
    new_start.tm_year = NEW_START_YEAR - 1900;
    new_start.tm_mon = NEW_START_MONTH - 1;
    new_start.tm_mday = NEW_START_DAY;

    long long int start_time = timegm(&new_start);

    struct tm date;

    int year, month, day;
    while (scanf("%d %d %d", &year, &month, &day) == SCANF_ARGS) {
        memset(&date, 0, sizeof(date));
        date.tm_isdst = -1;
        date.tm_year = year - 1900;
        date.tm_mon = month - 1;
        date.tm_mday = day;

        long long int sec = timegm(&date) - start_time;

        int days = sec / SECONDS_IN_DAY;
        int new_year = days / (DAYS_IN_YEAR) + 1;
        int new_month = (days / DAYS_IN_MONTH) % MONTHS_IN_YEAR + 1;
        int new_days = days % DAYS_IN_MONTH + 1;

        printf("%d %d %d\n", new_year, new_month, new_days);
    }

    return 0;
}
