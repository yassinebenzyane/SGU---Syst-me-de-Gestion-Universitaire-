/**
 * @file gestion_etudiants.h
 * @brief Functions for managing students in the school management system
 */

#ifndef GESTION_ETUDIANTS_H
#define GESTION_ETUDIANTS_H

#include "etudiant.h"
#include "notes.h"
#include "inscriptions.h"

/**
 * @brief Menu for student management
 * @param etudiants Pointer to the list of students
 */
void menu_gestion_etudiants(NodeEtudiant** etudiants);

/**
 * @brief Display statistics about students
 * @param etudiants List of students
 * @param notes List of grades
 */
void afficher_statistiques_etudiants(NodeEtudiant* etudiants, NodeNote* notes);

/**
 * @brief Display performance report for a specific student
 * @param etudiants List of students
 * @param notes List of grades
 * @param inscriptions List of course registrations
 */
void afficher_rapport_etudiant(NodeEtudiant* etudiants, NodeNote* notes, NodeInscription* inscriptions);

/**
 * @brief Manage student registrations for courses
 * @param inscriptions Pointer to the list of course registrations
 * @param id_etudiant ID of the student
 */
void menu_inscriptions_etudiant(NodeInscription** inscriptions, int id_etudiant);

#endif /* GESTION_ETUDIANTS_H */