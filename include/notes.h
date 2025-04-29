/**
 * @file notes.h
 * @brief Header file for grades management
 */

#ifndef NOTES_H
#define NOTES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Structure to represent a grade
 */
typedef struct {
    int id;
    int id_etudiant;
    int id_matiere;
    int id_enseignant;  // ID of the teacher who gave the grade
    float note;
    char commentaire[200];
    char date_evaluation[20];
} Note;

/**
 * Node for grade linked list
 */
typedef struct NodeNote {
    Note note;
    struct NodeNote* suivant;
} NodeNote;

/**
 * Load grades from file into a linked list
 */
int charger_notes(NodeNote** tete);

/**
 * Save grades from linked list to file
 */
int sauvegarder_notes(NodeNote* tete);

/**
 * Add a new grade to the linked list
 */
void ajouter_note(NodeNote** tete, int id_enseignant);

/**
 * Modify an existing grade
 */
void modifier_note(NodeNote* tete, int id_enseignant);

/**
 * Delete a grade from the linked list
 */
void supprimer_note(NodeNote** tete, int id_enseignant);

/**
 * Display all grades in the linked list
 */
void afficher_notes(NodeNote* tete);

/**
 * Display grades by student ID
 */
void afficher_notes_par_etudiant(NodeNote* tete, int id_etudiant);

/**
 * Display grades by subject ID
 */
void afficher_notes_par_matiere(NodeNote* tete, int id_matiere);

/**
 * Display grades by teacher ID
 */
void afficher_notes_par_enseignant(NodeNote* tete, int id_enseignant);

/**
 * Calculate the average grade for a student across all subjects
 */
float calculer_moyenne_etudiant(NodeNote* tete, int id_etudiant);

/**
 * Calculate the average grade for a subject across all students
 */
float calculer_moyenne_matiere(NodeNote* tete, int id_matiere);

/**
 * Find a grade by ID
 */
NodeNote* trouver_note_par_id(NodeNote* tete, int id);

/**
 * Free memory allocated for grade linked list
 */
void liberer_notes(NodeNote** tete);

#endif /* NOTES_H */