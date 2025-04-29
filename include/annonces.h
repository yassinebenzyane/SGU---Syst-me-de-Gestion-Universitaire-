/**
 * @file annonces.h
 * @brief Header file for announcement management
 */

#ifndef ANNONCES_H
#define ANNONCES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Structure to represent an announcement
 */
typedef struct {
    int id;
    char titre[100];
    char contenu[1000];
    char auteur[100];
    char date_creation[20];
    int id_matiere;  // ID of the related subject, 0 if general announcement
    int id_enseignant; // ID of the teacher who created the announcement
} Annonce;

/**
 * Node for announcement linked list
 */
typedef struct NodeAnnonce {
    Annonce annonce;
    struct NodeAnnonce* suivant;
} NodeAnnonce;

/**
 * Load announcements from file into a linked list
 */
int charger_annonces(NodeAnnonce** tete);

/**
 * Save announcements from linked list to file
 */
int sauvegarder_annonces(NodeAnnonce* tete);

/**
 * Add a new announcement to the linked list
 */
void ajouter_annonce(NodeAnnonce** tete, int id_enseignant);

/**
 * Modify an existing announcement
 */
void modifier_annonce(NodeAnnonce* tete, int id_enseignant);

/**
 * Delete an announcement from the linked list
 */
void supprimer_annonce(NodeAnnonce** tete, int id_enseignant);

/**
 * Display all announcements in the linked list
 */
void afficher_annonces(NodeAnnonce* tete);

/**
 * Display announcements by subject ID
 */
void afficher_annonces_par_matiere(NodeAnnonce* tete, int id_matiere);

/**
 * Display announcements by teacher ID
 */
void afficher_annonces_par_enseignant(NodeAnnonce* tete, int id_enseignant);

/**
 * Find an announcement by ID
 */
NodeAnnonce* trouver_annonce_par_id(NodeAnnonce* tete, int id);

/**
 * Free memory allocated for announcement linked list
 */
void liberer_annonces(NodeAnnonce** tete);

#endif /* ANNONCES_H */