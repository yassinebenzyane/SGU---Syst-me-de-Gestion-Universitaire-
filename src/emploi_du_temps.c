/**
 * @file emploi_du_temps.c
 * @brief Implementation of timetable management module
 */

#include "../include/emploi_du_temps.h"
#include "../include/utils.h"

#define EDT_FILE "data/emploi_du_temps.txt"

// Helper function to convert day number to string
const char* jour_to_string(int jour) {
    static const char* jours[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi"};
    
    if (jour >= 0 && jour < JOURS_PAR_SEMAINE) {
        return jours[jour];
    }
    
    return "Inconnu";
}

// Helper function to convert hour number to string
const char* heure_to_string(int heure) {
    static const char* heures[] = {"8h-10h", "10h-12h", "14h-16h", "16h-18h"};
    
    if (heure >= 0 && heure < CRENEAUX_PAR_JOUR) {
        return heures[heure];
    }
    
    return "Inconnu";
}

/**
 * Load timetable from file
 */
int charger_emploi_du_temps(EmploiDuTemps *edt) {
    FILE *file;
    
    create_data_dir();
    
    // Initialize empty timetable
    initialiser_emploi_du_temps(edt);
    
    if (!file_exists(EDT_FILE)) {
        // Create empty file if it doesn't exist
        create_file_if_not_exists(EDT_FILE);
        return 1;
    }
    
    file = fopen(EDT_FILE, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier d'emploi du temps.\n");
        return 0;
    }
    
    char line[256];
    edt->nb_creneaux = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline
        
        Creneau creneau;
        // Parse the line (id|id_matiere|nom_matiere|id_enseignant|nom_enseignant|salle|jour|heure)
        sscanf(line, "%d|%d|%[^|]|%d|%[^|]|%[^|]|%d|%d",
               &creneau.id,
               &creneau.id_matiere,
               creneau.nom_matiere,
               &creneau.id_enseignant,
               creneau.nom_enseignant,
               creneau.salle,
               &creneau.jour,
               &creneau.heure);
        
        // Place in the right spot in the timetable
        if (creneau.jour >= 0 && creneau.jour < JOURS_PAR_SEMAINE &&
            creneau.heure >= 0 && creneau.heure < CRENEAUX_PAR_JOUR) {
            edt->creneaux[creneau.jour][creneau.heure] = creneau;
            edt->nb_creneaux++;
        }
    }
    
    fclose(file);
    return 1;
}

/**
 * Save timetable to file
 */
int sauvegarder_emploi_du_temps(const EmploiDuTemps *edt) {
    FILE *file = fopen(EDT_FILE, "w");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier d'emploi du temps en écriture.\n");
        return 0;
    }
    
    // Loop through each cell in the timetable
    for (int jour = 0; jour < JOURS_PAR_SEMAINE; jour++) {
        for (int heure = 0; heure < CRENEAUX_PAR_JOUR; heure++) {
            Creneau creneau = edt->creneaux[jour][heure];
            
            // Only save non-empty slots
            if (creneau.id != 0) {
                fprintf(file, "%d|%d|%s|%d|%s|%s|%d|%d\n",
                        creneau.id,
                        creneau.id_matiere,
                        creneau.nom_matiere,
                        creneau.id_enseignant,
                        creneau.nom_enseignant,
                        creneau.salle,
                        creneau.jour,
                        creneau.heure);
            }
        }
    }
    
    fclose(file);
    return 1;
}

/**
 * Generate an automatic timetable
 */
