# Репозиторий с заданиями по конфигурационному управлению
## Загрузка репозитория на компьютер
1. Откройте терминал
2. Перейдите в папку, где вы хотите разместить проект
3. Выполните команду:
```
git clone https://github.com/jack1591/conf-upr.git
```
## Запуск программы
1. Перейти в папку build любой из задач в терминале
```
cd second/build
```  
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


## Задание 2
Разработать инструмент командной строки для визуализации графа
зависимостей, включая транзитивные зависимости. Сторонние средства для
получения зависимостей использовать нельзя.
Зависимости определяются по имени пакета языка Java **(Maven)**. Для
описания графа зависимостей используется представление **Mermaid**.
Визуализатор должен выводить результат в виде сообщения об успешном
выполнении и сохранять граф в файле формата **png**.  
Ключами командной строки задаются:  
• Путь к программе для визуализации графов.  
• Имя анализируемого пакета.  
• Путь к файлу с изображением графа зависимостей.  
• Максимальная глубина анализа зависимостей.  
• URL-адрес репозитория.  

Все функции визуализатора зависимостей должны быть покрыты тестами.  

## Функции визуализатора  
### downloadFile
С помощью библиотеки curl скопировать содержимое xml файла текущего пакета

### constructPomUrl
Сделать ссылку на xml файл пакета по его названию  

### buildDependencyGraph
Построить графа зависимостей 

### generateMermaidCode
Создать кода mermaid

### generatePngFromMermaid
Сгенерировать png по содержимому файла mmd

