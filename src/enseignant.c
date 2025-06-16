/**
 * @file enseignant.c
 * @brief Implementation of teacher management module
 */

#include "../include/enseignant.h"
#include "../include/authentication.h"
#include "../include/utils.h"

#define ENSEIGNANTS_FILE "data/enseignants.txt"

/**
 * Load teachers from file into a linked list
 */
int charger_enseignants(NodeEnseignant** tete) {
    FILE *file;
    *tete = NULL;
    
    create_data_dir();
    
    if (!file_exists(ENSEIGNANTS_FILE)) {
        // Create empty file if it doesn't exist
        create_file_if_not_exists(ENSEIGNANTS_FILE);
        return 1;
    }
    
    file = fopen(ENSEIGNANTS_FILE, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des enseignants.\n");
        return 0;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline
        
        NodeEnseignant *nouveau_node = (NodeEnseignant*)malloc(sizeof(NodeEnseignant));
        if (nouveau_node == NULL) {
            printf("Erreur: Échec d'allocation de mémoire.\n");
            fclose(file);
            return 0;
        }
        
        // Parse the line (id|prenom|nom|email|code_enseignant|matiere_enseignee)
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
               &nouveau_node->enseignant.id,
               nouveau_node->enseignant.prenom,
               nouveau_node->enseignant.nom,
               nouveau_node->enseignant.email,
               nouveau_node->enseignant.code_enseignant,
               nouveau_node->enseignant.matiere_enseignee);
        
        // Add to the beginning of the list
        nouveau_node->suivant = *tete;
        *tete = nouveau_node;
    }
    
    fclose(file);
    
    // Synchronize teacher IDs with user IDs
    synchroniser_ids_enseignants(tete);
    
    return 1;
}

/**
 * Synchronize teacher IDs with user IDs based on email
 */
void synchroniser_ids_enseignants(NodeEnseignant** tete) {
    // Load users
    Utilisateur utilisateurs[MAX_USERS];
    int nb_utilisateurs = 0;
    
    if (!charger_utilisateurs(utilisateurs, &nb_utilisateurs)) {
        return;
    }
    
    // For each teacher, find the corresponding user ID
    NodeEnseignant* courant = *tete;
    int synchronisation_effectuee = 0;
    
    while (courant != NULL) {
        for (int i = 0; i < nb_utilisateurs; i++) {
            // Compare emails to find the matching user
            if (strcmp(courant->enseignant.email, utilisateurs[i].email) == 0 && 
                strcmp(utilisateurs[i].role, "enseignant") == 0) {
                
                // If IDs don't match, update the teacher ID
                if (courant->enseignant.id != utilisateurs[i].id) {
                    courant->enseignant.id = utilisateurs[i].id;
                    synchronisation_effectuee = 1;
                }
                break;
            }
        }
        courant = courant->suivant;
    }
    
    // If any ID was changed, save the changes to file
    if (synchronisation_effectuee) {
        sauvegarder_enseignants(*tete);
    }
}

/**
 * Save teachers from linked list to file
 */
int sauvegarder_enseignants(NodeEnseignant* tete) {
    FILE *file = fopen(ENSEIGNANTS_FILE, "w");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des enseignants en écriture.\n");
        return 0;
    }
    
    NodeEnseignant* courant = tete;
    while (courant != NULL) {
        if (courant->enseignant.id>0){
        fprintf(file, "%d|%s|%s|%s|%s|%s\n",
                courant->enseignant.id,
                courant->enseignant.prenom,
                courant->enseignant.nom,
                courant->enseignant.email,
                courant->enseignant.code_enseignant,
                courant->enseignant.matiere_enseignee);
        }
        courant = courant->suivant;
        
    }
    
    fclose(file);
    return 1;
}

/**
 * Add a new teacher to the linked list
 */
void ajouter_enseignant(NodeEnseignant** tete) {
    NodeEnseignant *nouveau_node = (NodeEnseignant*)malloc(sizeof(NodeEnseignant));
    if (nouveau_node == NULL) {
        printf("Erreur: Échec d'allocation de mémoire.\n");
        return;
    }
    
    // Generate a new ID
    int new_id = generate_id();
    nouveau_node->enseignant.id = new_id;
        
    // Get teacher details
    printf("\n=== Ajouter un nouvel enseignant ===\n");
    
    get_input("Prénom: ", nouveau_node->enseignant.prenom, sizeof(nouveau_node->enseignant.prenom));
    get_input("Nom: ", nouveau_node->enseignant.nom, sizeof(nouveau_node->enseignant.nom));
    
    // Generate automatic email based on first name and last name
    char email[50];
    generer_email_unique(nouveau_node->enseignant.prenom, nouveau_node->enseignant.nom, 0, 
                       email, sizeof(email));
    
    printf("Email généré automatiquement: %s\n", email);
    strcpy(nouveau_node->enseignant.email, email);
    
    get_input("Code Enseignant: ", nouveau_node->enseignant.code_enseignant, 
             sizeof(nouveau_node->enseignant.code_enseignant));
    get_input("Matière Enseignée: ", nouveau_node->enseignant.matiere_enseignee, 
             sizeof(nouveau_node->enseignant.matiere_enseignee));
    
    // Add to the beginning of the list
    nouveau_node->suivant = *tete;
    *tete = nouveau_node;
    
    // Save to file
    if (sauvegarder_enseignants(*tete)) {
        printf("Enseignant ajouté avec succès.\n");
        
        // Create user account
        ajouter_utilisateur_auto(new_id,
                             nouveau_node->enseignant.prenom, 
                             nouveau_node->enseignant.nom, 
                             nouveau_node->enseignant.email, 
                             "enseignant");
    } else {
        printf("Erreur lors de la sauvegarde des enseignants.\n");
    }
}

