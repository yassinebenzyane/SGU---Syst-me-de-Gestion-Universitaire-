/**
 * @file inscriptions.h
 * @brief Header file for course enrollment management
 */

#ifndef INSCRIPTIONS_H
#define INSCRIPTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Structure to represent a course enrollment
 */
typedef struct {
    int id;
    int id_etudiant;
    int id_matiere;
    char date_inscription[20];
    int statut; // 1 = enrolled, 0 = unenrolled
} Inscription;

/**
 * Node for enrollment linked list
 */
typedef struct NodeInscription {
    Inscription inscription;
    struct NodeInscription* suivant;
} NodeInscription;

/**
 * Load enrollments from file into a linked list
 */
int charger_inscriptions(NodeInscription** tete);

/**
 * Save enrollments from linked list to file
 */
int sauvegarder_inscriptions(NodeInscription* tete);

/**
 * Enroll a student in a course
 */
void inscrire_etudiant_cours(NodeInscription** tete, int id_etudiant, int id_matiere);

/**
 * Unenroll a student from a course
 */
void desinscrire_etudiant_cours(NodeInscription** tete, int id_etudiant, int id_matiere);

/**
 * Display all enrollments in the linked list
 */
void afficher_inscriptions(NodeInscription* tete);

/**
 * Display enrollments by student ID
 */
void afficher_inscriptions_par_etudiant(NodeInscription* tete, int id_etudiant);

/**
 * Display enrollments by subject ID
 */
void afficher_inscriptions_par_matiere(NodeInscription* tete, int id_matiere);

/**
 * Check if a student is enrolled in a specific course
 * @return 1 if enrolled, 0 if not enrolled
 */
int est_inscrit(NodeInscription* tete, int id_etudiant, int id_matiere);

/**
 * Get student enrollment management menu
 */
void menu_inscriptions_etudiant(NodeInscription** tete, int id_etudiant);

/**
 * Free memory allocated for enrollment linked list
 */
void liberer_inscriptions(NodeInscription** tete);

#endif /* INSCRIPTIONS_H */