// Fonction pour créer manuellement l'emploi du temps
void creation_manuelle_emploi_du_temps(EmploiDuTemps *edt, NodeEnseignant* enseignants) {
    // Initialiser l'emploi du temps
    initialiser_emploi_du_temps(edt);
    
    printf("\n=== Création Manuelle de l'Emploi du Temps ===\n");
    
    for (int i = 0; i < JOURS_PAR_SEMAINE; i++) {
        for (int j = 0; j < CRENEAUX_PAR_JOUR; j++) {
            printf("\nSélectionnez le jour (0: Lundi, 1: Mardi, 2: Mercredi, 3: Jeudi, 4: Vendredi): ");
            int jour = get_int_input("Jour: ", 0, JOURS_PAR_SEMAINE - 1);
            printf("Sélectionnez l'heure (0: 8h-10h, 1: 10h-12h, 2: 14h-16h, 3: 16h-18h): ");
            int heure = get_int_input("Heure: ", 0, CRENEAUX_PAR_JOUR - 1);
            
            // Vérification de la disponibilité du créneau
            if (edt->creneaux[jour][heure].id != 0) {
                printf("⚠️ Ce créneau est déjà occupé par %s (%s).\n", 
                       edt->creneaux[jour][heure].nom_enseignant, 
                       edt->creneaux[jour][heure].nom_matiere);
                continue; // Passer au créneau suivant
            }
            
            // Afficher la liste des enseignants
            afficher_enseignants(enseignants); // Fonction pour afficher tous les enseignants
            
            // Saisie du nom de l'enseignant
            char nom_enseignant[50];
            get_input("Nom de l'enseignant: ", nom_enseignant, sizeof(nom_enseignant));
            
            // Remplissage automatique de la matière
            NodeEnseignant* enseignant = trouver_enseignant_par_id(enseignants, nom_enseignant);
            if (enseignant == NULL) {
                printf("❌ Matière introuvable pour cet enseignant. Opération annulée.\n");
                continue; // Passer au créneau suivant
            }
            
            // Saisie de la section
            char section[20];
            get_input("Section: ", section, sizeof(section));
            
            // Saisie du type de séance
            char type_seance[20];
            get_input("Type de séance (TP ou Cours): ", type_seance, sizeof(type_seance));
            
            // Saisie de la salle
            char salle[MAX_SALLE_LEN];
            get_input("Salle: ", salle, sizeof(salle));
            
            // Enregistrement du créneau
            Creneau creneau;
            creneau.id = generate_id(); // Générer un nouvel ID
            creneau.id_matiere = enseignant->enseignant.id; // Associer la matière
            strcpy(creneau.nom_matiere, enseignant->enseignant.matiere_enseignee);
            creneau.id_enseignant = enseignant->enseignant.id;
            strcpy(creneau.nom_enseignant, enseignant->enseignant.prenom);
            strcpy(creneau.salle, salle);
            creneau.jour = jour;
            creneau.heure = heure;
            
            // Ajouter le créneau à l'emploi du temps
            edt->creneaux[jour][heure] = creneau;
            edt->nb_creneaux++;
        }
    }
    
    // Sauvegarder l'emploi du temps
    if (sauvegarder_emploi_du_temps(edt)) {
        printf("Emploi du temps créé avec succès.\n");
    } else {
        printf("Erreur lors de la sauvegarde de l'emploi du temps.\n");
    }
}
/**
 * Add a new slot to the timetable
 */
void ajouter_creneau(EmploiDuTemps *edt, NodeEnseignant* enseignants, NodeMatiere* matieres) {
    // Check if there are subjects and teachers
    if (matieres == NULL) {
        printf("Erreur: Aucune matière n'est enregistrée. Veuillez d'abord ajouter des matières.\n");
        return;
    }
    
    if (enseignants == NULL) {
        printf("Erreur: Aucun enseignant n'est enregistré. Veuillez d'abord ajouter des enseignants.\n");
        return;
    }
    
    printf("\n=== Ajouter un nouveau créneau ===\n");
    
    // Display the current timetable
    afficher_emploi_du_temps(edt);
    
    // Get the day and time for the new slot
    int jour = get_int_input("Jour (0: Lundi, 1: Mardi, 2: Mercredi, 3: Jeudi, 4: Vendredi): ", 
                           0, JOURS_PAR_SEMAINE - 1);
    
    int heure = get_int_input("Heure (0: 8h-10h, 1: 10h-12h, 2: 14h-16h, 3: 16h-18h): ", 
                            0, CRENEAUX_PAR_JOUR - 1);
    
    // Check if the slot is already occupied
    if (edt->creneaux[jour][heure].id != 0) {
        printf("Ce créneau est déjà occupé. Veuillez le modifier ou le supprimer d'abord.\n");
        return;
    }
    
    // Display subjects and choose one
    afficher_matieres(matieres);
    int id_matiere = get_int_input("ID de la matière: ", 1, 999);
    
    NodeMatiere* matiere = trouver_matiere_par_id(matieres, id_matiere);
    if (matiere == NULL) {
        printf("Matière avec ID %d non trouvée.\n", id_matiere);
        return;
    }
    
    // Display teachers and choose one
    afficher_enseignants(enseignants);
    int id_enseignant = get_int_input("ID de l'enseignant: ", 1, 999);
    
    NodeEnseignant* enseignant = trouver_enseignant_par_id(enseignants, id_enseignant);
    if (enseignant == NULL) {
        printf("Enseignant avec ID %d non trouvé.\n", id_enseignant);
        return;
    }
    
    // Get room information
    char salle[MAX_SALLE_LEN];
    get_input("Salle: ", salle, sizeof(salle));
    
    // Generate a new ID for the slot
    int new_id = 1;
    for (int j = 0; j < JOURS_PAR_SEMAINE; j++) {
        for (int h = 0; h < CRENEAUX_PAR_JOUR; h++) {
            if (edt->creneaux[j][h].id >= new_id) {
                new_id = edt->creneaux[j][h].id + 1;
            }
        }
    }
    
    // Create the new slot
    Creneau creneau;
    creneau.id = new_id;
    creneau.id_matiere = matiere->matiere.id;
    strcpy(creneau.nom_matiere, matiere->matiere.nom_matiere);
    creneau.id_enseignant = enseignant->enseignant.id;
    snprintf(creneau.nom_enseignant, sizeof(creneau.nom_enseignant), 
            "%s %s", enseignant->enseignant.prenom, enseignant->enseignant.nom);
    strcpy(creneau.salle, salle);
    creneau.jour = jour;
    creneau.heure = heure;
    
    // Add to timetable
    edt->creneaux[jour][heure] = creneau;
    edt->nb_creneaux++;
    
    // Save the updated timetable
    if (sauvegarder_emploi_du_temps(edt)) {
        printf("Créneau ajouté avec succès.\n");
    } else {
        printf("Erreur lors de la sauvegarde de l'emploi du temps.\n");
    }
}

