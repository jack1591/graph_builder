#include <iostream>
#include <string>
#include <toml.hpp>
#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
using namespace std;
string command, tek_path,root_path;
vector <string> files_in_dir;

int count_slash(string str){
    int count = 0;
    for (auto c:str)
        if (c=='/')
            count++;
    return count;
}

void add_files(const char * root_path,string archive_path){
    
    files_in_dir.clear();

    struct archive* archive;
    struct archive_entry* entry;
    int result;

    // Создаем объект архива для чтения
    archive = archive_read_new();
    archive_read_support_format_all(archive); // Поддержка всех форматов

    // Открываем архив для чтения
    result = archive_read_open_filename(archive, root_path, 32768);
    if (result != ARCHIVE_OK) {
        cout<<"Archive opening error: "<<archive_error_string(archive);
        archive_read_free(archive); // Освобождаем ресурсы
        return;
    }

    // Чтение заголовков архива и данных файлов
    string tek_path="";
    int count_archive_slash = count_slash(archive_path);

    while ((result = archive_read_next_header(archive, &entry)) == ARCHIVE_OK) {
        tek_path = archive_entry_pathname(entry);
        if (tek_path==archive_path || archive_path==""){
            if (archive_path=="")
                files_in_dir.push_back(tek_path);
            break;
        }
        archive_read_data_skip(archive); // Пропускаем данные, можно и читать их
    }
    
    while ((result = archive_read_next_header(archive, &entry)) == ARCHIVE_OK){
        tek_path = archive_entry_pathname(entry);
        if (tek_path.find(archive_path)==0 || archive_path==""){            
            if (count_slash(tek_path)==count_archive_slash || (count_slash(tek_path)==count_archive_slash+1 && tek_path[tek_path.size()-1]=='/')){
                string s = "";
                tek_path='/'+tek_path;
                for (int i = tek_path.size()-1;i>0;i--)
                    if (tek_path[i]=='/' && i!=tek_path.size()-1)
                        break;
                    else s=tek_path[i]+s;
                files_in_dir.push_back(s);
            }
            archive_read_data_skip(archive);
        }
        else break;
    }

    // Закрытие архива
    archive_read_close(archive); // Закрываем архив
    archive_read_free(archive);  // Освобождаем ресурсы    
}

void ls(){
    for (auto c:files_in_dir)
        cout<<c<<" ";
    cout<<endl;
}

string cd(const char * root_path,string archive_path,string next){
    int i;
    string first_archive_path = archive_path;

    if (next.size()>1 && next[next.size()-1]==next[next.size()-2] && next[next.size()-2]=='.')
         next+='/';

    while (next!=""){
        string s = "";
        int j;

        for (j = 0;j<next.size();j++)
            if (next[j]=='/')
            break;
        if (j==next.size()){
            cout<<"This in not the directory\n";
            files_in_dir.clear();
            add_files(root_path,first_archive_path);        
            return first_archive_path;
        }
        s = next.substr(0,j+1);

        if (s=="../"){
            if (archive_path==""){
                cout<<"There is no parent directory!\n";
                return "";
            }
            else {
            for (i=archive_path.size()-2;i>0;i--)
                if (archive_path[i]=='/')
                    break;
            if (i==0)
                archive_path="";
            else archive_path = archive_path.substr(0,i+1);
            add_files(root_path,archive_path);
            }
        }
        else {
            int size_of_files_in_dir = files_in_dir.size();
            for (i=0;i<size_of_files_in_dir;i++){
                if (files_in_dir[i]==s){
                    files_in_dir.clear();
                    add_files(root_path,archive_path+s);
                    archive_path+=s;
                    break;
                }
            }

            if (i==size_of_files_in_dir){
                cout<<"Directory not found!\n";
                files_in_dir.clear();
                add_files(root_path,first_archive_path);        
                return first_archive_path;
            }
        }
        next = next.substr(j+1,next.size()-j);
    }

    return archive_path;

}
    
int main(){
    
    try {   
        auto config = toml::parse("config.toml");
        string hostname = toml::find<string>(config, "prompt", "hostname");
        root_path = toml::find<string>(config,"filesystem", "archive_path");
        cout<<root_path<<endl;

        add_files(root_path.c_str(),tek_path);

        while (true){
            cout << hostname << "$ ";
            cin>>command;
            if (command=="exit")
                break;
            if (command=="ls")
                ls();
            if (command=="cd"){
                string s;
                cin>>s;
                tek_path=cd(root_path.c_str(),tek_path,s);
            }
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


/*    
    struct archive* archive;
    struct archive_entry* entry;
    int result;

    // Создаем объект архива для чтения
    archive = archive_read_new();
    archive_read_support_format_all(archive); // Поддержка всех форматов

    result = archive_read_open_filename(archive, root_path, 32768);
    if (result != ARCHIVE_OK) {
        cout<<"Archive opening error: "<<archive_error_string(archive)<<endl;
        archive_read_free(archive); // Освобождаем ресурсы
        //return;
    }

    string tek_path="";
    while ((result = archive_read_next_header(archive, &entry)) == ARCHIVE_OK) {
        tek_path = archive_entry_pathname(entry);
        if (tek_path.find(archive_path) || archive_path==""){
            if (tek_path==archive_path+next)
                return archive_path+next;
        }

        archive_read_data_skip(archive); // Пропускаем данные, можно и читать их
    }
   
    archive_read_close(archive); // Закрываем архив
    archive_read_free(archive);  // Освобождаем ресурсы

    return "-1";
    */


/*
void ls(const char * root_path,string archive_path) {
    struct archive* archive;
    struct archive_entry* entry;
    int result;

    // Создаем объект архива для чтения
    archive = archive_read_new();
    archive_read_support_format_all(archive); // Поддержка всех форматов

    // Открываем архив для чтения
    result = archive_read_open_filename(archive, root_path, 32768);
    if (result != ARCHIVE_OK) {
        cout<<"Archive opening error: "<<archive_error_string(archive);
        archive_read_free(archive); // Освобождаем ресурсы
        return;
    }

    // Чтение заголовков архива и данных файлов

    string tek_path="";
    while ((result = archive_read_next_header(archive, &entry)) == ARCHIVE_OK) {
        tek_path = archive_entry_pathname(entry);
        
        if (tek_path==archive_path || archive_path==""){
            int count=0;
            for (auto c:tek_path)
                if (c=='/')
                    count++;

            if (count==0 || (count==1 && tek_path[tek_path.size()-1]=='/'))
            cout<<archive_entry_pathname(entry)<<" ";
            break;
        }

        archive_read_data_skip(archive); // Пропускаем данные, можно и читать их
    }
    
    while ((result = archive_read_next_header(archive, &entry)) == ARCHIVE_OK){
        tek_path = archive_entry_pathname(entry);
        if (tek_path.find(archive_path)==0 || archive_path==""){            
            int count=0;
            for (auto c:tek_path)
                if (c=='/')
                    count++;
            if (archive_path[archive_path.size()-1]=='/')
            count--;
            if (count==0 || (count==1 && tek_path[tek_path.size()-1]=='/'))
            cout<<tek_path<<" ";

            archive_read_data_skip(archive);
        }
        else break;
    }
    
    cout<<endl;

    if (result != ARCHIVE_EOF && archive_path=="") {
        // Если произошла ошибка, кроме конца файла
        printf("Error reading archive: %s\n", archive_error_string(archive));
    }

    // Закрытие архива
    archive_read_close(archive); // Закрываем архив
    archive_read_free(archive);  // Освобождаем ресурсы
}
*/