### Построенный граф зависимостей пакета org.eclipse.sisu:org.eclipse.sisu.inject:0.9.0.M3 с глубиной 1
![graph](https://github.com/user-attachments/assets/9ed3de94-07dd-4368-a0b8-d44bd8c69172)  

Запуск .exe файла программы:
```
./my_pack.exe (ваш путь до mmdc) org.apache.maven:maven-parent:43 (ваш путь)/conf_upr/second/ 1 https://repo.maven.apache.org/maven2/
```
Запуск .exe файла с тестами:
```
./my_tests.exe
```  

Список тестируемых команд:
1. **Попытка построение графа с глубиной меньше либо равной нолю**
2. **Проверка содержимого xml файла**
3. **Построение графа с глубиной 1**
4. **Построение графа с глубиной 2**
Содержимое файла my_tests.cpp:
```
#define UNIT_TEST // предотвращает компиляцию main() в main.cpp
#include <iostream>
#include "main.cpp"
#include <gtest/gtest.h>
#include <curl/curl.h>
#include <tinyxml2.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

//глубина графа меньше либо равно ноля
TEST(AddTest, HandlesPositiveInput_1) {
    initialization("C:/Users/jackt/conf_upr/second/", "org.apache.maven:maven-parent:43" ,"C:/Users/jackt/conf_upr/second/", "-1", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/43/maven-parent-43.pom";
    string pomXmlContent = downloadFile(url);
    Dependency rootDep;
    
    rootDep.groupId = "org.apache.mave";
    rootDep.artifactId = "maven-parent";
    rootDep.version = "43";
    unordered_map<string, pair<vector<Dependency>,int>> dependencyGraph;
    buildDependencyGraph(rootDep, -1, dependencyGraph,pomXmlContent);
    string mermaid_code = generateMermaidCode(dependencyGraph);
    cout<<mermaid_code<<endl;
    EXPECT_EQ(mermaid_code,"The graph is empty!");
}

//проверка содержимого xml файла
TEST(AddTest, HandlesPositiveInput_2) {
    initialization("C:/Users/jackt/conf_upr/second/", "org.codehaus.plexus:plexus-utils:4.0.1" ,"C:/Users/jackt/conf_upr/second/", "1", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/codehaus/plexus/plexus-utils/4.0.1/plexus-utils-4.0.1.pom";
    string pomXmlContent = downloadFile(url);
    int pos = pomXmlContent.find("<parent>", pos);
    int endPos = pomXmlContent.find("</parent>", pos);
    string tag = pomXmlContent.substr(pos + 1, endPos - pos - 1);
    string example = "parent>\n    <groupId>org.codehaus.plexus</groupId>\n    <artifactId>plexus</artifactId>\n    <version>17</version>\n  ";
    cout<<tag<<endl;
    EXPECT_EQ(tag,example);
}

//проверка mermaid кода для глубины 1
TEST(AddTest, HandlesPositiveInput_3) {
    initialization("C:/Users/jackt/conf_upr/second/", "org.apache.maven:maven-parent:43" ,"C:/Users/jackt/conf_upr/second/", "1", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/43/maven-parent-43.pom";
    string pomXmlContent = downloadFile(url);
    Dependency rootDep;
    
    rootDep.groupId = "org.apache.mave";
    rootDep.artifactId = "maven-parent";
    rootDep.version = "43";
    unordered_map<string, pair<vector<Dependency>,int>> dependencyGraph;
    buildDependencyGraph(rootDep, 1, dependencyGraph,pomXmlContent);
    string mermaid_code = generateMermaidCode(dependencyGraph);
    string example = "";
    example = "graph TD\n    org.apache.mave:maven-parent:43 --> org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin}\n    org.apache.mave:maven-parent:43 --> org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin}\n    org.apache.mave:maven-parent:43 --> org.codehaus.plexus:plexus-utils:${version.plexus-utils}\n    org.apache.mave:maven-parent:43 --> org.codehaus.plexus:plexus-xml:${version.plexus-xml}\n    org.apache.mave:maven-parent:43 --> org.junit:junit-bom:${versions.junit5}\n";
    cout<<example<<endl;
    EXPECT_EQ(mermaid_code,example);
}

//проверка mermaid кода для глубины 1
TEST(AddTest, HandlesPositiveInput_4) {
    initialization("C:/Users/jackt/conf_upr/second/", "org.apache.maven:maven-parent:43" ,"C:/Users/jackt/conf_upr/second/", "2", "https://repo.maven.apache.org/maven2/");
    url = "https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/43/maven-parent-43.pom";
    string pomXmlContent = downloadFile(url);
    Dependency rootDep;
    
    rootDep.groupId = "org.apache.mave";
    rootDep.artifactId = "maven-parent";
    rootDep.version = "43";
    unordered_map<string, pair<vector<Dependency>,int>> dependencyGraph;
    buildDependencyGraph(rootDep, 2, dependencyGraph,pomXmlContent);
    string mermaid_code = generateMermaidCode(dependencyGraph);
    string example = "graph TD\n    org.junit:junit-bom:${versions.junit5} --> org.junit.jupiter:junit-jupiter:5.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.jupiter:junit-jupiter-api:5.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.jupiter:junit-jupiter-engine:5.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.jupiter:junit-jupiter-migrationsupport:5.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.jupiter:junit-jupiter-params:5.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-commons:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-console:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-engine:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-jfr:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-launcher:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-reporting:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-runner:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-suite:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-suite-api:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-suite-commons:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-suite-engine:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.platform:junit-platform-testkit:1.10.3\n    org.junit:junit-bom:${versions.junit5} --> org.junit.vintage:junit-vintage-engine:5.10.3\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> org.openjdk.jmh:jmh-core:1.37\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> org.openjdk.jmh:jmh-generator-annprocess:1.37\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> org.junit.jupiter:junit-jupiter:\n    org.codehaus.plexus:plexus-xml:${version.plexus-xml} --> org.codehaus.plexus:plexus-utils:4.0.1\n    org.codehaus.plexus:plexus-utils:${version.plexus-utils} --> org.codehaus.plexus:plexus-xml:3.0.0\n    org.codehaus.plexus:plexus-utils:${version.plexus-utils} --> org.junit.jupiter:junit-jupiter-api:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> com.google.inject:guice:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> javax.annotation:javax.annotation-api:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> javax.enterprise:cdi-api:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.eclipse.sisu:org.eclipse.sisu.inject:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.codehaus.plexus:plexus-component-annotations:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.codehaus.plexus:plexus-classworlds:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.codehaus.plexus:plexus-utils:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.codehaus.plexus:plexus-xml:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.slf4j:slf4j-api:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> org.osgi:osgi.core:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> junit:junit:\n    org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin} --> ch.qos.logback:logback-classic:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> com.google.inject:guice:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> org.slf4j:slf4j-api:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> org.osgi:osgi.core:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> javax.annotation:javax.annotation-api:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> javax.enterprise:cdi-api:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> javax.servlet:servlet-api:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> com.google.inject.extensions:guice-servlet:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> junit:junit:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> org.junit.jupiter:junit-jupiter-api:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> org.testng:testng:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> ch.qos.logback:logback-classic:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> com.google.inject.extensions:guice-assistedinject:\n    org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin} --> org.apache.felix:org.apache.felix.framework:\n    org.apache.mave:maven-parent:43 --> org.eclipse.sisu:org.eclipse.sisu.inject:${version.sisu-maven-plugin}\n    org.apache.mave:maven-parent:43 --> org.eclipse.sisu:org.eclipse.sisu.plexus:${version.sisu-maven-plugin}\n    org.apache.mave:maven-parent:43 --> org.codehaus.plexus:plexus-utils:${version.plexus-utils}\n    org.apache.mave:maven-parent:43 --> org.codehaus.plexus:plexus-xml:${version.plexus-xml}\n    org.apache.mave:maven-parent:43 --> org.junit:junit-bom:${versions.junit5}\n";
    cout<<example<<endl;
    EXPECT_EQ(mermaid_code,example);
}

```

### Результат работы тестов
![tests](https://github.com/user-attachments/assets/0bc5be49-6516-49c7-b7d5-ed0087749e9e)
