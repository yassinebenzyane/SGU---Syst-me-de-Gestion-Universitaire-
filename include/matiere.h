/**
 * @file matiere.h
 * @brief Subject management module for the school management system
 */

#ifndef MATIERE_H
#define MATIERE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE_MATIERE_LEN 15
#define MAX_NOM_MATIERE_LEN 50

/**
 * @struct Matiere
 * @brief Structure for storing subject information
 */
typedef struct {
    int id;
    char code_matiere[MAX_CODE_MATIERE_LEN];
    char nom_matiere[MAX_NOM_MATIERE_LEN];
    float coefficient;
} Matiere;

/**
 * @struct NodeMatiere
 * @brief Node structure for subject linked list
 */
typedef struct NodeMatiere {
    Matiere matiere;
    struct NodeMatiere *suivant;
} NodeMatiere;

/**
 * @brief Load subjects from file into a linked list
 * @param tete Pointer to head of linked list
 * @return 1 if successful, 0 if error
 */
int charger_matieres(NodeMatiere** tete);

/**
 * @brief Save subjects from linked list to file
 * @param tete Head of linked list
 * @return 1 if successful, 0 if error
 */
int sauvegarder_matieres(NodeMatiere* tete);

/**
 * @brief Add a new subject to the linked list
 * @param tete Pointer to head of linked list
 */
void ajouter_matiere(NodeMatiere** tete);

/**
 * @brief Modify information of an existing subject
 * @param tete Pointer to head of linked list
 */
void modifier_matiere(NodeMatiere* tete);

/**
 * @brief Delete a subject from the linked list
 * @param tete Pointer to head of linked list
 */
void supprimer_matiere(NodeMatiere** tete);

/**
 * @brief Display all subjects in the linked list
 * @param tete Head of linked list
 */
void afficher_matieres(NodeMatiere* tete);

/**
 * @brief Find a subject by ID
 * @param tete Head of linked list
 * @param id Subject ID to search for
 * @return Pointer to found subject node, NULL if not found
 */
NodeMatiere* trouver_matiere_par_id(NodeMatiere* tete, int id);

/**
 * @brief Search subjects by multiple criteria
 * @param tete Head of linked list
 */
void rechercher_matieres(NodeMatiere* tete);

/**
 * @brief Free memory allocated for subject linked list
 * @param tete Pointer to head of linked list
 */
void liberer_matieres(NodeMatiere** tete);

#endif /* MATIERE_H */
