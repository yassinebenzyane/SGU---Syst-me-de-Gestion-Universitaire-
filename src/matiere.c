/**
 * @file matiere.c
 * @brief Implementation of subject management module
 */

#include "../include/matiere.h"
#include "../include/utils.h"

#define MATIERES_FILE "data/matieres.txt"

/**
 * Load subjects from file into a linked list
 */
int charger_matieres(NodeMatiere** tete) {
    FILE *file;
    *tete = NULL;
    
    create_data_dir();
    
    if (!file_exists(MATIERES_FILE)) {
        // Create empty file if it doesn't exist
        create_file_if_not_exists(MATIERES_FILE);
        return 1;
    }
    
    file = fopen(MATIERES_FILE, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des matières.\n");
        return 0;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline
        
        NodeMatiere *nouveau_node = (NodeMatiere*)malloc(sizeof(NodeMatiere));
        if (nouveau_node == NULL) {
            printf("Erreur: Échec d'allocation de mémoire.\n");
            fclose(file);
            return 0;
        }
        
        // Parse the line (id|code_matiere|nom_matiere|coefficient)
        sscanf(line, "%d|%[^|]|%[^|]|%f",
               &nouveau_node->matiere.id,
               nouveau_node->matiere.code_matiere,
               nouveau_node->matiere.nom_matiere,
               &nouveau_node->matiere.coefficient);
        
        // Add to the beginning of the list
        nouveau_node->suivant = *tete;
        *tete = nouveau_node;
    }
    
    fclose(file);
    return 1;
}

/**
 * Save subjects from linked list to file
 */
int sauvegarder_matieres(NodeMatiere* tete) {
    FILE *file = fopen(MATIERES_FILE, "w");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des matières en écriture.\n");
        return 0;
    }
    
    NodeMatiere* courant = tete;
    while (courant != NULL) {
        fprintf(file, "%d|%s|%s|%.2f\n",
                courant->matiere.id,
                courant->matiere.code_matiere,
                courant->matiere.nom_matiere,
                courant->matiere.coefficient);
        
        courant = courant->suivant;
    }
    
    fclose(file);
    return 1;
}

/**
 * Add a new subject to the linked list
 */
void ajouter_matiere(NodeMatiere** tete) {
    NodeMatiere *nouveau_node = (NodeMatiere*)malloc(sizeof(NodeMatiere));
    if (nouveau_node == NULL) {
        printf("Erreur: Échec d'allocation de mémoire.\n");
        return;
    }
    
    // Generate a new ID
    int new_id = 1;
    NodeMatiere *courant = *tete;
    while (courant != NULL) {
        if (courant->matiere.id >= new_id) {
            new_id = courant->matiere.id + 1;
        }
        courant = courant->suivant;
    }
    
    nouveau_node->matiere.id = new_id;
    
    // Get subject details
    printf("\n=== Ajouter une nouvelle matière ===\n");
    
    get_input("Code Matière: ", nouveau_node->matiere.code_matiere, 
             sizeof(nouveau_node->matiere.code_matiere));
    
    get_input("Nom Matière: ", nouveau_node->matiere.nom_matiere, 
             sizeof(nouveau_node->matiere.nom_matiere));
    
    nouveau_node->matiere.coefficient = get_float_input("Coefficient (0.1-10.0): ", 0.1, 10.0);
    
    // Add to the beginning of the list
    nouveau_node->suivant = *tete;
    *tete = nouveau_node;
    
    // Save to file
    if (sauvegarder_matieres(*tete)) {
        printf("Matière ajoutée avec succès.\n");
    } else {
        printf("Erreur lors de la sauvegarde des matières.\n");
    }
}

/**
 * Modify information of an existing subject
 */
void modifier_matiere(NodeMatiere* tete) {
    if (tete == NULL) {
        printf("Aucune matière n'est enregistrée.\n");
        return;
    }
    
    // Display list of subjects
    afficher_matieres(tete);
    
    // Get the ID of the subject to modify
    int id = get_int_input("Entrez l'ID de la matière à modifier (0 pour annuler): ", 0, 999);
    if (id == 0) {
        printf("Opération annulée.\n");
        return;
    }
    
    // Find the subject
    NodeMatiere* matiere = trouver_matiere_par_id(tete, id);
    if (matiere == NULL) {
        printf("Matière avec ID %d non trouvée.\n", id);
        return;
    }
    
    printf("\n=== Modifier la matière ===\n");
    printf("Laissez vide pour garder la valeur actuelle.\n");
    
    // Get new values or keep current ones
    char buffer[50];
    
    get_input_formatted("Code Matière [actuel: %s]: ", buffer, sizeof(buffer), 
             matiere->matiere.code_matiere);
    if (strlen(buffer) > 0) {
        strcpy(matiere->matiere.code_matiere, buffer);
    }
    
    get_input_formatted("Nom Matière [actuel: %s]: ", buffer, sizeof(buffer), 
             matiere->matiere.nom_matiere);
    if (strlen(buffer) > 0) {
        strcpy(matiere->matiere.nom_matiere, buffer);
    }
    
    char prompt[100];
    snprintf(prompt, sizeof(prompt), "Coefficient (0.1-10.0) [actuel: %.2f, 0 pour garder]: ", 
             matiere->matiere.coefficient);
    float new_coef = get_float_input(prompt, 0.0, 10.0);
    if (new_coef > 0.0) {
        matiere->matiere.coefficient = new_coef;
    }
    
    // Save changes
    if (sauvegarder_matieres(tete)) {
        printf("Matière modifiée avec succès.\n");
    } else {
        printf("Erreur lors de la sauvegarde des modifications.\n");
    }
}