/**
 * Modify information of an existing teacher
 */
void modifier_enseignant(NodeEnseignant* tete) {
    if (tete == NULL) {
        printf("Aucun enseignant n'est enregistré.\n");
        return;
    }
    
    // Display list of teachers
    afficher_enseignants(tete);
    
    // Get the ID of the teacher to modify
    int id = get_int_input("Entrez l'ID de l'enseignant à modifier (0 pour annuler): ", 0, 999);
    if (id == 0) {
        printf("Opération annulée.\n");
        return;
    }
    
    // Find the teacher
    NodeEnseignant* enseignant = trouver_enseignant_par_id(tete, id);
    if (enseignant == NULL) {
        printf("Enseignant avec ID %d non trouvé.\n", id);
        return;
    }
    
    printf("\n=== Modifier l'enseignant ===\n");
    printf("Laissez vide pour garder la valeur actuelle.\n");
    
    // Get new values or keep current ones
    char buffer[50];
    
    get_input_formatted("Prénom [actuel: %s]: ", buffer, sizeof(buffer), enseignant->enseignant.prenom);
    if (strlen(buffer) > 0) {
        strcpy(enseignant->enseignant.prenom, buffer);
    }
    
    get_input_formatted("Nom [actuel: %s]: ", buffer, sizeof(buffer), enseignant->enseignant.nom);
    if (strlen(buffer) > 0) {
        strcpy(enseignant->enseignant.nom, buffer);
    }
    
    // Ask whether to regenerate the email
    printf("Souhaitez-vous régénérer l'email automatiquement?\n");
    int regenerer_email = get_int_input("1. Oui, 2. Non: ", 1, 2);
    
    if (regenerer_email == 1) {
        // Generate automatic email
        char email[50];
        generer_email_unique(enseignant->enseignant.prenom, enseignant->enseignant.nom, 0, 
                          email, sizeof(email));
        
        printf("Email généré automatiquement: %s\n", email);
        strcpy(enseignant->enseignant.email, email);
    } else {
        // Manual email entry
        get_input_formatted("Email [actuel: %s]: ", buffer, sizeof(buffer), enseignant->enseignant.email);
        if (strlen(buffer) > 0) {
            if (validate_email(buffer)) {
                strcpy(enseignant->enseignant.email, buffer);
            } else {
                printf("Email invalide. L'email n'a pas été modifié.\n");
            }
        }
    }
    
    get_input_formatted("Code Enseignant [actuel: %s]: ", buffer, sizeof(buffer), 
             enseignant->enseignant.code_enseignant);
    if (strlen(buffer) > 0) {
        strcpy(enseignant->enseignant.code_enseignant, buffer);
    }
    
    get_input_formatted("Matière Enseignée [actuel: %s]: ", buffer, sizeof(buffer), 
             enseignant->enseignant.matiere_enseignee);
    if (strlen(buffer) > 0) {
        strcpy(enseignant->enseignant.matiere_enseignee, buffer);
    }
    
    // Save changes
    if (sauvegarder_enseignants(tete)) {
        printf("Enseignant modifié avec succès.\n");
    } else {
        printf("Erreur lors de la sauvegarde des modifications.\n");
    }
}

/**
 * Delete a teacher from the linked list
 */
void supprimer_enseignant(NodeEnseignant** tete) {
    if (*tete == NULL) {
        printf("Aucun enseignant n'est enregistré.\n");
        return;
    }
    
    // Display list of teachers
    afficher_enseignants(*tete);
    
    // Get the ID of the teacher to delete
    int id = get_int_input("Entrez l'ID de l'enseignant à supprimer (0 pour annuler): ", 0, 999);
    if (id == 0) {
        printf("Opération annulée.\n");
        return;
    }
    
    // Find and remove the teacher
    NodeEnseignant *courant = *tete;
    NodeEnseignant *precedent = NULL;
    
    while (courant != NULL && courant->enseignant.id != id) {
        precedent = courant;
        courant = courant->suivant;
    }
    
    if (courant == NULL) {
        printf("Enseignant avec ID %d non trouvé.\n", id);
        return;
    }
    
    // Confirmation
    printf("Êtes-vous sûr de vouloir supprimer l'enseignant %s %s?\n", 
           courant->enseignant.prenom, courant->enseignant.nom);
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
        if (sauvegarder_enseignants(*tete)) {
            printf("Enseignant supprimé avec succès.\n");
        } else {
            printf("Erreur lors de la sauvegarde des modifications.\n");
        }
    } else {
        printf("Opération annulée.\n");
    }
}

