#include "user.h"

User::User(std::shared_ptr<Database> db) : db(db), userID(0), brokerID(0) {
}

bool User::authenticate(const std::string& username, const std::string& password) {
    try {
        auto result = db->executeQuery(
            "SELECT UserID, Username, Role, BrokerID FROM Users "
            "WHERE Username = '" + username + "' AND Password = '" + password + "'"
        );
        
        if (result.empty()) {
            return false;
        }
        
        userID = std::stoi(result[0][0]);
        this->username = result[0][1];
        role = result[0][2];
        brokerID = result[0][3].empty() ? 0 : std::stoi(result[0][3]);
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool User::createUser(const std::string& username, const std::string& password,
                     const std::string& role, int brokerID) {
    try {
        std::string query;
        if (brokerID > 0) {
            query = "INSERT INTO Users (Username, Password, Role, BrokerID) VALUES ('"
                    + username + "', '" + password + "', '" + role + "', " + std::to_string(brokerID) + ")";
        } else {
            query = "INSERT INTO Users (Username, Password, Role) VALUES ('"
                    + username + "', '" + password + "', '" + role + "')";
        }
        
        db->executeQuery(query);
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool User::updatePassword(const std::string& newPassword) {
    if (userID == 0) {
        return false;
    }
    
    try {
        db->executeQuery(
            "UPDATE Users SET Password = '" + newPassword + "' WHERE UserID = " + std::to_string(userID)
        );
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

std::string User::getRole() const {
    return role;
}

int User::getBrokerID() const {
    return brokerID;
}

std::string User::getUsername() const {
    return username;
}