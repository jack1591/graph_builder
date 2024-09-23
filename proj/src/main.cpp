#include <iostream>
#include <string>
#include <toml.hpp>
#include <archive.h>
#include <archive_entry.h>
using namespace std;
string command, tek_path;

void ls(const char * archive_path) {
    struct archive* archive;
    struct archive_entry* entry;
    int result;

    // Создаем объект архива для чтения
    archive = archive_read_new();
    archive_read_support_format_all(archive); // Поддержка всех форматов

    // Открываем архив для чтения
    result = archive_read_open_filename(archive, archive_path, 32768);
    if (result != ARCHIVE_OK) {
        cout<<"Archive opening error: "<<archive_error_string(archive);
        archive_read_free(archive); // Освобождаем ресурсы
        return;
    }

    // Чтение заголовков архива и данных файлов
    while ((result = archive_read_next_header(archive, &entry)) == ARCHIVE_OK) {
        string s = archive_entry_pathname(entry);
        int count=0;
        for (auto c:s)
        if (c=='/')
        count++;

        if (count==0 || (count==1 && s[s.size()-1]=='/'))
        cout<<archive_entry_pathname(entry)<<" ";
        archive_read_data_skip(archive); // Пропускаем данные, можно и читать их
    }
    cout<<endl;

    if (result != ARCHIVE_EOF) {
        // Если произошла ошибка, кроме конца файла
        printf("Error reading archive: %s\n", archive_error_string(archive));
    }

    // Закрытие архива
    archive_read_close(archive); // Закрываем архив
    archive_read_free(archive);  // Освобождаем ресурсы
}

void cd(){
    struct archive* archive;
    struct archive_entry* entry;
    int result;

    // Создаем объект архива для чтения
    archive = archive_read_new();
    archive_read_support_format_all(archive); // Поддержка всех форматов

    // Открываем архив для чтения
    result = archive_read_open_filename(archive, tek_path.c_str(), 32768);
    if (result != ARCHIVE_OK) {
        cout<<"Archive opening error: "<<archive_error_string(archive);
        archive_read_free(archive); // Освобождаем ресурсы
        return;
    }

    // Чтение заголовков архива и данных файлов
    result = archive_read_next_header(archive, &entry);
    tek_path=archive_entry_pathname(entry);
    cout<<"New path: "<<tek_path<<endl;

    // Закрытие архива
    archive_read_close(archive); // Закрываем архив
    archive_read_free(archive);  // Освобождаем ресурсы
}

int main(){
    try {   
        auto config = toml::parse("config.toml");
        string hostname = toml::find<std::string>(config, "prompt", "hostname");
        string archive_path = toml::find<std::string>(config,"filesystem", "archive_path");
        cout<<archive_path<<endl;

        tek_path=archive_path;
        while (true){
            cout << hostname << "$ ";
            getline(cin,command);
            if (command=="exit")
            break;
            if (command=="ls")
            ls(tek_path.c_str());
            if (command=="cd")
            cd();
            if (command=="whoami")
            cout<<hostname<<endl;
        }
    }
    catch (const toml::syntax_error& err) {
        std::cerr << "Ошибка синтаксиса в конфигурационном файле: " << err.what() << std::endl;
    }
    catch (const std::out_of_range& err) {
        std::cerr << "Ошибка: параметр не найден в конфигурационном файле" << std::endl;
    }

    return 0;
}