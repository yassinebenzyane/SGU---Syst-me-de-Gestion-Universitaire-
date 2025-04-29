/**
 * @file annonces.c
 * @brief Implementation of announcement management module
 */

#include "../include/annonces.h"
#include "../include/matiere.h"
#include "../include/enseignant.h"
#include "../include/utils.h"
#include <time.h>

#define ANNONCES_FILE "data/annonces.txt"

/**
 * Load announcements from file into a linked list
 */
int charger_annonces(NodeAnnonce** tete) {
    *tete = NULL;
    
    // Ensure data directory exists
    if (!create_data_dir()) {
        printf("Erreur: Impossible de créer le répertoire de données.\n");
        return 0;
    }
    
    // Create file if it doesn't exist
    if (!create_file_if_not_exists(ANNONCES_FILE)) {
        printf("Erreur: Impossible de créer le fichier des annonces.\n");
        return 0;
    }
    
    FILE *file = fopen(ANNONCES_FILE, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des annonces.\n");
        return 0;
    }
    
    char line[1200]; // Buffer for reading lines (increased for long announcement content)
    NodeAnnonce *dernier = NULL;
    
    while (fgets(line, sizeof(line), file)) {
        // Create a new node
        NodeAnnonce *nouveau_node = (NodeAnnonce*)malloc(sizeof(NodeAnnonce));
        if (nouveau_node == NULL) {
            printf("Erreur: Échec d'allocation de mémoire.\n");
            fclose(file);
            return 0;
        }
        
        // Parse the line
        char *token = strtok(line, "|");
        if (token) {
            nouveau_node->annonce.id = atoi(token);
            
            token = strtok(NULL, "|");
            if (token) strncpy(nouveau_node->annonce.titre, token, sizeof(nouveau_node->annonce.titre) - 1);
            
            token = strtok(NULL, "|");
            if (token) strncpy(nouveau_node->annonce.contenu, token, sizeof(nouveau_node->annonce.contenu) - 1);
            
            token = strtok(NULL, "|");
            if (token) strncpy(nouveau_node->annonce.auteur, token, sizeof(nouveau_node->annonce.auteur) - 1);
            
            token = strtok(NULL, "|");
            if (token) strncpy(nouveau_node->annonce.date_creation, token, sizeof(nouveau_node->annonce.date_creation) - 1);
            
            token = strtok(NULL, "|");
            if (token) nouveau_node->annonce.id_matiere = atoi(token);
            
            token = strtok(NULL, "|");
            if (token) nouveau_node->annonce.id_enseignant = atoi(token);
            else nouveau_node->annonce.id_enseignant = 0; // Default value if not present in file
            
            // Ensure null termination of strings
            nouveau_node->annonce.titre[sizeof(nouveau_node->annonce.titre) - 1] = '\0';
            nouveau_node->annonce.contenu[sizeof(nouveau_node->annonce.contenu) - 1] = '\0';
            nouveau_node->annonce.auteur[sizeof(nouveau_node->annonce.auteur) - 1] = '\0';
            nouveau_node->annonce.date_creation[sizeof(nouveau_node->annonce.date_creation) - 1] = '\0';
        }
        
        nouveau_node->suivant = NULL;
        
        // Add to the list
        if (*tete == NULL) {
            *tete = nouveau_node;
        } else {
            dernier->suivant = nouveau_node;
        }
        
        dernier = nouveau_node;
    }
    
    fclose(file);
    return 1;
}

/**
 * Save announcements from linked list to file
 */
int sauvegarder_annonces(NodeAnnonce* tete) {
    FILE *file = fopen(ANNONCES_FILE, "w");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des annonces en écriture.\n");
        return 0;
    }
    
    NodeAnnonce* courant = tete;
    while (courant != NULL) {
        fprintf(file, "%d|%s|%s|%s|%s|%d|%d\n",
                courant->annonce.id,
                courant->annonce.titre,
                courant->annonce.contenu,
                courant->annonce.auteur,
                courant->annonce.date_creation,
                courant->annonce.id_matiere,
                courant->annonce.id_enseignant);
        
        courant = courant->suivant;
    }
    
    fclose(file);
    return 1;
}

/**
 * Add a new announcement to the linked list
 */
