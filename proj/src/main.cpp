#include <iostream>
#include <toml.hpp>
#include <archive.h>
#include <archive_entry.h>
using namespace std;
int main(){
    try {   
        auto config = toml::parse("config.toml");
        string hostname = toml::find<std::string>(config, "prompt", "hostname");
        cout << hostname << "$ ";
    }
    catch (const toml::syntax_error& err) {
        std::cerr << "Ошибка синтаксиса в конфигурационном файле: " << err.what() << std::endl;
    }
    catch (const std::out_of_range& err) {
        std::cerr << "Ошибка: параметр не найден в конфигурационном файле" << std::endl;
    }

    return 0;
}