/**
 * Modify an existing slot in the timetable
 */
void modifier_creneau(EmploiDuTemps *edt, NodeEnseignant* enseignants, NodeMatiere* matieres) {
    if (edt->nb_creneaux == 0) {
        printf("Aucun créneau n'est enregistré dans l'emploi du temps.\n");
        return;
    }
    
    printf("\n=== Modifier un créneau ===\n");
    
    // Display the current timetable
    afficher_emploi_du_temps(edt);
    
    // Get the day and time of the slot to modify
    int jour = get_int_input("Jour du créneau à modifier (0: Lundi, 1: Mardi, 2: Mercredi, 3: Jeudi, 4: Vendredi): ", 
                           0, JOURS_PAR_SEMAINE - 1);
    
    int heure = get_int_input("Heure du créneau à modifier (0: 8h-10h, 1: 10h-12h, 2: 14h-16h, 3: 16h-18h): ", 
                            0, CRENEAUX_PAR_JOUR - 1);
    
    // Check if the slot exists
    if (edt->creneaux[jour][heure].id == 0) {
        printf("Ce créneau est vide. Veuillez utiliser l'option 'Ajouter un créneau' à la place.\n");
        return;
    }
    
    Creneau *creneau = &edt->creneaux[jour][heure];
    
    printf("\nModification du créneau: %s, %s - %s avec %s en salle %s\n",
           jour_to_string(jour), heure_to_string(heure),
           creneau->nom_matiere, creneau->nom_enseignant, creneau->salle);
    
    printf("Laissez vide pour garder la valeur actuelle.\n");
    
    // Choose new subject if needed
    printf("\nMatière actuelle: %s\n", creneau->nom_matiere);
    int modifier_matiere = get_int_input("Modifier la matière? (1: Oui, 0: Non): ", 0, 1);
    
    if (modifier_matiere) {
        afficher_matieres(matieres);
        int id_matiere = get_int_input("Nouvelle ID de matière: ", 1, 999);
        
        NodeMatiere* matiere = trouver_matiere_par_id(matieres, id_matiere);
        if (matiere == NULL) {
            printf("Matière avec ID %d non trouvée. La matière n'a pas été modifiée.\n", id_matiere);
        } else {
            creneau->id_matiere = matiere->matiere.id;
            strcpy(creneau->nom_matiere, matiere->matiere.nom_matiere);
        }
    }
    
    // Choose new teacher if needed
    printf("\nEnseignant actuel: %s\n", creneau->nom_enseignant);
    int modifier_enseignant = get_int_input("Modifier l'enseignant? (1: Oui, 0: Non): ", 0, 1);
    
    if (modifier_enseignant) {
        afficher_enseignants(enseignants);
        int id_enseignant = get_int_input("Nouvelle ID d'enseignant: ", 1, 999);
        
        NodeEnseignant* enseignant = trouver_enseignant_par_id(enseignants, id_enseignant);
        if (enseignant == NULL) {
            printf("Enseignant avec ID %d non trouvé. L'enseignant n'a pas été modifié.\n", id_enseignant);
        } else {
            creneau->id_enseignant = enseignant->enseignant.id;
            snprintf(creneau->nom_enseignant, sizeof(creneau->nom_enseignant), 
                    "%s %s", enseignant->enseignant.prenom, enseignant->enseignant.nom);
        }
    }
    
    // Update room if needed
    printf("\nSalle actuelle: %s\n", creneau->salle);
    char nouvelle_salle[MAX_SALLE_LEN];
    get_input("Nouvelle salle (laisser vide pour garder): ", nouvelle_salle, sizeof(nouvelle_salle));
    
    if (strlen(nouvelle_salle) > 0) {
        strcpy(creneau->salle, nouvelle_salle);
    }
    
    // Save the updated timetable
    if (sauvegarder_emploi_du_temps(edt)) {
        printf("Créneau modifié avec succès.\n");
    } else {
        printf("Erreur lors de la sauvegarde de l'emploi du temps.\n");
    }
}

