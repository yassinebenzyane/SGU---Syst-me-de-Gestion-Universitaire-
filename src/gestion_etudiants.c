/**
 * @file gestion_etudiants.c
 * @brief Implementation of student management functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_etudiants.h"
#include "etudiant.h"
#include "notes.h"
#include "inscriptions.h"
#include "utils.h"

/**
 * Menu for student management
 */
void menu_gestion_etudiants(NodeEtudiant** etudiants) {
    int choix = 0;
    
    do {
        clear_screen();
        printf("\n=== Gestion des Étudiants ===\n");
        printf("1. Afficher tous les étudiants\n");
        printf("2. Ajouter un étudiant\n");
        printf("3. Modifier un étudiant\n");
        printf("4. Supprimer un étudiant\n");
        printf("5. Rechercher des étudiants\n");
        printf("0. Retour\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 5);
        
        switch (choix) {
            case 0:
                // Retour
                break;
            case 1:
                // Afficher tous les étudiants
                afficher_etudiants(*etudiants);
                pause_screen();
                break;
            case 2:
                // Ajouter un étudiant
                ajouter_etudiant(etudiants);
                pause_screen();
                break;
            case 3:
                // Modifier un étudiant
                modifier_etudiant(*etudiants);
                pause_screen();
                break;
            case 4:
                // Supprimer un étudiant
                supprimer_etudiant(etudiants);
                pause_screen();
                break;
            case 5:
                // Rechercher des étudiants
                rechercher_etudiants(*etudiants);
                pause_screen();
                break;
        }
    } while (choix != 0);
}

/**
 * Display statistics about students
 */
void afficher_statistiques_etudiants(NodeEtudiant* etudiants, NodeNote* notes) {
    clear_screen();
    printf("\n=== Statistiques des Étudiants ===\n");
    
    // Count total number of students
    int nb_etudiants = 0;
    NodeEtudiant* etudiant_courant = etudiants;
    while (etudiant_courant != NULL) {
        nb_etudiants++;
        etudiant_courant = etudiant_courant->suivant;
    }
    
    printf("Nombre total d'étudiants: %d\n\n", nb_etudiants);
    
    // Count students by section
    printf("Répartition par section:\n");
    char sections[20][50]; // Assuming max 20 different sections
    int count_sections[20] = {0};
    int nb_sections = 0;
    
    etudiant_courant = etudiants;
    while (etudiant_courant != NULL) {
        // Check if section already counted
        int section_found = 0;
        for (int i = 0; i < nb_sections; i++) {
            if (strcmp(sections[i], etudiant_courant->etudiant.section) == 0) {
                count_sections[i]++;
                section_found = 1;
                break;
            }
        }
        
        // If not found, add new section
        if (!section_found && nb_sections < 20) {
            strcpy(sections[nb_sections], etudiant_courant->etudiant.section);
            count_sections[nb_sections] = 1;
            nb_sections++;
        }
        
        etudiant_courant = etudiant_courant->suivant;
    }
    
    // Display section statistics
    for (int i = 0; i < nb_sections; i++) {
        float percentage = (float)count_sections[i] / nb_etudiants * 100;
        printf("- %s: %d étudiants (%.1f%%)\n", sections[i], count_sections[i], percentage);
    }
    
    printf("\n");
    
    // Calculate performance statistics
    printf("Statistiques de performance:\n");
    
    int nb_etudiants_avec_notes = 0;
    int nb_etudiants_reussite = 0; // moyenne >= 10
    
    etudiant_courant = etudiants;
    while (etudiant_courant != NULL) {
        float moyenne = calculer_moyenne_etudiant(notes, etudiant_courant->etudiant.id);
        
        // Only count students with grades
        if (moyenne > 0) {
            nb_etudiants_avec_notes++;
            
            if (moyenne >= 10.0f) {
                nb_etudiants_reussite++;
            }
        }
        
        etudiant_courant = etudiant_courant->suivant;
    }
    
    // Avoid division by zero
    if (nb_etudiants_avec_notes > 0) {
        float taux_reussite = (float)nb_etudiants_reussite / nb_etudiants_avec_notes * 100;
        printf("- Étudiants avec notes: %d\n", nb_etudiants_avec_notes);
        printf("- Taux de réussite (moyenne >= 10): %.1f%% (%d étudiants)\n", 
               taux_reussite, nb_etudiants_reussite);
    } else {
        printf("Aucun étudiant n'a de notes enregistrées.\n");
    }
}

/**
 * Display performance report for a specific student
 */
void afficher_rapport_etudiant(NodeEtudiant* etudiants, NodeNote* notes, NodeInscription* inscriptions) {
    int id_etudiant = get_int_input("Entrez l'ID de l'étudiant: ", 1, 9999);
    
    // Find student
    NodeEtudiant* etudiant = trouver_etudiant_par_id(etudiants, id_etudiant);
    if (etudiant == NULL) {
        printf("Étudiant avec ID %d non trouvé.\n", id_etudiant);
        return;
    }
    
    clear_screen();
    printf("\n=== Rapport de Performance: %s %s ===\n", 
           etudiant->etudiant.prenom, etudiant->etudiant.nom);
    printf("ID: %d\n", etudiant->etudiant.id);
    printf("Email: %s\n", etudiant->etudiant.email);
    printf("CNE: %s\n", etudiant->etudiant.cne);
    printf("Section: %s\n", etudiant->etudiant.section);
    printf("Filière: %s\n\n", etudiant->etudiant.filiere);
    
    // Display grades
    printf("=== Notes ===\n");
    afficher_notes_par_etudiant(notes, id_etudiant);
    
    // Calculate average to determine if there are notes
    float moyenne = calculer_moyenne_etudiant(notes, id_etudiant);
    if (moyenne > 0) {
        // Display average
        printf("\nMoyenne générale: %.2f\n", moyenne);
        
        // Display performance assessment
        printf("\nÉvaluation: ");
        if (moyenne >= 16) {
            printf("Très bien\n");
        } else if (moyenne >= 14) {
            printf("Bien\n");
        } else if (moyenne >= 12) {
            printf("Assez bien\n");
        } else if (moyenne >= 10) {
            printf("Passable\n");
        } else {
            printf("Insuffisant\n");
        }
    } else {
        printf("Aucune note enregistrée pour cet étudiant.\n");
    }
    
    // Display course registrations
    printf("\n=== Inscriptions aux Cours ===\n");
    int nb_inscriptions = 0;
    NodeInscription* inscription = inscriptions;
    
    while (inscription != NULL) {
        if (inscription->inscription.id_etudiant == id_etudiant) {
            printf("- Cours ID: %d, Date: %s, Statut: %s\n", 
                   inscription->inscription.id_matiere,
                   inscription->inscription.date_inscription,
                   inscription->inscription.statut == 1 ? "Inscrit" : "Désinscrit");
            nb_inscriptions++;
        }
        inscription = inscription->suivant;
    }
    
    if (nb_inscriptions == 0) {
        printf("Aucune inscription aux cours pour cet étudiant.\n");
    }
}

// Note: menu_inscriptions_etudiant est maintenant implémenté dans inscriptions.c