#ifndef USER_H
#define USER_H

#include <string>
#include <memory>
#include "database.h"

/**
 * @brief Класс для работы с пользователями системы
 */
class User {
public:
    /**
     * @brief Конструктор класса User
     * @param db Указатель на объект базы данных
     */
    User(std::shared_ptr<Database> db);
    
    /**
     * @brief Аутентификация пользователя
     * @param username Имя пользователя
     * @param password Пароль
     * @return true - если аутентификация успешна, false - в противном случае
     */
    bool authenticate(const std::string& username, const std::string& password);
    
    /**
     * @brief Создание нового пользователя
     * @param username Имя пользователя
     * @param password Пароль
     * @param role Роль пользователя
     * @param brokerID Идентификатор маклера (если пользователь - маклер)
     * @return true - если пользователь создан успешно, false - в противном случае
     */
    bool createUser(const std::string& username, const std::string& password, 
                   const std::string& role, int brokerID);
    
    /**
     * @brief Обновление пароля пользователя
     * @param newPassword Новый пароль
     * @return true - если пароль обновлен успешно, false - в противном случае
     */
    bool updatePassword(const std::string& newPassword);
    
    /**
     * @brief Получение роли пользователя
     * @return Роль пользователя
     */
    std::string getRole() const;
    
    /**
     * @brief Получение идентификатора маклера (если пользователь - маклер)
     * @return Идентификатор маклера
     */
    int getBrokerID() const;
    
    /**
     * @brief Получение имени пользователя
     * @return Имя пользователя
     */
    std::string getUsername() const;

private:
    std::shared_ptr<Database> db;  ///< Указатель на объект базы данных
    int userID;         ///< Идентификатор пользователя
    std::string username;  ///< Имя пользователя
    std::string role;      ///< Роль пользователя
    int brokerID;       ///< Идентификатор маклера
};

#endif // USER_H