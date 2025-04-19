#ifndef BROKER_H
#define BROKER_H

#include <string>
#include <vector>
#include <memory>
#include "database.h"

/**
 * @brief Структура для хранения информации о сделке
 */
struct TransactionInfo {
    int transactionID;     ///< Идентификатор сделки
    std::string date;      ///< Дата сделки
    int productID;         ///< Идентификатор товара
    int quantitySold;      ///< Количество проданных единиц
    int brokerID;          ///< Идентификатор маклера
    int buyerID;           ///< Идентификатор покупателя
};

/**
 * @brief Структура для хранения статистики маклера
 */
struct BrokerStatistics {
    int totalUnitsSold;    ///< Общее количество проданных единиц
    double totalAmount;    ///< Общая сумма продаж
};

/**
 * @brief Класс для работы с маклерами
 */
class Broker {
public:
    /**
     * @brief Конструктор класса Broker
     * @param db Указатель на объект базы данных
     * @param brokerID Идентификатор маклера
     */
    Broker(std::shared_ptr<Database> db, int brokerID);
    
    /**
     * @brief Получение фамилии маклера
     * @return Фамилия маклера
     */
    std::string getLastName() const;
    
    /**
     * @brief Получение адреса маклера
     * @return Адрес маклера
     */
    std::string getAddress() const;
    
    /**
     * @brief Получение года рождения маклера
     * @return Год рождения маклера
     */
    int getBirthYear() const;
    
    /**
     * @brief Обновление информации о маклере
     * @param lastName Фамилия маклера
     * @param address Адрес маклера
     * @param birthYear Год рождения маклера
     * @return true - если информация обновлена успешно, false - в противном случае
     */
    bool updateInfo(const std::string& lastName, const std::string& address, int birthYear);
    
    /**
     * @brief Создание новой сделки
     * @param date Дата сделки
     * @param productID Идентификатор товара
     * @param quantitySold Количество проданных единиц
     * @param buyerID Идентификатор покупателя
     * @return true - если сделка создана успешно, false - в противном случае
     */
    bool createTransaction(const std::string& date, int productID, int quantitySold, int buyerID);
    
    /**
     * @brief Получение всех сделок маклера
     * @return Вектор структур TransactionInfo со сделками маклера
     */
    std::vector<TransactionInfo> getTransactions() const;
    
    /**
     * @brief Получение сделок маклера за определенную дату
     * @param date Дата сделок
     * @return Вектор структур TransactionInfo со сделками маклера за указанную дату
     */
    std::vector<TransactionInfo> getTransactionsByDate(const std::string& date) const;
    
    /**
     * @brief Получение статистики маклера
     * @return Структура BrokerStatistics со статистикой маклера
     */
    BrokerStatistics getStatistics() const;
    
    /**
     * @brief Получение всех маклеров из базы данных
     * @param db Указатель на объект базы данных
     * @return Вектор структур с информацией о маклерах
     */
    static std::vector<std::pair<int, std::string>> getAllBrokers(std::shared_ptr<Database> db);

private:
    std::shared_ptr<Database> db;  ///< Указатель на объект базы данных
    int brokerID;       ///< Идентификатор маклера
    std::string lastName;  ///< Фамилия маклера
    std::string address;   ///< Адрес маклера
    int birthYear;      ///< Год рождения маклера
    
    /**
     * @brief Загрузка информации о маклере из базы данных
     */
    void loadBrokerInfo();
};

#endif // BROKER_H