void ajouter_annonce(NodeAnnonce** tete, int id_enseignant) {
    NodeAnnonce *nouveau_node = (NodeAnnonce*)malloc(sizeof(NodeAnnonce));
    if (nouveau_node == NULL) {
        printf("Erreur: Échec d'allocation de mémoire.\n");
        return;
    }
    
    // Generate a new ID
    int new_id = 1;
    NodeAnnonce *courant = *tete;
    while (courant != NULL) {
        if (courant->annonce.id >= new_id) {
            new_id = courant->annonce.id + 1;
        }
        courant = courant->suivant;
    }
    
    nouveau_node->annonce.id = new_id;
    
    // Get announcement details
    printf("\n=== Ajouter une nouvelle annonce ===\n");
    
    get_input("Titre: ", nouveau_node->annonce.titre, sizeof(nouveau_node->annonce.titre));
    printf("Contenu (tapez 'fin' sur une nouvelle ligne pour terminer):\n");
    
    char buffer[1024] = {0};
    char line[100];
    while (1) {
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = 0; // Remove trailing newline
        
        if (strcmp(line, "fin") == 0) {
            break;
        }
        
        strcat(buffer, line);
        strcat(buffer, "\n");
        
        if (strlen(buffer) + 10 >= sizeof(nouveau_node->annonce.contenu)) {
            printf("Contenu trop long, arrêt.\n");
            break;
        }
    }
    
    strncpy(nouveau_node->annonce.contenu, buffer, sizeof(nouveau_node->annonce.contenu) - 1);
    nouveau_node->annonce.contenu[sizeof(nouveau_node->annonce.contenu) - 1] = '\0';
    
    // Get author information from teacher ID
    // In a real implementation, you'd query the teacher's name
    // For now, we'll use a placeholder
    snprintf(nouveau_node->annonce.auteur, sizeof(nouveau_node->annonce.auteur), "Enseignant ID: %d", id_enseignant);
    
    // Set creation date
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(nouveau_node->annonce.date_creation, sizeof(nouveau_node->annonce.date_creation), 
             "%d/%m/%Y %H:%M", t);
    
    // Get subject ID
    // You need to implement a way to list available subjects and select one
    printf("\nMatière concernée (0 pour annonce générale): ");
    scanf("%d", &nouveau_node->annonce.id_matiere);
    getchar(); // Clear the newline from input buffer
    
    // Set the teacher ID
    nouveau_node->annonce.id_enseignant = id_enseignant;
    
    // Add to the beginning of the list
    nouveau_node->suivant = *tete;
    *tete = nouveau_node;
    
    // Save to file
    if (sauvegarder_annonces(*tete)) {
        printf("Annonce ajoutée avec succès.\n");
    } else {
        printf("Erreur lors de la sauvegarde des annonces.\n");
    }
}

/**
 * Modify an existing announcement
 */
