/**
 * @file utils.c
 * @brief Implementation of utility functions
 */

#include "../include/utils.h"
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define ETUDIANTS_FILE "data/etudiants.txt"
#define ENSEIGNANTS_FILE "data/enseignants.txt"

/**
 * Clear the console screen
 */
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/**
 * Pause execution until user presses Enter
 */
void pause_screen() {
    printf("\nAppuyez sur Entrée pour continuer...");
    getchar(); // Consume newline from previous input
    getchar(); // Wait for Enter
}

/**
 * Get a string input from the user
 */
void get_input(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    
    // Remove trailing newline
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        // Clear input buffer if input was too long
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

/**
 * Get a formatted string input from the user
 */
void get_input_formatted(const char *prompt, char *buffer, size_t size, ...) {
    char formatted_prompt[256];
    va_list args;
    
    // Format the prompt with variable arguments
    va_start(args, size);
    vsnprintf(formatted_prompt, sizeof(formatted_prompt), prompt, args);
    va_end(args);
    
    // Use the basic get_input with the formatted prompt
    get_input(formatted_prompt, buffer, size);
}

/**
 * Get an integer input from the user
 */
int get_int_input(const char *prompt, int min, int max) {
    int value;
    char buffer[20];
    
    while (1) {
        printf("%s", prompt);
        fgets(buffer, sizeof(buffer), stdin);
        
        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            // Clear input buffer if input was too long
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        
        // Check for empty input
        if (strlen(buffer) == 0) {
            printf("Veuillez entrer une valeur.\n");
            continue;
        }
        
        // Check if the input is a valid integer
        char *endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (*endptr != '\0') {
            printf("Veuillez entrer un nombre entier valide.\n");
            continue;
        }
        
        // Check if the value is in range
        if (value < min || value > max) {
            printf("Veuillez entrer un nombre entre %d et %d.\n", min, max);
            continue;
        }
        
        break;
    }
    
    return value;
}

/**
 * Get a float input from the user
 */
float get_float_input(const char *prompt, float min, float max) {
    float value;
    char buffer[20];
    
    while (1) {
        printf("%s", prompt);
        fgets(buffer, sizeof(buffer), stdin);
        
        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            // Clear input buffer if input was too long
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        
        // Check for empty input
        if (strlen(buffer) == 0) {
            printf("Veuillez entrer une valeur.\n");
            continue;
        }
        
        // Check if the input is a valid float
        char *endptr;
        value = strtof(buffer, &endptr);
        
        if (*endptr != '\0') {
            printf("Veuillez entrer un nombre décimal valide.\n");
            continue;
        }
        
        // Check if the value is in range
        if (value < min || value > max) {
            printf("Veuillez entrer un nombre entre %.2f et %.2f.\n", min, max);
            continue;
        }
        
        break;
    }
    
    return value;
}

/**
 * Check if a file exists
 */
int file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

/**
 * Create an empty file if it doesn't exist
 */
int create_file_if_not_exists(const char *filename) {
    if (file_exists(filename)) {
        return 1;
    }
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return 0;
    }
    
    fclose(file);
    return 1;
}



/**
 * Validate an email address format
 */
int validate_email(const char *email) {
    // Very basic email validation
    // Check for @ and at least one . after @
    const char *at_sign = strchr(email, '@');
    if (at_sign == NULL) {
        return 0;
    }
    
    // Check if there's at least one dot after @
    const char *dot = strchr(at_sign, '.');
    if (dot == NULL) {
        return 0;
    }
    
    // Check if there are at least 2 characters after the last dot
    if (strlen(dot) < 3) {
        return 0;
    }
    
    return 1;
}

/**
 * Create data directory if it doesn't exist
 */
int create_data_dir() {
    struct stat st = {0};
    
    if (stat("data", &st) == -1) {
        #ifdef _WIN32
            return (mkdir("data") == 0);
        #else
            return (mkdir("data", 0700) == 0);
        #endif
    }
    
    return 1;
}

/**
 * Case-insensitive substring search (like strcasestr)
 */
char* str_case_search(const char* haystack, const char* needle) {
    if (!haystack || !needle) {
        return NULL;
    }
    
    // Empty needle matches everything
    if (*needle == '\0') {
        return (char*)haystack;
    }
    
    size_t needle_len = strlen(needle);
    size_t haystack_len = strlen(haystack);
    
    // If needle is longer than haystack, it can't be a substring
    if (needle_len > haystack_len) {
        return NULL;
    }
    
    // Loop through each position in haystack
    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        // Check if this position matches the needle (case-insensitive)
        size_t j;
        for (j = 0; j < needle_len; j++) {
            char h = tolower((unsigned char)haystack[i + j]);
            char n = tolower((unsigned char)needle[j]);
            
            if (h != n) {
                break;  // No match, try next position
            }
        }
        
        // If we went through the entire needle, we found a match
        if (j == needle_len) {
            return (char*)(haystack + i);
        }
    }
    
    return NULL;  // No match found
}

