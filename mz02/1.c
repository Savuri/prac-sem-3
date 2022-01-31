//про ssh
/*
Практическое занятие «Введение в UNIX», задача 1
Задание должно выполняться на сервере prac.cs.msu.ru. Логин и пароль от этого сервера совпадает с логином и паролем, выданным вам для машзала.

Если вы уже настроили вход по ключу на сервер prac.cs.msu.ru, перейдите к разделу "Отчет о выполнении задания".

Первый вход в систему
Чтобы зайти в сессию shell на сервере prac.cs.msu.ru выполните команду

ssh LOGIN@prac.cs.msu.ru
LOGIN замените на ваш логин от машзала (и от ejudge), который состоит из буквы "s" и номера вашего студенческого билета. Например, если номер вашего студенческого билета 02130666, команда для входа будет:

ssh s02130666@prac.cs.msu.ru
Вам будет выдано приглашение на ввод пароля. Введите ваш пароль от машзала (от машзала, а не от ejudge!).

Если пароль введен правильно, вы получите в ответ некоторый текст, за которым следуюет приглашение ко вводу команд.

Last login: ...
FreeBSD 11.3-RELEASE-p11 (MINI) #2 r363310: Sat Jul 18 19:00:35 MSK 2020

Welcome to FreeBSD!

Release Notes, Errata: https://www.FreeBSD.org/releases/
Security Advisories:   https://www.FreeBSD.org/security/
FreeBSD Handbook:      https://www.FreeBSD.org/handbook/
FreeBSD FAQ:           https://www.FreeBSD.org/faq/
Questions List: https://lists.FreeBSD.org/mailman/listinfo/freebsd-questions/
FreeBSD Forums:        https://forums.FreeBSD.org/

Documents installed with the system are in the /usr/local/share/doc/freebsd/
directory, or can be installed later with:  pkg install en-freebsd-doc
For other languages, replace "en" with a language code like de or fr.

Show the version of FreeBSD installed:  freebsd-version ; uname -a
Please include that output and any error messages when posting questions.
Introduction to manual pages:  man man
FreeBSD directory layout:      man hier

Edit /etc/motd to change this login announcement.
[LOGIN@freebsd ~]$
Нажмите Ctrl-D, на этом первый сеанс работы с системой завершится.

Создание ключа ssh для последующего входа без ввода пароля.
Чтобы не вводить пароль каждый раз при входе на сервер создадим ключ ssh. Если у вас уже есть ключ ssh, новый ключ создавать не нужно, вполне можно использовать существующий.

Для создания ключа на своем собственном локальном компьютере выполните команду:

ssh-keygen -t rsa -b 2048
Вам будет задано три вопроса, каждый раз просто нажмите ENTER

В итоге вывод на терминал будет примерно следующим:

Generating public/private rsa key pair.
Enter file in which to save the key (/home/LOGIN/.ssh/id_rsa):
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in /home/LOGIN/.ssh/id_rsa.
Your public key has been saved in /home/LOGIN/.ssh/id_rsa.pub.
The key fingerprint is:
SHA256:3xHD+NR3rwzlxckrZncJtNDQmn7cnOQDE/NNZSuY3ow LOGIN@freebsd.prac
The key's randomart image is:
+---[RSA 2048]----+
|           o+.  +|
|           o=*oo+|
|          .oB+*=*|
|          .=+Bo=*|
|        S .E=*O++|
|         . oo*oB.|
|          . o o .|
|                 |
|                 |
+----[SHA256]-----+
[LOGIN@freebsd ~]$
В Windows 10 ssh есть "из коробки". Если же команда ssh недоступна, то вы можете установить, например, GIT for Windows (GIT). После инсталляции у вас появится GIT Bash, в котором есть ssh.

Настройка входа без пароля
Чтобы настроить вход на сервер без пароля вам потребуется скопировать публичный ключ из файла /home/LOGIN/.ssh/id_rsa.pub (путь к файлу id_rsa.pub вам был выдан при генерации ключа - см. вывод выше). на сервер. Для этого выполните команду

scp /home/LOGIN/.ssh/id_rsa.pub LOGIN@prac.cs.msu.ru:
(не забудьте знак двоеточия после prac.cs.msu.ru!). Еще раз введите пароль. В случае успешного выполнения вывод будет примерно таким:

scp .ssh/id_rsa.pub LOGIN@prac.cs.msu.ru:
Password for LOGIN@freebsd.prac:
id_rsa.pub                                    100%  408    73.4KB/s   00:00
После этого еще раз войдите на сервер prac.cs.msu.ru с помощью ssh.

На сервере выполните следующие команды:

mkdir .ssh
chmod 700 .ssh
cat id_rsa.pub >> .ssh/authorized_keys
rm id_rsa.pub
chmod 600 .ssh/authorized_keys
Завершите сессию с помощью Ctrl-D. Если все было сделано правильно, то следующие команды ssh для входа на сервер prac.cs.msu.ru не потребуют пароля.

Отчет о выполнении задания
В качестве ответа на это задание сдайте файл output.txt, который был получен на вашем локальном компьютере в результате выполнения следующей команды.

ssh -v LOGIN@prac.cs.msu.ru exit 2> output.txt

*/
