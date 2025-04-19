#include <gtest/gtest.h>
#include "user.h"
#include "database.h"
#include <memory>

class UserTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем временную базу данных в памяти для тестов
        db = std::make_shared<Database>(":memory:");
        db->executeQuery("CREATE TABLE Users (UserID INTEGER PRIMARY KEY AUTOINCREMENT, Username TEXT UNIQUE, Password TEXT, Role TEXT, BrokerID INTEGER)");
        db->executeQuery("INSERT INTO Users (Username, Password, Role, BrokerID) VALUES ('admin', 'admin123', 'Management', NULL)");
        db->executeQuery("INSERT INTO Users (Username, Password, Role, BrokerID) VALUES ('ivanov', 'pass123', 'Broker', 1)");
    }

    void TearDown() override {
        // Очистка ресурсов после каждого теста
        db.reset();
    }

    std::shared_ptr<Database> db;
};

// Тест 1: Проверка аутентификации с правильными учетными данными
TEST_F(UserTest, AuthenticateWithValidCredentials) {
    User user(db);
    EXPECT_TRUE(user.authenticate("admin", "admin123"));
    EXPECT_EQ(user.getRole(), "Management");
    EXPECT_EQ(user.getBrokerID(), 0);
}

// Тест 2: Проверка аутентификации с неправильным паролем
TEST_F(UserTest, AuthenticateWithInvalidPassword) {
    User user(db);
    EXPECT_FALSE(user.authenticate("admin", "wrongpassword"));
}

// Тест 3: Проверка аутентификации с несуществующим пользователем
TEST_F(UserTest, AuthenticateWithNonExistentUser) {
    User user(db);
    EXPECT_FALSE(user.authenticate("nonexistent", "password"));
}

// Тест 4: Проверка создания нового пользователя
TEST_F(UserTest, CreateNewUser) {
    User user(db);
    EXPECT_TRUE(user.createUser("newuser", "newpass", "Broker", 2));
    
    // Проверяем, что пользователь создан
    EXPECT_TRUE(user.authenticate("newuser", "newpass"));
    EXPECT_EQ(user.getRole(), "Broker");
    EXPECT_EQ(user.getBrokerID(), 2);
}

// Тест 5: Проверка обновления пароля пользователя
TEST_F(UserTest, UpdateUserPassword) {
    User user(db);
    EXPECT_TRUE(user.authenticate("ivanov", "pass123"));
    
    // Обновляем пароль
    EXPECT_TRUE(user.updatePassword("newpassword"));
    
    // Проверяем, что пароль обновлен
    EXPECT_FALSE(user.authenticate("ivanov", "pass123"));
    EXPECT_TRUE(user.authenticate("ivanov", "newpassword"));
}
  