/**
 * Check if an email already exists in the system
 */
int email_existe_deja(const char* email) {
    // The implementation will check if the email exists in both students and teachers files
    FILE *file;
    char line[256];
    char line_copy[256];
    
    // Check in students file
    file = fopen(ETUDIANTS_FILE, "r");
    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            // Make a copy of the line because strtok modifies the string
            strcpy(line_copy, line);
            
            // Parse the line (format: id|prenom|nom|email|cne|section|filiere)
            char *token = strtok(line_copy, "|");  // id
            if (!token) continue;
            
            token = strtok(NULL, "|");           // prenom
            if (!token) continue;
            
            token = strtok(NULL, "|");           // nom
            if (!token) continue;
            
            token = strtok(NULL, "|");           // email
            if (token && strcmp(token, email) == 0) {
                fclose(file);
                return 1; // Email exists
            }
        }
        fclose(file);
    }
    
    // Check in teachers file
    file = fopen(ENSEIGNANTS_FILE, "r");
    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            // Make a copy of the line because strtok modifies the string
            strcpy(line_copy, line);
            
            // Parse the line (format: id|prenom|nom|email|code_enseignant|matiere_enseignee)
            char *token = strtok(line_copy, "|");  // id
            if (!token) continue;
            
            token = strtok(NULL, "|");           // prenom
            if (!token) continue;
            
            token = strtok(NULL, "|");           // nom
            if (!token) continue;
            
            token = strtok(NULL, "|");           // email
            if (token && strcmp(token, email) == 0) {
                fclose(file);
                return 1; // Email exists
            }
        }
        fclose(file);
    }
    
    return 0; // Email does not exist
}

/**
 * Generate a unique email address based on first name and last name
 */
void generer_email_unique(const char* prenom, const char* nom, int est_etudiant,
                         char* email_buffer, size_t size) {
    // Make sure the buffer is clean
    memset(email_buffer, 0, size);
    
    // Convert first name and last name to lowercase
    char prenom_lower[50] = {0};
    char nom_lower[50] = {0};
    size_t prenom_len = strlen(prenom);
    size_t nom_len = strlen(nom);
    
    for (size_t i = 0; i < prenom_len && i < sizeof(prenom_lower) - 1; i++) {
        prenom_lower[i] = tolower((unsigned char)prenom[i]);
    }
    
    for (size_t i = 0; i < nom_len && i < sizeof(nom_lower) - 1; i++) {
        nom_lower[i] = tolower((unsigned char)nom[i]);
    }
    
    // Determine the domain based on person type
    const char* domain = est_etudiant ? "@edu.umi.ac.ma" : "@umi.ac.ma";
    
    // Try to generate email without counter first
    snprintf(email_buffer, size, "%c.%s%s", prenom_lower[0], nom_lower, domain);
    
    // If email already exists add a counter
    int counter = 1;
    char temp_email[100];
    strncpy(temp_email, email_buffer, sizeof(temp_email) - 1);
    
    while (email_existe_deja(temp_email)) {
        // Generate a new email with counter
        snprintf(temp_email, sizeof(temp_email), "%c.%s%d%s", 
                 prenom_lower[0], nom_lower, counter, domain);
        counter++;
    }
    
    // Copy the final unique email to the buffer
    strncpy(email_buffer, temp_email, size - 1);
    email_buffer[size - 1] = '\0'; // Ensure null termination
}



int charger_derniere_id() {
    FILE *file = fopen("data/enseignants.txt", "r");
    if (file == NULL) {
        return 0; // Retourne 0 si le fichier n'existe pas ou ne peut pas être ouvert
    }

    int last_id = 0;
    char line[256];

    // Lire chaque ligne pour trouver le dernier ID
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d|", &id); // Extraire l'ID de la ligne
        if (id > last_id) {
            last_id = id; // Mettre à jour le dernier ID
        }
    }

    fclose(file);
    return last_id;
}


int generate_id() {
    static int last_id = 0; // Initialisé à 0

    // Charger le dernier ID à partir du fichier si ce n'est pas déjà fait
    if (last_id == 0) {
        last_id = charger_derniere_id(); // Charger le dernier ID
    }

    last_id++; // Incrémente l'ID
    return last_id; // Retourne le nouvel ID
}