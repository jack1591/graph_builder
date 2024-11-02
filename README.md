# Репозиторий с заданиями по конфигурационному управлению
## Загрузка репозитория на компьютер
1. Откройте терминал
2. Перейдите в папку, где вы хотите разместить проект
3. Выполните команду:
```
git clone https://github.com/jack1591/conf-upr.git
```
## Запуск программы
1. Перейти в папку build в терминале
2. Ввести следующие команды:  
При первом запуске программы:
```
cmake -G "MinGW Makefiles" ..
cmake ..
mingw32-make
```  

## Задание 1
Разработать эмулятор для языка оболочки ОС. Необходимо сделать работу
эмулятора как можно более похожей на сеанс shell в UNIX-подобной ОС.
Эмулятор должен запускаться из реальной командной строки, а файл с
виртуальной файловой системой не нужно распаковывать у пользователя.
Эмулятор принимает образ виртуальной файловой системы в виде файла формата
**tar**. Эмулятор должен работать в режиме **CLI**.
Конфигурационный файл имеет формат **toml** и содержит:  
• Имя компьютера для показа в приглашении к вводу.  
• Путь к архиву виртуальной файловой системы.  
  
Необходимо поддержать в эмуляторе команды **ls**, **cd** и **exit**, а также
следующие команды:
```
1. uniq.
2. whoami.  
3. find.
``` 
Все функции эмулятора должны быть покрыты тестами, а для каждой из
поддерживаемых команд необходимо написать 2 теста.

Запуск .exe файла программы:
```
./my_shell.exe
```
Запуск .exe файла с тестами:
```
./utest.exe
```

Список тестируемых команд:
1. cd first/my-proj/2/ - **попытка последовательного перехода в несколько директорий**
2. cd fourth/ - **попытка перехода в несуществующую папку**
3. cd first/abc2.txt - **попытка перехода в .txt файл**
4. cd first/my-proj/.. - **переход в my-proj и обратно в first**
5. whoami - **вывод имени пользователя**
6. ls - **вывод всех файлов в текущей директории**
7. uniq abc2.txt - **вывод всех слов из abc2.txt, без повторений**
8. uniq abc1.txt - **вывод всех слов из abc1.txt, без повторений**
9. find 1 - **поиск в архиве всех файлов, которые содержат символ "1"**
10. find 2/ - **поиск в архиве всех файлов, которые содержат сочетание символов "2/"**

Содержимое файла utest.cpp с юнит-тестами:
```
#define UNIT_TEST // предотвращает компиляцию main() в main.cpp
#include "main.cpp" 
#include <gtest/gtest.h>
#include <string>
#include <set>
#include <vector>
using namespace std;
string s = "../src/my-proj.tar";
set<string> example = {"project","artem","qwerty","rybinsk","mike","true","false","abcde","detective","twin","bin","bash"};
vector<string> example_names = {"first/abc1.txt","first/my-proj/1/","first/my-proj/2/bin/ahahaha/ahahah1.txt"};

TEST(AddTest, HandlesPositiveInput_cd1) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(cd(s.c_str(), "", "first/my-proj/2/") == "first/my-proj/2/");
}


TEST(AddTest, HandlesPositiveInput_cd2) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(cd(s.c_str(), "", "fourth/") == "");
}

TEST(AddTest, HandlesPositiveInput_cd3) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(cd(s.c_str(), "", "first/abc2.txt") == "");
}


TEST(AddTest, HandlesPositiveInput_cd4) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(cd(s.c_str(), "", "first/my-proj/..") == "first/");
}

TEST(AddTest, HandlesPositiveInput_whoami) {
    hostname = "PC_folene";
    EXPECT_TRUE(whoami() == "PC_folene");
}

TEST(AddTest, HandlesPositiveInput_ls) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(files_in_dir[0]=="first/" && files_in_dir[1]=="second/" && files_in_dir[2]=="tekst.txt" && files_in_dir[3]=="third/");
}

TEST(AddTest, HandlesPositiveInput_uniq1) {
    add_files(s.c_str(),"first/");
    EXPECT_TRUE(uniq(s.c_str(),"first/","abc2.txt") == example);
}

TEST(AddTest, HandlesPositiveInput_uniq2) {
    add_files(s.c_str(),"first/");
    EXPECT_TRUE(uniq(s.c_str(),"first/","abc1.txt") != example);
}

TEST(AddTest, HandlesPositiveInput_find1) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(find(s.c_str(),"1") == example_names);
}

TEST(AddTest, HandlesPositiveInput_find2) {
    add_files(s.c_str(),"");
    EXPECT_TRUE(find(s.c_str(),"2/") != example_names);
}
```
