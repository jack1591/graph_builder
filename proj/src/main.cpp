#include <iostream>
#include <toml.hpp>
using namespace std;
int main(){
    try {   
        auto config = toml::parse("config.toml");

        // Получаем имя хоста из TOML файла
        std::string hostname = toml::find<std::string>(config, "prompt", "hostname");

        // Выводим имя хоста в приглашении
        std::cout << hostname << "$ ";
    }
    catch (const toml::syntax_error& err) {
        std::cerr << "Ошибка синтаксиса в конфигурационном файле: " << err.what() << std::endl;
    }
    catch (const std::out_of_range& err) {
        std::cerr << "Ошибка: параметр не найден в конфигурационном файле" << std::endl;
    }

    return 0;
}