/**
 * Delete a slot from the timetable
 */
void supprimer_creneau(EmploiDuTemps *edt) {
    if (edt->nb_creneaux == 0) {
        printf("Aucun créneau n'est enregistré dans l'emploi du temps.\n");
        return;
    }
    
    printf("\n=== Supprimer un créneau ===\n");
    
    // Display the current timetable
    afficher_emploi_du_temps(edt);
    
    // Get the day and time of the slot to delete
    int jour = get_int_input("Jour du créneau à supprimer (0: Lundi, 1: Mardi, 2: Mercredi, 3: Jeudi, 4: Vendredi): ", 
                           0, JOURS_PAR_SEMAINE - 1);
    
    int heure = get_int_input("Heure du créneau à supprimer (0: 8h-10h, 1: 10h-12h, 2: 14h-16h, 3: 16h-18h): ", 
                            0, CRENEAUX_PAR_JOUR - 1);
    
    // Check if the slot exists
    if (edt->creneaux[jour][heure].id == 0) {
        printf("Ce créneau est déjà vide.\n");
        return;
    }
    
    // Confirmation
    printf("\nVous allez supprimer le créneau: %s, %s - %s avec %s en salle %s\n",
           jour_to_string(jour), heure_to_string(heure),
           edt->creneaux[jour][heure].nom_matiere, 
           edt->creneaux[jour][heure].nom_enseignant, 
           edt->creneaux[jour][heure].salle);
    
    int confirmation = get_int_input("Confirmer la suppression? (1: Oui, 0: Non): ", 0, 1);
    
    if (confirmation) {
        // Clear the slot
        memset(&edt->creneaux[jour][heure], 0, sizeof(Creneau));
        edt->nb_creneaux--;
        
        // Save the updated timetable
        if (sauvegarder_emploi_du_temps(edt)) {
            printf("Créneau supprimé avec succès.\n");
        } else {
            printf("Erreur lors de la sauvegarde de l'emploi du temps.\n");
        }
    } else {
        printf("Opération annulée.\n");
    }
}

/**
 * Display the complete timetable
 */
void afficher_emploi_du_temps(const EmploiDuTemps *edt) {
    printf("\n=== Emploi du Temps ===\n\n");
    
    // Column headers (hours)
    printf("%-10s |", "");
    for (int h = 0; h < CRENEAUX_PAR_JOUR; h++) {
        printf(" %-20s |", heure_to_string(h));
    }
    printf("\n");
    
    // Separator line
    for (int i = 0; i < 10 + 6 * (23); i++) {
        printf("-");
    }
    printf("\n");
    
    // Print each day
    for (int j = 0; j < JOURS_PAR_SEMAINE; j++) {
        printf("%-10s |", jour_to_string(j));
        
        for (int h = 0; h < CRENEAUX_PAR_JOUR; h++) {
            if (edt->creneaux[j][h].id != 0) {
                printf(" %-10s (%s) |", edt->creneaux[j][h].nom_matiere, edt->creneaux[j][h].salle);
            } else {
                printf(" %-20s |", "");
            }
        }
        printf("\n");
        
        // Print teacher names in second line
        printf("%-10s |", "");
        for (int h = 0; h < CRENEAUX_PAR_JOUR; h++) {
            if (edt->creneaux[j][h].id != 0) {
                printf(" %-20s |", edt->creneaux[j][h].nom_enseignant);
            } else {
                printf(" %-20s |", "");
            }
        }
        printf("\n");
        
        // Separator line
        for (int i = 0; i < 10 + 6 * (23); i++) {
            printf("-");
        }
        printf("\n");
    }
    
    printf("\n");
}

/**
 * Initialize an empty timetable
 */
void initialiser_emploi_du_temps(EmploiDuTemps *edt) {
    // Initialize all slots to empty
    for (int jour = 0; jour < JOURS_PAR_SEMAINE; jour++) {
        for (int heure = 0; heure < CRENEAUX_PAR_JOUR; heure++) {
            memset(&edt->creneaux[jour][heure], 0, sizeof(Creneau));
        }
    }
    
    edt->nb_creneaux = 0;
}
