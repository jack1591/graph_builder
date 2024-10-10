# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(proj)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем путь к заголовочным файлам toml11
include_directories(${CMAKE_SOURCE_DIR}/include)


# Указываем каталог с исходным кодом
add_executable(my_shell src/main.cpp)

# Линкуем библиотеку libarchive
link_directories(${CMAKE_SOURCE_DIR}/liba)
target_link_libraries(my_shell archive stdc++fs)

# Включение расширенной диагностики
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")