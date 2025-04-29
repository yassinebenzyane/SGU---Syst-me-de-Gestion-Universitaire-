/**
 * @file menu.c
 * @brief Implementation of menu management module
 */

#include "../include/menu.h"
#include "../include/utils.h"

/**
 * Display the admin menu and handle admin operations
 */
void menu_admin(Utilisateur utilisateurs[], int *nb_utilisateurs, 
                NodeEtudiant** etudiants, NodeEnseignant** enseignants, 
                NodeMatiere** matieres, EmploiDuTemps *edt) {
    int choix = 0;
    
    do {
        clear_screen();
        printf("\n=== Menu Administrateur ===\n");
        printf("1. Gestion des Utilisateurs\n");
        printf("2. Gestion des Étudiants\n");
        printf("3. Gestion des Enseignants\n");
        printf("4. Gestion des Matières\n");
        printf("5. Gestion de l'Emploi du Temps\n");
        printf("0. Déconnexion\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 5);
        
        switch (choix) {
            case 1: {
                // Gestion des Utilisateurs
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Utilisateurs ===\n");
                    printf("1. Afficher tous les utilisateurs\n");
                    printf("2. Ajouter un utilisateur\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 2);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_utilisateurs();
                            pause_screen();
                            break;
                        case 2: {
                            char prenom[50], nom[50], email[50], role[20];
                            
                            printf("\n=== Ajouter un nouvel utilisateur ===\n");
                            
                            get_input("Prénom: ", prenom, sizeof(prenom));
                            get_input("Nom: ", nom, sizeof(nom));
                            
                            // Email validation
                            int valid_email = 0;
                            while (!valid_email) {
                                get_input("Email: ", email, sizeof(email));
                                if (validate_email(email)) {
                                    valid_email = 1;
                                } else {
                                    printf("Email invalide. Veuillez réessayer.\n");
                                }
                            }
                            
                            printf("Rôles disponibles: admin, enseignant, etudiant\n");
                            get_input("Rôle: ", role, sizeof(role));
                            
                            // Validate role
                            if (strcmp(role, "admin") != 0 && 
                                strcmp(role, "enseignant") != 0 && 
                                strcmp(role, "etudiant") != 0) {
                                printf("Rôle invalide. Utilisation du rôle 'etudiant' par défaut.\n");
                                strcpy(role, "etudiant");
                            }
                            
                            ajouter_utilisateur_auto(prenom, nom, email, role);
                            pause_screen();
                            break;
                        }
                    }
                } while (sous_choix != 0);
                break;
            }
            case 2: {
                // Gestion des Étudiants
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Étudiants ===\n");
                    printf("1. Afficher tous les étudiants\n");
                    printf("2. Rechercher des étudiants\n");
                    printf("3. Ajouter un étudiant\n");
                    printf("4. Modifier un étudiant\n");
                    printf("5. Supprimer un étudiant\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 5);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_etudiants(*etudiants);
                            pause_screen();
                            break;
                        case 2:
                            rechercher_etudiants(*etudiants);
                            pause_screen();
                            break;
                        case 3:
                            ajouter_etudiant(etudiants);
                            pause_screen();
                            break;
                        case 4:
                            modifier_etudiant(*etudiants);
                            pause_screen();
                            break;
                        case 5:
                            supprimer_etudiant(etudiants);
                            pause_screen();
                            break;
                    }
                } while (sous_choix != 0);
                break;
            }
            case 3: {
                // Gestion des Enseignants
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Enseignants ===\n");
                    printf("1. Afficher tous les enseignants\n");
                    printf("2. Rechercher des enseignants\n");
                    printf("3. Ajouter un enseignant\n");
                    printf("4. Modifier un enseignant\n");
                    printf("5. Supprimer un enseignant\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 5);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_enseignants(*enseignants);
                            pause_screen();
                            break;
                        case 2:
                            rechercher_enseignants(*enseignants);
                            pause_screen();
                            break;
                        case 3:
                            ajouter_enseignant(enseignants);
                            pause_screen();
                            break;
                        case 4:
                            modifier_enseignant(*enseignants);
                            pause_screen();
                            break;
                        case 5:
                            supprimer_enseignant(enseignants);
                            pause_screen();
                            break;
                    }
                } while (sous_choix != 0);
                break;
            }
            case 4: {
                // Gestion des Matières
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Matières ===\n");
                    printf("1. Afficher toutes les matières\n");
                    printf("2. Rechercher des matières\n");
                    printf("3. Ajouter une matière\n");
                    printf("4. Modifier une matière\n");
                    printf("5. Supprimer une matière\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 5);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_matieres(*matieres);
                            pause_screen();
                            break;
                        case 2:
                            rechercher_matieres(*matieres);
                            pause_screen();
                            break;
                        case 3:
                            ajouter_matiere(matieres);
                            pause_screen();
                            break;
                        case 4:
                            modifier_matiere(*matieres);
                            pause_screen();
                            break;
                        case 5:
                            supprimer_matiere(matieres);
                            pause_screen();
                            break;
                    }
                } while (sous_choix != 0);
                break;
            }
            case 5: {
                // Gestion de l'Emploi du Temps
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion de l'Emploi du Temps ===\n");
                    printf("1. Afficher l'emploi du temps\n");
                    printf("2. Générer un emploi du temps automatique\n");
                    printf("3. Ajouter un créneau\n");
                    printf("4. Modifier un créneau\n");
                    printf("5. Supprimer un créneau\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 5);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_emploi_du_temps(edt);
                            pause_screen();
                            break;
                        case 2:
                            generer_emploi_du_temps(edt, *enseignants, *matieres);
                            pause_screen();
                            break;
                        case 3:
                            ajouter_creneau(edt, *enseignants, *matieres);
                            pause_screen();
                            break;
                        case 4:
                            modifier_creneau(edt, *enseignants, *matieres);
                            pause_screen();
                            break;
                        case 5:
                            supprimer_creneau(edt);
                            pause_screen();
                            break;
                    }
                } while (sous_choix != 0);
                break;
            }
        }
    } while (choix != 0);
}

