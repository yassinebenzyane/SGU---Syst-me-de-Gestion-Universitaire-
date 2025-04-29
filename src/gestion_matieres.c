/**
 * @file gestion_matieres.c
 * @brief Implementation of subject management functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_matieres.h"
#include "matiere.h"
#include "notes.h"
#include "inscriptions.h"
#include "etudiant.h"
#include "utils.h"

/**
 * Menu for subject management
 */
void menu_gestion_matieres(NodeMatiere** matieres) {
    int choix = 0;
    
    do {
        clear_screen();
        printf("\n=== Gestion des Matières ===\n");
        printf("1. Afficher toutes les matières\n");
        printf("2. Ajouter une matière\n");
        printf("3. Modifier une matière\n");
        printf("4. Supprimer une matière\n");
        printf("5. Rechercher des matières\n");
        printf("0. Retour\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 5);
        
        switch (choix) {
            case 0:
                // Retour
                break;
            case 1:
                // Afficher toutes les matières
                afficher_matieres(*matieres);
                pause_screen();
                break;
            case 2:
                // Ajouter une matière
                ajouter_matiere(matieres);
                pause_screen();
                break;
            case 3:
                // Modifier une matière
                modifier_matiere(*matieres);
                pause_screen();
                break;
            case 4:
                // Supprimer une matière
                supprimer_matiere(matieres);
                pause_screen();
                break;
            case 5:
                // Rechercher des matières
                rechercher_matieres(*matieres);
                pause_screen();
                break;
        }
    } while (choix != 0);
}

/**
 * Display statistics about subjects
 */
void afficher_statistiques_matieres(NodeMatiere* matieres, NodeNote* notes, NodeInscription* inscriptions) {
    clear_screen();
    printf("\n=== Statistiques des Matières ===\n");
    
    // Count total number of subjects
    int nb_matieres = 0;
    NodeMatiere* matiere_courant = matieres;
    while (matiere_courant != NULL) {
        nb_matieres++;
        matiere_courant = matiere_courant->suivant;
    }
    
    printf("Nombre total de matières: %d\n\n", nb_matieres);
    
    // Display subject list with coefficient
    printf("Liste des matières et coefficients:\n");
    printf("%-4s | %-20s | %-15s | %-10s\n", "ID", "Nom", "Code", "Coefficient");
    printf("---------------------------------------------------------------\n");
    
    matiere_courant = matieres;
    while (matiere_courant != NULL) {
        printf("%-4d | %-20s | %-15s | %-10.2f\n", 
               matiere_courant->matiere.id,
               matiere_courant->matiere.nom_matiere,
               matiere_courant->matiere.code_matiere,
               matiere_courant->matiere.coefficient);
        
        matiere_courant = matiere_courant->suivant;
    }
    
    printf("\n");
    
    // Display registration statistics
    printf("Statistiques d'inscription:\n");
    
    matiere_courant = matieres;
    while (matiere_courant != NULL) {
        int nb_inscriptions = 0;
        
        // Count registrations for this subject
        NodeInscription* inscription = inscriptions;
        while (inscription != NULL) {
            if (inscription->inscription.id_matiere == matiere_courant->matiere.id) {
                nb_inscriptions++;
            }
            inscription = inscription->suivant;
        }
        
        printf("- %s (%s): %d inscriptions\n", 
               matiere_courant->matiere.nom_matiere,
               matiere_courant->matiere.code_matiere,
               nb_inscriptions);
        
        matiere_courant = matiere_courant->suivant;
    }
}

/**
 * Calculate subject statistics and averages
 */
void calculer_statistiques_matieres(NodeMatiere* matieres, NodeNote* notes, NodeEtudiant* etudiants) {
    clear_screen();
    printf("\n=== Moyennes par Matière ===\n");
    
    printf("%-4s | %-20s | %-10s | %-15s\n", "ID", "Matière", "Moyenne", "Nb. Étudiants");
    printf("------------------------------------------------------\n");
    
    NodeMatiere* matiere_courant = matieres;
    while (matiere_courant != NULL) {
        // Count number of students with grades in this subject
        int nb_etudiants = 0;
        NodeNote* note_courant = notes;
        while (note_courant != NULL) {
            if (note_courant->note.id_matiere == matiere_courant->matiere.id) {
                nb_etudiants++;
            }
            note_courant = note_courant->suivant;
        }
        
        // Calculate average for this subject
        float moyenne = calculer_moyenne_matiere(notes, matiere_courant->matiere.id);
        
        printf("%-4d | %-20s | %-10.2f | %-15d\n", 
               matiere_courant->matiere.id,
               matiere_courant->matiere.nom_matiere,
               moyenne,
               nb_etudiants);
        
        matiere_courant = matiere_courant->suivant;
    }
}

/**
 * Display detailed information about a subject
 */
void afficher_details_matiere(NodeMatiere* matieres, NodeNote* notes, int id_matiere) {
    // Find subject
    NodeMatiere* matiere = trouver_matiere_par_id(matieres, id_matiere);
    if (matiere == NULL) {
        printf("Matière avec ID %d non trouvée.\n", id_matiere);
        return;
    }
    
    clear_screen();
    printf("\n=== Détails de la Matière ===\n");
    printf("ID: %d\n", matiere->matiere.id);
    printf("Code: %s\n", matiere->matiere.code_matiere);
    printf("Nom: %s\n", matiere->matiere.nom_matiere);
    printf("Coefficient: %.2f\n\n", matiere->matiere.coefficient);
    
    // Display grade statistics
    printf("=== Statistiques des Notes ===\n");
    
    // Count grades and calculate statistics
    int nb_notes = 0;
    float somme_notes = 0.0f;
    float note_min = 20.0f;
    float note_max = 0.0f;
    
    NodeNote* note_courant = notes;
    while (note_courant != NULL) {
        if (note_courant->note.id_matiere == id_matiere) {
            nb_notes++;
            somme_notes += note_courant->note.note;
            
            if (note_courant->note.note < note_min) {
                note_min = note_courant->note.note;
            }
            
            if (note_courant->note.note > note_max) {
                note_max = note_courant->note.note;
            }
        }
        note_courant = note_courant->suivant;
    }
    
    if (nb_notes > 0) {
        float moyenne = somme_notes / nb_notes;
        printf("Nombre de notes: %d\n", nb_notes);
        printf("Moyenne: %.2f\n", moyenne);
        printf("Note minimale: %.2f\n", note_min);
        printf("Note maximale: %.2f\n", note_max);
        
        // Count success rate (note >= 10)
        int nb_reussites = 0;
        note_courant = notes;
        while (note_courant != NULL) {
            if (note_courant->note.id_matiere == id_matiere && note_courant->note.note >= 10.0f) {
                nb_reussites++;
            }
            note_courant = note_courant->suivant;
        }
        
        float taux_reussite = (float)nb_reussites / nb_notes * 100;
        printf("Taux de réussite: %.1f%% (%d sur %d)\n", taux_reussite, nb_reussites, nb_notes);
    } else {
        printf("Aucune note enregistrée pour cette matière.\n");
    }
}