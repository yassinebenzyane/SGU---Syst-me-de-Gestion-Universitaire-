/**
 * @file inscriptions.c
 * @brief Implementation of course enrollment management module
 */

#include "../include/inscriptions.h"
#include "../include/matiere.h"
#include "../include/etudiant.h"
#include "../include/utils.h"
#include <time.h>

#define INSCRIPTIONS_FILE "data/inscriptions.txt"

/**
 * Load enrollments from file into a linked list
 */
int charger_inscriptions(NodeInscription** tete) {
    *tete = NULL;
    
    // Ensure data directory exists
    if (!create_data_dir()) {
        printf("Erreur: Impossible de créer le répertoire de données.\n");
        return 0;
    }
    
    // Create file if it doesn't exist
    if (!create_file_if_not_exists(INSCRIPTIONS_FILE)) {
        printf("Erreur: Impossible de créer le fichier des inscriptions.\n");
        return 0;
    }
    
    FILE *file = fopen(INSCRIPTIONS_FILE, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des inscriptions.\n");
        return 0;
    }
    
    char line[100]; // Buffer for reading lines
    NodeInscription *dernier = NULL;
    
    while (fgets(line, sizeof(line), file)) {
        // Create a new node
        NodeInscription *nouveau_node = (NodeInscription*)malloc(sizeof(NodeInscription));
        if (nouveau_node == NULL) {
            printf("Erreur: Échec d'allocation de mémoire.\n");
            fclose(file);
            return 0;
        }
        
        // Parse the line
        char *token = strtok(line, "|");
        if (token) {
            nouveau_node->inscription.id = atoi(token);
            
            token = strtok(NULL, "|");
            if (token) nouveau_node->inscription.id_etudiant = atoi(token);
            
            token = strtok(NULL, "|");
            if (token) nouveau_node->inscription.id_matiere = atoi(token);
            
            token = strtok(NULL, "|");
            if (token) strncpy(nouveau_node->inscription.date_inscription, token, sizeof(nouveau_node->inscription.date_inscription) - 1);
            
            token = strtok(NULL, "|");
            if (token) nouveau_node->inscription.statut = atoi(token);
            
            // Ensure null termination of strings
            nouveau_node->inscription.date_inscription[sizeof(nouveau_node->inscription.date_inscription) - 1] = '\0';
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
 * Save enrollments from linked list to file
 */
int sauvegarder_inscriptions(NodeInscription* tete) {
    FILE *file = fopen(INSCRIPTIONS_FILE, "w");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des inscriptions en écriture.\n");
        return 0;
    }
    
    NodeInscription* courant = tete;
    while (courant != NULL) {
        fprintf(file, "%d|%d|%d|%s|%d\n",
                courant->inscription.id,
                courant->inscription.id_etudiant,
                courant->inscription.id_matiere,
                courant->inscription.date_inscription,
                courant->inscription.statut);
        
        courant = courant->suivant;
    }
    
    fclose(file);
    return 1;
}

/**
 * Enroll a student in a course
 */
void inscrire_etudiant_cours(NodeInscription** tete, int id_etudiant, int id_matiere) {
    // Check if the student is already enrolled in this course
    NodeInscription* courant = *tete;
    while (courant != NULL) {
        if (courant->inscription.id_etudiant == id_etudiant && 
            courant->inscription.id_matiere == id_matiere) {
            if (courant->inscription.statut == 1) {
                printf("L'étudiant est déjà inscrit à ce cours.\n");
                return;
            } else {
                // The student was previously unenrolled, just update the status
                courant->inscription.statut = 1;
                
                // Update inscription date
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                strftime(courant->inscription.date_inscription, sizeof(courant->inscription.date_inscription), 
                         "%d/%m/%Y", t);
                
                if (sauvegarder_inscriptions(*tete)) {
                    printf("Inscription réussie.\n");
                } else {
                    printf("Erreur lors de la sauvegarde de l'inscription.\n");
                }
                return;
            }
        }
        courant = courant->suivant;
    }
    
    // Create a new enrollment
    NodeInscription *nouveau_node = (NodeInscription*)malloc(sizeof(NodeInscription));
    if (nouveau_node == NULL) {
        printf("Erreur: Échec d'allocation de mémoire.\n");
        return;
    }
    
    // Generate a new ID
    int new_id = 1;
    courant = *tete;
    while (courant != NULL) {
        if (courant->inscription.id >= new_id) {
            new_id = courant->inscription.id + 1;
        }
        courant = courant->suivant;
    }
    
    // Fill the new enrollment
    nouveau_node->inscription.id = new_id;
    nouveau_node->inscription.id_etudiant = id_etudiant;
    nouveau_node->inscription.id_matiere = id_matiere;
    
    // Set enrollment date
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(nouveau_node->inscription.date_inscription, sizeof(nouveau_node->inscription.date_inscription), 
             "%d/%m/%Y", t);
    
    nouveau_node->inscription.statut = 1; // Enrolled
    
    // Add to the beginning of the list
    nouveau_node->suivant = *tete;
    *tete = nouveau_node;
    
    // Save to file
    if (sauvegarder_inscriptions(*tete)) {
        printf("Inscription réussie.\n");
    } else {
        printf("Erreur lors de la sauvegarde de l'inscription.\n");
    }
}

/**
 * Unenroll a student from a course
 */
void desinscrire_etudiant_cours(NodeInscription** tete, int id_etudiant, int id_matiere) {
    // Find the enrollment
    NodeInscription* courant = *tete;
    while (courant != NULL) {
        if (courant->inscription.id_etudiant == id_etudiant && 
            courant->inscription.id_matiere == id_matiere) {
            if (courant->inscription.statut == 0) {
                printf("L'étudiant n'est pas inscrit à ce cours.\n");
                return;
            } else {
                // Update the status
                courant->inscription.statut = 0;
                
                // Update inscription date (date of unenrollment)
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                strftime(courant->inscription.date_inscription, sizeof(courant->inscription.date_inscription), 
                         "%d/%m/%Y", t);
                
                if (sauvegarder_inscriptions(*tete)) {
                    printf("Désinscription réussie.\n");
                } else {
                    printf("Erreur lors de la sauvegarde de la désinscription.\n");
                }
                return;
            }
        }
        courant = courant->suivant;
    }
    
    printf("L'étudiant n'est pas inscrit à ce cours.\n");
}

/**
 * Display all enrollments in the linked list
 */
void afficher_inscriptions(NodeInscription* tete) {
    if (tete == NULL) {
        printf("Aucune inscription n'est enregistrée.\n");
        return;
    }
    
    printf("\n=== Liste des Inscriptions ===\n");
    printf("%-4s | %-10s | %-10s | %-12s | %-8s\n", 
           "ID", "ID Étudiant", "ID Matière", "Date", "Statut");
    printf("------------------------------------------------------------\n");
    
    NodeInscription* courant = tete;
    while (courant != NULL) {
        printf("%-4d | %-10d | %-10d | %-12s | %-8s\n", 
               courant->inscription.id,
               courant->inscription.id_etudiant,
               courant->inscription.id_matiere,
               courant->inscription.date_inscription,
               (courant->inscription.statut == 1) ? "Inscrit" : "Désinscrit");
        
        courant = courant->suivant;
    }
    
    printf("------------------------------------------------------------\n");
}

/**
 * Display enrollments by student ID
 */
void afficher_inscriptions_par_etudiant(NodeInscription* tete, int id_etudiant) {
    if (tete == NULL) {
        printf("Aucune inscription n'est enregistrée.\n");
        return;
    }
    
    printf("\n=== Inscriptions de l'Étudiant (ID: %d) ===\n", id_etudiant);
    printf("%-4s | %-10s | %-12s | %-8s\n", 
           "ID", "ID Matière", "Date", "Statut");
    printf("-----------------------------------------------------\n");
    
    NodeInscription* courant = tete;
    int found = 0;
    
    while (courant != NULL) {
        if (courant->inscription.id_etudiant == id_etudiant && courant->inscription.statut == 1) {
            printf("%-4d | %-10d | %-12s | %-8s\n", 
                   courant->inscription.id,
                   courant->inscription.id_matiere,
                   courant->inscription.date_inscription,
                   "Inscrit");
            found = 1;
        }
        
        courant = courant->suivant;
    }
    
    if (!found) {
        printf("Aucune inscription active trouvée pour cet étudiant.\n");
    }
    
    printf("-----------------------------------------------------\n");
}

/**
 * Display enrollments by subject ID
 */
void afficher_inscriptions_par_matiere(NodeInscription* tete, int id_matiere) {
    if (tete == NULL) {
        printf("Aucune inscription n'est enregistrée.\n");
        return;
    }
    
    printf("\n=== Inscriptions pour la Matière (ID: %d) ===\n", id_matiere);
    printf("%-4s | %-10s | %-12s | %-8s\n", 
           "ID", "ID Étudiant", "Date", "Statut");
    printf("-----------------------------------------------------\n");
    
    NodeInscription* courant = tete;
    int found = 0;
    int count = 0;
    
    while (courant != NULL) {
        if (courant->inscription.id_matiere == id_matiere && courant->inscription.statut == 1) {
            printf("%-4d | %-10d | %-12s | %-8s\n", 
                   courant->inscription.id,
                   courant->inscription.id_etudiant,
                   courant->inscription.date_inscription,
                   "Inscrit");
            found = 1;
            count++;
        }
        
        courant = courant->suivant;
    }
    
    if (!found) {
        printf("Aucune inscription active trouvée pour cette matière.\n");
    } else {
        printf("-----------------------------------------------------\n");
        printf("Nombre total d'inscrits: %d\n", count);
    }
}

/**
 * Check if a student is enrolled in a specific course
 */
int est_inscrit(NodeInscription* tete, int id_etudiant, int id_matiere) {
    NodeInscription* courant = tete;
    
    while (courant != NULL) {
        if (courant->inscription.id_etudiant == id_etudiant && 
            courant->inscription.id_matiere == id_matiere &&
            courant->inscription.statut == 1) {
            return 1; // Enrolled
        }
        courant = courant->suivant;
    }
    
    return 0; // Not enrolled
}

/**
 * Get student enrollment management menu
 */
void menu_inscriptions_etudiant(NodeInscription** tete, int id_etudiant) {
    int choix = 0;
    
    do {
        clear_screen();
        printf("\n=== Gestion des Inscriptions aux Cours ===\n");
        printf("1. Consulter mes inscriptions\n");
        printf("2. M'inscrire à un cours\n");
        printf("3. Me désinscrire d'un cours\n");
        printf("0. Retour\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 3);
        
        switch (choix) {
            case 1:
                // Display student's enrollments
                afficher_inscriptions_par_etudiant(*tete, id_etudiant);
                pause_screen();
                break;
                
            case 2: {
                // Enroll in a course
                // In a real app, you'd display a list of available courses here
                printf("\n=== Inscription à un Cours ===\n");
                printf("Veuillez entrer l'ID de la matière à laquelle vous souhaitez vous inscrire.\n");
                int id_matiere = get_int_input("ID Matière: ", 1, 9999);
                
                inscrire_etudiant_cours(tete, id_etudiant, id_matiere);
                pause_screen();
                break;
            }
                
            case 3: {
                // Unenroll from a course
                // First display the courses the student is enrolled in
                printf("\n=== Désinscription d'un Cours ===\n");
                afficher_inscriptions_par_etudiant(*tete, id_etudiant);
                
                printf("\nVeuillez entrer l'ID de la matière dont vous souhaitez vous désinscrire.\n");
                int id_matiere = get_int_input("ID Matière (0 pour annuler): ", 0, 9999);
                
                if (id_matiere != 0) {
                    desinscrire_etudiant_cours(tete, id_etudiant, id_matiere);
                }
                pause_screen();
                break;
            }
        }
    } while (choix != 0);
}

/**
 * Free memory allocated for enrollment linked list
 */
void liberer_inscriptions(NodeInscription** tete) {
    NodeInscription *courant = *tete;
    NodeInscription *suivant;
    
    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    
    *tete = NULL;
}