#include "AuthService.hpp"

AuthService::AuthService(Database& db) : db_(db) {}

std::optional<User> AuthService::login(const std::string& username, const std::string& password) {
    return db_.findUser(username, password);
}
