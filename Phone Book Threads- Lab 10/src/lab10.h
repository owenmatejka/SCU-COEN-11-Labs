/*
lab10.h
Owen Matejka

This file contains all directives, declarations, and struct + array of head nodes for the program. Additionally, the configuration settings are below.
*/

#ifndef LAB10_H
#define LAB10_H

// Directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>

// Configuration
#define MAX_CHARS 40
#define MAX_NUMBERLEN 15
#define HEADER "Names\tNumbers\n----------------------------------\n"
#define AUTOSAVE_INTERVAL 5
#define AUTOSAVE_FILENAME "auto-save"

// Method Declarations
int getIndexByLetter(char Letter);
void displayContacts();
void displayContactsByLetter();
void displayContactsByLetterInReverse();
void displayContactsInReverse();

int insertContact(char *Name, char *Number);
void manualInsert();
void deleteContact();
void reverseAll();

void save_file(char *filename);
void read_file(char *filename);

void write_binary(char *binaryfile);
void read_binary(char *binaryfile);
void *autosave(void *arg);

// Structures
typedef struct Contact {
    char Name[MAX_CHARS + 1];
    char Number[MAX_NUMBERLEN + 1];
    struct Contact *next;
    struct Contact *prev;
} Contact;

// Data
Contact *headNodes[26];

pthread_mutex_t autosave_mutex;

#endif