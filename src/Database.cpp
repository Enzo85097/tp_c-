#include "Database.hpp"

#include <iostream>
#include <stdexcept>

Database::Database(const std::string& path) {
    if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
        throw std::runtime_error("Impossible d'ouvrir la base de donnees.");
    }
}

Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

void Database::execute(const std::string& sql) {
    char* errorMessage = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::string message = errorMessage ? errorMessage : "Erreur SQL inconnue";
        sqlite3_free(errorMessage);
        throw std::runtime_error(message);
    }
}

void Database::initialize() {
    execute(
        "CREATE TABLE IF NOT EXISTS students("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "first_name TEXT NOT NULL,"
        "last_name TEXT NOT NULL,"
        "email TEXT NOT NULL,"
        "grade REAL NOT NULL DEFAULT 0"
        ");");

    execute(
        "CREATE TABLE IF NOT EXISTS users("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "role TEXT NOT NULL,"
        "student_id INTEGER,"
        "FOREIGN KEY(student_id) REFERENCES students(id)"
        ");");

    execute(
        "INSERT OR IGNORE INTO students(id, first_name, last_name, email, grade) VALUES"
        "(1, 'Ali', 'Bennani', 'ali@school.com', 14.5),"
        "(2, 'Sara', 'Idrissi', 'sara@school.com', 16.0),"
        "(3, 'Yassine', 'Tazi', 'yassine@school.com', 12.0);"
    );

    execute(
        "INSERT OR IGNORE INTO users(id, username, password, role, student_id) VALUES"
        "(1, 'admin', 'admin123', 'ADMIN', NULL),"
        "(2, 'prof', 'prof123', 'PROF', NULL),"
        "(3, 'ali', 'ali123', 'STUDENT', 1),"
        "(4, 'sara', 'sara123', 'STUDENT', 2),"
        "(5, 'yassine', 'yas123', 'STUDENT', 3);"
    );
}

std::optional<User> Database::findUser(const std::string& username, const std::string& password) {
    const char* sql = "SELECT id, username, password, role, COALESCE(student_id, -1) FROM users WHERE username = ? AND password = ?";

    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(db_, sql, -1, &statement, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_text(statement, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    std::optional<User> user;
    if (sqlite3_step(statement) == SQLITE_ROW) {
        user = User{sqlite3_column_int(statement, 0),
                    reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)),
                    reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)),
                    roleFromString(reinterpret_cast<const char*>(sqlite3_column_text(statement, 3))),
                    sqlite3_column_int(statement, 4)};
    }

    sqlite3_finalize(statement);
    return user;
}

std::vector<Student> Database::getAllStudents() {
    const char* sql = "SELECT id, first_name, last_name, email, grade FROM students ORDER BY id";
    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &statement, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Lecture des etudiants impossible");
    }

    std::vector<Student> students;
    while (sqlite3_step(statement) == SQLITE_ROW) {
        students.push_back(Student{sqlite3_column_int(statement, 0),
                                   reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)),
                                   reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)),
                                   reinterpret_cast<const char*>(sqlite3_column_text(statement, 3)),
                                   sqlite3_column_double(statement, 4)});
    }

    sqlite3_finalize(statement);
    return students;
}

std::optional<Student> Database::getStudentById(int id) {
    const char* sql = "SELECT id, first_name, last_name, email, grade FROM students WHERE id = ?";
    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &statement, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_int(statement, 1, id);

    std::optional<Student> student;
    if (sqlite3_step(statement) == SQLITE_ROW) {
        student = Student{sqlite3_column_int(statement, 0),
                          reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)),
                          reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)),
                          reinterpret_cast<const char*>(sqlite3_column_text(statement, 3)),
                          sqlite3_column_double(statement, 4)};
    }

    sqlite3_finalize(statement);
    return student;
}

bool Database::updateGrade(int id, double newGrade) {
    const char* sql = "UPDATE students SET grade = ? WHERE id = ?";

    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(db_, sql, -1, &statement, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_double(statement, 1, newGrade);
    sqlite3_bind_int(statement, 2, id);

    bool ok = sqlite3_step(statement) == SQLITE_DONE && sqlite3_changes(db_) > 0;
    sqlite3_finalize(statement);
    return ok;
}
