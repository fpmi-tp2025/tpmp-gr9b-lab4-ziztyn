#include <gtest/gtest.h>
#include "product.h"
#include "database.h"
#include <memory>

class ProductTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем временную базу данных в памяти для тестов
        db = std::make_shared<Database>(":memory:");
        
        // Создаем необходимые таблицы
        db->executeQuery("CREATE TABLE Products (ProductID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, Type TEXT, UnitPrice REAL, SupplierID INTEGER, ExpiryDate TEXT, QuantitySupplied INTEGER)");
        db->executeQuery("CREATE TABLE Suppliers (SupplierID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT)");
        db->executeQuery("CREATE TABLE Transactions (TransactionID INTEGER PRIMARY KEY AUTOINCREMENT, TransactionDate TEXT, ProductID INTEGER, QuantitySold INTEGER, BrokerID INTEGER, BuyerID INTEGER)");
        
        // Добавляем тестовые данные
        db->executeQuery("INSERT INTO Suppliers (SupplierID, Name) VALUES (1, 'Парфюм-Люкс')");
        db->executeQuery("INSERT INTO Products (ProductID, Name, Type, UnitPrice, SupplierID, ExpiryDate, QuantitySupplied) VALUES (1, 'Шанель №5', 'Духи', 5000.0, 1, '2025-12-31', 100)");
        db->executeQuery("INSERT INTO Products (ProductID, Name, Type, UnitPrice, SupplierID, ExpiryDate, QuantitySupplied) VALUES (2, 'Диор Саваж', 'Туалетная вода', 3500.0, 1, '2026-06-30', 150)");
    }

    void TearDown() override {
        // Очистка ресурсов после каждого теста
        db.reset();
    }

    std::shared_ptr<Database> db;
};

// Тест 1: Проверка получения информации о товаре
TEST_F(ProductTest, GetProductInfo) {
    Product product(db, 1);
    EXPECT_EQ(product.getName(), "Шанель №5");
    EXPECT_EQ(product.getType(), "Духи");
    EXPECT_FLOAT_EQ(product.getUnitPrice(), 5000.0);
    EXPECT_EQ(product.getSupplierID(), 1);
    EXPECT_EQ(product.getExpiryDate(), "2025-12-31");
    EXPECT_EQ(product.getQuantitySupplied(), 100);
}

// Тест 2: Проверка обновления информации о товаре
TEST_F(ProductTest, UpdateProductInfo) {
    Product product(db, 1);
    
    // Обновляем информацию
    EXPECT_TRUE(product.updateInfo("Шанель №5 Премиум", "Духи", 5500.0, 1, "2026-01-31", 120));
    
    // Проверяем обновленную информацию
    EXPECT_EQ(product.getName(), "Шанель №5 Премиум");
    EXPECT_FLOAT_EQ(product.getUnitPrice(), 5500.0);
    EXPECT_EQ(product.getExpiryDate(), "2026-01-31");
    EXPECT_EQ(product.getQuantitySupplied(), 120);
}

// Тест 3: Проверка получения всех товаров
TEST_F(ProductTest, GetAllProducts) {
    auto products = Product::getAllProducts(db);
    EXPECT_EQ(products.size(), 2);
    EXPECT_EQ(products[0].getName(), "Шанель №5");
    EXPECT_EQ(products[1].getName(), "Диор Саваж");
}

// Тест 4: Проверка обновления количества товара
TEST_F(ProductTest, UpdateQuantity) {
    Product product(db, 1);
    EXPECT_EQ(product.getQuantitySupplied(), 100);
    
    // Уменьшаем количество
    EXPECT_TRUE(product.updateQuantity(80));
    EXPECT_EQ(product.getQuantitySupplied(), 80);
    
    // Увеличиваем количество
    EXPECT_TRUE(product.updateQuantity(120));
    EXPECT_EQ(product.getQuantitySupplied(), 120);
}

// Тест 5: Проверка добавления нового товара
TEST_F(ProductTest, AddNewProduct) {
    // Добавляем новый товар
    EXPECT_TRUE(Product::addProduct(db, "Герлен Шалимар", "Духи", 4800.0, 1, "2026-03-15", 50));
    
    // Проверяем, что товар добавлен
    Product product(db, 3);
    EXPECT_EQ(product.getName(), "Герлен Шалимар");
    EXPECT_EQ(product.getType(), "Духи");
    EXPECT_FLOAT_EQ(product.getUnitPrice(), 4800.0);
}