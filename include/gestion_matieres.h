/**
 * @file gestion_matieres.h
 * @brief Functions for managing subjects in the school management system
 */

#ifndef GESTION_MATIERES_H
#define GESTION_MATIERES_H

#include "matiere.h"
#include "notes.h"
#include "inscriptions.h"
#include "etudiant.h"

/**
 * @brief Menu for subject management
 * @param matieres Pointer to the list of subjects
 */
void menu_gestion_matieres(NodeMatiere** matieres);

/**
 * @brief Display statistics about subjects
 * @param matieres List of subjects
 * @param notes List of grades
 * @param inscriptions List of course registrations
 */
void afficher_statistiques_matieres(NodeMatiere* matieres, NodeNote* notes, NodeInscription* inscriptions);

/**
 * @brief Calculate subject statistics and averages
 * @param matieres List of subjects
 * @param notes List of grades
 * @param etudiants List of students
 */
void calculer_statistiques_matieres(NodeMatiere* matieres, NodeNote* notes, NodeEtudiant* etudiants);

/**
 * @brief Display detailed information about a subject
 * @param matieres List of subjects
 * @param notes List of grades
 * @param id_matiere ID of the subject
 */
void afficher_details_matiere(NodeMatiere* matieres, NodeNote* notes, int id_matiere);

#endif /* GESTION_MATIERES_H */