/**
 * Display the teacher menu and handle teacher operations
 */
void menu_enseignant(int id_enseignant, NodeEtudiant* etudiants, 
                    NodeEnseignant* enseignants, NodeMatiere* matieres, 
                    EmploiDuTemps *edt) {
    int choix = 0;
    
    // Find the teacher info
    NodeEnseignant* enseignant = trouver_enseignant_par_id(enseignants, id_enseignant);
    if (enseignant == NULL) {
        printf("Erreur: Enseignant avec ID %d non trouvé.\n", id_enseignant);
        pause_screen();
        return;
    }
    
    do {
        clear_screen();
        printf("\n=== Menu Enseignant ===\n");
        printf("Bienvenue, %s %s\n", enseignant->enseignant.prenom, enseignant->enseignant.nom);
        printf("1. Consulter les Étudiants\n");
        printf("2. Consulter les Matières\n");
        printf("3. Consulter l'Emploi du Temps\n");
        printf("0. Déconnexion\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 3);
        
        switch (choix) {
            case 1: {
                int option_etudiant = get_int_input("1. Afficher tous les étudiants\n2. Rechercher des étudiants\nChoisissez une option: ", 1, 2);
                if (option_etudiant == 1) {
                    afficher_etudiants(etudiants);
                } else {
                    rechercher_etudiants(etudiants);
                }
                pause_screen();
                break;
            }
            case 2: {
                int option_matiere = get_int_input("1. Afficher toutes les matières\n2. Rechercher des matières\nChoisissez une option: ", 1, 2);
                if (option_matiere == 1) {
                    afficher_matieres(matieres);
                } else {
                    rechercher_matieres(matieres);
                }
                pause_screen();
                break;
            }
            case 3:
                afficher_emploi_du_temps(edt);
                pause_screen();
                break;
        }
    } while (choix != 0);
}

/**
 * Display the student menu and handle student operations
 */
void menu_etudiant(int id_etudiant, NodeEtudiant* etudiants, 
                  NodeEnseignant* enseignants, NodeMatiere* matieres, 
                  EmploiDuTemps *edt) {
    int choix = 0;
    
    // Find the student info
    NodeEtudiant* etudiant = trouver_etudiant_par_id(etudiants, id_etudiant);
    if (etudiant == NULL) {
        printf("Erreur: Étudiant avec ID %d non trouvé.\n", id_etudiant);
        pause_screen();
        return;
    }
    
    do {
        clear_screen();
        printf("\n=== Menu Étudiant ===\n");
        printf("Bienvenue, %s %s\n", etudiant->etudiant.prenom, etudiant->etudiant.nom);
        printf("1. Consulter les Enseignants\n");
        printf("2. Consulter les Matières\n");
        printf("3. Consulter l'Emploi du Temps\n");
        printf("0. Déconnexion\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 3);
        
        switch (choix) {
            case 1: {
                int option_enseignant = get_int_input("1. Afficher tous les enseignants\n2. Rechercher des enseignants\nChoisissez une option: ", 1, 2);
                if (option_enseignant == 1) {
                    afficher_enseignants(enseignants);
                } else {
                    rechercher_enseignants(enseignants);
                }
                pause_screen();
                break;
            }
            case 2: {
                int option_matiere = get_int_input("1. Afficher toutes les matières\n2. Rechercher des matières\nChoisissez une option: ", 1, 2);
                if (option_matiere == 1) {
                    afficher_matieres(matieres);
                } else {
                    rechercher_matieres(matieres);
                }
                pause_screen();
                break;
            }
            case 3:
                afficher_emploi_du_temps(edt);
                pause_screen();
                break;
        }
    } while (choix != 0);
}
