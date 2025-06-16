/**
 * @file notes.c
 * @brief Implementation of grades management module
 */

#include "../include/notes.h"
#include "../include/matiere.h"
#include "../include/etudiant.h"
#include "../include/enseignant.h"
#include "../include/utils.h"
#include <time.h>

#define NOTES_FILE "data/notes.txt"

/**
 * Load grades from file into a linked list
 */
int charger_notes(NodeNote** tete) {
    *tete = NULL;
    
    // Ensure data directory exists
    if (!create_data_dir()) {
        printf("Erreur: Impossible de créer le répertoire de données.\n");
        return 0;
    }
    
    // Create file if it doesn't exist
    if (!create_file_if_not_exists(NOTES_FILE)) {
        printf("Erreur: Impossible de créer le fichier des notes.\n");
        return 0;
    }
    
    FILE *file = fopen(NOTES_FILE, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des notes.\n");
        return 0;
    }
    
    char line[300]; // Buffer for reading lines
    NodeNote *dernier = NULL;
    
    while (fgets(line, sizeof(line), file)) {
        // Create a new node
        NodeNote *nouveau_node = (NodeNote*)malloc(sizeof(NodeNote));
        if (nouveau_node == NULL) {
            printf("Erreur: Échec d'allocation de mémoire.\n");
            fclose(file);
            return 0;
        }
        
        // Parse the line
        char *token = strtok(line, "|");
        if (token) {
            nouveau_node->note.id = atoi(token);
            
            token = strtok(NULL, "|");
            if (token) nouveau_node->note.id_etudiant = atoi(token);
            
            token = strtok(NULL, "|");
            if (token) nouveau_node->note.id_matiere = atoi(token);
            
            token = strtok(NULL, "|");
            if (token) nouveau_node->note.note = atof(token);
            
            token = strtok(NULL, "|");
            if (token) strncpy(nouveau_node->note.commentaire, token, sizeof(nouveau_node->note.commentaire) - 1);
            
            token = strtok(NULL, "|");
            if (token) strncpy(nouveau_node->note.date_evaluation, token, sizeof(nouveau_node->note.date_evaluation) - 1);
            
            // Ensure null termination of strings
            nouveau_node->note.commentaire[sizeof(nouveau_node->note.commentaire) - 1] = '\0';
            nouveau_node->note.date_evaluation[sizeof(nouveau_node->note.date_evaluation) - 1] = '\0';
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
 * Save grades from linked list to file
 */
int sauvegarder_notes(NodeNote* tete) {
    FILE *file = fopen(NOTES_FILE, "w");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des notes en écriture.\n");
        return 0;
    }
    
    NodeNote* courant = tete;
    while (courant != NULL) {
        fprintf(file, "%d|%d|%d|%.2f|%s|%s\n",
                courant->note.id,
                courant->note.id_etudiant,
                courant->note.id_matiere,
                courant->note.note,
                courant->note.commentaire,
                courant->note.date_evaluation);
        
        courant = courant->suivant;
    }
    
    fclose(file);
    return 1;
}

/**
 * Add a new grade to the linked list
 */
void ajouter_note(NodeNote** tete, int id_enseignant) {
    NodeNote *nouveau_node = (NodeNote*)malloc(sizeof(NodeNote));
    if (nouveau_node == NULL) {
        printf("Erreur: Échec d'allocation de mémoire.\n");
        return;
    }
    
    // Generate a new ID
    int new_id = 1;
    NodeNote *courant = *tete;
    while (courant != NULL) {
        if (courant->note.id >= new_id) {
            new_id = courant->note.id + 1;
        }
        courant = courant->suivant;
    }
    
    nouveau_node->note.id = new_id;
    
    // Get grade details
    printf("\n=== Ajouter une nouvelle note ===\n");
    
    // Get student ID - in a real app, you'd present a list of students to choose from
    nouveau_node->note.id_etudiant = get_int_input("ID de l'étudiant: ", 1, 9999);
    
    // Get subject ID - in a real app, you'd present a list of subjects taught by this teacher
    nouveau_node->note.id_matiere = get_int_input("ID de la matière: ", 1, 9999);
    
    // Get the grade
    nouveau_node->note.note = get_float_input("Note (0-20): ", 0, 20);
    
    // Get comment
    get_input("Commentaire: ", nouveau_node->note.commentaire, sizeof(nouveau_node->note.commentaire));
    
    // Set evaluation date
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(nouveau_node->note.date_evaluation, sizeof(nouveau_node->note.date_evaluation), 
             "%d/%m/%Y", t);
    
    // Add to the beginning of the list
    nouveau_node->suivant = *tete;
    *tete = nouveau_node;
    
    // Save to file
    if (sauvegarder_notes(*tete)) {
        printf("Note ajoutée avec succès.\n");
    } else {
        printf("Erreur lors de la sauvegarde des notes.\n");
    }
}

/**
 * Modify an existing grade
 */
void modifier_note(NodeNote* tete, int id_enseignant) {
    if (tete == NULL) {
        printf("Aucune note n'est enregistrée.\n");
        return;
    }
    
    // Display list of grades - in a real app, you'd filter by subjects taught by this teacher
    printf("\n=== Liste des Notes ===\n");
    printf("%-4s | %-10s | %-10s | %-5s | %-30s | %-12s\n", 
           "ID", "ID Étudiant", "ID Matière", "Note", "Commentaire", "Date");
    printf("--------------------------------------------------------------------------------------\n");
    
    NodeNote* courant = tete;
    while (courant != NULL) {
        printf("%-4d | %-10d | %-10d | %-5.2f | %-30s | %-12s\n", 
               courant->note.id,
               courant->note.id_etudiant,
               courant->note.id_matiere,
               courant->note.note,
               courant->note.commentaire,
               courant->note.date_evaluation);
        
        courant = courant->suivant;
    }
    
    printf("--------------------------------------------------------------------------------------\n");
    
    // Get the ID of the grade to modify
    int id = get_int_input("Entrez l'ID de la note à modifier (0 pour annuler): ", 0, 9999);
    if (id == 0) {
        printf("Opération annulée.\n");
        return;
    }
    
    // Find the grade
    NodeNote* note = trouver_note_par_id(tete, id);
    if (note == NULL) {
        printf("Note avec ID %d non trouvée.\n", id);
        return;
    }
    
    // In a real app, you'd verify that this teacher teaches the subject
    
    printf("\n=== Modifier la note ===\n");
    printf("Laissez vide pour garder la valeur actuelle.\n");
    
    // Get new values or keep current ones
    char buffer[50];
    
    printf("ID Étudiant actuel: %d\n", note->note.id_etudiant);
    int new_id_etudiant = get_int_input("Nouvel ID Étudiant (0 pour garder l'actuel): ", 0, 9999);
    if (new_id_etudiant != 0) {
        note->note.id_etudiant = new_id_etudiant;
    }
    
    printf("ID Matière actuel: %d\n", note->note.id_matiere);
    int new_id_matiere = get_int_input("Nouvel ID Matière (0 pour garder l'actuel): ", 0, 9999);
    if (new_id_matiere != 0) {
        note->note.id_matiere = new_id_matiere;
    }
    
    printf("Note actuelle: %.2f\n", note->note.note);
    float new_note = get_float_input("Nouvelle note (0-20, -1 pour garder l'actuelle): ", -1, 20);
    if (new_note >= 0) {
        note->note.note = new_note;
    }
    
    get_input_formatted("Commentaire [actuel: %s]: ", buffer, sizeof(buffer), note->note.commentaire);
    if (strlen(buffer) > 0) {
        strcpy(note->note.commentaire, buffer);
    }
    
    // Update evaluation date
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(note->note.date_evaluation, sizeof(note->note.date_evaluation), 
             "%d/%m/%Y", t);
    
    // Save changes
    if (sauvegarder_notes(tete)) {
        printf("Note modifiée avec succès.\n");
    } else {
        printf("Erreur lors de la sauvegarde des modifications.\n");
    }
}

/**
 * Delete a grade from the linked list
 */
void supprimer_note(NodeNote** tete, int id_enseignant) {
    if (*tete == NULL) {
        printf("Aucune note n'est enregistrée.\n");
        return;
    }
    
    // Display list of grades - in a real app, you'd filter by subjects taught by this teacher
    printf("\n=== Liste des Notes ===\n");
    printf("%-4s | %-10s | %-10s | %-5s | %-30s | %-12s\n", 
           "ID", "ID Étudiant", "ID Matière", "Note", "Commentaire", "Date");
    printf("--------------------------------------------------------------------------------------\n");
    
    NodeNote* courant = *tete;
    while (courant != NULL) {
        printf("%-4d | %-10d | %-10d | %-5.2f | %-30s | %-12s\n", 
               courant->note.id,
               courant->note.id_etudiant,
               courant->note.id_matiere,
               courant->note.note,
               courant->note.commentaire,
               courant->note.date_evaluation);
        
        courant = courant->suivant;
    }
    
    printf("--------------------------------------------------------------------------------------\n");
    
    // Get the ID of the grade to delete
    int id = get_int_input("Entrez l'ID de la note à supprimer (0 pour annuler): ", 0, 9999);
    if (id == 0) {
        printf("Opération annulée.\n");
        return;
    }
    
    // Find and remove the grade
    NodeNote *courant_del = *tete;
    NodeNote *precedent = NULL;
    
    while (courant_del != NULL && courant_del->note.id != id) {
        precedent = courant_del;
        courant_del = courant_del->suivant;
    }
    
    if (courant_del == NULL) {
        printf("Note avec ID %d non trouvée.\n", id);
        return;
    }
    
    // In a real app, you'd verify that this teacher teaches the subject
    
    // Confirmation
    printf("Êtes-vous sûr de vouloir supprimer cette note?\n");
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
        if (sauvegarder_notes(*tete)) {
            printf("Note supprimée avec succès.\n");
        } else {
            printf("Erreur lors de la sauvegarde des modifications.\n");
        }
    } else {
        printf("Opération annulée.\n");
    }
}

/**
 * Display all grades in the linked list
 */
void afficher_notes(NodeNote* tete) {
    if (tete == NULL) {
        printf("Aucune note n'est enregistrée.\n");
        return;
    }
    
    printf("\n=== Liste des Notes ===\n");
    printf("%-4s | %-10s | %-10s | %-5s | %-30s | %-12s\n", 
           "ID", "ID Étudiant", "ID Matière", "Note", "Commentaire", "Date");
    printf("--------------------------------------------------------------------------------------\n");
    
    NodeNote* courant = tete;
    while (courant != NULL) {
        printf("%-4d | %-10d | %-10d | %-5.2f | %-30s | %-12s\n", 
               courant->note.id,
               courant->note.id_etudiant,
               courant->note.id_matiere,
               courant->note.note,
               courant->note.commentaire,
               courant->note.date_evaluation);
        
        courant = courant->suivant;
    }
    
    printf("--------------------------------------------------------------------------------------\n");
}

/**
 * Display grades by student ID
 */
void afficher_notes_par_etudiant(NodeNote* tete, int id_etudiant) {
    if (tete == NULL) {
        printf("Aucune note n'est enregistrée.\n");
        return;
    }
    
    printf("\n=== Notes de l'Étudiant (ID: %d) ===\n", id_etudiant);
    printf("%-4s | %-10s | %-5s | %-30s | %-12s\n", 
           "ID", "ID Matière", "Note", "Commentaire", "Date");
    printf("-------------------------------------------------------------------------------\n");
    
    NodeNote* courant = tete;
    int found = 0;
    
    while (courant != NULL) {
        if (courant->note.id_etudiant == id_etudiant) {
            printf("%-4d | %-10d | %-5.2f | %-30s | %-12s\n", 
                   courant->note.id,
                   courant->note.id_matiere,
                   courant->note.note,
                   courant->note.commentaire,
                   courant->note.date_evaluation);
            found = 1;
        }
        
        courant = courant->suivant;
    }
    
    if (!found) {
        printf("Aucune note trouvée pour cet étudiant.\n");
    } else {
        // Also display the average grade for this student
        printf("-------------------------------------------------------------------------------\n");
        printf("Moyenne: %.2f\n", calculer_moyenne_etudiant(tete, id_etudiant));
    }
}

/**
 * Display grades by subject ID
 */
void afficher_notes_par_matiere(NodeNote* tete, int id_matiere) {
    if (tete == NULL) {
        printf("Aucune note n'est enregistrée.\n");
        return;
    }
    
    printf("\n=== Notes pour la Matière (ID: %d) ===\n", id_matiere);
    printf("%-4s | %-10s | %-5s | %-30s | %-12s\n", 
           "ID", "ID Étudiant", "Note", "Commentaire", "Date");
    printf("-------------------------------------------------------------------------------\n");
    
    NodeNote* courant = tete;
    int found = 0;
    
    while (courant != NULL) {
        if (courant->note.id_matiere == id_matiere) {
            printf("%-4d | %-10d | %-5.2f | %-30s | %-12s\n", 
                   courant->note.id,
                   courant->note.id_etudiant,
                   courant->note.note,
                   courant->note.commentaire,
                   courant->note.date_evaluation);
            found = 1;
        }
        
        courant = courant->suivant;
    }
    
    if (!found) {
        printf("Aucune note trouvée pour cette matière.\n");
    } else {
        // Also display the average grade for this subject
        printf("-------------------------------------------------------------------------------\n");
        printf("Moyenne de la classe: %.2f\n", calculer_moyenne_matiere(tete, id_matiere));
    }
}

/**
 * Calculate the average grade for a student across all subjects
 */
float calculer_moyenne_etudiant(NodeNote* tete, int id_etudiant) {
    if (tete == NULL) {
        return 0.0f;
    }
    
    float sum = 0.0f;
    int count = 0;
    
    NodeNote* courant = tete;
    while (courant != NULL) {
        if (courant->note.id_etudiant == id_etudiant) {
            sum += courant->note.note;
            count++;
        }
        
        courant = courant->suivant;
    }
    
    if (count == 0) {
        return 0.0f;
    }
    
    return sum / count;
}

/**
 * Calculate the average grade for a subject across all students
 */
float calculer_moyenne_matiere(NodeNote* tete, int id_matiere) {
    if (tete == NULL) {
        return 0.0f;
    }
    
    float sum = 0.0f;
    int count = 0;
    
    NodeNote* courant = tete;
    while (courant != NULL) {
        if (courant->note.id_matiere == id_matiere) {
            sum += courant->note.note;
            count++;
        }
        
        courant = courant->suivant;
    }
    
    if (count == 0) {
        return 0.0f;
    }
    
    return sum / count;
}

/**
 * Find a grade by ID
 */
NodeNote* trouver_note_par_id(NodeNote* tete, int id) {
    NodeNote* courant = tete;
    
    while (courant != NULL) {
        if (courant->note.id == id) {
            return courant;
        }
        courant = courant->suivant;
    }
    
    return NULL; // Not found
}

/**
 * Free memory allocated for grade linked list
 */
void liberer_notes(NodeNote** tete) {
    NodeNote *courant = *tete;
    NodeNote *suivant;
    
    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    
    *tete = NULL;
}