void modifier_annonce(NodeAnnonce* tete, int id_enseignant) {
    if (tete == NULL) {
        printf("Aucune annonce n'est enregistrée.\n");
        return;
    }
    
    // Display list of announcements by this teacher
    printf("\n=== Vos Annonces ===\n");
    printf("%-4s | %-30s | %-20s | %-15s\n", 
           "ID", "Titre", "Date", "Matière");
    printf("---------------------------------------------------------------------------\n");
    
    NodeAnnonce* courant = tete;
    int found = 0;
    while (courant != NULL) {
        // Check if this announcement belongs to the current teacher using id_enseignant field
        if (courant->annonce.id_enseignant == id_enseignant) {
            printf("%-4d | %-30s | %-20s | %-15d\n", 
                   courant->annonce.id,
                   courant->annonce.titre,
                   courant->annonce.date_creation,
                   courant->annonce.id_matiere);
            found = 1;
        }
        
        courant = courant->suivant;
    }
    
    if (!found) {
        printf("Vous n'avez pas encore créé d'annonces.\n");
        return;
    }
    
    printf("---------------------------------------------------------------------------\n");
    
    // Get the ID of the announcement to modify
    int id = get_int_input("Entrez l'ID de l'annonce à modifier (0 pour annuler): ", 0, 9999);
    if (id == 0) {
        printf("Opération annulée.\n");
        return;
    }
    
    // Find the announcement
    NodeAnnonce* annonce = trouver_annonce_par_id(tete, id);
    if (annonce == NULL) {
        printf("Annonce avec ID %d non trouvée.\n", id);
        return;
    }
    
    // Verify that this teacher is the author
    if (annonce->annonce.id_enseignant != id_enseignant) {
        printf("Vous n'êtes pas autorisé à modifier cette annonce.\n");
        return;
    }
    
    printf("\n=== Modifier l'annonce ===\n");
    printf("Laissez vide pour garder la valeur actuelle.\n");
    
    // Get new values or keep current ones
    char buffer[1024];
    
    get_input_formatted("Titre [actuel: %s]: ", buffer, sizeof(buffer), annonce->annonce.titre);
    if (strlen(buffer) > 0) {
        strcpy(annonce->annonce.titre, buffer);
    }
    
    printf("Contenu actuel:\n%s\n", annonce->annonce.contenu);
    printf("Nouveau contenu (tapez 'fin' sur une nouvelle ligne pour terminer, ou juste 'fin' pour garder l'actuel):\n");
    
    char content_buffer[1024] = {0};
    char line[100];
    fgets(line, sizeof(line), stdin); // Clear any previous input
    line[strcspn(line, "\n")] = 0; // Remove trailing newline
    
    if (strcmp(line, "fin") != 0) {
        strcat(content_buffer, line);
        strcat(content_buffer, "\n");
        
        while (1) {
            fgets(line, sizeof(line), stdin);
            line[strcspn(line, "\n")] = 0; // Remove trailing newline
            
            if (strcmp(line, "fin") == 0) {
                break;
            }
            
            strcat(content_buffer, line);
            strcat(content_buffer, "\n");
            
            if (strlen(content_buffer) + 10 >= sizeof(annonce->annonce.contenu)) {
                printf("Contenu trop long, arrêt.\n");
                break;
            }
        }
        
        if (strlen(content_buffer) > 0) {
            strncpy(annonce->annonce.contenu, content_buffer, sizeof(annonce->annonce.contenu) - 1);
            annonce->annonce.contenu[sizeof(annonce->annonce.contenu) - 1] = '\0';
        }
    }
    
    // Update matière
    printf("Matière concernée [actuel: %d] (0 pour annonce générale): ", annonce->annonce.id_matiere);
    char matiere_buffer[10];
    fgets(matiere_buffer, sizeof(matiere_buffer), stdin);
    int new_matiere_id = atoi(matiere_buffer);
    if (strlen(matiere_buffer) > 1) { // more than just newline
        annonce->annonce.id_matiere = new_matiere_id;
    }
    
    // Update creation date
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(annonce->annonce.date_creation, sizeof(annonce->annonce.date_creation), 
             "%d/%m/%Y %H:%M", t);
    
    // Save changes
    if (sauvegarder_annonces(tete)) {
        printf("Annonce modifiée avec succès.\n");
    } else {
        printf("Erreur lors de la sauvegarde des modifications.\n");
    }
}

/**
 * Delete an announcement from the linked list
 */
void supprimer_annonce(NodeAnnonce** tete, int id_enseignant) {
    if (*tete == NULL) {
        printf("Aucune annonce n'est enregistrée.\n");
        return;
    }
    
    // Display list of announcements by this teacher
    printf("\n=== Vos Annonces ===\n");
    printf("%-4s | %-30s | %-20s | %-15s\n", 
           "ID", "Titre", "Date", "Matière");
    printf("---------------------------------------------------------------------------\n");
    
    NodeAnnonce* courant = *tete;
    int found = 0;
    while (courant != NULL) {
        // Check if this announcement belongs to the current teacher using id_enseignant field
        if (courant->annonce.id_enseignant == id_enseignant) {
            printf("%-4d | %-30s | %-20s | %-15d\n", 
                   courant->annonce.id,
                   courant->annonce.titre,
                   courant->annonce.date_creation,
                   courant->annonce.id_matiere);
            found = 1;
        }
        
        courant = courant->suivant;
    }
    
    if (!found) {
        printf("Vous n'avez pas encore créé d'annonces.\n");
        return;
    }
    
    printf("---------------------------------------------------------------------------\n");
    
    // Get the ID of the announcement to delete
    int id = get_int_input("Entrez l'ID de l'annonce à supprimer (0 pour annuler): ", 0, 9999);
    if (id == 0) {
        printf("Opération annulée.\n");
        return;
    }
    
    // Find the announcement
    NodeAnnonce *courant_del = *tete;
    NodeAnnonce *precedent = NULL;
    
    while (courant_del != NULL && courant_del->annonce.id != id) {
        precedent = courant_del;
        courant_del = courant_del->suivant;
    }
    
    if (courant_del == NULL) {
        printf("Annonce avec ID %d non trouvée.\n", id);
        return;
    }
    
    // Verify that this teacher is the author using id_enseignant field
    if (courant_del->annonce.id_enseignant != id_enseignant) {
        printf("Vous n'êtes pas autorisé à supprimer cette annonce.\n");
        return;
    }
    
    // Confirmation
    printf("Êtes-vous sûr de vouloir supprimer l'annonce '%s'?\n", courant_del->annonce.titre);
    int confirmation = get_int_input("1. Oui, 2. Non: ", 1, 2);
    
    if (confirmation == 1) {
        // Remove the node
        if (precedent == NULL) {
            // It's the first node
            *tete = courant_del->suivant;
        } else {
            precedent->suivant = courant_del->suivant;
        }
        
        free(courant_del);
        
        // Save changes
        if (sauvegarder_annonces(*tete)) {
            printf("Annonce supprimée avec succès.\n");
        } else {
            printf("Erreur lors de la sauvegarde des modifications.\n");
        }
    } else {
        printf("Opération annulée.\n");
    }
}

