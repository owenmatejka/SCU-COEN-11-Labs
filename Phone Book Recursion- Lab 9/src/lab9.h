/*
lab9.h
Owen Matejka

This file contains all directives, declarations, and struct + array of head nodes for the program. Additionally, the configuration settings are below.
*/

#ifndef LAB9_H
#define LAB9_H

// Directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Configuration
#define MAX_CHARS 40
#define MAX_NUMBERLEN 15
#define HEADER "Names\tNumbers\n----------------------------------\n"

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

// Structures
typedef struct Contact {
    char Name[MAX_CHARS + 1];
    char Number[MAX_NUMBERLEN + 1];
    struct Contact *next;
    struct Contact *prev;
} Contact;

// Data
Contact *headNodes[26];

#endif