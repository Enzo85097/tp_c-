#pragma once

#include "Database.hpp"

class StudentService {
   public:
    explicit StudentService(Database& db);

    void showForAdmin();
    void showForProf();
    void showForStudent(int studentId);

    void exportForRole(Role role, int studentId = -1);
    void importGradesFromFile(Role role);

   private:
    Database& db_;

    void printHeader();
    void printStudent(const Student& s, bool showEmail);
};
