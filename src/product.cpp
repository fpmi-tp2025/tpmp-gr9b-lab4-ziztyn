#include "product.h"

Product::Product(std::shared_ptr<Database> db, int productID) : db(db), productID(productID) {
    loadProductInfo();
}

void Product::loadProductInfo() {
    try {
        auto result = db->executeQuery(
            "SELECT Name, Type, UnitPrice, SupplierID, ExpiryDate, QuantitySupplied "
            "FROM Products WHERE ProductID = " + std::to_string(productID)
        );
        
        if (!result.empty()) {
            name = result[0][0];
            type = result[0][1];
            unitPrice = std::stod(result[0][2]);
            supplierID = std::stoi(result[0][3]);
            expiryDate = result[0][4];
            quantitySupplied = std::stoi(result[0][5]);
        }
    } catch (const std::exception& e) {
        // Обработка ошибок
    }
}

std::string Product::getName() const {
    return name;
}

std::string Product::getType() const {
    return type;
}

double Product::getUnitPrice() const {
    return unitPrice;
}

int Product::getSupplierID() const {
    return supplierID;
}

std::string Product::getExpiryDate() const {
    return expiryDate;
}

int Product::getQuantitySupplied() const {
    return quantitySupplied;
}

bool Product::updateInfo(const std::string& name, const std::string& type, double unitPrice,
                       int supplierID, const std::string& expiryDate, int quantitySupplied) {
    try {
        db->executeQuery(
            "UPDATE Products SET Name = '" + name + "', Type = '" + type + "', "
            "UnitPrice = " + std::to_string(unitPrice) + ", SupplierID = " + std::to_string(supplierID) + ", "
            "ExpiryDate = '" + expiryDate + "', QuantitySupplied = " + std::to_string(quantitySupplied) + " "
            "WHERE ProductID = " + std::to_string(productID)
        );
        
        this->name = name;
        this->type = type;
        this->unitPrice = unitPrice;
        this->supplierID = supplierID;
        this->expiryDate = expiryDate;
        this->quantitySupplied = quantitySupplied;
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool Product::updateQuantity(int newQuantity) {
    try {
        db->executeQuery(
            "UPDATE Products SET QuantitySupplied = " + std::to_string(newQuantity) + " "
            "WHERE ProductID = " + std::to_string(productID)
        );
        
        this->quantitySupplied = newQuantity;
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

std::vector<Product> Product::getAllProducts(std::shared_ptr<Database> db) {
    std::vector<Product> products;
    
    try {
        auto result = db->executeQuery("SELECT ProductID FROM Products");
        
        for (const auto& row : result) {
            products.emplace_back(db, std::stoi(row[0]));
        }
    } catch (const std::exception& e) {
        // Обработка ошибок
    }
    
    return products;
}

bool Product::addProduct(std::shared_ptr<Database> db, const std::string& name, const std::string& type,
                       double unitPrice, int supplierID, const std::string& expiryDate, int quantitySupplied) {
    try {
        db->executeQuery(
            "INSERT INTO Products (Name, Type, UnitPrice, SupplierID, ExpiryDate, QuantitySupplied) "
            "VALUES ('" + name + "', '" + type + "', " + std::to_string(unitPrice) + ", "
            + std::to_string(supplierID) + ", '" + expiryDate + "', " + std::to_string(quantitySupplied) + ")"
        );
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}