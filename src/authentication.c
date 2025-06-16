/**
 * @file authentication.c
 * @brief Implementation of authentication module
 */

#include "../include/authentication.h"
#include "../include/utils.h"

#define USERS_FILE "data/utilisateurs.txt"

/**
 * Load users from file into memory
 */
int charger_utilisateurs(Utilisateur utilisateurs[], int *nb_utilisateurs) {
    FILE *file;
    *nb_utilisateurs = 0;
    
    create_data_dir();
    
    // Create file if it doesn't exist
    if (!file_exists(USERS_FILE)) {
        // Create default admin user if the file doesn't exist
        file = fopen(USERS_FILE, "w");
        if (file == NULL) {
            printf("Erreur: Impossible de créer le fichier utilisateurs.\n");
            return 0;
        }
        
        // Add default admin user
        fprintf(file, "1|admin|admin|admin@ecole.com|admin123|admin\n");
        fclose(file);
    }
    
    file = fopen(USERS_FILE, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier utilisateurs.\n");
        return 0;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file) && *nb_utilisateurs < MAX_USERS) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline
        
        // Parse the line (id|prenom|nom|email|password|role)
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
               &utilisateurs[*nb_utilisateurs].id,
               utilisateurs[*nb_utilisateurs].prenom,
               utilisateurs[*nb_utilisateurs].nom,
               utilisateurs[*nb_utilisateurs].email,
               utilisateurs[*nb_utilisateurs].password,
               utilisateurs[*nb_utilisateurs].role);
        
        (*nb_utilisateurs)++;
    }
    
    fclose(file);
    return 1;
}

/**
 * Save users from memory to file
 */
int sauvegarder_utilisateurs(Utilisateur utilisateurs[], int nb_utilisateurs) {
    FILE *file = fopen(USERS_FILE, "w");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier utilisateurs en écriture.\n");
        return 0;
    }
    
    for (int i = 0; i < nb_utilisateurs; i++) {
        fprintf(file, "%d|%s|%s|%s|%s|%s\n",
                utilisateurs[i].id,
                utilisateurs[i].prenom,
                utilisateurs[i].nom,
                utilisateurs[i].email,
                utilisateurs[i].password,
                utilisateurs[i].role);
    }
    
    fclose(file);
    return 1;
}

/**
 * Authenticate a user with email and password
 */
int authentifier_utilisateur(char *email, char *password, int *id, char *role, char *prenom, char *nom) {
    Utilisateur utilisateurs[MAX_USERS];
    int nb_utilisateurs = 0;
    
    if (!charger_utilisateurs(utilisateurs, &nb_utilisateurs)) {
        return 0;
    }
    
    for (int i = 0; i < nb_utilisateurs; i++) {
        if (strcmp(utilisateurs[i].email, email) == 0 && 
            strcmp(utilisateurs[i].password, password) == 0) {
            
            *id = utilisateurs[i].id;
            strcpy(role, utilisateurs[i].role);
            strcpy(prenom, utilisateurs[i].prenom);
            strcpy(nom, utilisateurs[i].nom);
            
            return 1;
        }
    }
    
    return 0; // Authentication failed
}

/**
 * Display information of the currently logged in user
 */
void afficher_infos_utilisateur(int id, char *role, char *prenom, char *nom) {
    printf("\n=== Informations Utilisateur ===\n");
    printf("ID: %d\n", id);
    printf("Nom: %s\n", nom);
    printf("Prénom: %s\n", prenom);
    printf("Rôle: %s\n", role);
    printf("================================\n\n");
}

/**
 * Automatically add a new user
 */
void ajouter_utilisateur_auto(const int id,const char *prenom, const char *nom, const char *email, const char *role) {
    Utilisateur utilisateurs[MAX_USERS];
    int nb_utilisateurs = 0;
    
    if (!charger_utilisateurs(utilisateurs, &nb_utilisateurs)) {
        printf("Erreur lors du chargement des utilisateurs.\n");
        return;
    }
    
    if (nb_utilisateurs >= MAX_USERS) {
        printf("Erreur: Nombre maximum d'utilisateurs atteint.\n");
        return;
    }
    
    // Check if email already exists
    for (int i = 0; i < nb_utilisateurs; i++) {
        if (strcmp(utilisateurs[i].email, email) == 0) {
            printf("Erreur: Un utilisateur avec cet email existe déjà.\n");
            return;
        }
    }
    
    // Generate a new ID
    int new_id = id;
    
    // Add the new user
    Utilisateur new_user;
    new_user.id = new_id;
    strncpy(new_user.prenom, prenom, MAX_NAME_LEN - 1);
    strncpy(new_user.nom, nom, MAX_NAME_LEN - 1);
    strncpy(new_user.email, email, MAX_EMAIL_LEN - 1);
    
    // Generate default password (firstname + "123")
    snprintf(new_user.password, MAX_PASSWORD_LEN, "%s123", prenom);
    strncpy(new_user.role, role, MAX_ROLE_LEN - 1);
    
    utilisateurs[nb_utilisateurs] = new_user;
    nb_utilisateurs++;
    
    if (sauvegarder_utilisateurs(utilisateurs, nb_utilisateurs)) {
        printf("Utilisateur ajouté avec succès.\n");
        printf("Mot de passe par défaut: %s\n", new_user.password);
    } else {
        printf("Erreur lors de l'ajout de l'utilisateur.\n");
    }
}

/**
 * Display all users (admin only)
 */
void afficher_utilisateurs() {
    Utilisateur utilisateurs[MAX_USERS];
    int nb_utilisateurs = 0;
    
    if (!charger_utilisateurs(utilisateurs, &nb_utilisateurs)) {
        printf("Erreur lors du chargement des utilisateurs.\n");
        return;
    }
    
    printf("\n=== Liste des Utilisateurs ===\n");
    printf("%-4s | %-15s | %-15s | %-25s | %-10s\n", 
           "ID", "Prénom", "Nom", "Email", "Rôle");
    printf("---------------------------------------------------------------\n");
    
    for (int i = 0; i < nb_utilisateurs; i++) {
        printf("%-4d | %-15s | %-15s | %-25s | %-10s\n", 
               utilisateurs[i].id,
               utilisateurs[i].prenom,
               utilisateurs[i].nom,
               utilisateurs[i].email,
               utilisateurs[i].role);
    }
    
    printf("===============================================================\n\n");
}
