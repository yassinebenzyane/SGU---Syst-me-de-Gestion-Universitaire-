/**
 * @file etudiant.c
 * @brief Implementation of student management module
 */

#include "../include/etudiant.h"
#include "../include/authentication.h"
#include "../include/utils.h"

#define ETUDIANTS_FILE "data/etudiants.txt"

/**
 * Load students from file into a linked list
 */
int charger_etudiants(NodeEtudiant** tete) {
    FILE *file;
    *tete = NULL;
    
    create_data_dir();
    
    if (!file_exists(ETUDIANTS_FILE)) {
        // Create empty file if it doesn't exist
        create_file_if_not_exists(ETUDIANTS_FILE);
        return 1;
    }
    
    file = fopen(ETUDIANTS_FILE, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des étudiants.\n");
        return 0;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline
        
        NodeEtudiant *nouveau_node = (NodeEtudiant*)malloc(sizeof(NodeEtudiant));
        if (nouveau_node == NULL) {
            printf("Erreur: Échec d'allocation de mémoire.\n");
            fclose(file);
            return 0;
        }
        
        // Parse the line (id|prenom|nom|email|cne|section|filiere)
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
               &nouveau_node->etudiant.id,
               nouveau_node->etudiant.prenom,
               nouveau_node->etudiant.nom,
               nouveau_node->etudiant.email,
               nouveau_node->etudiant.cne,
               nouveau_node->etudiant.section,
               nouveau_node->etudiant.filiere);
        
        // Add to the beginning of the list
        nouveau_node->suivant = *tete;
        *tete = nouveau_node;
    }
    
    fclose(file);
    return 1;
}

/**
 * Save students from linked list to file
 */
int sauvegarder_etudiants(NodeEtudiant* tete) {
    FILE *file = fopen(ETUDIANTS_FILE, "w");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des étudiants en écriture.\n");
        return 0;
    }
    
    NodeEtudiant* courant = tete;
    while (courant != NULL) {
        fprintf(file, "%d|%s|%s|%s|%s|%s|%s\n",
                courant->etudiant.id,
                courant->etudiant.prenom,
                courant->etudiant.nom,
                courant->etudiant.email,
                courant->etudiant.cne,
                courant->etudiant.section,
                courant->etudiant.filiere);
        
        courant = courant->suivant;
    }
    
    fclose(file);
    return 1;
}

/**
 * Add a new student to the linked list
 */
void ajouter_etudiant(NodeEtudiant** tete) {
    NodeEtudiant *nouveau_node = (NodeEtudiant*)malloc(sizeof(NodeEtudiant));
    if (nouveau_node == NULL) {
        printf("Erreur: Échec d'allocation de mémoire.\n");
        return;
    }
    
    // Generate a new ID
    int new_id = 1;
    NodeEtudiant *courant = *tete;
    while (courant != NULL) {
        if (courant->etudiant.id >= new_id) {
            new_id = courant->etudiant.id + 1;
        }
        courant = courant->suivant;
    }
    
    nouveau_node->etudiant.id = new_id;
    
    // Get student details
    printf("\n=== Ajouter un nouvel étudiant ===\n");
    
    get_input("Prénom: ", nouveau_node->etudiant.prenom, sizeof(nouveau_node->etudiant.prenom));
    get_input("Nom: ", nouveau_node->etudiant.nom, sizeof(nouveau_node->etudiant.nom));
    
    // Generate automatic email based on first name and last name
    char email[50];
    generer_email_unique(nouveau_node->etudiant.prenom, nouveau_node->etudiant.nom, 1, 
                        email, sizeof(email));
    
    printf("Email généré automatiquement: %s\n", email);
    strcpy(nouveau_node->etudiant.email, email);
    
    get_input("CNE: ", nouveau_node->etudiant.cne, sizeof(nouveau_node->etudiant.cne));
    get_input("Section: ", nouveau_node->etudiant.section, sizeof(nouveau_node->etudiant.section));
    get_input("Filière: ", nouveau_node->etudiant.filiere, sizeof(nouveau_node->etudiant.filiere));
    
    // Add to the beginning of the list
    nouveau_node->suivant = *tete;
    *tete = nouveau_node;
    
    // Save to file
    if (sauvegarder_etudiants(*tete)) {
        printf("Étudiant ajouté avec succès.\n");
        
        // Create user account
        ajouter_utilisateur_auto(nouveau_node->etudiant.prenom, 
                             nouveau_node->etudiant.nom, 
                             nouveau_node->etudiant.email, 
                             "etudiant");
    } else {
        printf("Erreur lors de la sauvegarde des étudiants.\n");
    }
}

/**
 * Modify information of an existing student
 */
