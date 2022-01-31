/*
Практическое занятие «Введение в UNIX», задача 4
Задание должно выполняться на сервере prac.cs.msu.ru.

Правила оформления ответов описаны в предыдущем задании.

Шаг 12. У каждого пользователя есть его личный каталог для хранения файлов в системе. Обычно, этот каталог находится в каталоге /home. Личный каталог пользователя называется «домашним» каталогом. Для смены текущего каталога используется команда cd. Команда cd без аргументов устанавливает домашний каталог в качестве текущего. Например,

cd /tmp
переход в каталог /tmp

cd
переход в домашний каталог Для получения полного пути к текущему каталогу используется команда

pwd
Вопрос 17. Выполните команды

cd
pwd
что было напечатано на стандартный поток вывода? Что это за путь?

Вопрос 18. Выполните команду

cd /usr/local/bin/./../../include/sys/..
какой каталог будет установлен в качестве текущего?

Шаг 13. Для создания каталогов используется команда mkdir.

Вопрос 19. Выполните команды

cd
mkdir newtask1
Каково будет время последней модификации каталога newtask1? Каковы права доступа к этому каталогу (восьмеричные)?

Шаг 14. Для удаления отдельных файлов используется команда rm. Например,

rm ~/file2
Шаг 15. Для удаления пустого каталога используется команда rmdir. Например,

rmdir newtask1
Шаг 16. Для удаления файлов и каталогов со всем их содержимым используется команда rm с опцией -rf, например,

rm -rf .mozilla
Внимание! Используйте с осторожностью!!!

Шаг 17. Команда echo выводит на стандартный поток вывода свои аргументы командной строки. Например,

echo Hello world
Вопрос 20. Выполните команду

echo 1 ~ 2 ~ 3 > ~/file7
каков получился размер файла ~/file7? Каково его содержимое?

Шаг 18. В командной строке допускается использовать шаблоны имен файлов. В шаблонах имен файлов символ '*' (звездочка) означает произвольное количество (в том числе 0) произвольных символов, а символ '?' (вопрос) означает ровно один произвольный символ. Шаблоны имен файлов обрабатываются командным процессором bash перед вызовом запускаемой программы, и поэтому запускаемая программа получает в качестве аргументов командной строки все файлы, удовлетворяющие шаблону, а не сам шаблон.

Вопрос 21. Выполните команду

echo /etc/c?o*
что выведено на стандартный поток вывода?

Шаг 19. Для копирования файлов используется команда cp. Например,

cp /etc/passwd ~/file9
скопирует файл /etc/passwd в файл ~/file9

cp /etc/hosts /etc/group ~
скопирует файлы /etc/hosts и /etc/group в каталог /tmp под своими именами (hosts, group)

cp /etc/p* ~
скопирует все файлы, удовлетворяющие шаблону /etc/p* в каталог /tmp

Вопрос 22. В домашнем каталоге создайте каталог newtask2, затем скопируйте в него файлы /etc/passwd и /etc/hosts. Что будет выведено командой ls -l при распечатке списка файлов в каталоге newtask2?

Шаг 20. Для переноса (переименования) файлов используется команда mv, использование которой аналогично использованию команды cp.

Вопрос 23. В каталоге newtask2 переименуйте файл hosts в hosts2. Что будет выведено командой ls -l при распечатке списка файлов в каталоге newtask2?

Шаг 21. Переменные окружения — это способ хранения дополнительной информации (как правило, о настройках пользователя), доступной каждой выполняющейся программе. Переменная окружения ставит в соответствие некоторому имени произвольное текстовое значение. Например, переменная окружения HOME задает путь к домашнему каталогу. Чтобы получить список всех установленных переменных окружения используется команда set без аргументов.

Вопрос 24. Выполните команду set для просмотра всех переменных окружения. Какое назначение у переменных окружения USER и UID?

Шаг 22. Вывести значение некоторой конкретной переменной окружения можно с помощью команды

echo $VAR
где VAR – имя переменной окружения. Например,

echo $USER
Конструкция $VAR в аргументах командной строки обрабатывается командным процессором bash, поэтому запускаемая программа вместо записи $VAR получает в качестве аргумента командной строки значение переменной окружения $VAR.

Вопрос 25. Каково значение переменной окружения SHELL?

Шаг 23. Установить или изменить значение переменной окруженияможно с помощью команды

VAR=value
здесь VAR – имя переменной окружения, value – значение переменной окружения. value не должно содержать пробелы. Если value содержит пробелы (которые могут появиться и из-за использования значений других переменных окружения), необходимо value заключить в кавычки:

VAR=”value”
В строке value могут использоваться имена переменных, например,

WORKDIR=$HOME/newtask2
Вопрос 26. Выполните команду

PATH=”/etc:$PATH:/usr/include”
каково будет значение переменной окружения PATH?
Шаг 24. Переменная окружения PATH задает список каталогов, которые просматриваются, если запускается команда, в имени которой отсутствует символ '/'. Каталоги в переменной окружения PATH разделяются символом ':' (двоеточие). Например, при выполнении команды

ls /etc
для поиска программы (исполняемого файла) ls будет использоваться переменная окружения PATH. При выполнении команды

/bin/ls /etc
всегда будет использоваться программа /bin/ls независимо от значения переменной окружения PATH.

Вопрос 27. В каком каталоге из списка каталогов в переменной окружения PATH находится программа who?

Шаг 25. Для получения кода завершения процесса используется специальная переменная $?. Если процесс завершился успешно, код завершения равен 0, если процесс завершился с ошибкой, код завершения не 0.

Вопрос 28. Запустите ls /123 и ls /home, с какими кодами возврата завершился каждый из процессов? используйте команду echo.

Отчет о выполнении задания
Перед сдачей файла report.txt в тестирующую систему выполните следующую команду:

openssl dgst -sha256 -sign LOGIN.pem -hex < report.txt > sig.txt
cat sig.txt >> report.txt
Здесь LOGIN.pem - приватный ключ (см. предыдущие задачи). В результате в конец файла report.txt будет дописана цифровая подпись.

В тестирующую систему сдайте подписанный вами файл report.txt
*/
ANSWERS
***Answer 17:
/home/s02190703
Домашний каталог пользователя.
***Answer 18:
include (/usr/include).
***Answer 19:
18:10. Права доступа: 755
***Answer 20:
Размер 38 байт. 1 /home/s02190703 2 /home/s02190703 3
***Answer 21:
/etc/crontab
***Answer 22:
hosts	passwd
***Answer 23:
-rwxr-xr--  1 s02190703  Domain Admins  1090 20 сент. 18:30 hosts2
-rwxr-xr--  1 s02190703  Domain Admins  2515 20 сент. 18:29 passwd
***Answer 24:
UID=29142
USER=s02190703
***Answer 25:
/usr/local/bin/bash
***Answer 26:
”/etc:/sbin:/bin:/usr/sbin:/usr/bin:/usr/games:/usr/local/sbin:/usr/local/bin:/home/s02190703/bin:/usr/include”
***Answer 27:
/usr/bin/who
***Answer 28:
У ls /123 код возврата 1. У ls /home код возврата 0.

(stdin)= 304402202c09ff6b6bd3af7fb9e53c718f08b2a886ed84dbd5ad30d653418ed01f072cde02202057053c6d274e7e230e057f86e09660431f48f21a25330ec6d5bca5ff94a798
