#include <gtest/gtest.h>
#include "database.h"
#include <memory>

class DatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем временную базу данных в памяти для тестов
        db = std::make_shared<Database>(":memory:");
    }

    void TearDown() override {
        // Очистка ресурсов после каждого теста
        db.reset();
    }

    std::shared_ptr<Database> db;
};

// Тест 1: Проверка выполнения простого запроса CREATE TABLE
TEST_F(DatabaseTest, ExecuteCreateTableQuery) {
    auto result = db->executeQuery("CREATE TABLE TestTable (ID INTEGER PRIMARY KEY, Name TEXT)");
    EXPECT_TRUE(result.empty()); // DDL-запросы не возвращают строки
    
    // Проверяем, что таблица создана
    result = db->executeQuery("SELECT name FROM sqlite_master WHERE type='table' AND name='TestTable'");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0][0], "TestTable");
}

// Тест 2: Проверка выполнения запроса INSERT и получения данных
TEST_F(DatabaseTest, ExecuteInsertAndSelectQuery) {
    db->executeQuery("CREATE TABLE TestTable (ID INTEGER PRIMARY KEY, Name TEXT)");
    
    // Вставляем данные
    db->executeQuery("INSERT INTO TestTable (Name) VALUES ('Test1')");
    db->executeQuery("INSERT INTO TestTable (Name) VALUES ('Test2')");
    
    // Получаем данные
    auto result = db->executeQuery("SELECT ID, Name FROM TestTable ORDER BY ID");
    
    // Проверяем результат
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0][0], "1");
    EXPECT_EQ(result[0][1], "Test1");
    EXPECT_EQ(result[1][0], "2");
    EXPECT_EQ(result[1][1], "Test2");
}

// Тест 3: Проверка выполнения запроса UPDATE
TEST_F(DatabaseTest, ExecuteUpdateQuery) {
    db->executeQuery("CREATE TABLE TestTable (ID INTEGER PRIMARY KEY, Name TEXT)");
    db->executeQuery("INSERT INTO TestTable (Name) VALUES ('Test1')");
    
    // Обновляем данные
    db->executeQuery("UPDATE TestTable SET Name = 'UpdatedTest' WHERE ID = 1");
    
    // Проверяем результат
    auto result = db->executeQuery("SELECT Name FROM TestTable WHERE ID = 1");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0][0], "UpdatedTest");
}

// Тест 4: Проверка выполнения запроса DELETE
TEST_F(DatabaseTest, ExecuteDeleteQuery) {
    db->executeQuery("CREATE TABLE TestTable (ID INTEGER PRIMARY KEY, Name TEXT)");
    db->executeQuery("INSERT INTO TestTable (Name) VALUES ('Test1')");
    db->executeQuery("INSERT INTO TestTable (Name) VALUES ('Test2')");
    
    // Удаляем данные
    db->executeQuery("DELETE FROM TestTable WHERE ID = 1");
    
    // Проверяем результат
    auto result = db->executeQuery("SELECT COUNT(*) FROM TestTable");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0][0], "1");
    
    result = db->executeQuery("SELECT ID FROM TestTable");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0][0], "2");
}

// Тест 5: Проверка на обработку ошибок при выполнении некорректного запроса
TEST_F(DatabaseTest, HandleErrorInQuery) {
    try {
        db->executeQuery("SELECT * FROM NonExistentTable");
        FAIL() << "Expected std::runtime_error";
    }
    catch (const std::runtime_error& e) {
        EXPECT_TRUE(std::string(e.what()).find("no such table") != std::string::npos);
    }
    catch (...) {
        FAIL() << "Expected std::runtime_error";
    }
}