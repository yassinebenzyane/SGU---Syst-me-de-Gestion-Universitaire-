/**
 * @file menu.c
 * @brief Implementation of menu management module
 */

#include "../include/menu.h"
#include "../include/utils.h"

/**
 * Display the admin menu and handle admin operations
 */
void menu_admin(Utilisateur utilisateurs[], int *nb_utilisateurs, 
                NodeEtudiant** etudiants, NodeEnseignant** enseignants, 
                NodeMatiere** matieres, EmploiDuTemps *edt) {
    int choix = 0;
    
    // Initialize necessary modules
    NodeAnnonce* annonces = NULL;
    charger_annonces(&annonces);
    
    NodeNote* notes = NULL;
    charger_notes(&notes);
    
    NodeInscription* inscriptions = NULL;
    charger_inscriptions(&inscriptions);
    
    do {
        clear_screen();
        printf("\n=== Menu Administrateur ===\n");
        printf("1. Gestion des Utilisateurs\n");
        printf("2. Gestion des Étudiants\n");
        printf("3. Gestion des Enseignants\n");
        printf("4. Gestion des Matières\n");
        printf("5. Gestion de l'Emploi du Temps\n");
        printf("6. Gestion des Annonces\n");
        printf("7. Gestion des Notes\n");
        printf("8. Gestion des Inscriptions\n");
        printf("9. Rapports et Statistiques\n");
        printf("0. Déconnexion\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 9);
        
        switch (choix) {
            case 1: {
                // Gestion des Utilisateurs
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Utilisateurs ===\n");
                    printf("1. Afficher tous les utilisateurs\n");
                    printf("2. Ajouter un utilisateur\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 2);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_utilisateurs();
                            pause_screen();
                            break;
                        case 2: {
                            int new_id=generate_id();
                            char prenom[50], nom[50], email[50], role[20];
                            
                            printf("\n=== Ajouter un nouvel utilisateur ===\n");
                            
                            get_input("Prénom: ", prenom, sizeof(prenom));
                            get_input("Nom: ", nom, sizeof(nom));
                            
                            // Generate automatic email
                            printf("Souhaitez-vous générer l'email automatiquement?\n");
                            int generer_email = get_int_input("1. Oui, 2. Non: ", 1, 2);
                            
                            if (generer_email == 1) {
                                // Determine role first to use proper email domain
                                printf("Rôles disponibles: admin, enseignant, etudiant\n");
                                get_input("Rôle: ", role, sizeof(role));
                                
                                // Validate role
                                if (strcmp(role, "admin") != 0 && 
                                    strcmp(role, "enseignant") != 0 && 
                                    strcmp(role, "etudiant") != 0) {
                                    printf("Rôle invalide. Utilisation du rôle 'etudiant' par défaut.\n");
                                    strcpy(role, "etudiant");
                                }
                                
                                // Generate email based on role
                                int est_etudiant = (strcmp(role, "etudiant") == 0) ? 1 : 0;
                                generer_email_unique(prenom, nom, est_etudiant, email, sizeof(email));
                                printf("Email généré automatiquement: %s\n", email);
                            } else {
                                // Email validation
                                int valid_email = 0;
                                while (!valid_email) {
                                    get_input("Email: ", email, sizeof(email));
                                    if (validate_email(email)) {
                                        valid_email = 1;
                                    } else {
                                        printf("Email invalide. Veuillez réessayer.\n");
                                    }
                                }
                                
                                printf("Rôles disponibles: admin, enseignant, etudiant\n");
                                get_input("Rôle: ", role, sizeof(role));
                                
                                // Validate role
                                if (strcmp(role, "admin") != 0 && 
                                    strcmp(role, "enseignant") != 0 && 
                                    strcmp(role, "etudiant") != 0) {
                                    printf("Rôle invalide. Utilisation du rôle 'etudiant' par défaut.\n");
                                    strcpy(role, "etudiant");
                                }
                            }
                            
                            ajouter_utilisateur_auto(new_id,prenom, nom, email, role);
                            pause_screen();
                            break;
                        }
                    }
                } while (sous_choix != 0);
                break;
            }
            case 2: {
                // Gestion des Étudiants
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Étudiants ===\n");
                    printf("1. Afficher tous les étudiants\n");
                    printf("2. Rechercher des étudiants\n");
                    printf("3. Ajouter un étudiant\n");
                    printf("4. Modifier un étudiant\n");
                    printf("5. Supprimer un étudiant\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 5);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_etudiants(*etudiants);
                            pause_screen();
                            break;
                        case 2:
                            rechercher_etudiants(*etudiants);
                            pause_screen();
                            break;
                        case 3:
                            ajouter_etudiant(etudiants);
                            pause_screen();
                            break;
                        case 4:
                            modifier_etudiant(*etudiants);
                            pause_screen();
                            break;
                        case 5:
                            supprimer_etudiant(etudiants);
                            pause_screen();
                            break;
                    }
                } while (sous_choix != 0);
                break;
            }
            case 3: {
                // Gestion des Enseignants
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Enseignants ===\n");
                    printf("1. Afficher tous les enseignants\n");
                    printf("2. Rechercher des enseignants\n");
                    printf("3. Ajouter un enseignant\n");
                    printf("4. Modifier un enseignant\n");
                    printf("5. Supprimer un enseignant\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 5);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_enseignants(*enseignants);
                            pause_screen();
                            break;
                        case 2:
                            rechercher_enseignants(*enseignants);
                            pause_screen();
                            break;
                        case 3:
                            ajouter_enseignant(enseignants);
                            pause_screen();
                            break;
                        case 4:
                            modifier_enseignant(*enseignants);
                            pause_screen();
                            break;
                        case 5:
                            supprimer_enseignant(enseignants);
                            pause_screen();
                            break;
                    }
                } while (sous_choix != 0);
                break;
            }
            case 4: {
                // Gestion des Matières
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Matières ===\n");
                    printf("1. Afficher toutes les matières\n");
                    printf("2. Rechercher des matières\n");
                    printf("3. Ajouter une matière\n");
                    printf("4. Modifier une matière\n");
                    printf("5. Supprimer une matière\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 5);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_matieres(*matieres);
                            pause_screen();
                            break;
                        case 2:
                            rechercher_matieres(*matieres);
                            pause_screen();
                            break;
                        case 3:
                            ajouter_matiere(matieres);
                            pause_screen();
                            break;
                        case 4:
                            modifier_matiere(*matieres);
                            pause_screen();
                            break;
                        case 5:
                            supprimer_matiere(matieres);
                            pause_screen();
                            break;
                    }
                } while (sous_choix != 0);
                break;
            }
            case 5: {
                // Gestion de l'Emploi du Temps
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion de l'Emploi du Temps ===\n");
                    printf("1. Afficher l'emploi du temps\n");
                    printf("2. Générer un emploi du temps\n");
                    printf("3. Ajouter un créneau\n");
                    printf("4. Modifier un créneau\n");
                    printf("5. Supprimer un créneau\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 5);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_emploi_du_temps(edt);
                            pause_screen();
                            break;
                        case 2:
                        creation_manuelle_emploi_du_temps(edt,  *enseignants) ;
                            pause_screen();                          
                            break;
                        case 3:
                            ajouter_creneau(edt, *enseignants, *matieres);
                            pause_screen();
                            break;
                        case 4:
                            modifier_creneau(edt, *enseignants, *matieres);
                            pause_screen();
                            break;
                        case 5:
                            supprimer_creneau(edt);
                            pause_screen();
                            break;
                    }
                } while (sous_choix != 0);
                break;
            }
            case 6: {
                // Gestion des Annonces
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Annonces ===\n");
                    printf("1. Afficher toutes les annonces\n");
                    printf("2. Créer une nouvelle annonce\n");
                    printf("3. Modifier une annonce\n");
                    printf("4. Supprimer une annonce\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 4);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_annonces(annonces);
                            pause_screen();
                            break;
                        case 2:
                            // Admin creates announcement as admin, not linked to teacher
                            ajouter_annonce(&annonces, -1); // Use -1 to indicate admin
                            pause_screen();
                            break;
                        case 3: {
                            // First display all announcements
                            afficher_annonces(annonces);
                            
                            // Get the ID of the announcement to modify
                            int id = get_int_input("Entrez l'ID de l'annonce à modifier (0 pour annuler): ", 0, 9999);
                            if (id == 0) {
                                printf("Opération annulée.\n");
                                continue;
                            }
                            
                            // Find the announcement
                            NodeAnnonce* annonce = trouver_annonce_par_id(annonces, id);
                            if (annonce == NULL) {
                                printf("Annonce avec ID %d non trouvée.\n", id);
                                pause_screen();
                                continue;
                            }
                            
                            // Admin can modify any announcement
                            // (The modification logic would need to be extracted from the current function)
                            
                            printf("\n=== Modifier l'annonce ===\n");
                            printf("Laissez vide pour garder la valeur actuelle.\n");
                            
                            // Get new values or keep current ones
                            char buffer[1024];
                            
                            get_input_formatted("Titre [actuel: %s]: ", buffer, sizeof(buffer), annonce->annonce.titre);
                            if (strlen(buffer) > 0) {
                                strcpy(annonce->annonce.titre, buffer);
                            }
                            
                            printf("Contenu actuel:\n%s\n", annonce->annonce.contenu);
                            printf("Nouveau contenu (tapez 'fin' sur une nouvelle ligne pour terminer, ou juste 'fin' pour garder l'actuel):\n");
                            
                            char content_buffer[1024] = {0};
                            char line[100];
                            fgets(line, sizeof(line), stdin); // Clear any previous input
                            line[strcspn(line, "\n")] = 0; // Remove trailing newline
                            
                            if (strcmp(line, "fin") != 0) {
                                strcat(content_buffer, line);
                                strcat(content_buffer, "\n");
                                
                                while (1) {
                                    fgets(line, sizeof(line), stdin);
                                    line[strcspn(line, "\n")] = 0; // Remove trailing newline
                                    
                                    if (strcmp(line, "fin") == 0) {
                                        break;
                                    }
                                    
                                    strcat(content_buffer, line);
                                    strcat(content_buffer, "\n");
                                    
                                    if (strlen(content_buffer) + 10 >= sizeof(annonce->annonce.contenu)) {
                                        printf("Contenu trop long, arrêt.\n");
                                        break;
                                    }
                                }
                                
                                if (strlen(content_buffer) > 0) {
                                    strncpy(annonce->annonce.contenu, content_buffer, sizeof(annonce->annonce.contenu) - 1);
                                    annonce->annonce.contenu[sizeof(annonce->annonce.contenu) - 1] = '\0';
                                }
                            }
                            
                            // Update matière
                            printf("Matière concernée [actuel: %d] (0 pour annonce générale): ", annonce->annonce.id_matiere);
                            char matiere_buffer[10];
                            fgets(matiere_buffer, sizeof(matiere_buffer), stdin);
                            int new_matiere_id = atoi(matiere_buffer);
                            if (strlen(matiere_buffer) > 1) { // more than just newline
                                annonce->annonce.id_matiere = new_matiere_id;
                            }
                            
                            // Update auteur to indicate admin edit
                            strcpy(annonce->annonce.auteur, "Administrateur");
                            
                            // Update creation date
                            time_t now = time(NULL);
                            struct tm *t = localtime(&now);
                            strftime(annonce->annonce.date_creation, sizeof(annonce->annonce.date_creation), 
                                     "%d/%m/%Y %H:%M", t);
                            
                            // Save changes
                            if (sauvegarder_annonces(annonces)) {
                                printf("Annonce modifiée avec succès.\n");
                            } else {
                                printf("Erreur lors de la sauvegarde des modifications.\n");
                            }
                            
                            pause_screen();
                            break;
                        }
                        case 4: {
                            // First display all announcements
                            afficher_annonces(annonces);
                            
                            // Get the ID of the announcement to delete
                            int id = get_int_input("Entrez l'ID de l'annonce à supprimer (0 pour annuler): ", 0, 9999);
                            if (id == 0) {
                                printf("Opération annulée.\n");
                                continue;
                            }
                            
                            // Find the announcement
                            NodeAnnonce *courant = annonces;
                            NodeAnnonce *precedent = NULL;
                            
                            while (courant != NULL && courant->annonce.id != id) {
                                precedent = courant;
                                courant = courant->suivant;
                            }
                            
                            if (courant == NULL) {
                                printf("Annonce avec ID %d non trouvée.\n", id);
                                pause_screen();
                                continue;
                            }
                            
                            // Admin can delete any announcement
                            
                            // Confirmation
                            printf("Êtes-vous sûr de vouloir supprimer l'annonce '%s'?\n", courant->annonce.titre);
                            int confirmation = get_int_input("1. Oui, 2. Non: ", 1, 2);
                            
                            if (confirmation == 1) {
                                // Remove the node
                                if (precedent == NULL) {
                                    // It's the first node
                                    annonces = courant->suivant;
                                } else {
                                    precedent->suivant = courant->suivant;
                                }
                                
                                free(courant);
                                
                                // Save changes
                                if (sauvegarder_annonces(annonces)) {
                                    printf("Annonce supprimée avec succès.\n");
                                } else {
                                    printf("Erreur lors de la sauvegarde des modifications.\n");
                                }
                            } else {
                                printf("Opération annulée.\n");
                            }
                            
                            pause_screen();
                            break;
                        }
                    }
                } while (sous_choix != 0);
                break;
            }
            case 7: {
                // Gestion des Notes
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Notes ===\n");
                    printf("1. Afficher toutes les notes\n");
                    printf("2. Afficher les notes par étudiant\n");
                    printf("3. Afficher les notes par matière\n");
                    printf("4. Ajouter une note\n");
                    printf("5. Modifier une note\n");
                    printf("6. Supprimer une note\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 6);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_notes(notes);
                            pause_screen();
                            break;
                        case 2: {
                            int id_etudiant = get_int_input("Entrez l'ID de l'étudiant: ", 1, 9999);
                            afficher_notes_par_etudiant(notes, id_etudiant);
                            pause_screen();
                            break;
                        }
                        case 3: {
                            int id_matiere = get_int_input("Entrez l'ID de la matière: ", 1, 9999);
                            afficher_notes_par_matiere(notes, id_matiere);
                            pause_screen();
                            break;
                        }
                        case 4:
                            ajouter_note(&notes, -1); // Admin adds note
                            pause_screen();
                            break;
                        case 5:
                            modifier_note(notes, -1); // Admin modifies note
                            pause_screen();
                            break;
                        case 6:
                            supprimer_note(&notes, -1); // Admin deletes note
                            pause_screen();
                            break;
                    }
                } while (sous_choix != 0);
                break;
            }
            case 8: {
                // Gestion des Inscriptions
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Inscriptions ===\n");
                    printf("1. Afficher toutes les inscriptions\n");
                    printf("2. Afficher les inscriptions par étudiant\n");
                    printf("3. Afficher les inscriptions par matière\n");
                    printf("4. Inscrire un étudiant à un cours\n");
                    printf("5. Désinscrire un étudiant d'un cours\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 5);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_inscriptions(inscriptions);
                            pause_screen();
                            break;
                        case 2: {
                            int id_etudiant = get_int_input("Entrez l'ID de l'étudiant: ", 1, 9999);
                            afficher_inscriptions_par_etudiant(inscriptions, id_etudiant);
                            pause_screen();
                            break;
                        }
                        case 3: {
                            int id_matiere = get_int_input("Entrez l'ID de la matière: ", 1, 9999);
                            afficher_inscriptions_par_matiere(inscriptions, id_matiere);
                            pause_screen();
                            break;
                        }
                        case 4: {
                            int id_etudiant = get_int_input("Entrez l'ID de l'étudiant: ", 1, 9999);
                            int id_matiere = get_int_input("Entrez l'ID de la matière: ", 1, 9999);
                            inscrire_etudiant_cours(&inscriptions, id_etudiant, id_matiere);
                            pause_screen();
                            break;
                        }
                        case 5: {
                            int id_etudiant = get_int_input("Entrez l'ID de l'étudiant: ", 1, 9999);
                            int id_matiere = get_int_input("Entrez l'ID de la matière: ", 1, 9999);
                            desinscrire_etudiant_cours(&inscriptions, id_etudiant, id_matiere);
                            pause_screen();
                            break;
                        }
                    }
                } while (sous_choix != 0);
                break;
            }
            case 9: {
                // Rapports et Statistiques
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Rapports et Statistiques ===\n");
                    printf("1. Statistiques des Étudiants\n");
                    printf("2. Statistiques des Matières\n");
                    printf("3. Rapports de Performance\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 3);
                    
                    switch (sous_choix) {
                        case 1: {
                            // Statistiques des Étudiants
                            clear_screen();
                            printf("\n=== Statistiques des Étudiants ===\n");
                            
                            // Count students
                            int nombre_etudiants = 0;
                            NodeEtudiant* etudiant_courant = *etudiants;
                            while (etudiant_courant != NULL) {
                                nombre_etudiants++;
                                etudiant_courant = etudiant_courant->suivant;
                            }
                            
                            printf("Nombre total d'étudiants: %d\n\n", nombre_etudiants);
                            
                            // Print top 5 students by average grade
                            printf("Top 5 des étudiants par moyenne:\n");
                            printf("%-4s | %-30s | %-10s\n", "ID", "Nom", "Moyenne");
                            printf("----------------------------------------------\n");
                            
                            // Create an array to store student IDs and averages
                            int max_students = 100;  // reasonable limit
                            int student_ids[max_students];
                            float student_averages[max_students];
                            int num_students = 0;
                            
                            // Fill the arrays
                            etudiant_courant = *etudiants;
                            while (etudiant_courant != NULL && num_students < max_students) {
                                student_ids[num_students] = etudiant_courant->etudiant.id;
                                student_averages[num_students] = calculer_moyenne_etudiant(notes, etudiant_courant->etudiant.id);
                                num_students++;
                                etudiant_courant = etudiant_courant->suivant;
                            }
                            
                            // Sort students by average (bubble sort for simplicity)
                            for (int i = 0; i < num_students - 1; i++) {
                                for (int j = 0; j < num_students - i - 1; j++) {
                                    if (student_averages[j] < student_averages[j + 1]) {
                                        // Swap averages
                                        float temp_avg = student_averages[j];
                                        student_averages[j] = student_averages[j + 1];
                                        student_averages[j + 1] = temp_avg;
                                        
                                        // Swap IDs
                                        int temp_id = student_ids[j];
                                        student_ids[j] = student_ids[j + 1];
                                        student_ids[j + 1] = temp_id;
                                    }
                                }
                            }
                            
                            // Display top 5 students
                            int top_count = (num_students < 5) ? num_students : 5;
                            for (int i = 0; i < top_count; i++) {
                                NodeEtudiant* etudiant = trouver_etudiant_par_id(*etudiants, student_ids[i]);
                                if (etudiant != NULL) {
                                    printf("%-4d | %-15s %-14s | %-10.2f\n", 
                                           etudiant->etudiant.id,
                                           etudiant->etudiant.prenom,
                                           etudiant->etudiant.nom,
                                           student_averages[i]);
                                }
                            }
                            
                            pause_screen();
                            break;
                        }
                        case 2: {
                            // Statistiques des Matières
                            clear_screen();
                            printf("\n=== Statistiques des Matières ===\n");
                            
                            // Count subjects
                            int nombre_matieres = 0;
                            NodeMatiere* matiere_courant = *matieres;
                            while (matiere_courant != NULL) {
                                nombre_matieres++;
                                matiere_courant = matiere_courant->suivant;
                            }
                            
                            printf("Nombre total de matières: %d\n\n", nombre_matieres);
                            
                            // Print subject averages
                            printf("Moyennes par matière:\n");
                            printf("%-4s | %-20s | %-10s | %-15s\n", "ID", "Nom", "Moyenne", "Nb. étudiants");
                            printf("----------------------------------------------------------\n");
                            
                            matiere_courant = *matieres;
                            while (matiere_courant != NULL) {
                                // Count enrolled students
                                int nb_etudiants = 0;
                                NodeInscription* insc_courant = inscriptions;
                                while (insc_courant != NULL) {
                                    if (insc_courant->inscription.id_matiere == matiere_courant->matiere.id &&
                                        insc_courant->inscription.statut == 1) {
                                        nb_etudiants++;
                                    }
                                    insc_courant = insc_courant->suivant;
                                }
                                
                                // Calculate average
                                float moyenne = calculer_moyenne_matiere(notes, matiere_courant->matiere.id);
                                
                                printf("%-4d | %-20s | %-10.2f | %-15d\n", 
                                       matiere_courant->matiere.id,
                                       matiere_courant->matiere.nom_matiere,
                                       moyenne,
                                       nb_etudiants);
                                
                                matiere_courant = matiere_courant->suivant;
                            }
                            
                            pause_screen();
                            break;
                        }
                        case 3: {
                            // Rapports de Performance
                            clear_screen();
                            printf("\n=== Rapport de Performance ===\n");
                            
                            // Calculate overall average
                            float somme_totale = 0.0f;
                            int nb_notes = 0;
                            
                            NodeNote* note_courant = notes;
                            while (note_courant != NULL) {
                                somme_totale += note_courant->note.note;
                                nb_notes++;
                                note_courant = note_courant->suivant;
                            }
                            
                            float moyenne_generale = (nb_notes > 0) ? (somme_totale / nb_notes) : 0.0f;
                            
                            printf("Moyenne générale de l'établissement: %.2f\n\n", moyenne_generale);
                            
                            // Count students with average above 10
                            int nb_etudiants_reussite = 0;
                            int nb_etudiants_total = 0;
                            
                            NodeEtudiant* etudiant_courant = *etudiants;
                            while (etudiant_courant != NULL) {
                                nb_etudiants_total++;
                                
                                float moyenne_etudiant = calculer_moyenne_etudiant(notes, etudiant_courant->etudiant.id);
                                if (moyenne_etudiant >= 10.0f) {
                                    nb_etudiants_reussite++;
                                }
                                
                                etudiant_courant = etudiant_courant->suivant;
                            }
                            
                            float taux_reussite = (nb_etudiants_total > 0) ? ((float)nb_etudiants_reussite / nb_etudiants_total * 100.0f) : 0.0f;
                            
                            printf("Taux de réussite (moyenne >= 10): %.2f%% (%d sur %d étudiants)\n", 
                                   taux_reussite, nb_etudiants_reussite, nb_etudiants_total);
                            
                            pause_screen();
                            break;
                        }
                    }
                } while (sous_choix != 0);
                break;
            }
        }
    } while (choix != 0);
    
    // Free resources
    liberer_annonces(&annonces);
    liberer_notes(&notes);
    liberer_inscriptions(&inscriptions);
}

