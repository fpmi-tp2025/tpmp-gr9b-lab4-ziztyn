#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <memory>
#include "database.h"

/**
 * @brief Класс для работы со сделками
 */
class Transaction {
public:
    /**
     * @brief Конструктор класса Transaction
     * @param db Указатель на объект базы данных
     * @param transactionID Идентификатор сделки
     */
    Transaction(std::shared_ptr<Database> db, int transactionID);
    
    /**
     * @brief Получение даты сделки
     * @return Дата сделки
     */
    std::string getTransactionDate() const;
    
    /**
     * @brief Получение идентификатора товара
     * @return Идентификатор товара
     */
    int getProductID() const;
    
    /**
     * @brief Получение количества проданных единиц
     * @return Количество проданных единиц
     */
    int getQuantitySold() const;
    
    /**
     * @brief Получение идентификатора маклера
     * @return Идентификатор маклера
     */
    int getBrokerID() const;
    
    /**
     * @brief Получение идентификатора покупателя
     * @return Идентификатор покупателя
     */
    int getBuyerID() const;
    
    /**
     * @brief Создание новой сделки
     * @param db Указатель на объект базы данных
     * @param date Дата сделки
     * @param productID Идентификатор товара
     * @param quantitySold Количество проданных единиц
     * @param brokerID Идентификатор маклера
     * @param buyerID Идентификатор покупателя
     * @return true - если сделка создана успешно, false - в противном случае
     */
    static bool createTransaction(std::shared_ptr<Database> db, const std::string& date, int productID,
                              int quantitySold, int brokerID, int buyerID);
    
    /**
     * @brief Получение всех сделок из базы данных
     * @param db Указатель на объект базы данных
     * @return Вектор объектов класса Transaction
     */
    static std::vector<Transaction> getAllTransactions(std::shared_ptr<Database> db);
    
    /**
     * @brief Получение сделок за определенную дату
     * @param db Указатель на объект базы данных
     * @param date Дата сделок
     * @return Вектор объектов класса Transaction
     */
    static std::vector<Transaction> getTransactionsByDate(std::shared_ptr<Database> db, const std::string& date);
    
    /**
     * @brief Обновление количества товара и удаление сделок до определенной даты
     * @param db Указатель на объект базы данных
     * @param cutoffDate Дата отсечения
     * @return true - если операция выполнена успешно, false - в противном случае
     */
    static bool updateProductQuantities(std::shared_ptr<Database> db, const std::string& cutoffDate);

private:
    std::shared_ptr<Database> db;  ///< Указатель на объект базы данных
    int transactionID;    ///< Идентификатор сделки
    std::string date;     ///< Дата сделки
    int productID;        ///< Идентификатор товара
    int quantitySold;     ///< Количество проданных единиц
    int brokerID;         ///< Идентификатор маклера
    int buyerID;          ///< Идентификатор покупателя
    
    /**
     * @brief Загрузка информации о сделке из базы данных
     */
    void loadTransactionInfo();
    
    /**
     * @brief Обновление статистики маклера после создания сделки
     * @param db Указатель на объект базы данных
     * @param brokerID Идентификатор маклера
     * @param productID Идентификатор товара
     * @param quantitySold Количество проданных единиц
     * @return true - если статистика обновлена успешно, false - в противном случае
     */
    static bool updateBrokerStatistics(std::shared_ptr<Database> db, int brokerID, int productID, int quantitySold);
};

#endif // TRANSACTION_H
  