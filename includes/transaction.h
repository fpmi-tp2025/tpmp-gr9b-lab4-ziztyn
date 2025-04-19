#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>
#include <memory>
#include "database.h"

/**
 * @brief Структура для хранения информации о товаре
 */
struct ProductInfo {
    int productID;         ///< Идентификатор товара
    std::string name;      ///< Название товара
    std::string type;      ///< Вид товара
    double unitPrice;      ///< Цена единицы товара
    int supplierID;        ///< Идентификатор поставщика
    std::string expiryDate; ///< Срок годности
    int quantitySupplied;  ///< Количество поставленных единиц
};

/**
 * @brief Класс для работы с товарами
 */
class Product {
public:
    /**
     * @brief Конструктор класса Product
     * @param db Указатель на объект базы данных
     * @param productID Идентификатор товара
     */
    Product(std::shared_ptr<Database> db, int productID);
    
    /**
     * @brief Получение названия товара
     * @return Название товара
     */
    std::string getName() const;
    
    /**
     * @brief Получение вида товара
     * @return Вид товара
     */
    std::string getType() const;
    
    /**
     * @brief Получение цены единицы товара
     * @return Цена единицы товара
     */
    double getUnitPrice() const;
    
    /**
     * @brief Получение идентификатора поставщика
     * @return Идентификатор поставщика
     */
    int getSupplierID() const;
    
    /**
     * @brief Получение срока годности товара
     * @return Срок годности товара
     */
    std::string getExpiryDate() const;
    
    /**
     * @brief Получение количества поставленных единиц товара
     * @return Количество поставленных единиц товара
     */
    int getQuantitySupplied() const;
    
    /**
     * @brief Обновление информации о товаре
     * @param name Название товара
     * @param type Вид товара
     * @param unitPrice Цена единицы товара
     * @param supplierID Идентификатор поставщика
     * @param expiryDate Срок годности
     * @param quantitySupplied Количество поставленных единиц
     * @return true - если информация обновлена успешно, false - в противном случае
     */
    bool updateInfo(const std::string& name, const std::string& type, double unitPrice,
                   int supplierID, const std::string& expiryDate, int quantitySupplied);
    
    /**
     * @brief Обновление количества поставленных единиц товара
     * @param newQuantity Новое количество поставленных единиц
     * @return true - если количество обновлено успешно, false - в противном случае
     */
    bool updateQuantity(int newQuantity);
    
    /**
     * @brief Получение всех товаров из базы данных
     * @param db Указатель на объект базы данных
     * @return Вектор объектов класса Product
     */
    static std::vector<Product> getAllProducts(std::shared_ptr<Database> db);
    
    /**
     * @brief Добавление нового товара в базу данных
     * @param db Указатель на объект базы данных
     * @param name Название товара
     * @param type Вид товара
     * @param unitPrice Цена единицы товара
     * @param supplierID Идентификатор поставщика
     * @param expiryDate Срок годности
     * @param quantitySupplied Количество поставленных единиц
     * @return true - если товар добавлен успешно, false - в противном случае
     */
    static bool addProduct(std::shared_ptr<Database> db, const std::string& name, const std::string& type,
                         double unitPrice, int supplierID, const std::string& expiryDate, int quantitySupplied);

private:
    std::shared_ptr<Database> db;  ///< Указатель на объект базы данных
    int productID;       ///< Идентификатор товара
    std::string name;    ///< Название товара
    std::string type;    ///< Вид товара
    double unitPrice;    ///< Цена единицы товара
    int supplierID;      ///< Идентификатор поставщика
    std::string expiryDate; ///< Срок годности
    int quantitySupplied; ///< Количество поставленных единиц
    
    /**
     * @brief Загрузка информации о товаре из базы данных
     */
    void loadProductInfo();
};

#endif // PRODUCT_H