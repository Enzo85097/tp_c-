#pragma once

#include "Models.hpp"

#include <optional>
#include <sqlite3.h>
#include <string>
#include <vector>

class Database {
   public:
    explicit Database(const std::string& path);
    ~Database();

    void initialize();
    std::optional<User> findUser(const std::string& username, const std::string& password);

    std::vector<Student> getAllStudents();
    std::optional<Student> getStudentById(int id);
    bool updateGrade(int id, double newGrade);

   private:
    sqlite3* db_ = nullptr;

    void execute(const std::string& sql);
};
