/*
Задача ht01-2: ДЗ 01-2
Определите структуру RandomOperations с полями destroy и next — указателями на функции.

Определите структуру RandomGenerator с необходимыми полями.

typedef struct RandomGenerator
{
//…
} RandomGenerator;
напишите реализацию функции

RandomGenerator *random_create(int seed);
напишите реализации недостающих функций таким образом, чтобы фрагмент

RandomGenerator *rr = random_create(1234);
for (int j = 0; j < 100; ++j) {
  printf("%d\n", rr->ops->next(rr));
}
rr->ops->destroy(rr);
выводил на печать 100 псевдослучайных чисел, полученных с помощью линейного конгруэнтного генератора (Linear congruential generator) с начальным значением 1234, мультипликатором (a) 1103515245, инкрементом (c) 12345 и модулем 2^31. Затем вызов destroy освобождает память, выделенную функцией random_create.

Функция next должна возвращать следующий элемент псевдослучайной последовательности отнсительно текущего состояния.

Сдаваемый на проверку фрагмент программы должен содержать определение структур RandomOperations, RandomGenerator определение функции random_create и других функций, кроме main.

Указание. Структуру с указателями на функции можно сделать глобальной константой и использовать ее для всех создаваемых объектов.
*/
#include <stdlib.h>

struct RandomGenerator;
struct RandomOperations;


enum RandomConstans
{
    A = 1103515245,
    C = 12345,
    D = 0x80000000
};

typedef struct RandomOperations
{
    int (*next) (struct RandomGenerator *);
    void (*destroy) (struct RandomGenerator *);
} RandomOperations;

typedef struct RandomGenerator
{
    RandomOperations *ops;
    int prev;
} RandomGenerator;

int my_next(RandomGenerator *rg) {
    return rg->prev = ((unsigned int) A * (unsigned int) rg->prev + (unsigned int) C) % (unsigned int ) D;
}

void my_destroy(RandomGenerator *rg) {
    free(rg);
}

const RandomOperations operations = {my_next, my_destroy};

RandomGenerator *random_create(int seed) {
    RandomGenerator *rg = calloc(1, sizeof(*rg));
    rg->prev = seed;
    rg->ops = (void *) &operations;
    return rg;
}