/**
 * Display the teacher menu and handle teacher operations
 */
void menu_enseignant(int id_enseignant, NodeEtudiant* etudiants, 
                    NodeEnseignant* enseignants, NodeMatiere* matieres, 
                    EmploiDuTemps *edt) {
    int choix = 0;
    
    // Find the teacher info
    NodeEnseignant* enseignant = trouver_enseignant_par_id(enseignants, id_enseignant);
    if (enseignant == NULL) {
        printf("Erreur: Enseignant avec ID %d non trouvé.\n", id_enseignant);
        pause_screen();
        return;
    }
    
    // Initialize necessary modules
    NodeAnnonce* annonces = NULL;
    charger_annonces(&annonces);
    
    NodeNote* notes = NULL;
    charger_notes(&notes);
    
    do {
        clear_screen();
        printf("\n=== Menu Enseignant ===\n");
        printf("Bienvenue, %s %s\n", enseignant->enseignant.prenom, enseignant->enseignant.nom);
        printf("1. Gestion des Annonces\n");
        printf("2. Gestion des Notes\n");
        printf("3. Suivi des Étudiants\n");
        printf("4. Gestion des Cours\n");
        printf("5. Mon Profil\n");
        printf("6. Consulter l'Emploi du Temps\n");
        printf("0. Déconnexion\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 6);
        
        switch (choix) {
            case 1: {
                // Gestion des Annonces
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Annonces ===\n");
                    printf("1. Consulter toutes les annonces\n");
                    printf("2. Consulter mes annonces\n");
                    printf("3. Créer une nouvelle annonce\n");
                    printf("4. Modifier une annonce\n");
                    printf("5. Supprimer une annonce\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 5);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_annonces(annonces);
                            pause_screen();
                            break;
                        case 2:
                            afficher_annonces_par_enseignant(annonces, id_enseignant);
                            pause_screen();
                            break;
                        case 3:
                            ajouter_annonce(&annonces, id_enseignant);
                            pause_screen();
                            break;
                        case 4:
                            modifier_annonce(annonces, id_enseignant);
                            pause_screen();
                            break;
                        case 5:
                            supprimer_annonce(&annonces, id_enseignant);
                            pause_screen();
                            break;
                    }
                } while (sous_choix != 0);
                break;
            }
                
            case 2: {
                // Gestion des Notes
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Notes ===\n");
                    printf("1. Consulter toutes les notes\n");
                    printf("2. Consulter les notes par étudiant\n");
                    printf("3. Consulter les notes par matière\n");
                    printf("4. Ajouter une note\n");
                    printf("5. Modifier une note\n");
                    printf("6. Supprimer une note\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 6);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_notes(notes);
                            pause_screen();
                            break;
                        case 2: {
                            int id_etudiant = get_int_input("Entrez l'ID de l'étudiant: ", 1, 9999);
                            afficher_notes_par_etudiant(notes, id_etudiant);
                            pause_screen();
                            break;
                        }
                        case 3: {
                            int id_matiere = get_int_input("Entrez l'ID de la matière: ", 1, 9999);
                            afficher_notes_par_matiere(notes, id_matiere);
                            pause_screen();
                            break;
                        }
                        case 4:
                            ajouter_note(&notes, id_enseignant);
                            pause_screen();
                            break;
                        case 5:
                            modifier_note(notes, id_enseignant);
                            pause_screen();
                            break;
                        case 6:
                            supprimer_note(&notes, id_enseignant);
                            pause_screen();
                            break;
                    }
                } while (sous_choix != 0);
                break;
            }
                
            case 3: {
                // Suivi des Étudiants
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Suivi des Étudiants ===\n");
                    printf("1. Afficher tous les étudiants\n");
                    printf("2. Rechercher des étudiants\n");
                    printf("3. Consulter les performances d'un étudiant\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 3);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_etudiants(etudiants);
                            pause_screen();
                            break;
                        case 2:
                            rechercher_etudiants(etudiants);
                            pause_screen();
                            break;
                        case 3: {
                            int id_etudiant = get_int_input("Entrez l'ID de l'étudiant: ", 1, 9999);
                            
                            // Afficher les infos de l'étudiant
                            NodeEtudiant* etudiant = trouver_etudiant_par_id(etudiants, id_etudiant);
                            if (etudiant != NULL) {
                                printf("\n=== Informations de l'Étudiant ===\n");
                                printf("ID: %d\n", etudiant->etudiant.id);
                                printf("Nom: %s %s\n", etudiant->etudiant.prenom, etudiant->etudiant.nom);
                                printf("Email: %s\n", etudiant->etudiant.email);
                                printf("CNE: %s\n", etudiant->etudiant.cne);
                                printf("Section: %s\n", etudiant->etudiant.section);
                                printf("Filière: %s\n\n", etudiant->etudiant.filiere);
                                
                                // Afficher les notes de l'étudiant
                                printf("=== Notes de l'Étudiant ===\n");
                                afficher_notes_par_etudiant(notes, id_etudiant);
                                
                                // Afficher la moyenne de l'étudiant
                                printf("\nMoyenne générale: %.2f\n", calculer_moyenne_etudiant(notes, id_etudiant));
                            } else {
                                printf("Étudiant avec ID %d non trouvé.\n", id_etudiant);
                            }
                            pause_screen();
                            break;
                        }
                    }
                } while (sous_choix != 0);
                break;
            }
                
            case 4: {
                // Gestion des Cours
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Gestion des Cours ===\n");
                    printf("1. Afficher toutes les matières\n");
                    printf("2. Rechercher des matières\n");
                    printf("3. Consulter une matière en détail\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 3);
                    
                    switch (sous_choix) {
                        case 1:
                            afficher_matieres(matieres);
                            pause_screen();
                            break;
                        case 2:
                            rechercher_matieres(matieres);
                            pause_screen();
                            break;
                        case 3: {
                            int id_matiere = get_int_input("Entrez l'ID de la matière: ", 1, 9999);
                            
                            // Afficher les infos de la matière
                            NodeMatiere* matiere = trouver_matiere_par_id(matieres, id_matiere);
                            if (matiere != NULL) {
                                printf("\n=== Informations de la Matière ===\n");
                                printf("ID: %d\n", matiere->matiere.id);
                                printf("Code: %s\n", matiere->matiere.code_matiere);
                                printf("Nom: %s\n", matiere->matiere.nom_matiere);
                                printf("Coefficient: %.2f\n\n", matiere->matiere.coefficient);
                                
                                // Afficher les notes pour cette matière
                                printf("=== Notes pour cette Matière ===\n");
                                afficher_notes_par_matiere(notes, id_matiere);
                            } else {
                                printf("Matière avec ID %d non trouvée.\n", id_matiere);
                            }
                            pause_screen();
                            break;
                        }
                    }
                } while (sous_choix != 0);
                break;
            }
                
            case 5: {
                // Mon Profil
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Mon Profil ===\n");
                    printf("ID: %d\n", enseignant->enseignant.id);
                    printf("Prénom: %s\n", enseignant->enseignant.prenom);
                    printf("Nom: %s\n", enseignant->enseignant.nom);
                    printf("Email: %s\n", enseignant->enseignant.email);
                    printf("Code Enseignant: %s\n", enseignant->enseignant.code_enseignant);
                    printf("Matière Enseignée: %s\n", enseignant->enseignant.matiere_enseignee);
                    
                    printf("\n1. Modifier mon profil\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 1);
                    
                    if (sous_choix == 1) {
                        // Get new values or keep current ones
                        char buffer[50];
                        
                        printf("\n=== Modifier mon profil ===\n");
                        printf("Laissez vide pour garder la valeur actuelle.\n");
                        
                        get_input_formatted("Prénom [actuel: %s]: ", buffer, sizeof(buffer), enseignant->enseignant.prenom);
                        if (strlen(buffer) > 0) {
                            strcpy(enseignant->enseignant.prenom, buffer);
                        }
                        
                        get_input_formatted("Nom [actuel: %s]: ", buffer, sizeof(buffer), enseignant->enseignant.nom);
                        if (strlen(buffer) > 0) {
                            strcpy(enseignant->enseignant.nom, buffer);
                        }
                        
                        // Email can only be changed by admin or auto-generated
                        
                        get_input_formatted("Code Enseignant [actuel: %s]: ", buffer, sizeof(buffer), 
                                 enseignant->enseignant.code_enseignant);
                        if (strlen(buffer) > 0) {
                            strcpy(enseignant->enseignant.code_enseignant, buffer);
                        }
                        
                        // Save changes
                        if (sauvegarder_enseignants(enseignants)) {
                            printf("Profil modifié avec succès.\n");
                        } else {
                            printf("Erreur lors de la sauvegarde des modifications.\n");
                        }
                        
                        pause_screen();
                    }
                } while (sous_choix != 0);
                break;
            }
                
            case 6:
                // Consulter l'Emploi du Temps
                afficher_emploi_du_temps(edt);
                pause_screen();
                break;
        }
    } while (choix != 0);
    
    // Free resources
    liberer_annonces(&annonces);
    liberer_notes(&notes);
}

