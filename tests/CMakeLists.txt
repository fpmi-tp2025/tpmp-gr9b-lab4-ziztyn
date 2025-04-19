# Находим Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Добавляем исходные файлы для тестов
set(TEST_SOURCES
    main.cpp           # Добавляем main.cpp
    test_user.cpp
    test_broker.cpp
    test_product.cpp
    test_transaction.cpp
    test_database.cpp
)

# Создаем тестовый исполняемый файл
add_executable(run_tests ${TEST_SOURCES})

# Линкуем с библиотекой тестов и основным кодом проекта
target_link_libraries(run_tests 
    ${GTEST_LIBRARIES} 
    pthread 
    ${SQLite3_LIBRARIES}
)

# Указываем пути к исходным файлам проекта
target_include_directories(run_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/includes
)

# Добавляем исходные файлы из основного проекта, исключая main.cpp
target_sources(run_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/user.cpp
    ${CMAKE_SOURCE_DIR}/src/broker.cpp
    ${CMAKE_SOURCE_DIR}/src/product.cpp
    ${CMAKE_SOURCE_DIR}/src/transaction.cpp
    ${CMAKE_SOURCE_DIR}/src/database.cpp
)

# Добавляем тест в систему CMake
add_test(NAME AllTests COMMAND run_tests)

# Копируем тестовую базу данных в директорию сборки тестов
add_custom_command(TARGET run_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data
)

# Создаем директорию для SQL-скриптов
add_custom_command(TARGET run_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/sql
)

# Копируем SQL-скрипт в директорию сборки тестов
add_custom_command(TARGET run_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/sql/init_db.sql
    ${CMAKE_CURRENT_BINARY_DIR}/sql/init_db.sql
    COMMENT "Copying SQL script for testing"
)

# Если SQLite3 найден, добавляем команду для создания тестовой базы данных
find_program(SQLITE3_EXECUTABLE sqlite3)
if(SQLITE3_EXECUTABLE)
    add_custom_command(TARGET run_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Initializing test database..."
        COMMAND ${SQLITE3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/data/test_db.sqlite ".read ${CMAKE_CURRENT_BINARY_DIR}/sql/init_db.sql"
        COMMENT "Initializing test database with SQL script"
    )
endif()
