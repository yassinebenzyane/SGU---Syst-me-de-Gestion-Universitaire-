/**
 * @file emploi_du_temps.h
 * @brief Timetable management module for the school management system
 */

#ifndef EMPLOI_DU_TEMPS_H
#define EMPLOI_DU_TEMPS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "enseignant.h"
#include "matiere.h"

#define JOURS_PAR_SEMAINE 5
#define CRENEAUX_PAR_JOUR 4
#define MAX_SALLE_LEN 10

/**
 * @struct Creneau
 * @brief Structure for storing timetable slot information
 */
typedef struct {
    int id;
    int id_matiere;
    char nom_matiere[MAX_NOM_MATIERE_LEN];
    int id_enseignant;
    char nom_enseignant[100];
    char salle[MAX_SALLE_LEN];
    int jour; // 0-4 pour lundi à vendredi
    int heure; // 0-5 pour les créneaux horaires
} Creneau;

/**
 * @struct EmploiDuTemps
 * @brief Structure for storing the complete timetable
 */
typedef struct {
    Creneau creneaux[JOURS_PAR_SEMAINE][CRENEAUX_PAR_JOUR];
    int nb_creneaux;
} EmploiDuTemps;

/**
 * @brief Load timetable from file
 * @param edt Pointer to timetable structure
 * @return 1 if successful, 0 if error
 */
int charger_emploi_du_temps(EmploiDuTemps *edt);

/**
 * @brief Save timetable to file
 * @param edt Pointer to timetable structure
 * @return 1 if successful, 0 if error
 */
int sauvegarder_emploi_du_temps(const EmploiDuTemps *edt);

/**
 * @brief Generate an automatic timetable
 * @param edt Pointer to timetable structure
 * @param enseignants Array of teachers
 * @param nb_enseignants Number of teachers
 * @param matieres Array of subjects
 * @param nb_matieres Number of subjects
 */
void generer_emploi_du_temps(EmploiDuTemps *edt, NodeEnseignant* enseignants, NodeMatiere* matieres);

/**
 * @brief Add a new slot to the timetable
 * @param edt Pointer to timetable structure
 * @param enseignants Linked list of teachers
 * @param matieres Linked list of subjects
 */
void ajouter_creneau(EmploiDuTemps *edt, NodeEnseignant* enseignants, NodeMatiere* matieres);

/**
 * @brief Modify an existing slot in the timetable
 * @param edt Pointer to timetable structure
 * @param enseignants Linked list of teachers
 * @param matieres Linked list of subjects
 */
void modifier_creneau(EmploiDuTemps *edt, NodeEnseignant* enseignants, NodeMatiere* matieres);

/**
 * @brief Delete a slot from the timetable
 * @param edt Pointer to timetable structure
 */
void supprimer_creneau(EmploiDuTemps *edt);

/**
 * @brief Display the complete timetable
 * @param edt Pointer to timetable structure
 */
void afficher_emploi_du_temps(const EmploiDuTemps *edt);

/**
 * @brief Initialize an empty timetable
 * @param edt Pointer to timetable structure
 */
void initialiser_emploi_du_temps(EmploiDuTemps *edt);

#endif /* EMPLOI_DU_TEMPS_H */