/**
 * Display the student menu and handle student operations
 */
void menu_etudiant(int id_etudiant, NodeEtudiant* etudiants, 
                  NodeEnseignant* enseignants, NodeMatiere* matieres, 
                  EmploiDuTemps *edt) {
    int choix = 0;
    
    // Find the student info
    NodeEtudiant* etudiant = trouver_etudiant_par_id(etudiants, id_etudiant);
    if (etudiant == NULL) {
        printf("Erreur: Étudiant avec ID %d non trouvé.\n", id_etudiant);
        pause_screen();
        return;
    }
    
    // Initialize necessary modules
    NodeAnnonce* annonces = NULL;
    charger_annonces(&annonces);
    
    NodeNote* notes = NULL;
    charger_notes(&notes);
    
    NodeInscription* inscriptions = NULL;
    charger_inscriptions(&inscriptions);
    
    do {
        clear_screen();
        printf("\n=== Menu Étudiant ===\n");
        printf("Bienvenue, %s %s\n", etudiant->etudiant.prenom, etudiant->etudiant.nom);
        printf("1. Consulter les Annonces\n");
        printf("2. Consulter mes Notes\n");
        printf("3. Gestion des Inscriptions aux Cours\n");
        printf("4. Mon Profil\n");
        printf("5. Consulter les Enseignants\n");
        printf("6. Consulter les Matières\n");
        printf("7. Consulter l'Emploi du Temps\n");
        printf("0. Déconnexion\n");
        
        choix = get_int_input("Choisissez une option: ", 0, 7);
        
        switch (choix) {
            case 1:
                // Consulter les Annonces
                clear_screen();
                printf("\n=== Annonces ===\n");
                afficher_annonces(annonces);
                pause_screen();
                break;
                
            case 2:
                // Consulter mes Notes
                clear_screen();
                printf("\n=== Mes Notes ===\n");
                afficher_notes_par_etudiant(notes, id_etudiant);
                pause_screen();
                break;
                
            case 3:
                // Gestion des Inscriptions aux Cours
                menu_inscriptions_etudiant(&inscriptions, id_etudiant);
                break;
                
            case 4: {
                // Mon Profil
                int sous_choix = 0;
                do {
                    clear_screen();
                    printf("\n=== Mon Profil ===\n");
                    printf("ID: %d\n", etudiant->etudiant.id);
                    printf("Prénom: %s\n", etudiant->etudiant.prenom);
                    printf("Nom: %s\n", etudiant->etudiant.nom);
                    printf("Email: %s\n", etudiant->etudiant.email);
                    printf("CNE: %s\n", etudiant->etudiant.cne);
                    printf("Section: %s\n", etudiant->etudiant.section);
                    printf("Filière: %s\n", etudiant->etudiant.filiere);
                    
                    printf("\n1. Modifier mon profil\n");
                    printf("0. Retour\n");
                    
                    sous_choix = get_int_input("Choisissez une option: ", 0, 1);
                    
                    if (sous_choix == 1) {
                        // Get new values or keep current ones
                        char buffer[50];
                        
                        printf("\n=== Modifier mon profil ===\n");
                        printf("Laissez vide pour garder la valeur actuelle.\n");
                        
                        get_input_formatted("Prénom [actuel: %s]: ", buffer, sizeof(buffer), etudiant->etudiant.prenom);
                        if (strlen(buffer) > 0) {
                            strcpy(etudiant->etudiant.prenom, buffer);
                        }
                        
                        get_input_formatted("Nom [actuel: %s]: ", buffer, sizeof(buffer), etudiant->etudiant.nom);
                        if (strlen(buffer) > 0) {
                            strcpy(etudiant->etudiant.nom, buffer);
                        }
                        
                        // Email can only be changed by admin or auto-generated
                        
                        get_input_formatted("CNE [actuel: %s]: ", buffer, sizeof(buffer), etudiant->etudiant.cne);
                        if (strlen(buffer) > 0) {
                            strcpy(etudiant->etudiant.cne, buffer);
                        }
                        
                        // Save changes
                        if (sauvegarder_etudiants(etudiants)) {
                            printf("Profil modifié avec succès.\n");
                        } else {
                            printf("Erreur lors de la sauvegarde des modifications.\n");
                        }
                        
                        pause_screen();
                    }
                } while (sous_choix != 0);
                break;
            }
                
            case 5: {
                // Consulter les Enseignants
                int option_enseignant = get_int_input("1. Afficher tous les enseignants\n2. Rechercher des enseignants\nChoisissez une option: ", 1, 2);
                if (option_enseignant == 1) {
                    afficher_enseignants(enseignants);
                } else {
                    rechercher_enseignants(enseignants);
                }
                pause_screen();
                break;
            }
                
            case 6: {
                // Consulter les Matières
                int option_matiere = get_int_input("1. Afficher toutes les matières\n2. Rechercher des matières\nChoisissez une option: ", 1, 2);
                if (option_matiere == 1) {
                    afficher_matieres(matieres);
                } else {
                    rechercher_matieres(matieres);
                }
                pause_screen();
                break;
            }
                
            case 7:
                // Consulter l'Emploi du Temps
                afficher_emploi_du_temps(edt);
                pause_screen();
                break;
        }
    } while (choix != 0);
    
    // Free resources
    liberer_annonces(&annonces);
    liberer_notes(&notes);
    liberer_inscriptions(&inscriptions);
}