void modifier_etudiant(NodeEtudiant* tete) {
    if (tete == NULL) {
        printf("Aucun étudiant n'est enregistré.\n");
        return;
    }
    
    // Display list of students
    afficher_etudiants(tete);
    
    // Get the ID of the student to modify
    int id = get_int_input("Entrez l'ID de l'étudiant à modifier (0 pour annuler): ", 0, 999);
    if (id == 0) {
        printf("Opération annulée.\n");
        return;
    }
    
    // Find the student
    NodeEtudiant* etudiant = trouver_etudiant_par_id(tete, id);
    if (etudiant == NULL) {
        printf("Étudiant avec ID %d non trouvé.\n", id);
        return;
    }
    
    printf("\n=== Modifier l'étudiant ===\n");
    printf("Laissez vide pour garder la valeur actuelle.\n");
    
    // Get new values or keep current ones
    char buffer[50];
    
    get_input_formatted("Prénom [actuel: %s]: ", buffer, sizeof(buffer), etudiant->etudiant.prenom);
    if (strlen(buffer) > 0) {
        strcpy(etudiant->etudiant.prenom, buffer);
    }
    
    get_input_formatted("Nom [actuel: %s]: ", buffer, sizeof(buffer), etudiant->etudiant.nom);
    if (strlen(buffer) > 0) {
        strcpy(etudiant->etudiant.nom, buffer);
    }
    
    // Ask whether to regenerate the email
    printf("Souhaitez-vous régénérer l'email automatiquement?\n");
    int regenerer_email = get_int_input("1. Oui, 2. Non: ", 1, 2);
    
    if (regenerer_email == 1) {
        // Generate automatic email
        char email[50];
        generer_email_unique(etudiant->etudiant.prenom, etudiant->etudiant.nom, 1, 
                          email, sizeof(email));
        
        printf("Email généré automatiquement: %s\n", email);
        strcpy(etudiant->etudiant.email, email);
    } else {
        // Manual email entry
        get_input_formatted("Email [actuel: %s]: ", buffer, sizeof(buffer), etudiant->etudiant.email);
        if (strlen(buffer) > 0) {
            if (validate_email(buffer)) {
                strcpy(etudiant->etudiant.email, buffer);
            } else {
                printf("Email invalide. L'email n'a pas été modifié.\n");
            }
        }
    }
    
    get_input_formatted("CNE [actuel: %s]: ", buffer, sizeof(buffer), etudiant->etudiant.cne);
    if (strlen(buffer) > 0) {
        strcpy(etudiant->etudiant.cne, buffer);
    }
    
    get_input_formatted("Section [actuel: %s]: ", buffer, sizeof(buffer), etudiant->etudiant.section);
    if (strlen(buffer) > 0) {
        strcpy(etudiant->etudiant.section, buffer);
    }
    
    get_input_formatted("Filière [actuel: %s]: ", buffer, sizeof(buffer), etudiant->etudiant.filiere);
    if (strlen(buffer) > 0) {
        strcpy(etudiant->etudiant.filiere, buffer);
    }
    
    // Save changes
    if (sauvegarder_etudiants(tete)) {
        printf("Étudiant modifié avec succès.\n");
    } else {
        printf("Erreur lors de la sauvegarde des modifications.\n");
    }
}

/**
 * Delete a student from the linked list
 */
void supprimer_etudiant(NodeEtudiant** tete) {
    if (*tete == NULL) {
        printf("Aucun étudiant n'est enregistré.\n");
        return;
    }
    
    // Display list of students
    afficher_etudiants(*tete);
    
    // Get the ID of the student to delete
    int id = get_int_input("Entrez l'ID de l'étudiant à supprimer (0 pour annuler): ", 0, 999);
    if (id == 0) {
        printf("Opération annulée.\n");
        return;
    }
    
    // Find and remove the student
    NodeEtudiant *courant = *tete;
    NodeEtudiant *precedent = NULL;
    
    while (courant != NULL && courant->etudiant.id != id) {
        precedent = courant;
        courant = courant->suivant;
    }
    
    if (courant == NULL) {
        printf("Étudiant avec ID %d non trouvé.\n", id);
        return;
    }
    
    // Confirmation
    printf("Êtes-vous sûr de vouloir supprimer l'étudiant %s %s?\n", 
           courant->etudiant.prenom, courant->etudiant.nom);
    int confirmation = get_int_input("1. Oui, 2. Non: ", 1, 2);
    
    if (confirmation == 1) {
        // Remove the node
        if (precedent == NULL) {
            // It's the first node
            *tete = courant->suivant;
        } else {
            precedent->suivant = courant->suivant;
        }
        
        free(courant);
        
        // Save changes
        if (sauvegarder_etudiants(*tete)) {
            printf("Étudiant supprimé avec succès.\n");
        } else {
            printf("Erreur lors de la sauvegarde des modifications.\n");
        }
    } else {
        printf("Opération annulée.\n");
    }
}

/**
 * Display all students in the linked list
 */
