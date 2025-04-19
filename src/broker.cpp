#include "broker.h"
#include "transaction.h"

Broker::Broker(std::shared_ptr<Database> db, int brokerID) : db(db), brokerID(brokerID) {
    loadBrokerInfo();
}

void Broker::loadBrokerInfo() {
    try {
        auto result = db->executeQuery(
            "SELECT LastName, Address, BirthYear FROM Brokers WHERE BrokerID = " + std::to_string(brokerID)
        );
        
        if (!result.empty()) {
            lastName = result[0][0];
            address = result[0][1];
            birthYear = std::stoi(result[0][2]);
        }
    } catch (const std::exception& e) {
        // Обработка ошибок
    }
}

std::string Broker::getLastName() const {
    return lastName;
}

std::string Broker::getAddress() const {
    return address;
}

int Broker::getBirthYear() const {
    return birthYear;
}

bool Broker::updateInfo(const std::string& lastName, const std::string& address, int birthYear) {
    try {
        db->executeQuery(
            "UPDATE Brokers SET LastName = '" + lastName + "', Address = '" + address + "', "
            "BirthYear = " + std::to_string(birthYear) + " WHERE BrokerID = " + std::to_string(brokerID)
        );
        
        this->lastName = lastName;
        this->address = address;
        this->birthYear = birthYear;
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool Broker::createTransaction(const std::string& date, int productID, int quantitySold, int buyerID) {
    return Transaction::createTransaction(db, date, productID, quantitySold, brokerID, buyerID);
}

std::vector<TransactionInfo> Broker::getTransactions() const {
    std::vector<TransactionInfo> transactions;
    
    try {
        auto result = db->executeQuery(
            "SELECT TransactionID, TransactionDate, ProductID, QuantitySold, BrokerID, BuyerID "
            "FROM Transactions WHERE BrokerID = " + std::to_string(brokerID)
        );
        
        for (const auto& row : result) {
            TransactionInfo transaction;
            transaction.transactionID = std::stoi(row[0]);
            transaction.date = row[1];
            transaction.productID = std::stoi(row[2]);
            transaction.quantitySold = std::stoi(row[3]);
            transaction.brokerID = std::stoi(row[4]);
            transaction.buyerID = std::stoi(row[5]);
            
            transactions.push_back(transaction);
        }
    } catch (const std::exception& e) {
        // Обработка ошибок
    }
    
    return transactions;
}

std::vector<TransactionInfo> Broker::getTransactionsByDate(const std::string& date) const {
    std::vector<TransactionInfo> transactions;
    
    try {
        auto result = db->executeQuery(
            "SELECT TransactionID, TransactionDate, ProductID, QuantitySold, BrokerID, BuyerID "
            "FROM Transactions WHERE BrokerID = " + std::to_string(brokerID) + " AND TransactionDate = '" + date + "'"
        );
        
        for (const auto& row : result) {
            TransactionInfo transaction;
            transaction.transactionID = std::stoi(row[0]);
            transaction.date = row[1];
            transaction.productID = std::stoi(row[2]);
            transaction.quantitySold = std::stoi(row[3]);
            transaction.brokerID = std::stoi(row[4]);
            transaction.buyerID = std::stoi(row[5]);
            
            transactions.push_back(transaction);
        }
    } catch (const std::exception& e) {
        // Обработка ошибок
    }
    
    return transactions;
}

BrokerStatistics Broker::getStatistics() const {
    BrokerStatistics stats = {0, 0.0};
    
    try {
        auto result = db->executeQuery(
            "SELECT TotalUnitsSold, TotalAmount FROM BrokerStatistics WHERE BrokerID = " + std::to_string(brokerID)
        );
        
        if (!result.empty()) {
            stats.totalUnitsSold = std::stoi(result[0][0]);
            stats.totalAmount = std::stod(result[0][1]);
        }
    } catch (const std::exception& e) {
        // Обработка ошибок
    }
    
    return stats;
}

std::vector<std::pair<int, std::string>> Broker::getAllBrokers(std::shared_ptr<Database> db) {
    std::vector<std::pair<int, std::string>> brokers;
    
    try {
        auto result = db->executeQuery("SELECT BrokerID, LastName FROM Brokers");
        
        for (const auto& row : result) {
            brokers.emplace_back(std::stoi(row[0]), row[1]);
        }
    } catch (const std::exception& e) {
        // Обработка ошибок
    }
    
    return brokers;
}