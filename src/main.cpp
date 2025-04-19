#include <iostream>
#include <memory>
#include <string>
#include <iomanip>
#include <limits>
#include <vector>
#include "database.h"
#include "user.h"
#include "broker.h"
#include "product.h"
#include "transaction.h"

// Функция для очистки экрана (кроссплатформенная)
void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

// Функция для ожидания нажатия клавиши пользователем
void waitForKey() {
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Функция для отображения меню управления базаром
void showManagementMenu(std::shared_ptr<Database> db, User& user) {
    int choice = 0;
    
    while (true) {
        clearScreen();
        std::cout << "=== Парфюмерный базар (Управление) ===\n";
        std::cout << "Пользователь: " << user.getUsername() << " | Роль: " << user.getRole() << "\n\n";
        
        std::cout << "1. Просмотр информации о маклерах\n";
        std::cout << "2. Просмотр информации о товарах\n";
        std::cout << "3. Просмотр информации о сделках\n";
        std::cout << "4. Анализ продаж по товарам\n";
        std::cout << "5. Анализ продаж по фирмам-покупателям\n";
        std::cout << "6. Анализ работы маклеров\n";
        std::cout << "7. Анализ поставщиков\n";
        std::cout << "8. Обновление остатков товаров\n";
        std::cout << "9. Сменить пароль\n";
        std::cout << "0. Выход\n\n";
        
        std::cout << "Выберите действие: ";
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                // Просмотр информации о маклерах
                clearScreen();
                std::cout << "=== Информация о маклерах ===\n\n";
                
                auto result = db->executeQuery("SELECT BrokerID, LastName, Address, BirthYear FROM Brokers");
                
                if (result.empty()) {
                    std::cout << "Нет данных о маклерах.\n";
                } else {
                    std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Фамилия"
                              << std::setw(30) << "Адрес" << "Год рождения\n";
                    std::cout << std::string(70, '-') << "\n";
                    
                    for (const auto& row : result) {
                        std::cout << std::left << std::setw(5) << row[0] << std::setw(20) << row[1]
                                  << std::setw(30) << row[2] << row[3] << "\n";
                    }
                }
                
                waitForKey();
                break;
            }
            case 2: {
                // Просмотр информации о товарах
                clearScreen();
                std::cout << "=== Информация о товарах ===\n\n";
                
                auto result = db->executeQuery(
                    "SELECT p.ProductID, p.Name, p.Type, p.UnitPrice, s.Name, p.ExpiryDate, p.QuantitySupplied "
                    "FROM Products p "
                    "JOIN Suppliers s ON p.SupplierID = s.SupplierID"
                );
                
                if (result.empty()) {
                    std::cout << "Нет данных о товарах.\n";
                } else {
                    std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Название"
                              << std::setw(15) << "Вид" << std::setw(10) << "Цена" << std::setw(20)
                              << "Поставщик" << std::setw(15) << "Срок годности" << "Количество\n";
                    std::cout << std::string(100, '-') << "\n";
                    
                    for (const auto& row : result) {
                        std::cout << std::left << std::setw(5) << row[0] << std::setw(20) << row[1]
                                  << std::setw(15) << row[2] << std::setw(10) << row[3] << std::setw(20)
                                  << row[4] << std::setw(15) << row[5] << row[6] << "\n";
                    }
                }
                
                waitForKey();
                break;
            }
            case 3: {
                // Просмотр информации о сделках
                clearScreen();
                std::cout << "=== Информация о сделках ===\n\n";
                
                auto result = db->executeQuery(
                    "SELECT t.TransactionID, t.TransactionDate, p.Name, p.Type, t.QuantitySold, "
                    "b.LastName, buy.Name "
                    "FROM Transactions t "
                    "JOIN Products p ON t.ProductID = p.ProductID "
                    "JOIN Brokers b ON t.BrokerID = b.BrokerID "
                    "JOIN Buyers buy ON t.BuyerID = buy.BuyerID"
                );
                
                if (result.empty()) {
                    std::cout << "Нет данных о сделках.\n";
                } else {
                    std::cout << std::left << std::setw(5) << "ID" << std::setw(12) << "Дата"
                              << std::setw(20) << "Товар" << std::setw(15) << "Вид" << std::setw(10)
                              << "Кол-во" << std::setw(15) << "Маклер" << "Покупатель\n";
                    std::cout << std::string(100, '-') << "\n";
                    
                    for (const auto& row : result) {
                        std::cout << std::left << std::setw(5) << row[0] << std::setw(12) << row[1]
                                  << std::setw(20) << row[2] << std::setw(15) << row[3] << std::setw(10)
                                  << row[4] << std::setw(15) << row[5] << row[6] << "\n";
                    }
                }
                
                waitForKey();
                break;
            }
            case 4: {
                // Анализ продаж по товарам
                clearScreen();
                std::cout << "=== Анализ продаж по товарам ===\n\n";
                
                std::string startDate, endDate;
                std::cout << "Введите начальную дату (ГГГГ-ММ-ДД): ";
                std::getline(std::cin, startDate);
                std::cout << "Введите конечную дату (ГГГГ-ММ-ДД): ";
                std::getline(std::cin, endDate);
                
                auto result = db->executeQuery(
                    "SELECT p.Name, p.Type, SUM(t.QuantitySold) as TotalSold, "
                    "SUM(t.QuantitySold * p.UnitPrice) as TotalAmount "
                    "FROM Transactions t "
                    "JOIN Products p ON t.ProductID = p.ProductID "
                    "WHERE t.TransactionDate BETWEEN '" + startDate + "' AND '" + endDate + "' "
                    "GROUP BY p.Name, p.Type "
                    "ORDER BY TotalAmount DESC"
                );
                
                if (result.empty()) {
                    std::cout << "Нет данных о продажах за указанный период.\n";
                } else {
                    std::cout << std::left << std::setw(20) << "Товар" << std::setw(15) << "Вид"
                              << std::setw(15) << "Кол-во продано" << "Общая стоимость\n";
                    std::cout << std::string(70, '-') << "\n";
                    
                    for (const auto& row : result) {
                        std::cout << std::left << std::setw(20) << row[0] << std::setw(15) << row[1]
                                  << std::setw(15) << row[2] << row[3] << "\n";
                    }
                }
                
                waitForKey();
                break;
            }
            case 5: {
                // Анализ продаж по фирмам-покупателям
                clearScreen();
                std::cout << "=== Анализ продаж по фирмам-покупателям ===\n\n";
                
                auto products = db->executeQuery("SELECT ProductID, Name FROM Products");
                
                std::cout << "Выберите товар:\n";
                for (size_t i = 0; i < products.size(); ++i) {
                    std::cout << i + 1 << ". " << products[i][1] << "\n";
                }
                
                std::cout << "\nВведите номер товара: ";
                int productChoice;
                std::cin >> productChoice;
                
                if (std::cin.fail() || productChoice < 1 || productChoice > static_cast(products.size())) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Некорректный выбор.\n";
                    waitForKey();
                    break;
                }
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                int productID = std::stoi(products[productChoice - 1][0]);
                
                auto result = db->executeQuery(
                    "SELECT b.Name, SUM(t.QuantitySold) as TotalSold, "
                    "SUM(t.QuantitySold * p.UnitPrice) as TotalAmount "
                    "FROM Transactions t "
                    "JOIN Products p ON t.ProductID = p.ProductID "
                    "JOIN Buyers b ON t.BuyerID = b.BuyerID "
                    "WHERE t.ProductID = " + std::to_string(productID) + " "
                    "GROUP BY b.Name "
                    "ORDER BY TotalAmount DESC"
                );
                
                clearScreen();
                std::cout << "=== Анализ продаж товара '" << products[productChoice - 1][1] << "' по фирмам-покупателям ===\n\n";
                
                if (result.empty()) {
                    std::cout << "Нет данных о продажах для указанного товара.\n";
                } else {
                    std::cout << std::left << std::setw(30) << "Фирма-покупатель" << std::setw(15)
                              << "Кол-во единиц" << "Общая стоимость\n";
                    std::cout << std::string(70, '-') << "\n";
                    
                    for (const auto& row : result) {
                        std::cout << std::left << std::setw(30) << row[0] << std::setw(15) << row[1]
                                  << row[2] << "\n";
                    }
                }
                
                waitForKey();
                break;
            }
            case 6: {
                // Анализ работы маклеров
                clearScreen();
                std::cout << "=== Анализ работы маклеров ===\n\n";
                
                // Находим маклера с максимальным количеством сделок
                auto topBrokerResult = db->executeQuery(
                    "SELECT b.BrokerID, b.LastName, COUNT(t.TransactionID) as TransactionCount "
                    "FROM Brokers b "
                    "JOIN Transactions t ON b.BrokerID = t.BrokerID "
                    "GROUP BY b.BrokerID, b.LastName "
                    "ORDER BY TransactionCount DESC "
                    "LIMIT 1"
                );
                
                if (topBrokerResult.empty()) {
                    std::cout << "Нет данных о сделках маклеров.\n";
                } else {
                    int topBrokerID = std::stoi(topBrokerResult[0][0]);
                    std::string topBrokerName = topBrokerResult[0][1];
                    
                    std::cout << "Маклер с максимальным количеством сделок: " << topBrokerName
                              << " (ID: " << topBrokerID << ")\n";
                    std::cout << "Количество сделок: " << topBrokerResult[0][2] << "\n\n";
                    
                    // Информация о маклере и фирмах-поставщиках
                    auto brokerInfo = db->executeQuery(
                        "SELECT b.LastName, b.Address, b.BirthYear "
                        "FROM Brokers b "
                        "WHERE b.BrokerID = " + std::to_string(topBrokerID)
                    );
                    
                    std::cout << "Сведения о маклере:\n";
                    std::cout << "Фамилия: " << brokerInfo[0][0] << "\n";
                    std::cout << "Адрес: " << brokerInfo[0][1] << "\n";
                    std::cout << "Год рождения: " << brokerInfo[0][2] << "\n\n";
                    
                    // Информация о фирмах-поставщиках
                    auto supplierResult = db->executeQuery(
                        "SELECT DISTINCT s.Name "
                        "FROM Suppliers s "
                        "JOIN Products p ON s.SupplierID = p.SupplierID "
                        "JOIN Transactions t ON p.ProductID = t.ProductID "
                        "WHERE t.BrokerID = " + std::to_string(topBrokerID)
                    );
                    
                    std::cout << "Фирмы-поставщики:\n";
                    if (supplierResult.empty()) {
                        std::cout << "Нет данных о фирмах-поставщиках.\n";
                    } else {
                        for (const auto& row : supplierResult) {
                            std::cout << "- " << row[0] << "\n";
                        }
                    }
                }
                
                waitForKey();
                break;
            }
            case 7: {
                // Анализ поставщиков
                clearScreen();
                std::cout << "=== Анализ поставщиков ===\n\n";
                
                auto suppliers = db->executeQuery("SELECT SupplierID, Name FROM Suppliers");
                
                if (suppliers.empty()) {
                    std::cout << "Нет данных о поставщиках.\n";
                    waitForKey();
                    break;
                }
                
                std::cout << "Выберите поставщика:\n";
                for (size_t i = 0; i < suppliers.size(); ++i) {
                    std::cout << i + 1 << ". " << suppliers[i][1] << "\n";
                }
                
                std::cout << "\nВведите номер поставщика: ";
                int supplierChoice;
                std::cin >> supplierChoice;
                
                if (std::cin.fail() || supplierChoice < 1 || supplierChoice > static_cast(suppliers.size())) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Некорректный выбор.\n";
                    waitForKey();
                    break;
                }
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                int supplierID = std::stoi(suppliers[supplierChoice - 1][0]);
                
                auto result = db->executeQuery(
                    "SELECT b.LastName, COUNT(t.TransactionID) as TransactionCount, "
                    "SUM(t.QuantitySold) as TotalSold, "
                    "SUM(t.QuantitySold * p.UnitPrice) as TotalAmount "
                    "FROM Transactions t "
                    "JOIN Products p ON t.ProductID = p.ProductID "
                    "JOIN Brokers b ON t.BrokerID = b.BrokerID "
                    "WHERE p.SupplierID = " + std::to_string(supplierID) + " "
                    "GROUP BY b.LastName "
                    "ORDER BY TotalAmount DESC"
                );
                
                clearScreen();
                std::cout << "=== Анализ продаж товаров поставщика '" << suppliers[supplierChoice - 1][1]
                          << "' по маклерам ===\n\n";
                
                if (result.empty()) {
                    std::cout << "Нет данных о продажах для указанного поставщика.\n";
                } else {
                    std::cout << std::left << std::setw(20) << "Маклер" << std::setw(15) << "Кол-во сделок"
                              << std::setw(15) << "Кол-во единиц" << "Общая стоимость\n";
                    std::cout << std::string(70, '-') << "\n";
                    
                    for (const auto& row : result) {
                        std::cout << std::left << std::setw(20) << row[0] << std::setw(15) << row[1]
                                  << std::setw(15) << row[2] << row[3] << "\n";
                    }
                }
                
                waitForKey();
                break;
            }
            case 8: {
                // Обновление остатков товаров
                clearScreen();
                std::cout << "=== Обновление остатков товаров ===\n\n";
                
                std::string cutoffDate;
                std::cout << "Введите дату отсечения (ГГГГ-ММ-ДД): ";
                std::getline(std::cin, cutoffDate);
                
                if (Transaction::updateProductQuantities(db, cutoffDate)) {
                    std::cout << "Остатки товаров успешно обновлены.\n";
                    std::cout << "Сделки до " << cutoffDate << " удалены из базы данных.\n";
                } else {
                    std::cout << "Ошибка при обновлении остатков товаров.\n";
                }
                
                waitForKey();
                break;
            }
            case 9: {
                // Сменить пароль
                clearScreen();
                std::cout << "=== Сменить пароль ===\n\n";
                
                std::string newPassword;
                std::cout << "Введите новый пароль: ";
                std::getline(std::cin, newPassword);
                
                if (user.updatePassword(newPassword)) {
                    std::cout << "Пароль успешно изменен.\n";
                } else {
                    std::cout << "Ошибка при изменении пароля.\n";
                }
                
                waitForKey();
                break;
            }
            case 0:
                // Выход
                return;
            default:
                std::cout << "Некорректный выбор. Попробуйте еще раз.\n";
                waitForKey();
        }
    }
}

