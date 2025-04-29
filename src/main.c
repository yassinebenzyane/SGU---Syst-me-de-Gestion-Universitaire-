/**
 * @file main.c
 * @brief Main entry point for the school management system
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/authentication.h"
#include "../include/etudiant.h"
#include "../include/enseignant.h"
#include "../include/matiere.h"
#include "../include/emploi_du_temps.h"
#include "../include/menu.h"
#include "../include/utils.h"
#include "../include/annonces.h"
#include "../include/notes.h"
#include "../include/inscriptions.h"

/**
 * Main function - Entry point of the application
 */
int main() {
    // Variables for user authentication
    char email[50];
    char password[50];
    int id;
    char role[20];
    char prenom[50];
    char nom[50];
    
    // Create data structures
    Utilisateur utilisateurs[MAX_USERS];
    int nb_utilisateurs = 0;
    NodeEtudiant* etudiants = NULL;
    NodeEnseignant* enseignants = NULL;
    NodeMatiere* matieres = NULL;
    EmploiDuTemps edt;
    
    // Create data directory if it doesn't exist
    create_data_dir();
    
    // Load data
    charger_utilisateurs(utilisateurs, &nb_utilisateurs);
    charger_etudiants(&etudiants);
    charger_enseignants(&enseignants);
    charger_matieres(&matieres);
    charger_emploi_du_temps(&edt);
    
    int connected = 0;
    
    // Main loop
    while (1) {
        if (!connected) {
            clear_screen();
            printf("\n=== Système de Gestion Scolaire ===\n");
            printf("1. Se connecter\n");
            printf("2. Quitter\n");
            
            int choix = get_int_input("Choisissez une option: ", 1, 2);
            
            if (choix == 2) {
                break;
            }
            
            // Authentication
            clear_screen();
            printf("\n=== Connexion ===\n");
            get_input("Email: ", email, sizeof(email));
            get_input("Mot de passe: ", password, sizeof(password));
            
            if (authentifier_utilisateur(email, password, &id, role, prenom, nom)) {
                connected = 1;
                clear_screen();
                afficher_infos_utilisateur(id, role, prenom, nom);
                pause_screen();
                
                // Redirect to appropriate menu based on role
                if (strcmp(role, "admin") == 0) {
                    menu_admin(utilisateurs, &nb_utilisateurs, &etudiants, 
                              &enseignants, &matieres, &edt);
                } else if (strcmp(role, "enseignant") == 0) {
                    menu_enseignant(id, etudiants, enseignants, matieres, &edt);
                } else if (strcmp(role, "etudiant") == 0) {
                    menu_etudiant(id, etudiants, enseignants, matieres, &edt);
                }
                
                connected = 0;
            } else {
                printf("Erreur: Email ou mot de passe incorrect.\n");
                pause_screen();
            }
        }
    }
    
    // Free allocated memory
    liberer_etudiants(&etudiants);
    liberer_enseignants(&enseignants);
    liberer_matieres(&matieres);
    
    printf("\nMerci d'avoir utilisé le Système de Gestion Scolaire.\n");
    
    return 0;
}
