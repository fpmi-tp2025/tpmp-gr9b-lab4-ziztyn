#include "transaction.h"
#include "product.h"

Transaction::Transaction(std::shared_ptr<Database> db, int transactionID) : db(db), transactionID(transactionID) {
    loadTransactionInfo();
}

void Transaction::loadTransactionInfo() {
    try {
        auto result = db->executeQuery(
            "SELECT TransactionDate, ProductID, QuantitySold, BrokerID, BuyerID "
            "FROM Transactions WHERE TransactionID = " + std::to_string(transactionID)
        );
        
        if (!result.empty()) {
            date = result[0][0];
            productID = std::stoi(result[0][1]);
            quantitySold = std::stoi(result[0][2]);
            brokerID = std::stoi(result[0][3]);
            buyerID = std::stoi(result[0][4]);
        }
    } catch (const std::exception& e) {
        // Обработка ошибок
    }
}

std::string Transaction::getTransactionDate() const {
    return date;
}

int Transaction::getProductID() const {
    return productID;
}

int Transaction::getQuantitySold() const {
    return quantitySold;
}

int Transaction::getBrokerID() const {
    return brokerID;
}

int Transaction::getBuyerID() const {
    return buyerID;
}

bool Transaction::createTransaction(std::shared_ptr<Database> db, const std::string& date, int productID,
                                int quantitySold, int brokerID, int buyerID) {
    try {
        db->executeQuery(
            "INSERT INTO Transactions (TransactionDate, ProductID, QuantitySold, BrokerID, BuyerID) "
            "VALUES ('" + date + "', " + std::to_string(productID) + ", " + std::to_string(quantitySold) + ", "
            + std::to_string(brokerID) + ", " + std::to_string(buyerID) + ")"
        );
        
        // Обновляем статистику маклера
        updateBrokerStatistics(db, brokerID, productID, quantitySold);
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool Transaction::updateBrokerStatistics(std::shared_ptr<Database> db, int brokerID, int productID, int quantitySold) {
    try {
        // Получаем цену товара
        auto productResult = db->executeQuery(
            "SELECT UnitPrice FROM Products WHERE ProductID = " + std::to_string(productID)
        );
        
        if (productResult.empty()) {
            return false;
        }
        
        double unitPrice = std::stod(productResult[0][0]);
        double amount = unitPrice * quantitySold;
        
        // Проверяем, существует ли запись статистики для данного маклера
        auto statsResult = db->executeQuery(
            "SELECT COUNT(*) FROM BrokerStatistics WHERE BrokerID = " + std::to_string(brokerID)
        );
        
        if (std::stoi(statsResult[0][0]) == 0) {
            // Создаем новую запись статистики
            db->executeQuery(
                "INSERT INTO BrokerStatistics (BrokerID, TotalUnitsSold, TotalAmount) "
                "VALUES (" + std::to_string(brokerID) + ", " + std::to_string(quantitySold) + ", "
                + std::to_string(amount) + ")"
            );
        } else {
            // Обновляем существующую запись статистики
            db->executeQuery(
                "UPDATE BrokerStatistics SET "
                "TotalUnitsSold = TotalUnitsSold + " + std::to_string(quantitySold) + ", "
                "TotalAmount = TotalAmount + " + std::to_string(amount) + " "
                "WHERE BrokerID = " + std::to_string(brokerID)
            );
        }
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

std::vector<Transaction> Transaction::getAllTransactions(std::shared_ptr<Database> db) {
    std::vector<Transaction> transactions;
    
    try {
        auto result = db->executeQuery("SELECT TransactionID FROM Transactions");
        
        for (const auto& row : result) {
            transactions.emplace_back(db, std::stoi(row[0]));
        }
    } catch (const std::exception& e) {
        // Обработка ошибок
    }
    
    return transactions;
}

std::vector<Transaction> Transaction::getTransactionsByDate(std::shared_ptr<Database> db, const std::string& date) {
    std::vector<Transaction> transactions;
    
    try {
        auto result = db->executeQuery(
            "SELECT TransactionID FROM Transactions WHERE TransactionDate = '" + date + "'"
        );
        
        for (const auto& row : result) {
            transactions.emplace_back(db, std::stoi(row[0]));
        }
    } catch (const std::exception& e) {
        // Обработка ошибок
    }
    
    return transactions;
}

bool Transaction::updateProductQuantities(std::shared_ptr<Database> db, const std::string& cutoffDate) {
    try {
        // Обновляем количество товара в соответствии с проданным количеством
        db->executeQuery(
            "UPDATE Products "
            "SET QuantitySupplied = QuantitySupplied - ("
            "  SELECT COALESCE(SUM(t.QuantitySold), 0) "
            "  FROM Transactions t "
            "  WHERE t.ProductID = Products.ProductID AND t.TransactionDate <= '" + cutoffDate + "'"
            ")"
        );
        
        // Удаляем сделки до указанной даты
        db->executeQuery(
            "DELETE FROM Transactions WHERE TransactionDate <= '" + cutoffDate + "'"
        );
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}