/**
 * Display all announcements in the linked list
 */
void afficher_annonces(NodeAnnonce* tete) {
    if (tete == NULL) {
        printf("Aucune annonce n'est enregistrée.\n");
        return;
    }
    
    printf("\n=== Liste des Annonces ===\n");
    
    NodeAnnonce* courant = tete;
    while (courant != NULL) {
        printf("=== Annonce #%d ===\n", courant->annonce.id);
        printf("Titre: %s\n", courant->annonce.titre);
        printf("Date: %s\n", courant->annonce.date_creation);
        printf("Auteur: %s\n", courant->annonce.auteur);
        if (courant->annonce.id_matiere > 0) {
            printf("Matière ID: %d\n", courant->annonce.id_matiere);
        } else {
            printf("Matière: Générale\n");
        }
        printf("Contenu:\n%s\n", courant->annonce.contenu);
        printf("------------------------\n\n");
        
        courant = courant->suivant;
    }
}

/**
 * Display announcements by subject ID
 */
void afficher_annonces_par_matiere(NodeAnnonce* tete, int id_matiere) {
    if (tete == NULL) {
        printf("Aucune annonce n'est enregistrée.\n");
        return;
    }
    
    printf("\n=== Annonces pour la Matière ID: %d ===\n", id_matiere);
    
    NodeAnnonce* courant = tete;
    int found = 0;
    
    while (courant != NULL) {
        if (courant->annonce.id_matiere == id_matiere) {
            printf("=== Annonce #%d ===\n", courant->annonce.id);
            printf("Titre: %s\n", courant->annonce.titre);
            printf("Date: %s\n", courant->annonce.date_creation);
            printf("Auteur: %s\n", courant->annonce.auteur);
            printf("Contenu:\n%s\n", courant->annonce.contenu);
            printf("------------------------\n\n");
            found = 1;
        }
        
        courant = courant->suivant;
    }
    
    if (!found) {
        printf("Aucune annonce trouvée pour cette matière.\n");
    }
}

/**
 * Display announcements by teacher ID
 */
void afficher_annonces_par_enseignant(NodeAnnonce* tete, int id_enseignant) {
    if (tete == NULL) {
        printf("Aucune annonce n'est enregistrée.\n");
        return;
    }
    
    printf("\n=== Vos Annonces ===\n");
    
    NodeAnnonce* courant = tete;
    int found = 0;
    
    while (courant != NULL) {
        // Check if this announcement belongs to the current teacher using the id_enseignant field
        if (courant->annonce.id_enseignant == id_enseignant) {
            printf("=== Annonce #%d ===\n", courant->annonce.id);
            printf("Titre: %s\n", courant->annonce.titre);
            printf("Date: %s\n", courant->annonce.date_creation);
            if (courant->annonce.id_matiere > 0) {
                printf("Matière ID: %d\n", courant->annonce.id_matiere);
            } else {
                printf("Matière: Générale\n");
            }
            printf("Contenu:\n%s\n", courant->annonce.contenu);
            printf("------------------------\n\n");
            found = 1;
        }
        
        courant = courant->suivant;
    }
    
    if (!found) {
        printf("Vous n'avez pas encore créé d'annonces.\n");
    }
}

/**
 * Find an announcement by ID
 */
NodeAnnonce* trouver_annonce_par_id(NodeAnnonce* tete, int id) {
    NodeAnnonce* courant = tete;
    
    while (courant != NULL) {
        if (courant->annonce.id == id) {
            return courant;
        }
        courant = courant->suivant;
    }
    
    return NULL; // Not found
}

/**
 * Free memory allocated for announcement linked list
 */
void liberer_annonces(NodeAnnonce** tete) {
    NodeAnnonce *courant = *tete;
    NodeAnnonce *suivant;
    
    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    
    *tete = NULL;
}