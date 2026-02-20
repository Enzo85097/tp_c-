#include "StudentService.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

StudentService::StudentService(Database& db) : db_(db) {}

void StudentService::printHeader() {
    std::cout << "ID | Nom | Prenom | Email | Note\n";
    std::cout << "----------------------------------------\n";
}

void StudentService::printStudent(const Student& s, bool showEmail) {
    std::cout << s.id << " | " << s.lastName << " | " << s.firstName << " | ";
    if (showEmail) {
        std::cout << s.email;
    } else {
        std::cout << "***";
    }
    std::cout << " | " << std::fixed << std::setprecision(2) << s.grade << "\n";
}

void StudentService::showForAdmin() {
    printHeader();
    for (const auto& student : db_.getAllStudents()) {
        printStudent(student, true);
    }
}

void StudentService::showForProf() {
    printHeader();
    for (const auto& student : db_.getAllStudents()) {
        printStudent(student, false);
    }

    std::cout << "\nEntrer l'ID de l'etudiant a modifier (0 pour annuler): ";
    int id = 0;
    std::cin >> id;
    if (id == 0) return;

    std::cout << "Nouvelle note: ";
    double grade = 0;
    std::cin >> grade;

    if (db_.updateGrade(id, grade)) {
        std::cout << "Note mise a jour.\n";
    } else {
        std::cout << "Echec de mise a jour.\n";
    }
}

void StudentService::showForStudent(int studentId) {
    auto student = db_.getStudentById(studentId);
    if (!student) {
        std::cout << "Aucune information trouvee.\n";
        return;
    }

    printHeader();
    printStudent(*student, true);
}

void StudentService::exportForRole(Role role, int studentId) {
    std::ofstream output("export.txt");
    if (!output) {
        std::cout << "Impossible de creer export.txt\n";
        return;
    }

    if (role == Role::Student) {
        auto student = db_.getStudentById(studentId);
        if (student) {
            output << student->id << ';' << student->firstName << ';' << student->lastName << ';' << student->email << ';'
                   << student->grade << '\n';
        }
    } else {
        bool showEmail = (role == Role::Admin);
        for (const auto& s : db_.getAllStudents()) {
            output << s.id << ';' << s.firstName << ';' << s.lastName << ';';
            output << (showEmail ? s.email : "***") << ';' << s.grade << '\n';
        }
    }

    std::cout << "Export termine dans export.txt\n";
}

void StudentService::importGradesFromFile(Role role) {
    if (role != Role::Admin && role != Role::Prof) {
        std::cout << "Vous n'avez pas le droit d'importer des notes.\n";
        return;
    }

    std::ifstream input("import.txt");
    if (!input) {
        std::cout << "Fichier import.txt introuvable.\n";
        return;
    }

    std::string line;
    int updated = 0;
    while (std::getline(input, line)) {
        std::stringstream ss(line);
        std::string idText;
        std::string gradeText;

        if (!std::getline(ss, idText, ';')) continue;
        if (!std::getline(ss, gradeText, ';')) continue;

        int id = std::stoi(idText);
        double grade = std::stod(gradeText);
        if (db_.updateGrade(id, grade)) {
            ++updated;
        }
    }

    std::cout << updated << " note(s) importee(s).\n";
}
