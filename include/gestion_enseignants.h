/**
 * @file gestion_enseignants.h
 * @brief Functions for managing teachers in the school management system
 */

#ifndef GESTION_ENSEIGNANTS_H
#define GESTION_ENSEIGNANTS_H

#include "enseignant.h"
#include "annonces.h"
#include "notes.h"

/**
 * @brief Menu for teacher management
 * @param enseignants Pointer to the list of teachers
 */
void menu_gestion_enseignants(NodeEnseignant** enseignants);

/**
 * @brief Display statistics about teachers
 * @param enseignants List of teachers
 * @param notes List of grades
 * @param annonces List of announcements
 */
void afficher_statistiques_enseignants(NodeEnseignant* enseignants, NodeNote* notes, NodeAnnonce* annonces);

/**
 * @brief Menu for managing announcements by a teacher
 * @param annonces Pointer to the list of announcements
 * @param id_enseignant ID of the teacher
 */
void menu_gestion_annonces_enseignant(NodeAnnonce** annonces, int id_enseignant);

/**
 * @brief Menu for managing grades by a teacher
 * @param notes Pointer to the list of grades
 * @param id_enseignant ID of the teacher
 */
void menu_gestion_notes_enseignant(NodeNote** notes, int id_enseignant);

#endif /* GESTION_ENSEIGNANTS_H */