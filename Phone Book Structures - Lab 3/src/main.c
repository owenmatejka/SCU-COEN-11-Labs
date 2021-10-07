/*
Lab #3
COEN 11 SCU
Programmer: Owen Matejka
Instructor: Yi Fang
Date: 7 October 2021
About: A simple phonebook where you can view, create, and delete contacts. This is a modified version of lab #2 using structures and pointers.
*/


// Directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Configuration
#define MAX_NAMES 20
#define MAX_CHARS 40
#define MAX_NUMBERLEN 15

// Method Declarations
void displayContacts();
int checkIfDuplicate(char Name[]);
void insertContact();
void deleteContact();

// Structures
typedef struct Contact {
    char Name[MAX_CHARS + 1];
    char Number[MAX_NUMBERLEN + 1];
} Contact;

// Data
Contact Contacts[MAX_NAMES] = {};
int Counter = 0;

// Main Method
int main() {
    printf("Welcome to the phone book!\n");

    int input;

    while (1) { // This loop repeats indefinitely unless the user enters 4
        printf("The following commands are avaliable:\n");
        printf("\
        1: Create contact\n\
        2: Delete contact\n\
        3: View all contacts\n\
        4: Quit\n"
        );

        scanf("%d", &input);
        printf("\n");

        switch(input) {
            case 1 :
                insertContact();
                break;
            case 2 :
                deleteContact();
                break;
            case 3 :
                displayContacts();
                break;
            case 4 :
                printf("Quitting . . .");
                break;
            default :
                printf("Invalid option. Please try again!");
        }

        if (input == 4) {
            break;
        }
    }
    
}

// Displays all contacts
void displayContacts() {
    int i; Contact *c = Contacts;
    for (i = 0; i < MAX_NAMES; i++) {
        if (strcmp((c + i) -> Name, "") != 0) { // Checks to see if the name field is empty so it won't display allocated, but empty rows
            printf("Name: %s, Number: %s\n", (c+i) -> Name, (c+i) -> Number);
        }
    }
    if (Counter == 0) {
        printf("There are no contacts to display.\n");
    }
    printf("\n");
}

// Checks for duplicate contacts
int checkIfDuplicate(char Name[]) {
    int i; Contact *c = Contacts;
    for (i = 0; i < MAX_NAMES; i++) {
        if (strcmp(Name, (c + i) -> Name) == 0) { // if the names match, it returns the index
            return i;
        }
    }

    return -1;
}

// Inserts a new contact
void insertContact() {
    if (Counter == MAX_NAMES) {
        printf("The phone book is full! Please delete a contact before trying to create a new one.\n\n");
        return;
    }

    char Name[MAX_CHARS + 1]; // Create a temp array so we can check the input against the array of names before adding
    char Number[MAX_CHARS + 1];

    printf("Please enter the name. **NOTE: Only enter your first name!**\n");
    scanf("%s", Name);

    if (checkIfDuplicate(Name) != -1) {
        printf("A contact with this name already exists! Please try again.\n\n");
        return;
    } else if (strlen(Name) > MAX_CHARS) {
        printf("The maximum number of allowed characters is %d! Please try again.\n\n", MAX_CHARS);
        return;
    }

    printf("Please enter a phone number to associate with this contact.\n");
    scanf("%s", Number);

    if (strlen(Number) > MAX_NUMBERLEN) {
        printf("The maximum number of allowed numbers/characters is %d! Please try again.\n\n", MAX_NUMBERLEN);
        return;
    }

    strcpy(Contacts[Counter].Name, Name); // Copies data to the array
    strcpy(Contacts[Counter].Number, Number);

    Counter += 1; // Increases counter for next contact slot to be used

    printf("Contact successfully created!\n\n");
}

// Deletes contact by supplied name
void deleteContact(){
    char Name[MAX_CHARS + 1];

    printf("Please enter the name to delete.\n");
    scanf("%s", Name);

    int indexToDelete = checkIfDuplicate(Name);

    if (indexToDelete == -1) {
        printf("That contact name does not exist in the phone book. Please try again!\n\n");
        return;
    }

    strcpy(Contacts[indexToDelete].Name, Contacts[Counter].Name); // Copies the newest contact to the deleted slot
    strcpy(Contacts[indexToDelete].Number, Contacts[Counter].Number);

    strcpy(Contacts[Counter].Name, ""); // Inserts blank data to the newest contact slot for reuse
    strcpy(Contacts[Counter].Number, "");

    printf("Contact successfully deleted!\n\n");

    Counter -= 1; // Lowers counter so the previous contact slot can be reused
}