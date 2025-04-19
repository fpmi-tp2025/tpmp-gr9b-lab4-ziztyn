#include <gtest/gtest.h>
#include "transaction.h"
#include "database.h"
#include <memory>

class TransactionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем временную базу данных в памяти для тестов
        db = std::make_shared<Database>(":memory:");
        
        // Создаем необходимые таблицы
        db->executeQuery("CREATE TABLE Transactions (TransactionID INTEGER PRIMARY KEY AUTOINCREMENT, TransactionDate TEXT, ProductID INTEGER, QuantitySold INTEGER, BrokerID INTEGER, BuyerID INTEGER)");
        db->executeQuery("CREATE TABLE Products (ProductID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, Type TEXT, UnitPrice REAL, SupplierID INTEGER, ExpiryDate TEXT, QuantitySupplied INTEGER)");
        db->executeQuery("CREATE TABLE Brokers (BrokerID INTEGER PRIMARY KEY AUTOINCREMENT, LastName TEXT, Address TEXT, BirthYear INTEGER)");
        db->executeQuery("CREATE TABLE Buyers (BuyerID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT)");
        db->executeQuery("CREATE TABLE BrokerStatistics (BrokerID INTEGER PRIMARY KEY, TotalUnitsSold INTEGER, TotalAmount REAL)");
        
        // Добавляем тестовые данные
        db->executeQuery("INSERT INTO Products (ProductID, Name, Type, UnitPrice, SupplierID, ExpiryDate, QuantitySupplied) VALUES (1, 'Шанель №5', 'Духи', 5000.0, 1, '2025-12-31', 100)");
        db->executeQuery("INSERT INTO Brokers (BrokerID, LastName, Address, BirthYear) VALUES (1, 'Иванов', 'Москва, ул. Ленина 10', 1985)");
        db->executeQuery("INSERT INTO Buyers (BuyerID, Name) VALUES (1, 'МегаПарфюм')");
        db->executeQuery("INSERT INTO BrokerStatistics (BrokerID, TotalUnitsSold, TotalAmount) VALUES (1, 0, 0)");
        db->executeQuery("INSERT INTO Transactions (TransactionID, TransactionDate, ProductID, QuantitySold, BrokerID, BuyerID) VALUES (1, '2023-04-01', 1, 10, 1, 1)");
    }

    void TearDown() override {
        // Очистка ресурсов после каждого теста
        db.reset();
    }

    std::shared_ptr<Database> db;
};

// Тест 1: Проверка получения информации о сделке
TEST_F(TransactionTest, GetTransactionInfo) {
    Transaction transaction(db, 1);
    EXPECT_EQ(transaction.getTransactionDate(), "2023-04-01");
    EXPECT_EQ(transaction.getProductID(), 1);
    EXPECT_EQ(transaction.getQuantitySold(), 10);
    EXPECT_EQ(transaction.getBrokerID(), 1);
    EXPECT_EQ(transaction.getBuyerID(), 1);
}

// Тест 2: Проверка создания новой сделки
TEST_F(TransactionTest, CreateTransaction) {
    // Создаем новую сделку
    EXPECT_TRUE(Transaction::createTransaction(db, "2023-04-02", 1, 5, 1, 1));
    
    // Проверяем, что сделка создана
    Transaction transaction(db, 2);
    EXPECT_EQ(transaction.getTransactionDate(), "2023-04-02");
    EXPECT_EQ(transaction.getQuantitySold(), 5);
}

// Тест 3: Проверка обновления статистики после создания сделки
TEST_F(TransactionTest, StatisticsUpdateAfterTransaction) {
    // Проверяем начальное состояние статистики
    auto result = db->executeQuery("SELECT TotalUnitsSold, TotalAmount FROM BrokerStatistics WHERE BrokerID = 1");
    EXPECT_EQ(result[0][0], "0");
    EXPECT_EQ(result[0][1], "0.0");
    
    // Создаем новую сделку
    EXPECT_TRUE(Transaction::createTransaction(db, "2023-04-02", 1, 5, 1, 1));
    
    // Проверяем обновленную статистику
    result = db->executeQuery("SELECT TotalUnitsSold, TotalAmount FROM BrokerStatistics WHERE BrokerID = 1");
    EXPECT_EQ(result[0][0], "5");
    EXPECT_EQ(result[0][1], "25000.0"); // 5 * 5000.0
}

// Тест 4: Проверка получения всех сделок
TEST_F(TransactionTest, GetAllTransactions) {
    auto transactions = Transaction::getAllTransactions(db);
    EXPECT_EQ(transactions.size(), 1);
    EXPECT_EQ(transactions[0].getTransactionDate(), "2023-04-01");
    EXPECT_EQ(transactions[0].getQuantitySold(), 10);
    
    // Добавляем еще одну сделку
    EXPECT_TRUE(Transaction::createTransaction(db, "2023-04-02", 1, 5, 1, 1));
    
    // Проверяем, что получаем все сделки
    transactions = Transaction::getAllTransactions(db);
    EXPECT_EQ(transactions.size(), 2);
}

// Тест 5: Проверка функции обновления остатков товара
TEST_F(TransactionTest, UpdateProductQuantities) {
    // Проверяем начальное количество товара
    auto result = db->executeQuery("SELECT QuantitySupplied FROM Products WHERE ProductID = 1");
    EXPECT_EQ(result[0][0], "100");
    
    // Создаем несколько сделок
    EXPECT_TRUE(Transaction::createTransaction(db, "2023-04-02", 1, 5, 1, 1));
    EXPECT_TRUE(Transaction::createTransaction(db, "2023-04-03", 1, 3, 1, 1));
    
    // Обновляем остатки товара до 2023-04-02
    EXPECT_TRUE(Transaction::updateProductQuantities(db, "2023-04-02"));
    
    // Проверяем обновленное количество товара (должно уменьшиться на 5)
    result = db->executeQuery("SELECT QuantitySupplied FROM Products WHERE ProductID = 1");
    EXPECT_EQ(result[0][0], "85");
    
    // Проверяем, что сделка на 2023-04-02 удалена
    result = db->executeQuery("SELECT COUNT(*) FROM Transactions WHERE TransactionDate <= '2023-04-02'");
    EXPECT_EQ(result[0][0], "0");
    
    // Проверяем, что более поздняя сделка осталась
    result = db->executeQuery("SELECT COUNT(*) FROM Transactions");
    EXPECT_EQ(result[0][0], "1");
}