/**
 * @file menu.h
 * @brief Menu management module for the school management system
 */

#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include "authentication.h"
#include "etudiant.h"
#include "enseignant.h"
#include "matiere.h"
#include "emploi_du_temps.h"
#include "annonces.h"
#include "notes.h"
#include "inscriptions.h"

/**
 * @brief Display the admin menu and handle admin operations
 * @param utilisateurs Array of users
 * @param nb_utilisateurs Number of users
 * @param etudiants Pointer to student linked list
 * @param enseignants Pointer to teacher linked list
 * @param matieres Pointer to subject linked list
 * @param edt Pointer to timetable structure
 */
void menu_admin(Utilisateur utilisateurs[], int *nb_utilisateurs, 
                NodeEtudiant** etudiants, NodeEnseignant** enseignants, 
                NodeMatiere** matieres, EmploiDuTemps *edt);

/**
 * @brief Display the teacher menu and handle teacher operations
 * @param id_enseignant ID of the logged in teacher
 * @param etudiants Pointer to student linked list
 * @param enseignants Pointer to teacher linked list
 * @param matieres Pointer to subject linked list
 * @param edt Pointer to timetable structure
 */
void menu_enseignant(int id_enseignant, NodeEtudiant* etudiants, 
                    NodeEnseignant* enseignants, NodeMatiere* matieres, 
                    EmploiDuTemps *edt);

/**
 * @brief Display the student menu and handle student operations
 * @param id_etudiant ID of the logged in student
 * @param etudiants Pointer to student linked list
 * @param enseignants Pointer to teacher linked list
 * @param matieres Pointer to subject linked list
 * @param edt Pointer to timetable structure
 */
void menu_etudiant(int id_etudiant, NodeEtudiant* etudiants, 
                  NodeEnseignant* enseignants, NodeMatiere* matieres, 
                  EmploiDuTemps *edt);

#endif /* MENU_H */
