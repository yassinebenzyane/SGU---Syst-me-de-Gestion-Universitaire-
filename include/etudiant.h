/**
 * @file etudiant.h
 * @brief Student management module for the school management system
 */

#ifndef ETUDIANT_H
#define ETUDIANT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CNE_LEN 15
#define MAX_SECTION_LEN 10
#define MAX_FILIERE_LEN 50

/**
 * @struct Etudiant
 * @brief Structure for storing student information
 */
typedef struct {
    int id;
    char prenom[50];
    char nom[50];
    char email[50];
    char cne[MAX_CNE_LEN];
    char section[MAX_SECTION_LEN];
    char filiere[MAX_FILIERE_LEN];
} Etudiant;

/**
 * @struct NodeEtudiant
 * @brief Node structure for student linked list
 */
typedef struct NodeEtudiant {
    Etudiant etudiant;
    struct NodeEtudiant *suivant;
} NodeEtudiant;

/**
 * @brief Load students from file into a linked list
 * @param tete Pointer to head of linked list
 * @return 1 if successful, 0 if error
 */
int charger_etudiants(NodeEtudiant** tete);

/**
 * @brief Save students from linked list to file
 * @param tete Head of linked list
 * @return 1 if successful, 0 if error
 */
int sauvegarder_etudiants(NodeEtudiant* tete);

/**
 * @brief Add a new student to the linked list
 * @param tete Pointer to head of linked list
 */
void ajouter_etudiant(NodeEtudiant** tete);

/**
 * @brief Modify information of an existing student
 * @param tete Head of linked list
 */
void modifier_etudiant(NodeEtudiant* tete);

/**
 * @brief Delete a student from the linked list
 * @param tete Pointer to head of linked list
 */
void supprimer_etudiant(NodeEtudiant** tete);

/**
 * @brief Display all students in the linked list
 * @param tete Head of linked list
 */
void afficher_etudiants(NodeEtudiant* tete);

/**
 * @brief Find a student by ID
 * @param tete Head of linked list
 * @param id Student ID to search for
 * @return Pointer to found student node, NULL if not found
 */
NodeEtudiant* trouver_etudiant_par_id(NodeEtudiant* tete, int id);

/**
 * @brief Search students by multiple criteria
 * @param tete Head of linked list
 * @param critere The criterion to search by (1=ID, 2=Nom, 3=Prenom, 4=Email, 5=CNE, 6=Section, 7=Filiere)
 */
void rechercher_etudiants(NodeEtudiant* tete);

/**
 * @brief Free memory allocated for student linked list
 * @param tete Pointer to head of linked list
 */
void liberer_etudiants(NodeEtudiant** tete);

#endif /* ETUDIANT_H */
