#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <sqlite3.h>
#include <memory>

/**
 * @brief Класс для работы с базой данных SQLite
 */
class Database {
public:
    /**
     * @brief Конструктор класса Database
     * @param dbPath Путь к файлу базы данных
     */
    Database(const std::string& dbPath);
    
    /**
     * @brief Деструктор класса Database
     */
    ~Database();
    
    /**
     * @brief Выполнение SQL-запроса
     * @param query SQL-запрос
     * @return Результат запроса в виде таблицы
     */
    std::vector<std::vector<std::string>> executeQuery(const std::string& query);
    
    /**
     * @brief Получение последнего идентификатора, назначенного в таблице
     * @return Последний идентификатор
     */
    int getLastInsertRowId() const;

private:
    sqlite3* db;  ///< Указатель на объект базы данных SQLite
};

#endif // DATABASE_H