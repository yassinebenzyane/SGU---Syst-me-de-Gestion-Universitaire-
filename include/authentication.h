/**
 * @file authentication.h
 * @brief Authentication module for the school management system
 */

#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_EMAIL_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_ROLE_LEN 20
#define MAX_NAME_LEN 50

/**
 * @struct Utilisateur
 * @brief Structure for storing user information
 */
typedef struct {
    int id;
    char prenom[MAX_NAME_LEN];
    char nom[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    char password[MAX_PASSWORD_LEN];
    char role[MAX_ROLE_LEN];
} Utilisateur;

/**
 * @brief Load users from file into memory
 * @param utilisateurs Array to store loaded users
 * @param nb_utilisateurs Pointer to store number of users loaded
 * @return 1 if successful, 0 if error
 */
int charger_utilisateurs(Utilisateur utilisateurs[], int *nb_utilisateurs);

/**
 * @brief Save users from memory to file
 * @param utilisateurs Array of users to save
 * @param nb_utilisateurs Number of users to save
 * @return 1 if successful, 0 if error
 */
int sauvegarder_utilisateurs(Utilisateur utilisateurs[], int nb_utilisateurs);

/**
 * @brief Authenticate a user with email and password
 * @param email User's email
 * @param password User's password
 * @param id Pointer to store authenticated user's ID
 * @param role Pointer to store authenticated user's role
 * @param prenom Pointer to store authenticated user's first name
 * @param nom Pointer to store authenticated user's last name
 * @return 1 if authentication successful, 0 if failed
 */
int authentifier_utilisateur(char *email, char *password, int *id, char *role, char *prenom, char *nom);

/**
 * @brief Display information of the currently logged in user
 * @param id User ID
 * @param role User role
 * @param prenom User first name
 * @param nom User last name
 */
void afficher_infos_utilisateur(int id, char *role, char *prenom, char *nom);

/**
 * @brief Automatically add a new user
 * @param prenom User first name
 * @param nom User last name
 * @param email User email
 * @param role User role
 */
void ajouter_utilisateur_auto(const char *prenom, const char *nom, const char *email, const char *role);

/**
 * @brief Display all users (admin only)
 */
void afficher_utilisateurs();

#endif /* AUTHENTICATION_H */