// Функция для отображения меню маклера
void showBrokerMenu(std::shared_ptr<Database> db, User& user, Broker& broker) {
    int choice = 0;
    
    while (true) {
        clearScreen();
        std::cout << "=== Парфюмерный базар (Маклер) ===\n";
        std::cout << "Пользователь: " << user.getUsername() << " | Роль: " << user.getRole()
                  << " | Маклер: " << broker.getLastName() << "\n\n";
        
        std::cout << "1. Просмотр своих сделок\n";
        std::cout << "2. Оформление новой сделки\n";
        std::cout << "3. Просмотр статистики продаж\n";
        std::cout << "4. Просмотр товаров в наличии\n";
        std::cout << "5. Анализ товаров с наибольшим спросом\n";
        std::cout << "6. Сменить пароль\n";
        std::cout << "0. Выход\n\n";
        
        std::cout << "Выберите действие: ";
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                // Просмотр своих сделок
                clearScreen();
                std::cout << "=== Мои сделки ===\n\n";
                
                auto transactions = broker.getTransactions();
                
                if (transactions.empty()) {
                    std::cout << "У вас пока нет сделок.\n";
                } else {
                    std::cout << std::left << std::setw(5) << "ID" << std::setw(12) << "Дата"
                              << std::setw(10) << "Товар ID" << std::setw(10) << "Кол-во"
                              << "Покупатель ID\n";
                    std::cout << std::string(50, '-') << "\n";
                    
                    for (const auto& t : transactions) {
                        std::cout << std::left << std::setw(5) << t.transactionID << std::setw(12) << t.date
                                  << std::setw(10) << t.productID << std::setw(10) << t.quantitySold
                                  << t.buyerID << "\n";
                    }
                }
                
                waitForKey();
                break;
            }
            case 2: {
                // Оформление новой сделки
                clearScreen();
                std::cout << "=== Оформление новой сделки ===\n\n";
                
                // Получаем список товаров
                auto products = db->executeQuery(
                    "SELECT ProductID, Name, Type, UnitPrice, QuantitySupplied FROM Products WHERE QuantitySupplied > 0"
                );
                
                if (products.empty()) {
                    std::cout << "Нет доступных товаров.\n";
                    waitForKey();
                    break;
                }
                
                std::cout << "Доступные товары:\n";
                std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Название"
                          << std::setw(15) << "Вид" << std::setw(10) << "Цена" << "Доступно\n";
                std::cout << std::string(70, '-') << "\n";
                
                for (const auto& row : products) {
                    std::cout << std::left << std::setw(5) << row[0] << std::setw(20) << row[1]
                              << std::setw(15) << row[2] << std::setw(10) << row[3] << row[4] << "\n";
                }
                
                std::cout << "\nВведите ID товара: ";
                int productID;
                std::cin >> productID;
                
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Некорректный ввод.\n";
                    waitForKey();
                    break;
                }
                
                bool validProduct = false;
                int availableQuantity = 0;
                
                for (const auto& row : products) {
                    if (std::stoi(row[0]) == productID) {
                        validProduct = true;
                        availableQuantity = std::stoi(row[4]);
                        break;
                    }
                }
                
                if (!validProduct) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Товар с указанным ID не найден.\n";
                    waitForKey();
                    break;
                }
                
                std::cout << "Введите количество единиц: ";
                int quantity;
                std::cin >> quantity;
                
                if (std::cin.fail() || quantity <= 0 || quantity > availableQuantity) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Некорректное количество.\n";
                    waitForKey();
                    break;
                }
                
                // Получаем список покупателей
                auto buyers = db->executeQuery("SELECT BuyerID, Name FROM Buyers");
                
                if (buyers.empty()) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Нет доступных покупателей.\n";
                    waitForKey();
                    break;
                }
                
                std::cout << "\nДоступные покупатели:\n";
                for (const auto& row : buyers) {
                    std::cout << row[0] << ". " << row[1] << "\n";
                }
                
                std::cout << "\nВведите ID покупателя: ";
                int buyerID;
                std::cin >> buyerID;
                
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Некорректный ввод.\n";
                    waitForKey();
                    break;
                }
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                bool validBuyer = false;
                
                for (const auto& row : buyers) {
                    if (std::stoi(row[0]) == buyerID) {
                        validBuyer = true;
                        break;
                    }
                }
                
                if (!validBuyer) {
                    std::cout << "Покупатель с указанным ID не найден.\n";
                    waitForKey();
                    break;
                }
                
                // Получаем текущую дату (для простоты используем ввод пользователя)
                std::string date;
                std::cout << "Введите дату сделки (ГГГГ-ММ-ДД): ";
                std::getline(std::cin, date);
                
                // Создаем сделку
                if (broker.createTransaction(date, productID, quantity, buyerID)) {
                    std::cout << "Сделка успешно создана.\n";
                    
                    // Обновляем количество товара
                    Product product(db, productID);
                    product.updateQuantity(product.getQuantitySupplied() - quantity);
                } else {
                    std::cout << "Ошибка при создании сделки.\n";
                }
                
                waitForKey();
                break;
            }
            case 3: {
                // Просмотр статистики продаж
                clearScreen();
                std::cout << "=== Моя статистика продаж ===\n\n";
                
                auto stats = broker.getStatistics();
                
                std::cout << "Общее количество проданных единиц: " << stats.totalUnitsSold << "\n";
                std::cout << "Общая сумма продаж: " << std::fixed << std::setprecision(2) << stats.totalAmount << "\n";
                
                waitForKey();
                break;
            }
            case 4: {
                // Просмотр товаров в наличии
                clearScreen();
                std::cout << "=== Товары в наличии ===\n\n";
                
                auto result = db->executeQuery(
                    "SELECT p.ProductID, p.Name, p.Type, p.UnitPrice, s.Name, p.ExpiryDate, p.QuantitySupplied "
                    "FROM Products p "
                    "JOIN Suppliers s ON p.SupplierID = s.SupplierID "
                    "WHERE p.QuantitySupplied > 0"
                );
                
                if (result.empty()) {
                    std::cout << "Нет товаров в наличии.\n";
                } else {
                    std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Название"
                              << std::setw(15) << "Вид" << std::setw(10) << "Цена" << std::setw(20)
                              << "Поставщик" << std::setw(15) << "Срок годности" << "Количество\n";
                    std::cout << std::string(100, '-') << "\n";
                    
                    for (const auto& row : result) {
                        std::cout << std::left << std::setw(5) << row[0] << std::setw(20) << row[1]
                                  << std::setw(15) << row[2] << std::setw(10) << row[3] << std::setw(20)
                                  << row[4] << std::setw(15) << row[5] << row[6] << "\n";
                    }
                }
                
                waitForKey();
                break;
            }
            case 5: {
                // Анализ товаров с наибольшим спросом
                clearScreen();
                std::cout << "=== Товары с наибольшим спросом ===\n\n";
                
                // Находим вид товара с наибольшим спросом
                auto popularTypeResult = db->executeQuery(
                    "SELECT p.Type, SUM(t.QuantitySold) as TotalSold "
                    "FROM Transactions t "
                    "JOIN Products p ON t.ProductID = p.ProductID "
                    "GROUP BY p.Type "
                    "ORDER BY TotalSold DESC "
                    "LIMIT 1"
                );
                
                if (popularTypeResult.empty()) {
                    std::cout << "Нет данных о продажах.\n";
                } else {
                    std::string popularType = popularTypeResult[0][0];
                    
                    std::cout << "Вид товара с наибольшим спросом: " << popularType << "\n";
                    std::cout << "Общее количество проданных единиц: " << popularTypeResult[0][1] << "\n\n";
                    
                    // Получаем детальную информацию о продажах этого вида товара по фирмам-покупателям
                    auto result = db->executeQuery(
                        "SELECT b.Name, SUM(t.QuantitySold) as TotalSold, "
                        "SUM(t.QuantitySold * p.UnitPrice) as TotalAmount "
                        "FROM Transactions t "
                        "JOIN Products p ON t.ProductID = p.ProductID "
                        "JOIN Buyers b ON t.BuyerID = b.BuyerID "
                        "WHERE p.Type = '" + popularType + "' "
                        "GROUP BY b.Name "
                        "ORDER BY TotalAmount DESC"
                    );
                    
                    std::cout << "Продажи по фирмам-покупателям:\n";
                    std::cout << std::left << std::setw(30) << "Фирма-покупатель" << std::setw(15)
                              << "Кол-во единиц" << "Общая стоимость\n";
                    std::cout << std::string(70, '-') << "\n";
                    
                    for (const auto& row : result) {
                        std::cout << std::left << std::setw(30) << row[0] << std::setw(15) << row[1]
                                  << row[2] << "\n";
                    }
                }
                
                waitForKey();
                break;
            }
            case 6: {
                // Сменить пароль
                clearScreen();
                std::cout << "=== Сменить пароль ===\n\n";
                
                std::string newPassword;
                std::cout << "Введите новый пароль: ";
                std::getline(std::cin, newPassword);
                
                if (user.updatePassword(newPassword)) {
                    std::cout << "Пароль успешно изменен.\n";
                } else {
                    std::cout << "Ошибка при изменении пароля.\n";
                }
                
                waitForKey();
                break;
            }
            case 0:
                // Выход
                return;
            default:
                std::cout << "Некорректный выбор. Попробуйте еще раз.\n";
                waitForKey();
        }
    }
}

