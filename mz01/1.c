/*
Задача up01-1: mz01-1 (дореш)
Напишите программу, проверяющую принадлежность точки на координатной плоскости заштрихованной фигуре, изображенной на рисунке.

figure
Точки на границе фигуры считаются принадлежащими фигуре. Вершины фигуры находятся в точках с целочисленными координатами. Линии внутри фигуры показаны для удобства.

Формат входных данных
На стандартном потоке ввода задаются две строки, содержащие ровно по одному вещественному числу: координаты x и y соответственно.

Формат результата
На стандартный поток вывода напечатайте число 1, если точка принадлежит фигуре, и 0, если точка не принадлежит фигуре.

Примеры
Входные данные
0
0
Результат работы
0

*/

#include <stdio.h>

int is_inside_of_figure(double x, double y) {
    return x >= 1 && y <= 5 && y + x <= 10
           && x <= 7 && y >= 2;
}

int main() {
    double x,y;
    scanf("%lf%lf", &x, &y);

    printf("%d\n", is_inside_of_figure(x, y));

    return 0;
}