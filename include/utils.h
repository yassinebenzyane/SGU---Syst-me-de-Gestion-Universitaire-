/**
 * @file utils.h
 * @brief Utility functions for the school management system
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Clear the console screen
 */
void clear_screen();

/**
 * @brief Pause execution until user presses Enter
 */
void pause_screen();

/**
 * @brief Get a string input from the user
 * @param prompt Message to display to the user
 * @param buffer Buffer to store the input
 * @param size Size of the buffer
 */
void get_input(const char *prompt, char *buffer, size_t size);

/**
 * @brief Get a formatted string input from the user
 * @param prompt Message to display to the user (can contain format specifiers)
 * @param buffer Buffer to store the input
 * @param size Size of the buffer
 * @param ... Additional arguments for format specifiers
 */
void get_input_formatted(const char *prompt, char *buffer, size_t size, ...);

/**
 * @brief Get an integer input from the user
 * @param prompt Message to display to the user
 * @param min Minimum valid value
 * @param max Maximum valid value
 * @return The input integer
 */
int get_int_input(const char *prompt, int min, int max);

/**
 * @brief Get a float input from the user
 * @param prompt Message to display to the user
 * @param min Minimum valid value
 * @param max Maximum valid value
 * @return The input float
 */
float get_float_input(const char *prompt, float min, float max);

/**
 * @brief Check if a file exists
 * @param filename Name of the file to check
 * @return 1 if exists, 0 if not
 */
int file_exists(const char *filename);

/**
 * @brief Create an empty file if it doesn't exist
 * @param filename Name of the file to create
 * @return 1 if successful, 0 if error
 */
int create_file_if_not_exists(const char *filename);

/**
 * @brief Generate a random ID
 * @return A random ID
 */
int generate_id();

/**
 * @brief Validate an email address format
 * @param email Email to validate
 * @return 1 if valid, 0 if invalid
 */
int validate_email(const char *email);

/**
 * @brief Create data directory if it doesn't exist
 * @return 1 if successful, 0 if error
 */
int create_data_dir();

/**
 * @brief Case-insensitive substring search (like strcasestr)
 * @param haystack String to be searched
 * @param needle String to search for
 * @return Pointer to the beginning of the substring, or NULL if not found
 */
char* str_case_search(const char* haystack, const char* needle);

#endif /* UTILS_H */
