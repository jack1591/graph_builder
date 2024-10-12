# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(proj)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем путь к заголовочным файлам toml11 и googletest
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/googtest)

# Добавляем googletest как поддиректорию
add_subdirectory(${CMAKE_SOURCE_DIR}/googtest)

# Указываем каталог с исходным кодом
add_executable(my_shell src/main.cpp)

# Линкуем библиотеку libarchive
link_directories(${CMAKE_SOURCE_DIR}/liba)
target_link_libraries(my_shell archive stdc++fs)


# Добавляем тестируемый исполняемый файл utest
add_executable(utest src/utest.cpp)

# Линкуем utest с библиотеками GoogleTest
target_link_libraries(utest gtest gtest_main archive)

# Включение расширенной диагностики
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")