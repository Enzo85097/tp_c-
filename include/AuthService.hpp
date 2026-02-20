#pragma once

#include "Database.hpp"

class AuthService {
   public:
    explicit AuthService(Database& db);
    std::optional<User> login(const std::string& username, const std::string& password);

   private:
    Database& db_;
};
