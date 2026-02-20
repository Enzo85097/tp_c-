#pragma once

#include <string>

enum class Role {
    Admin,
    Prof,
    Student
};

struct User {
    int id = 0;
    std::string username;
    std::string password;
    Role role = Role::Student;
    int studentId = -1;
};

struct Student {
    int id = 0;
    std::string firstName;
    std::string lastName;
    std::string email;
    double grade = 0.0;
};

inline std::string roleToString(Role role) {
    switch (role) {
        case Role::Admin:
            return "ADMIN";
        case Role::Prof:
            return "PROF";
        default:
            return "STUDENT";
    }
}

inline Role roleFromString(const std::string& value) {
    if (value == "ADMIN") return Role::Admin;
    if (value == "PROF") return Role::Prof;
    return Role::Student;
}
