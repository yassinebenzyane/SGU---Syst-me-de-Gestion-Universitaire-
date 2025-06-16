/**
 * @file gestion_enseignants.c
 * @brief Implementation of teacher management functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_enseignants.h"
#include "enseignant.h"
#include "annonces.h"
#include "notes.h"
#include "utils.h"

/**
 * Menu for teacher management
 */
void menu_gestion_enseignants(NodeEnseignant** enseignants) {
    int choix = 0;
    
    do {
        clear_screen();
        printf("\n=== Gestion des Enseignants ===\n");
        printf("1. Afficher tous les enseignants\n");
        printf("2. Ajouter un enseignant\n");
        printf("3. Modifier un enseignant\n");
        printf("4. Supprimer un enseignant\n");
        printf("5. Rechercher des enseignants\n");
        printf("0. Retour\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 5);
        
        switch (choix) {
            case 0:
                // Retour
                break;
            case 1:
                // Afficher tous les enseignants
                afficher_enseignants(*enseignants);
                pause_screen();
                break;
            case 2:
                // Ajouter un enseignant
                ajouter_enseignant(enseignants);
                pause_screen();
                break;
            case 3:
                // Modifier un enseignant
                modifier_enseignant(*enseignants);
                pause_screen();
                break;
            case 4:
                // Supprimer un enseignant
                supprimer_enseignant(enseignants);
                pause_screen();
                break;
            case 5:
                // Rechercher des enseignants
                rechercher_enseignants(*enseignants);
                pause_screen();
                break;
        }
    } while (choix != 0);
}

/**
 * Display statistics about teachers
 */
void afficher_statistiques_enseignants(NodeEnseignant* enseignants, NodeNote* notes, NodeAnnonce* annonces) {
    clear_screen();
    printf("\n=== Statistiques des Enseignants ===\n");
    
    // Count total number of teachers
    int nb_enseignants = 0;
    NodeEnseignant* enseignant_courant = enseignants;
    while (enseignant_courant != NULL) {
        nb_enseignants++;
        enseignant_courant = enseignant_courant->suivant;
    }
    
    printf("Nombre total d'enseignants: %d\n\n", nb_enseignants);
    
    // Count teachers by subject taught
    printf("Répartition par matière enseignée:\n");
    char matieres[20][50]; // Assuming max 20 different subjects
    int count_matieres[20] = {0};
    int nb_matieres = 0;
    
    enseignant_courant = enseignants;
    while (enseignant_courant != NULL) {
        // Check if subject already counted
        int matiere_found = 0;
        for (int i = 0; i < nb_matieres; i++) {
            if (strcmp(matieres[i], enseignant_courant->enseignant.matiere_enseignee) == 0) {
                count_matieres[i]++;
                matiere_found = 1;
                break;
            }
        }
        
        // If not found, add new subject
        if (!matiere_found && nb_matieres < 20) {
            strcpy(matieres[nb_matieres], enseignant_courant->enseignant.matiere_enseignee);
            count_matieres[nb_matieres] = 1;
            nb_matieres++;
        }
        
        enseignant_courant = enseignant_courant->suivant;
    }
    
    // Display subject statistics
    for (int i = 0; i < nb_matieres; i++) {
        float percentage = (float)count_matieres[i] / nb_enseignants * 100;
        printf("- %s: %d enseignants (%.1f%%)\n", matieres[i], count_matieres[i], percentage);
    }
    
    printf("\n");
    
    // Count number of announcements by teacher
    printf("Activité des enseignants:\n");
    
    enseignant_courant = enseignants;
    while (enseignant_courant != NULL) {
        int nb_annonces = 0;
        int nb_notes = 0;
        
        // Count announcements
        NodeAnnonce* annonce = annonces;
        while (annonce != NULL) {
            if (annonce->annonce.id_enseignant == enseignant_courant->enseignant.id) {
                nb_annonces++;
            }
            annonce = annonce->suivant;
        }
        
        // Count grades
        NodeNote* note = notes;
        while (note != NULL) {
            if (note->note.id_enseignant == enseignant_courant->enseignant.id) {
                nb_notes++;
            }
            note = note->suivant;
        }
        
        printf("- %s %s: %d annonces, %d notes enregistrées\n", 
               enseignant_courant->enseignant.prenom, 
               enseignant_courant->enseignant.nom,
               nb_annonces, nb_notes);
        
        enseignant_courant = enseignant_courant->suivant;
    }
}

/**
 * Menu for managing announcements by a teacher
 */
void menu_gestion_annonces_enseignant(NodeAnnonce** annonces, int id_enseignant) {
    int choix = 0;
    
    do {
        clear_screen();
        printf("\n=== Gestion des Annonces ===\n");
        printf("1. Consulter toutes les annonces\n");
        printf("2. Consulter mes annonces\n");
        printf("3. Créer une nouvelle annonce\n");
        printf("4. Modifier une annonce\n");
        printf("5. Supprimer une annonce\n");
        printf("0. Retour\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 5);
        
        switch (choix) {
            case 1:
                afficher_annonces(*annonces);
                pause_screen();
                break;
            case 2:
                afficher_annonces_par_enseignant(*annonces, id_enseignant);
                pause_screen();
                break;
            case 3:
                ajouter_annonce(annonces, id_enseignant);
                pause_screen();
                break;
            case 4:
                modifier_annonce(*annonces, id_enseignant);
                pause_screen();
                break;
            case 5:
                supprimer_annonce(annonces, id_enseignant);
                pause_screen();
                break;
        }
    } while (choix != 0);
}

/**
 * Menu for managing grades by a teacher
 */
void menu_gestion_notes_enseignant(NodeNote** notes, int id_enseignant) {
    int choix = 0;
    
    do {
        clear_screen();
        printf("\n=== Gestion des Notes ===\n");
        printf("1. Consulter toutes les notes\n");
        printf("2. Consulter les notes par étudiant\n");
        printf("3. Consulter les notes par matière\n");
        printf("4. Ajouter une note\n");
        printf("5. Modifier une note\n");
        printf("6. Supprimer une note\n");
        printf("0. Retour\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 6);
        
        switch (choix) {
            case 1:
                afficher_notes(*notes);
                pause_screen();
                break;
            case 2: {
                int id_etudiant = get_int_input("Entrez l'ID de l'étudiant: ", 1, 9999);
                afficher_notes_par_etudiant(*notes, id_etudiant);
                pause_screen();
                break;
            }
            case 3: {
                int id_matiere = get_int_input("Entrez l'ID de la matière: ", 1, 9999);
                afficher_notes_par_matiere(*notes, id_matiere);
                pause_screen();
                break;
            }
            case 4:
                ajouter_note(notes, id_enseignant);
                pause_screen();
                break;
            case 5:
                modifier_note(*notes, id_enseignant);
                pause_screen();
                break;
            case 6:
                supprimer_note(notes, id_enseignant);
                pause_screen();
                break;
        }
    } while (choix != 0);
}