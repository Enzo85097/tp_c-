#include "AuthService.hpp"
#include "StudentService.hpp"

#include <iostream>

int main() {
    try {
        Database db("school.db");
        db.initialize();

        AuthService auth(db);
        StudentService students(db);

        std::cout << "=== TP C++2 (version essentielle) ===\n";
        std::cout << "Login: ";
        std::string username;
        std::cin >> username;

        std::cout << "Mot de passe: ";
        std::string password;
        std::cin >> password;

        auto user = auth.login(username, password);
        if (!user) {
            std::cout << "Identifiants invalides.\n";
            return 0;
        }

        std::cout << "Connecte en tant que " << roleToString(user->role) << "\n\n";

        bool running = true;
        while (running) {
            std::cout << "1) Voir les donnees\n";
            std::cout << "2) Export texte\n";
            std::cout << "3) Import notes (admin/prof)\n";
            std::cout << "0) Quitter\n";
            std::cout << "Choix: ";

            int choice = 0;
            std::cin >> choice;

            switch (choice) {
                case 1:
                    if (user->role == Role::Admin) {
                        students.showForAdmin();
                    } else if (user->role == Role::Prof) {
                        students.showForProf();
                    } else {
                        students.showForStudent(user->studentId);
                    }
                    break;
                case 2:
                    students.exportForRole(user->role, user->studentId);
                    break;
                case 3:
                    students.importGradesFromFile(user->role);
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Choix invalide\n";
            }

            std::cout << "\n";
        }

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Erreur fatale: " << ex.what() << "\n";
        return 1;
    }
}