void afficher_etudiants(NodeEtudiant* tete) {
    if (tete == NULL) {
        printf("Aucun étudiant n'est enregistré.\n");
        return;
    }
    
    printf("\n=== Liste des Étudiants ===\n");
    printf("%-4s | %-15s | %-15s | %-25s | %-10s | %-8s | %-15s\n", 
           "ID", "Prénom", "Nom", "Email", "CNE", "Section", "Filière");
    printf("--------------------------------------------------------------------------------------------\n");
    
    NodeEtudiant* courant = tete;
    while (courant != NULL) {
        printf("%-4d | %-15s | %-15s | %-25s | %-10s | %-8s | %-15s\n", 
               courant->etudiant.id,
               courant->etudiant.prenom,
               courant->etudiant.nom,
               courant->etudiant.email,
               courant->etudiant.cne,
               courant->etudiant.section,
               courant->etudiant.filiere);
        
        courant = courant->suivant;
    }
    
    printf("============================================================================================\n\n");
}

/**
 * Find a student by ID
 */
NodeEtudiant* trouver_etudiant_par_id(NodeEtudiant* tete, int id) {
    NodeEtudiant* courant = tete;
    
    while (courant != NULL) {
        if (courant->etudiant.id == id) {
            return courant;
        }
        courant = courant->suivant;
    }
    
    return NULL; // Not found
}

/**
 * Search students by multiple criteria
 */
void rechercher_etudiants(NodeEtudiant* tete) {
    if (tete == NULL) {
        printf("Aucun étudiant n'est enregistré.\n");
        return;
    }
    
    printf("\n=== Recherche d'Étudiants ===\n");
    printf("1. Rechercher par ID\n");
    printf("2. Rechercher par Nom\n");
    printf("3. Rechercher par Prénom\n");
    printf("4. Rechercher par Email\n");
    printf("5. Rechercher par CNE\n");
    printf("6. Rechercher par Section\n");
    printf("7. Rechercher par Filière\n");
    printf("0. Annuler\n");
    
    int critere = get_int_input("Choisissez un critère de recherche: ", 0, 7);
    
    if (critere == 0) {
        return;
    }
    
    char terme_recherche[50];
    int id_recherche;
    int resultats = 0;
    
    // Get search term based on the selected criterion
    switch (critere) {
        case 1: // ID
            id_recherche = get_int_input("Entrez l'ID à rechercher: ", 1, 9999);
            break;
        case 2: // Nom
            get_input("Entrez le nom à rechercher: ", terme_recherche, sizeof(terme_recherche));
            break;
        case 3: // Prénom
            get_input("Entrez le prénom à rechercher: ", terme_recherche, sizeof(terme_recherche));
            break;
        case 4: // Email
            get_input("Entrez l'email à rechercher: ", terme_recherche, sizeof(terme_recherche));
            break;
        case 5: // CNE
            get_input("Entrez le CNE à rechercher: ", terme_recherche, sizeof(terme_recherche));
            break;
        case 6: // Section
            get_input("Entrez la section à rechercher: ", terme_recherche, sizeof(terme_recherche));
            break;
        case 7: // Filière
            get_input("Entrez la filière à rechercher: ", terme_recherche, sizeof(terme_recherche));
            break;
    }
    
    printf("\n=== Résultats de la Recherche ===\n");
    printf("%-4s | %-15s | %-15s | %-25s | %-10s | %-8s | %-15s\n", 
           "ID", "Prénom", "Nom", "Email", "CNE", "Section", "Filière");
    printf("--------------------------------------------------------------------------------------------\n");
    
    // Perform the search based on the selected criterion
    NodeEtudiant* courant = tete;
    while (courant != NULL) {
        int match = 0;
        
        switch (critere) {
            case 1: // ID
                match = (courant->etudiant.id == id_recherche);
                break;
            case 2: // Nom
                match = (str_case_search(courant->etudiant.nom, terme_recherche) != NULL);
                break;
            case 3: // Prénom
                match = (str_case_search(courant->etudiant.prenom, terme_recherche) != NULL);
                break;
            case 4: // Email
                match = (str_case_search(courant->etudiant.email, terme_recherche) != NULL);
                break;
            case 5: // CNE
                match = (str_case_search(courant->etudiant.cne, terme_recherche) != NULL);
                break;
            case 6: // Section
                match = (str_case_search(courant->etudiant.section, terme_recherche) != NULL);
                break;
            case 7: // Filière
                match = (str_case_search(courant->etudiant.filiere, terme_recherche) != NULL);
                break;
        }
        
        if (match) {
            printf("%-4d | %-15s | %-15s | %-25s | %-10s | %-8s | %-15s\n", 
                   courant->etudiant.id,
                   courant->etudiant.prenom,
                   courant->etudiant.nom,
                   courant->etudiant.email,
                   courant->etudiant.cne,
                   courant->etudiant.section,
                   courant->etudiant.filiere);
            resultats++;
        }
        
        courant = courant->suivant;
    }
    
    printf("============================================================================================\n");
    printf("%d résultat(s) trouvé(s).\n\n", resultats);
}

/**
 * Free memory allocated for student linked list
 */
void liberer_etudiants(NodeEtudiant** tete) {
    NodeEtudiant *courant = *tete;
    NodeEtudiant *suivant;
    
    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    
    *tete = NULL;
}
