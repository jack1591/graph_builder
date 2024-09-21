# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(proj)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем путь к заголовочным файлам toml11
include_directories(${CMAKE_SOURCE_DIR}/toml11/include)

# Указываем каталог с исходным кодом
add_executable(my_shell src/main.cpp)

# Включение расширенной диагностики
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")