// Функция для отображения меню авторизации
void showLoginMenu(std::shared_ptr<Database> db) {
    int choice = 0;
    
    while (true) {
        clearScreen();
        std::cout << "=== Парфюмерный базар ===\n\n";
        std::cout << "1. Вход в систему\n";
        std::cout << "0. Выход\n\n";
        
        std::cout << "Выберите действие: ";
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                // Вход в систему
                clearScreen();
                std::cout << "=== Вход в систему ===\n\n";
                
                std::string username, password;
                std::cout << "Имя пользователя: ";
                std::getline(std::cin, username);
                std::cout << "Пароль: ";
                std::getline(std::cin, password);
                
                User user(db);
                
                if (user.authenticate(username, password)) {
                    std::cout << "Вход выполнен успешно.\n";
                    
                    if (user.getRole() == "Management") {
                        showManagementMenu(db, user);
                    } else if (user.getRole() == "Broker") {
                        Broker broker(db, user.getBrokerID());
                        showBrokerMenu(db, user, broker);
                    }
                } else {
                    std::cout << "Ошибка входа. Проверьте имя пользователя и пароль.\n";
                    waitForKey();
                }
                
                break;
            }
            case 0:
                // Выход
                return;
            default:
                std::cout << "Некорректный выбор. Попробуйте еще раз.\n";
                waitForKey();
        }
    }
}

int main() {
    try {
        // Подключаемся к базе данных
        std::shared_ptr<Database> db = std::make_shared<Database>("data/perfume_db.sqlite");
        
        // Отображаем меню авторизации
        showLoginMenu(db);
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
}