/**
 * Display all teachers in the linked list
 */
void afficher_enseignants(NodeEnseignant* tete) {
    if (tete == NULL) {
        printf("Aucun enseignant n'est enregistré.\n");
        return;
    }
    
    printf("\n=== Liste des Enseignants ===\n");
    printf("%-4s | %-15s | %-15s | %-25s | %-15s | %-20s\n", 
           "ID", "Prénom", "Nom", "Email", "Code", "Matière");
    printf("-----------------------------------------------------------------------------------------\n");
    
    NodeEnseignant* courant = tete;
    while (courant != NULL) {
        printf("%-4d | %-15s | %-15s | %-25s | %-15s | %-20s\n", 
               courant->enseignant.id,
               courant->enseignant.prenom,
               courant->enseignant.nom,
               courant->enseignant.email,
               courant->enseignant.code_enseignant,
               courant->enseignant.matiere_enseignee);
        
        courant = courant->suivant;
    }
    
    printf("=========================================================================================\n\n");
}

/**
 * Find a teacher by ID
 */
NodeEnseignant* trouver_enseignant_par_id(NodeEnseignant* tete, int id) {
    NodeEnseignant* courant = tete;
    
    while (courant != NULL) {
        if (courant->enseignant.id == id) {
            return courant;
        }
        courant = courant->suivant;
    }
    
    return NULL; // Not found
}

/**
 * Search teachers by multiple criteria
 */
void rechercher_enseignants(NodeEnseignant* tete) {
    if (tete == NULL) {
        printf("Aucun enseignant n'est enregistré.\n");
        return;
    }
    
    printf("\n=== Recherche d'Enseignants ===\n");
    printf("1. Rechercher par ID\n");
    printf("2. Rechercher par Nom\n");
    printf("3. Rechercher par Prénom\n");
    printf("4. Rechercher par Email\n");
    printf("5. Rechercher par Code Enseignant\n");
    printf("6. Rechercher par Matière Enseignée\n");
    printf("0. Annuler\n");
    
    int critere = get_int_input("Choisissez un critère de recherche: ", 0, 6);
    
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
        case 5: // Code Enseignant
            get_input("Entrez le code enseignant à rechercher: ", terme_recherche, sizeof(terme_recherche));
            break;
        case 6: // Matière Enseignée
            get_input("Entrez la matière enseignée à rechercher: ", terme_recherche, sizeof(terme_recherche));
            break;
    }
    
    printf("\n=== Résultats de la Recherche ===\n");
    printf("%-4s | %-15s | %-15s | %-25s | %-15s | %-20s\n", 
           "ID", "Prénom", "Nom", "Email", "Code", "Matière");
    printf("-----------------------------------------------------------------------------------------\n");
    
    // Perform the search based on the selected criterion
    NodeEnseignant* courant = tete;
    while (courant != NULL) {
        int match = 0;
        
        switch (critere) {
            case 1: // ID
                match = (courant->enseignant.id == id_recherche);
                break;
            case 2: // Nom
                match = (str_case_search(courant->enseignant.nom, terme_recherche) != NULL);
                break;
            case 3: // Prénom
                match = (str_case_search(courant->enseignant.prenom, terme_recherche) != NULL);
                break;
            case 4: // Email
                match = (str_case_search(courant->enseignant.email, terme_recherche) != NULL);
                break;
            case 5: // Code Enseignant
                match = (str_case_search(courant->enseignant.code_enseignant, terme_recherche) != NULL);
                break;
            case 6: // Matière Enseignée
                match = (str_case_search(courant->enseignant.matiere_enseignee, terme_recherche) != NULL);
                break;
        }
        
        if (match) {
            printf("%-4d | %-15s | %-15s | %-25s | %-15s | %-20s\n", 
                   courant->enseignant.id,
                   courant->enseignant.prenom,
                   courant->enseignant.nom,
                   courant->enseignant.email,
                   courant->enseignant.code_enseignant,
                   courant->enseignant.matiere_enseignee);
            resultats++;
        }
        
        courant = courant->suivant;
    }
    
    printf("=========================================================================================\n");
    printf("%d résultat(s) trouvé(s).\n\n", resultats);
}

/**
 * Free memory allocated for teacher linked list
 */
void liberer_enseignants(NodeEnseignant** tete) {
    NodeEnseignant *courant = *tete;
    NodeEnseignant *suivant;
    
    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    
    *tete = NULL;
}
