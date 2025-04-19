BEGIN TRANSACTION;

-- Создание таблицы маклеров
CREATE TABLE IF NOT EXISTS Brokers (
    BrokerID INTEGER PRIMARY KEY AUTOINCREMENT,
    LastName TEXT NOT NULL,
    Address TEXT,
    BirthYear INTEGER CHECK (BirthYear > 1900 AND BirthYear < 2023)
);

-- Создание таблицы поставщиков
CREATE TABLE IF NOT EXISTS Suppliers (
    SupplierID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name TEXT NOT NULL
);

-- Создание таблицы покупателей
CREATE TABLE IF NOT EXISTS Buyers (
    BuyerID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name TEXT NOT NULL
);

-- Создание таблицы товаров
CREATE TABLE IF NOT EXISTS Products (
    ProductID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name TEXT NOT NULL,
    Type TEXT NOT NULL,
    UnitPrice REAL NOT NULL CHECK (UnitPrice > 0),
    SupplierID INTEGER,
    ExpiryDate TEXT,
    QuantitySupplied INTEGER NOT NULL CHECK (QuantitySupplied >= 0),
    FOREIGN KEY (SupplierID) REFERENCES Suppliers(SupplierID)
);

-- Создание таблицы сделок
CREATE TABLE IF NOT EXISTS Transactions (
    TransactionID INTEGER PRIMARY KEY AUTOINCREMENT,
    TransactionDate TEXT NOT NULL,
    ProductID INTEGER,
    QuantitySold INTEGER NOT NULL CHECK (QuantitySold > 0),
    BrokerID INTEGER,
    BuyerID INTEGER,
    FOREIGN KEY (ProductID) REFERENCES Products(ProductID),
    FOREIGN KEY (BrokerID) REFERENCES Brokers(BrokerID),
    FOREIGN KEY (BuyerID) REFERENCES Buyers(BuyerID)
);

-- Создание таблицы статистики маклеров
CREATE TABLE IF NOT EXISTS BrokerStatistics (
    BrokerID INTEGER PRIMARY KEY,
    TotalUnitsSold INTEGER DEFAULT 0,
    TotalAmount REAL DEFAULT 0,
    FOREIGN KEY (BrokerID) REFERENCES Brokers(BrokerID)
);

-- Создание таблицы пользователей для аутентификации
CREATE TABLE IF NOT EXISTS Users (
    UserID INTEGER PRIMARY KEY AUTOINCREMENT,
    Username TEXT UNIQUE NOT NULL,
    Password TEXT NOT NULL,
    Role TEXT NOT NULL CHECK (Role IN ('Broker', 'Management')),
    BrokerID INTEGER,
    FOREIGN KEY (BrokerID) REFERENCES Brokers(BrokerID)
);

-- Создание триггера для обновления статистики маклеров после каждой сделки
CREATE TRIGGER IF NOT EXISTS UpdateBrokerStatistics
AFTER INSERT ON Transactions
FOR EACH ROW
BEGIN
    INSERT OR IGNORE INTO BrokerStatistics (BrokerID, TotalUnitsSold, TotalAmount)
    VALUES (NEW.BrokerID, 0, 0);
    
    UPDATE BrokerStatistics
    SET 
        TotalUnitsSold = TotalUnitsSold + NEW.QuantitySold,
        TotalAmount = TotalAmount + (SELECT UnitPrice FROM Products WHERE ProductID = NEW.ProductID) * NEW.QuantitySold
    WHERE BrokerID = NEW.BrokerID;
END;

-- Заполнение таблиц тестовыми данными
INSERT OR IGNORE INTO Brokers (LastName, Address, BirthYear) VALUES
('Иванов', 'Москва, ул. Ленина 10', 1985),
('Петров', 'Санкт-Петербург, пр. Невский 100', 1990),
('Сидоров', 'Казань, ул. Баумана 5', 1982);

INSERT OR IGNORE INTO Suppliers (Name) VALUES
('Парфюм-Люкс'),
('Аромат-Престиж'),
('Эссенция');

INSERT OR IGNORE INTO Buyers (Name) VALUES
('МегаПарфюм'),
('Ароматный Мир'),
('ПарфюмСтиль');

INSERT OR IGNORE INTO Products (Name, Type, UnitPrice, SupplierID, ExpiryDate, QuantitySupplied) VALUES
('Шанель №5', 'Духи', 5000.0, 1, '2025-12-31', 100),
('Диор Саваж', 'Туалетная вода', 3500.0, 2, '2026-06-30', 150),
('Версаче Эрос', 'Парфюмерная вода', 4200.0, 3, '2026-03-15', 80),
('Гуччи Блум', 'Духи', 4800.0, 1, '2025-10-20', 120),
('Армани Код', 'Туалетная вода', 3200.0, 2, '2026-08-10', 200);

INSERT OR IGNORE INTO Users (Username, Password, Role, BrokerID) VALUES
('admin', 'admin123', 'Management', NULL),
('ivanov', 'pass123', 'Broker', 1),
('petrov', 'pass456', 'Broker', 2),
('sidorov', 'pass789', 'Broker', 3);

INSERT OR IGNORE INTO Transactions (TransactionDate, ProductID, QuantitySold, BrokerID, BuyerID) VALUES
('2023-01-15', 1, 10, 1, 1),
('2023-01-20', 2, 15, 2, 2),
('2023-01-25', 3, 8, 3, 3),
('2023-02-01', 4, 12, 1, 2),
('2023-02-10', 5, 20, 2, 1),
('2023-02-15', 1, 5, 3, 3),
('2023-02-20', 2, 10, 1, 1),
('2023-03-01', 3, 6, 2, 2),
('2023-03-10', 4, 8, 3, 3),
('2023-03-15', 5, 15, 1, 1);

COMMIT;
