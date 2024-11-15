#include <iostream>
#include <string>
#include <toml.hpp>
#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <fstream>
#include <set>
using namespace std;
string command, tek_path,root_path,hostname;
vector <string> files_in_dir;
int f;

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
        cout<<"Archive opening error: "<<archive_error_string(archive)<<endl;
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
                for (unsigned int i = tek_path.size()-1;i>0;i--)
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
        unsigned int j;
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

vector<string> find(const char * root_path, string path){
    struct archive* archive;
    struct archive_entry* entry;
    int result;
    vector<string> names;
    // Создаем объект архива для чтения
    archive = archive_read_new();
    archive_read_support_format_all(archive); // Поддержка всех форматов

    // Открываем архив для чтения
    result = archive_read_open_filename(archive, root_path, 32768);
    if (result != ARCHIVE_OK) {
        cout<<"Archive opening error: "<<archive_error_string(archive)<<endl;
        archive_read_free(archive); // Освобождаем ресурсы
        //return;
    }

    // Чтение заголовков архива и данных файлов
    string count_path;
    while ((result = archive_read_next_header(archive, &entry)) == ARCHIVE_OK){
        count_path = archive_entry_pathname(entry);
        if (count_path.find(path)!=std::string::npos){
            names.push_back(count_path);
            cout<<count_path<<endl;
            archive_read_data_skip(archive);
        }
    }

    // Закрытие архива
    archive_read_close(archive); // Закрываем архив
    archive_read_free(archive);  // Освобождаем ресурсы
    return names;    
}

set<string> uniq(const char * root_path, string archive_path,string name_file){
    struct archive* archive;
    struct archive_entry* entry;
    int result;
    // Создаем объект архива для чтения
    archive = archive_read_new();
    archive_read_support_format_all(archive); // Поддержка всех форматов

    // Открываем архив для чтения
    result = archive_read_open_filename(archive, root_path, 32768);
    if (result != ARCHIVE_OK) {
        cout<<"Archive opening error: "<<archive_error_string(archive)<<endl;
        archive_read_free(archive); // Освобождаем ресурсы
        //return;
    }

    // Чтение заголовков архива и данных файлов
    string count_path;
    set<string> words_in_file;
    while (archive_read_next_header(archive, &entry) == ARCHIVE_OK){
        count_path = archive_entry_pathname(entry);
        if (count_path.find(archive_path+name_file)!=std::string::npos){
            const void *buff;
            size_t size;
            la_int64_t offset;
            
            while ((result = archive_read_data_block(archive, &buff, &size, &offset)) == ARCHIVE_OK) {
                string s(reinterpret_cast<const char *>(buff));
                s+=" ";
                string word = "";
                for (auto c:s)
                    if (c==' ' || c=='\n' || c=='\r'){
                        if (word!="" && words_in_file.find(word)==words_in_file.end()){
                            words_in_file.insert(word);
                            cout<<word<<endl;
                        }
                        word="";
                    }
                    else word+=c;
            }

            if (result != ARCHIVE_EOF && result != ARCHIVE_OK) {
                std::cerr << "Ошибка при чтении файла: " << archive_error_string(archive) << endl;
            }
            break;
        }
    }

    // Закрытие архива
    archive_read_close(archive); // Закрываем архив
    archive_read_free(archive); 

    return words_in_file;
}

string whoami(){
    return hostname;
}

#ifndef UNIT_TEST
int main(){
    try {   
        auto config = toml::parse("config.toml");
        hostname = toml::find<string>(config, "prompt", "hostname");
        root_path = toml::find<string>(config,"filesystem", "archive_path");
        
        add_files(root_path.c_str(),tek_path);
        
        f=0;
        while (true){
            cout<<"$ ";
            getline(cin,command);
            if (command.substr(0,4)=="exit")
                break;
            else if (command.substr(0,2)=="ls")
                ls();
            else if (command.substr(0,2)=="cd"){
                if (command.size()<4)
                    cout<<"name is empty!\n";
                else {
                    string s = command.substr(3,command.size()-2);
                    tek_path=cd(root_path.c_str(),tek_path,s);
                }
            }
            else if (command.substr(0,6)=="whoami")
                    cout<<whoami()<<endl;
            else if (command.substr(0,4)=="find"){
                    if (command.size()<6)
                        cout<<"name is empty!\n";
                    else {
                        string path = command.substr(5,command.size()-4);
                        vector<string> tek_names = find(root_path.c_str(),path);
                    }
            }
            else if (command.substr(0,4)=="uniq"){
                    if (command.size()<6)
                        cout<<"name is empty!\n";
                    else {
                        string name_of_file = command.substr(5,command.size()-4);
                        set<string> exampl;
                        exampl = uniq(root_path.c_str(), tek_path, name_of_file);
                    }
            }
            else cout<<"Unknown command\n";
        }
    }
    catch (const toml::syntax_error& err) {
        std::cerr << "Syntap error in config file " << err.what() << endl;
    }
    catch (const std::out_of_range& err) {
        std::cerr << "Error: argiment is not found" << endl;
    }

    return 0;
}
#endif