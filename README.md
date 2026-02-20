# TP noté C++ 2 — Version essentielle

Ce projet implémente **uniquement le minimum obligatoire** demandé :
- Authentification (login + mot de passe)
- Gestion des rôles (`ADMIN`, `PROF`, `STUDENT`)
- Vues différentes selon le rôle
- Base de données SQL (SQLite)
- Export texte
- Import texte des notes (admin/prof)

> Rien d’optionnel n’a été ajouté (pas de chiffrement, pas de multithreading, pas de logs avancés).

---

## 1) Structure du projet

- `src/main.cpp` : boucle principale (menu)
- `src/Database.cpp` : accès SQL + création des tables
- `src/AuthService.cpp` : service d’authentification
- `src/StudentService.cpp` : affichage/exports/imports selon rôle
- `include/*.hpp` : modèles et interfaces

---

## 2) Étape 1 — Base SQL

Au démarrage, le programme crée automatiquement :
- Table `students`
- Table `users`

Puis il insère des données de test (si elles n’existent pas déjà).

### Comptes de test
- `admin / admin123`
- `prof / prof123`
- `ali / ali123`
- `sara / sara123`
- `yassine / yas123`

---

## 3) Étape 2 — Authentification

Le programme demande :
1. `username`
2. `password`

Ensuite il exécute une requête SQL :
- si utilisateur trouvé ➜ connexion
- sinon ➜ message “Identifiants invalides”

---

## 4) Étape 3 — Vues selon rôle

Après connexion, le menu propose les actions.

### ADMIN
- voit tous les étudiants + email + note
- peut exporter
- peut importer des notes

### PROF
- voit tous les étudiants (email masqué)
- peut modifier une note
- peut exporter
- peut importer des notes

### STUDENT
- voit uniquement sa propre fiche
- peut exporter sa propre fiche
- ne peut pas importer

---

## 5) Étape 4 — Export texte

Action menu `Export` :
- génère `export.txt`
- contenu dépend du rôle

---

## 6) Étape 5 — Import texte des notes

Action menu `Import` :
- lit `import.txt`
- format attendu par ligne :

```txt
id_etudiant;nouvelle_note;
```

Exemple :
```txt
1;17.5;
2;14;
```

Seuls `ADMIN` et `PROF` peuvent importer.

---

## 7) Compilation et exécution

```bash
cmake -S . -B build
cmake --build build
./build/tp_note_cpp2
```

---

## 8) Explication simple de la logique (pour l’oral)

1. **J’ouvre la base SQL**.
2. **Je crée les tables** si absentes.
3. **Je connecte l’utilisateur** avec login/mot de passe.
4. **Je lis son rôle** (`ADMIN`/`PROF`/`STUDENT`).
5. **J’affiche les actions autorisées** selon ce rôle.
6. **J’exécute les requêtes SQL** (SELECT/UPDATE).
7. **J’importe/exporte** via fichiers texte.

C’est une version courte, claire, et conforme au minimum demandé.
