#include <gtest/gtest.h>
#include "broker.h"
#include "database.h"
#include <memory>

class BrokerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем временную базу данных в памяти для тестов
        db = std::make_shared<Database>(":memory:");
        
        // Создаем необходимые таблицы
        db->executeQuery("CREATE TABLE Brokers (BrokerID INTEGER PRIMARY KEY AUTOINCREMENT, LastName TEXT, Address TEXT, BirthYear INTEGER)");
        db->executeQuery("CREATE TABLE BrokerStatistics (BrokerID INTEGER PRIMARY KEY, TotalUnitsSold INTEGER, TotalAmount REAL)");
        db->executeQuery("CREATE TABLE Transactions (TransactionID INTEGER PRIMARY KEY AUTOINCREMENT, TransactionDate TEXT, ProductID INTEGER, QuantitySold INTEGER, BrokerID INTEGER, BuyerID INTEGER)");
        db->executeQuery("CREATE TABLE Products (ProductID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, Type TEXT, UnitPrice REAL, SupplierID INTEGER, ExpiryDate TEXT, QuantitySupplied INTEGER)");
        db->executeQuery("CREATE TABLE Suppliers (SupplierID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT)");
        db->executeQuery("CREATE TABLE Buyers (BuyerID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT)");
        
        // Добавляем тестовые данные
        db->executeQuery("INSERT INTO Brokers (BrokerID, LastName, Address, BirthYear) VALUES (1, 'Иванов', 'Москва, ул. Ленина 10', 1985)");
        db->executeQuery("INSERT INTO BrokerStatistics (BrokerID, TotalUnitsSold, TotalAmount) VALUES (1, 0, 0)");
        db->executeQuery("INSERT INTO Suppliers (SupplierID, Name) VALUES (1, 'Парфюм-Люкс')");
        db->executeQuery("INSERT INTO Buyers (BuyerID, Name) VALUES (1, 'МегаПарфюм')");
        db->executeQuery("INSERT INTO Products (ProductID, Name, Type, UnitPrice, SupplierID, ExpiryDate, QuantitySupplied) VALUES (1, 'Шанель №5', 'Духи', 5000.0, 1, '2025-12-31', 100)");
    }

    void TearDown() override {
        // Очистка ресурсов после каждого теста
        db.reset();
    }

    std::shared_ptr<Database> db;
};

// Тест 1: Проверка получения информации о маклере
TEST_F(BrokerTest, GetBrokerInfo) {
    Broker broker(db, 1);
    EXPECT_EQ(broker.getLastName(), "Иванов");
    EXPECT_EQ(broker.getAddress(), "Москва, ул. Ленина 10");
    EXPECT_EQ(broker.getBirthYear(), 1985);
}

// Тест 2: Проверка создания новой сделки
TEST_F(BrokerTest, CreateTransaction) {
    Broker broker(db, 1);
    
    // Создаем новую сделку
    EXPECT_TRUE(broker.createTransaction("2023-04-01", 1, 10, 1));
    
    // Проверяем, что сделка создана
    auto transactions = broker.getTransactions();
    EXPECT_EQ(transactions.size(), 1);
    EXPECT_EQ(transactions[0].productID, 1);
    EXPECT_EQ(transactions[0].quantitySold, 10);
    EXPECT_EQ(transactions[0].buyerID, 1);
}

// Тест 3: Проверка обновления статистики после создания сделки
TEST_F(BrokerTest, StatisticsUpdateAfterTransaction) {
    Broker broker(db, 1);
    
    // Создаем новую сделку
    EXPECT_TRUE(broker.createTransaction("2023-04-01", 1, 10, 1));
    
    // Проверяем статистику
    auto stats = broker.getStatistics();
    EXPECT_EQ(stats.totalUnitsSold, 10);
    EXPECT_FLOAT_EQ(stats.totalAmount, 50000.0); // 10 * 5000.0
}

// Тест 4: Проверка получения сделок за определенную дату
TEST_F(BrokerTest, GetTransactionsByDate) {
    Broker broker(db, 1);
    
    // Создаем сделки с разными датами
    EXPECT_TRUE(broker.createTransaction("2023-04-01", 1, 10, 1));
    EXPECT_TRUE(broker.createTransaction("2023-04-02", 1, 5, 1));
    EXPECT_TRUE(broker.createTransaction("2023-04-01", 1, 3, 1));
    
    // Получаем сделки за 2023-04-01
    auto transactions = broker.getTransactionsByDate("2023-04-01");
    EXPECT_EQ(transactions.size(), 2);
    EXPECT_EQ(transactions[0].quantitySold + transactions[1].quantitySold, 13);
}

// Тест 5: Проверка обновления данных маклера
TEST_F(BrokerTest, UpdateBrokerInfo) {
    Broker broker(db, 1);
    
    // Обновляем данные
    EXPECT_TRUE(broker.updateInfo("Петров", "Санкт-Петербург, пр. Невский 100", 1990));
    
    // Проверяем обновленные данные
    EXPECT_EQ(broker.getLastName(), "Петров");
    EXPECT_EQ(broker.getAddress(), "Санкт-Петербург, пр. Невский 100");
    EXPECT_EQ(broker.getBirthYear(), 1990);
}