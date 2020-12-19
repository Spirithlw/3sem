#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//6.1 вывести список записей с типами в указаной папке
//opendir readdir + closedir + lstat
//писать тип записи(1 буква в ls -l) имя записи обрабатывать dt_unknown
//fstatat
//6.2получить файловый дескриптор - opendir(O_DIRECTORY)+dirfd или open+fdopendir +readdir + closedir
//6.3 open+getdents+fstatat+close
//6.4f(fd_subsubdir, level+1) дерево каталога через 6.2 6.3
//7.1 рекурсивное копирование диалога

int main()
{

    return 0;
}