/*
Lab #6
COEN 11 SCU
Programmer: Owen Matejka
Instructor: Yi Fang
Date: 28 October 2021
About: A simple phonebook where you can view, create, and delete contacts. This is a modified version of lab #5 using an array of linked lists and grouping of names by alphabetical order.
*/

// Directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Configuration
#define MAX_CHARS 40
#define MAX_NUMBERLEN 15

// Method Declarations
int getIndexByLetter(char Letter);
void displayContacts();
void displayContactsByLetter();
void insertContact();
void deleteContact();

// Structures
typedef struct Contact {
    char Name[MAX_CHARS + 1];
    char Number[MAX_NUMBERLEN + 1];
    struct Contact *next;
} Contact;

// Data
Contact *headNodes[26];

// Main Method
int main() {
    printf("Welcome to the phone book!\n");
    
    int input;

    while (1) { // This loop repeats indefinitely unless the user enters 5
        printf("The following commands are avaliable:\n");
        printf("\
        1: Create contact\n\
        2: Delete contact\n\
        3: View all contacts\n\
        4: View contacts by alphabetical group\n\
        5: Quit\n"
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
                displayContactsByLetter();
                break;
            case 5 :
                printf("Quitting . . .");
                break;
            default :
                printf("Invalid option. Please try again!");
        }

        if (input == 5) {
            break;
        }
    }
    
}

// Converts a letter to the index number for the array
int getIndexByLetter(char Letter) {
    int num;

    if (!isalpha(Letter)) return -1; // If it isn't a valid letter, we return -1

    Letter = toupper(Letter); // ASCII 65-91 are all uppercase

    num = Letter - 65; // Converts the letter to an index number to easily fetch from array
    if (num <= 26) {
        return num;
    } else {
        return -1; // Something went wrong so we return -1
    }
}

// Displays all contacts
void displayContacts() {
    int i, tDisplayed = 0;
    for (i = 0; i < 26; i++) {
        Contact *c = headNodes[i];
        while (c != NULL) {
            printf("Name: %s, Number: %s\n", c -> Name, c -> Number);
            c = c -> next;
            tDisplayed++;
        }
    }

    if (tDisplayed == 0) {
        printf("There are no contacts to display.\n");
    }

    printf("\n");
}

void displayContactsByLetter() {
    char input[100];

    printf("Please enter the letter group to view.\n");
    scanf("%s", input);
    printf("\n");

    int indexNum = getIndexByLetter(input[0]);
    if (indexNum == -1) {
        printf("This name does not contain a valid first character. Please try again! \n\n");
        return;
    }

    Contact *c = headNodes[indexNum];
    while (c != NULL) {
        printf("Name: %s, Number: %s\n", c -> Name, c -> Number);
        c = c -> next;
    }

    if (headNodes[indexNum] == NULL) {
        printf("There are no contacts to display in this letter group.\n");
    }

    printf("\n");
}

// Inserts a new contact
void insertContact() {

    char Name[MAX_CHARS + 1]; // Create a temp array so we can check the input against the array of names before adding
    char Number[MAX_CHARS + 1];

    printf("Please enter the name. **NOTE: Only enter your first name!**\n");
    scanf("%s", Name);

    int indexNum = getIndexByLetter(Name[0]);
    if (indexNum == -1) {
        printf("This name does not contain a valid first character. Please try again! \n");
        return;
    }

    Contact *properSlot = headNodes[indexNum];
    while (properSlot != NULL) {
        int result = strcmp(Name, properSlot -> Name);
        if (result == 0) { // if the names match, it returns the index
            properSlot = NULL;
            break;
        } else if (result > 0 && (properSlot -> next == NULL || strcmp(Name, properSlot -> next -> Name) < 0)) {
            break;
        } else if (result < 0 && properSlot == headNodes[indexNum]) {
            break;
        }

        properSlot = properSlot -> next;
    }
    
    if (properSlot == NULL && headNodes[indexNum] != NULL) {
        printf("A contact with this name already exists! Please try again.\n\n");
        return;
    } else if (strlen(Name) > MAX_CHARS) {
        printf("The maximum number of allowed characters is %d! Please try again.\n\n", MAX_CHARS);
        return;
    }

    printf("Please enter a phone number to associate with this contact.\n");
    scanf("%s", Number);
    printf("\n");

    if (strlen(Number) > MAX_NUMBERLEN) {
        printf("The maximum number of allowed numbers/characters is %d! Please try again.\n\n", MAX_NUMBERLEN);
        return;
    }

    Contact *New = NULL;
    New = (Contact *) malloc(sizeof(Contact)); // Allocates memory for new contact

    strcpy(New -> Name, Name); // Copies data to the Contact
    strcpy(New -> Number, Number);
    New -> next = NULL; // Need to indicate that next is null or the while loops will attempt to iterate over an empty object and crash

    if (headNodes[indexNum] == NULL) {
        headNodes[indexNum] = New;
    } else if (headNodes[indexNum] != NULL && properSlot == headNodes[indexNum] && strcmp(Name, properSlot -> Name) < 0) { // Checks to see if head needs to change
        Contact *temp = properSlot;
        headNodes[indexNum] = New;
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
    printf("\n");

    int indexNum = getIndexByLetter(Name[0]);

    Contact *toDelete = headNodes[indexNum];
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
    } else if (toDelete == headNodes[indexNum]) {
        if (headNodes[indexNum] -> next != NULL) {
            headNodes[indexNum] = headNodes[indexNum] -> next;
            free(toDelete);
        } else {
            free(toDelete);
            headNodes[indexNum] = NULL;
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