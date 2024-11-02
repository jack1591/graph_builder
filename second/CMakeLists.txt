# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(second)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Укажите путь к заголовкам и библиотекам CURL
set(CURL_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include")  
set(CURL_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/libcurl.lib")

# Укажите путь к заголовкам и библиотекам TinyXML2
set(TINYXML2_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}")
set(TINYXML2_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/tinyxml2.lib")

# Добавляем исполняемый файл и указываем исходный код
add_executable(my_pack src/main.cpp)

# Подключаем директории с заголовками и библиотеки CURL и TinyXML2
target_include_directories(my_pack PUBLIC ${CURL_INCLUDE_DIRS} ${TINYXML2_INCLUDE_DIRS})
target_link_libraries(my_pack ${CURL_LIBRARIES} ${TINYXML2_LIBRARIES})

# Копируем libcurl.dll и tinyxml2.dll в папку сборки рядом с исполняемым файлом
add_custom_command(TARGET my_pack POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/libcurl.dll"  # Путь к DLL libcurl
        $<TARGET_FILE_DIR:my_pack>              # Папка с исполняемым файлом
)

add_custom_command(TARGET my_pack POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/tinyxml2.dll" 
        $<TARGET_FILE_DIR:my_pack>              # Папка с исполняемым файлом
)

# Включаем дополнительные флаги для диагностики
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
