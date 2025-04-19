cmake_minimum_required(VERSION 3.10)
project(PerfumeBazaar VERSION 1.0)

# Находим необходимые пакеты
find_package(SQLite3 REQUIRED)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем тесты
enable_testing()

# Указываем пути к включаемым файлам
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${SQLite3_INCLUDE_DIRS}
)

# Указываем исходные файлы проекта
set(SOURCE_FILES
    src/main.cpp
    src/user.cpp
    src/broker.cpp
    src/product.cpp
    src/transaction.cpp
    src/database.cpp
)

# Создаем исполняемый файл
add_executable(perfume_bazaar ${SOURCE_FILES})

# Линкуем с SQLite
target_link_libraries(perfume_bazaar ${SQLite3_LIBRARIES})

# Создаем директорию для данных при сборке
add_custom_command(TARGET perfume_bazaar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data
)

# Копируем базу данных, если она существует, иначе создаем пустую базу
add_custom_command(TARGET perfume_bazaar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/data/perfume_db.sqlite
    ${CMAKE_CURRENT_BINARY_DIR}/data/perfume_db.sqlite
    COMMENT "Copying database file (if exists)"
)

# Копируем SQL-скрипт в директорию сборки
add_custom_command(TARGET perfume_bazaar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/sql/init_db.sql
    ${CMAKE_CURRENT_BINARY_DIR}/sql/init_db.sql
    COMMENT "Copying SQL script (if exists)"
)

# Находим SQLite3 для выполнения скрипта
find_program(SQLITE3_EXECUTABLE sqlite3)

# Если SQLite3 найден, добавляем команду для создания базы данных
if(SQLITE3_EXECUTABLE)
    add_custom_command(TARGET perfume_bazaar POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Initializing database..."
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/sql
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/sql/init_db.sql ${CMAKE_CURRENT_BINARY_DIR}/sql/
        COMMAND ${SQLITE3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/data/perfume_db.sqlite ".read ${CMAKE_CURRENT_BINARY_DIR}/sql/init_db.sql"
        COMMENT "Initializing database with SQL script"
    )
endif()

# Добавляем тесты, если они есть
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    add_subdirectory(tests)
endif()