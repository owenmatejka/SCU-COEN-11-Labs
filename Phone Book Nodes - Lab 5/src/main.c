/*
Lab #5
COEN 11 SCU
Programmer: Owen Matejka
Instructor: Yi Fang
Date: 21 October 2021
About: A simple phonebook where you can view, create, and delete contacts. This is a modified version of lab #3 using linked lists and sorting of names by alphabetical order.
*/

// Directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Configuration
#define MAX_CHARS 40
#define MAX_NUMBERLEN 15

// Method Declarations
void displayContacts();
void insertContact();
void deleteContact();

// Structures
typedef struct Contact {
    char Name[MAX_CHARS + 1];
    char Number[MAX_NUMBERLEN + 1];
    struct Contact *next;
} Contact;

// Data
Contact *headLL = NULL;

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
    Contact *c = headLL;
    while (c != NULL) {
        printf("Name: %s, Number: %s\n", c -> Name, c -> Number);
        c = c -> next;
    }
    
    if (headLL == NULL) {
        printf("There are no contacts to display.\n");
    }

    printf("\n");
}

// Inserts a new contact
void insertContact() {

    char Name[MAX_CHARS + 1]; // Create a temp array so we can check the input against the array of names before adding
    char Number[MAX_CHARS + 1];

    printf("Please enter the name. **NOTE: Only enter your first name!**\n");
    scanf("%s", Name);

    Contact *properSlot = headLL;
    while (properSlot != NULL) {
        int result = strcmp(Name, properSlot -> Name);
        if (result == 0) { // if the names match, it returns the index
            properSlot = NULL;
            break;
        } else if (result > 0 && (properSlot -> next == NULL || strcmp(Name, properSlot -> next -> Name) < 0)) {
            break;
        } else if (result < 0 && properSlot == headLL) {
            break;
        }

        properSlot = properSlot -> next;
    }
    
    if (properSlot == NULL && headLL != NULL) {
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

    Contact *New = NULL;
    New = (Contact *) malloc(sizeof(Contact)); // Allocates memory for new contact

    strcpy(New -> Name, Name); // Copies data to the Contact
    strcpy(New -> Number, Number);
    New -> next = NULL; // Need to indicate that next is null or the while loops will attempt to iterate over an empty object and crash

    if (headLL == NULL) {
        headLL = New;
    } else if (headLL != NULL && properSlot == headLL && strcmp(Name, properSlot -> Name) < 0) { // Checks to see if head needs to change
        Contact *temp = properSlot;
        headLL = New;
        New -> next = temp;
    } else {
        if (properSlot -> next != NULL) {
            Contact *temp = properSlot -> next;
            properSlot -> next = New;
            New -> next = temp;
        } else {
            properSlot -> next = New;
        }
    }

    printf("Contact successfully created!\n\n");
}

// Deletes contact by supplied name
void deleteContact(){
    char Name[MAX_CHARS + 1];

    printf("Please enter the name to delete.\n");
    scanf("%s", Name);

    Contact *toDelete = headLL;
    Contact *prevContact;

    while (toDelete != NULL) {
        int result = strcmp(Name, toDelete -> Name);
        if (result == 0) {
            break;
        }

        prevContact = toDelete;
        toDelete = toDelete -> next;
    }

    if (toDelete == NULL) {
        printf("That contact name does not exist in the phone book. Please try again!\n\n");
        return;
    } else if (toDelete == headLL) {
        if (headLL -> next != NULL) {
            headLL = headLL -> next;
            free(toDelete);
        } else {
            free(toDelete);
            headLL = NULL;
        }
    } else if (toDelete -> next != NULL) {
        prevContact -> next = toDelete -> next;
        free(toDelete);
    } else {
        prevContact -> next = NULL;
        free(toDelete);
    }

    printf("Contact successfully deleted!\n\n");
}