/**
 * Delete a subject from the linked list
 */
void supprimer_matiere(NodeMatiere** tete) {
    if (*tete == NULL) {
        printf("Aucune matière n'est enregistrée.\n");
        return;
    }
    
    // Display list of subjects
    afficher_matieres(*tete);
    
    // Get the ID of the subject to delete
    int id = get_int_input("Entrez l'ID de la matière à supprimer (0 pour annuler): ", 0, 999);
    if (id == 0) {
        printf("Opération annulée.\n");
        return;
    }
    
    // Find and remove the subject
    NodeMatiere *courant = *tete;
    NodeMatiere *precedent = NULL;
    
    while (courant != NULL && courant->matiere.id != id) {
        precedent = courant;
        courant = courant->suivant;
    }
    
    if (courant == NULL) {
        printf("Matière avec ID %d non trouvée.\n", id);
        return;
    }
    
    // Confirmation
    printf("Êtes-vous sûr de vouloir supprimer la matière %s?\n", 
           courant->matiere.nom_matiere);
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
        if (sauvegarder_matieres(*tete)) {
            printf("Matière supprimée avec succès.\n");
        } else {
            printf("Erreur lors de la sauvegarde des modifications.\n");
        }
    } else {
        printf("Opération annulée.\n");
    }
}

/**
 * Display all subjects in the linked list
 */
void afficher_matieres(NodeMatiere* tete) {
    if (tete == NULL) {
        printf("Aucune matière n'est enregistrée.\n");
        return;
    }
    
    printf("\n=== Liste des Matières ===\n");
    printf("%-4s | %-15s | %-25s | %-10s\n", 
           "ID", "Code", "Nom", "Coefficient");
    printf("------------------------------------------------------------\n");
    
    NodeMatiere* courant = tete;
    while (courant != NULL) {
        printf("%-4d | %-15s | %-25s | %-10.2f\n", 
               courant->matiere.id,
               courant->matiere.code_matiere,
               courant->matiere.nom_matiere,
               courant->matiere.coefficient);
        
        courant = courant->suivant;
    }
    
    printf("============================================================\n\n");
}

/**
 * Find a subject by ID
 */
NodeMatiere* trouver_matiere_par_id(NodeMatiere* tete, int id) {
    NodeMatiere* courant = tete;
    
    while (courant != NULL) {
        if (courant->matiere.id == id) {
            return courant;
        }
        courant = courant->suivant;
    }
    
    return NULL; // Not found
}

/**
 * Search subjects by multiple criteria
 */
void rechercher_matieres(NodeMatiere* tete) {
    if (tete == NULL) {
        printf("Aucune matière n'est enregistrée.\n");
        return;
    }
    
    printf("\n=== Recherche de Matières ===\n");
    printf("1. Rechercher par ID\n");
    printf("2. Rechercher par Code Matière\n");
    printf("3. Rechercher par Nom Matière\n");
    printf("4. Rechercher par Coefficient\n");
    printf("0. Annuler\n");
    
    int critere = get_int_input("Choisissez un critère de recherche: ", 0, 4);
    
    if (critere == 0) {
        return;
    }
    
    char terme_recherche[50];
    int id_recherche;
    float coef_min = 0.0, coef_max = 0.0;
    int resultats = 0;
    
    // Get search term based on the selected criterion
    switch (critere) {
        case 1: // ID
            id_recherche = get_int_input("Entrez l'ID à rechercher: ", 1, 9999);
            break;
        case 2: // Code Matière
            get_input("Entrez le code matière à rechercher: ", terme_recherche, sizeof(terme_recherche));
            break;
        case 3: // Nom Matière
            get_input("Entrez le nom matière à rechercher: ", terme_recherche, sizeof(terme_recherche));
            break;
        case 4: // Coefficient
            printf("Recherche par plage de coefficient\n");
            coef_min = get_float_input("Coefficient minimum: ", 0.0, 10.0);
            coef_max = get_float_input("Coefficient maximum: ", coef_min, 10.0);
            break;
    }
    
    printf("\n=== Résultats de la Recherche ===\n");
    printf("%-4s | %-15s | %-25s | %-10s\n", 
           "ID", "Code", "Nom", "Coefficient");
    printf("------------------------------------------------------------\n");
    
    // Perform the search based on the selected criterion
    NodeMatiere* courant = tete;
    while (courant != NULL) {
        int match = 0;
        
        switch (critere) {
            case 1: // ID
                match = (courant->matiere.id == id_recherche);
                break;
            case 2: // Code Matière
                match = (str_case_search(courant->matiere.code_matiere, terme_recherche) != NULL);
                break;
            case 3: // Nom Matière
                match = (str_case_search(courant->matiere.nom_matiere, terme_recherche) != NULL);
                break;
            case 4: // Coefficient
                match = (courant->matiere.coefficient >= coef_min && 
                        courant->matiere.coefficient <= coef_max);
                break;
        }
        
        if (match) {
            printf("%-4d | %-15s | %-25s | %-10.2f\n", 
                   courant->matiere.id,
                   courant->matiere.code_matiere,
                   courant->matiere.nom_matiere,
                   courant->matiere.coefficient);
            resultats++;
        }
        
        courant = courant->suivant;
    }
    
    printf("============================================================\n");
    printf("%d résultat(s) trouvé(s).\n\n", resultats);
}

/**
 * Free memory allocated for subject linked list
 */
void liberer_matieres(NodeMatiere** tete) {
    NodeMatiere *courant = *tete;
    NodeMatiere *suivant;
    
    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    
    *tete = NULL;
}
