/**
 * @file enseignant.h
 * @brief Teacher management module for the school management system
 */

#ifndef ENSEIGNANT_H
#define ENSEIGNANT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE_ENSEIGNANT_LEN 15
#define MAX_MATIERE_ENSEIGNEE_LEN 50

/**
 * @struct Enseignant
 * @brief Structure for storing teacher information
 */
typedef struct {
    int id;
    char prenom[50];
    char nom[50];
    char email[50];
    char code_enseignant[MAX_CODE_ENSEIGNANT_LEN];
    char matiere_enseignee[MAX_MATIERE_ENSEIGNEE_LEN];
} Enseignant;

/**
 * @struct NodeEnseignant
 * @brief Node structure for teacher linked list
 */
typedef struct NodeEnseignant {
    Enseignant enseignant;
    struct NodeEnseignant *suivant;
} NodeEnseignant;

/**
 * @brief Load teachers from file into a linked list
 * @param tete Pointer to head of linked list
 * @return 1 if successful, 0 if error
 */
int charger_enseignants(NodeEnseignant** tete);

/**
 * @brief Save teachers from linked list to file
 * @param tete Head of linked list
 * @return 1 if successful, 0 if error
 */
int sauvegarder_enseignants(NodeEnseignant* tete);

/**
 * @brief Add a new teacher to the linked list
 * @param tete Pointer to head of linked list
 */
void ajouter_enseignant(NodeEnseignant** tete);

/**
 * @brief Modify information of an existing teacher
 * @param tete Head of linked list
 */
void modifier_enseignant(NodeEnseignant* tete);

/**
 * @brief Delete a teacher from the linked list
 * @param tete Pointer to head of linked list
 */
void supprimer_enseignant(NodeEnseignant** tete);

/**
 * @brief Display all teachers in the linked list
 * @param tete Head of linked list
 */
void afficher_enseignants(NodeEnseignant* tete);

/**
 * @brief Find a teacher by ID
 * @param tete Head of linked list
 * @param id Teacher ID to search for
 * @return Pointer to found teacher node, NULL if not found
 */
NodeEnseignant* trouver_enseignant_par_id(NodeEnseignant* tete, int id);

/**
 * @brief Search teachers by multiple criteria
 * @param tete Head of linked list
 */
void rechercher_enseignants(NodeEnseignant* tete);

/**
 * @brief Free memory allocated for teacher linked list
 * @param tete Pointer to head of linked list
 */
void liberer_enseignants(NodeEnseignant** tete);

#endif /* ENSEIGNANT_H */
