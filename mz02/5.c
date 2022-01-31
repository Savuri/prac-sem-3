/*
Практическое занятие «Введение в UNIX», задача 5
Задание должно выполняться на сервере prac.cs.msu.ru.

Правила оформления ответов описаны ранее.

Шаг 26. Для компиляции программ на языке Си используется команда gcc, которая имеет вид

gcc OPTIONS-AND-FILES.
Например,

gcc prog.c -Wall -Werror -std=gnu11 -g -o prog
запустит компиляцию программы prog.c. В случае отсутствия ошибок будет создан исполняемый файл prog.

Вопрос 29. В домашнем каталоге создайте каталог newtask3. Сделайте этот каталог текущим. В этом каталоге с помощью команды cat создайте файл aplusb.c следующего содержания:

void main()
{
int a, b, c;
scanf("%d%d", a, &b);
c == a + b;
printf("%d\n", c);
}
Запустите компиляцию файла командой

gcc -Wall -Werror -std=gnu11 -O2 aplusb.c -o aplusb
Какие сообщения об ошибках будут напечатаны командой gcc?

Вопрос 30. Исправьте ошибки. Добейтесь, чтобы компиляция файла проходила без ошибок. Какую информацию печатает команда ls -l о файле aplusb?

Шаг 27. Для запуска программы из текущего каталога необходимо использовать явную запись текущего каталога '.', так как текущий каталог не находится в списке каталогов в переменной окружения PATH. Например,

./aplusb
Вопрос 31. Запустите программу aplusb из текущего каталога. В качестве первого числа введите ваш идентификатор пользователя, в качестве второго числа введите размер файла aplusb. Какой будет напечатан результат?

Шаг 28. В текущем каталоге создайте текстовый файл 001.dat, содержащий два целых числа. Первое число было напечатано командой aplusb в вопросе 26. Второе число — размер файла /etc/passwd. Не забудьте разделить их пробелом или переводом строки.

Вопрос 32. Какая командная строка необходима для запуска программы aplusb, перенаправления ей на стандартный ввод файла 001.dat и перенаправления ее стандартного вывода в файл 001.res?

Вопрос 33. Выполните эту команду. С помощью команды cat выведите содержимое файла 001.res.

Шаг 29. Для измерения времени работы программы используетсякоманда time. Например,

time ./aplusb < 001.dat
Время работы программы выводится в строке ”User”.

Вопрос 34. Чему равно это время (User)?

Шаг 30. В программу вставьте цикл задержки вида

for (int i = 0; i < N; ++i) {}
где константу N подберите таким образом, чтобы время работы программы (User) попало в промежуток от 1.000 до 1.100 секунды. При компиляции программы не используйте опции оптимизации (-O2)!

Вопрос 35. Чему равно значение N?

Шаг 31. Для перенаправления стандартного выходного потока одного процесса на стандартный входной поток другого процесса в командном интерпретаторе используется символ | (вертикальная черта).

Вопрос 36. Напишите и объясните, что выведет на экран команда:

ps -Al | grep sh
Шаг 32. Вопросы повышенной сложности

Вопрос 37. Что выводит ulimit -c, что делает ulimit -c 0?

Вопрос 38. Напишите программу, вызывающую segmentation fault, получите core dump-файл. Ответом на вопрос будет являться вывод

readelf -S <имя файла>
Вопрос 39. Напишите команду для командного интерпретатора, выводящую на экран полный листинг (результат работы ls -l) для 5 файлов с самыми маленькими длинами из каталога /etc, в именах которых содержится строка “.conf”, отсортированных по возрастанию размера файлов.

Вопрос 40. Напишите команду для командного интерпретатора, подсчитывающую число слов в файле /usr/share/dict/words , которые не содержат ни одну из латинских букв: a, e, i, o или u.

Отчет о выполнении задания
Перед сдачей файла report.txt в тестирующую систему выполните следующую команду:

openssl dgst -sha256 -sign LOGIN.pem -hex < report.txt > sig.txt
cat sig.txt >> report.txt
Здесь LOGIN.pem - приватный ключ (см. предыдущие задачи). В результате в конец файла report.txt будет дописана цифровая подпись.

В тестирующую систему сдайте подписанный вами файл report.txt
*/
ANSWERS
***Answer 29:
aplusb.c:1:6: error: return type of 'main' is not 'int' [-Werror=main]
    1 | void main()
      |      ^~~~
aplusb.c: In function 'main':
aplusb.c:4:1: error: implicit declaration of function 'scanf' [-Werror=implicit-function-declaration]
    4 | scanf("%d%d", a, &b);
      | ^~~~~
aplusb.c:4:1: error: incompatible implicit declaration of built-in function 'scanf' [-Werror]
aplusb.c:1:1: note: include '<stdio.h>' or provide a declaration of 'scanf'
  +++ |+#include <stdio.h>
    1 | void main()
aplusb.c:4:9: error: format '%d' expects argument of type 'int *', but argument 2 has type 'int' [-Werror=format=]
    4 | scanf("%d%d", a, &b);
      |        ~^     ~
      |         |     |
      |         int * int
aplusb.c:5:3: error: statement with no effect [-Werror=unused-value]
    5 | c == a + b;
      | ~~^~~~~~~~
aplusb.c:6:1: error: implicit declaration of function 'printf' [-Werror=implicit-function-declaration]
    6 | printf("%d\n", c);
      | ^~~~~~
aplusb.c:6:1: error: incompatible implicit declaration of built-in function 'printf' [-Werror]
aplusb.c:6:1: note: include '<stdio.h>' or provide a declaration of 'printf'
aplusb.c:4:1: error: 'a' is used uninitialized in this function [-Werror=uninitialized]
    4 | scanf("%d%d", a, &b);
      | ^~~~~~~~~~~~~~~~~~~~
aplusb.c:6:1: error: 'c' is used uninitialized in this function [-Werror=uninitialized]
    6 | printf("%d\n", c);
      | ^~~~~~~~~~~~~~~~~
cc1: all warnings being treated as errors
***Answer 30:
-rwxr-xr-x  1 s02190703  Domain Admins  7416 20 сент. 19:58 aplusb
***Answer 31:
27391
***Answer 32:
./aplusb < 001.dat > 001.res
***Answer 33:
29906
***Answer 34:
0m0,000s
***Answer 35:
440000000
***Answer 36:
Выводит процессы всех пользователей и информацию о них, причём отображая ещё и информацию связанную с: uid, pid, ppid, cpu, pri, nice, vsz, rss, mwchan, state, tt, time, command, отображаная при этом только те строки процееса и его описания, в строках которых есть сочетание sh
***Answer 37:
ulimit -c выводит максимальный размер дампа памяти (содержимое рабочей памяти одного процесса, ядра или всей операционной системы, в котором может содержаться дополнительная информация о состоянии программы)
ulimit -c 0 устанавливает максимальный размер дампа памяти.
***Answer 38:
There are no sections in this file.
***Answer 39:
ls -lS /etc | grep .conf | tail -n 5
***Answer 40:
С учётом регистра cat /usr/share/dict/words | egrep -cv 'a|e|i|o|u'
Без учёта регистра cat /usr/share/dict/words | egrep -civ 'a|e|i|o|u'
(stdin)= 304502201b2a128465b22704516d342552d207ee433a225440217e9b60fadb7316c01bb5022100bf9d60a851575eeb89cacb85ef5c9dc9f97e400855ab71c8eb71e08efbff32b3
