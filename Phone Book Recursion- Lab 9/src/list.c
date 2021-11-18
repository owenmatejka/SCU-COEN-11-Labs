/*
list.c
Owen Matejka

This file contains all the functions for modifying contact data.
*/

// Main Directive
#include "lab9.h"

// Local Function Declarations
Contact *Reverse(Contact *c);

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

void displayContactsInReverse() {
    int i, tDisplayed = 0;
    for (i = 25; i >= 0; i--) {
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

// Displays contacts by letter group
void displayContactsByLetter() {
    char input[MAX_CHARS + 1];

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

// Displays contacts by letter group
void displayContactsByLetterInReverse() {
    char input[MAX_CHARS + 1];

    printf("Please enter the letter group to view.\n");
    scanf("%s", input);
    printf("\n");

    int indexNum = getIndexByLetter(input[0]);
    if (indexNum == -1) {
        printf("This name does not contain a valid first character. Please try again! \n\n");
        return;
    }

    headNodes[indexNum] = Reverse(headNodes[indexNum]);
    Contact *c = headNodes[indexNum];
    while (c != NULL) {
        printf("Name: %s, Number: %s\n", c -> Name, c -> Number);
        c = c -> next;
    }

    headNodes[indexNum] = Reverse(headNodes[indexNum]);

    if (headNodes[indexNum] == NULL) {
        printf("There are no contacts to display in this letter group.\n");
    }
    
    printf("\n");
}

// Reverse a specific double linked list
Contact *Reverse(Contact *c) {
    if (c == NULL) {
        return NULL;
    }

    Contact *temp = c -> next; // Create temp pointer before swapping two nodes
    c -> next = c -> prev;
    c -> prev = temp;

    if (c -> prev == NULL) {
        return c;
    }

    return Reverse(c -> prev); // Traverse backwards
}

// Reverse every alphabetical group
void reverseAll() {
    int i;

    for (i = 0; i < 26; i++) {
        headNodes[i] = Reverse(headNodes[i]);
    }
}

// Inserts a new contact
int insertContact(char *Name, char *Number) { // Made this return an int so I could pass error codes

    int indexNum = getIndexByLetter(Name[0]);
    if (indexNum == -1) {
        return -1;
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
        return 0;
    } else if (strlen(Name) > MAX_CHARS) {
        return -2;
    }

    if (strlen(Number) > MAX_NUMBERLEN) {
        return -3;
    }

    Contact *New = NULL;
    New = (Contact *) malloc(sizeof(Contact)); // Allocates memory for new contact

    strcpy(New -> Name, Name); // Copies data to the Contact
    strcpy(New -> Number, Number);
    New -> next = NULL; // Need to indicate that next is null or the while loops will attempt to iterate over an empty object and crash
    New -> prev = NULL;

    if (headNodes[indexNum] == NULL) {
        headNodes[indexNum] = New;
    } else if (headNodes[indexNum] != NULL && properSlot == headNodes[indexNum] && strcmp(Name, properSlot -> Name) < 0) { // Checks to see if head needs to change
        Contact *temp = properSlot;
        headNodes[indexNum] = New;
        New -> next = temp;
        temp -> prev = New;
    } else {
        if (properSlot -> next != NULL) {
            Contact *temp = properSlot -> next;
            properSlot -> next = New;
            New -> next = temp;
            New -> prev = properSlot;
            temp -> prev = New;
        } else {
            properSlot -> next = New;
            New -> prev = properSlot;
        }
    }

    return 1;
}

// Manually insert a contact while program is running; this is not a part of main to keep the switch statement clean
void manualInsert(){
    char Name[MAX_CHARS + 1]; // Create a temp array so we can check the input against the array of names before adding
    char Number[MAX_NUMBERLEN + 1];

    printf("Please enter the name. **NOTE: Only enter your first name!**\n");
    scanf("%s", Name);

    printf("Please enter a phone number to associate with this contact.\n");
    scanf("%s", Number);
    printf("\n");

    int res = insertContact(Name, Number);

    if (res == -1){
        printf("This name does not contain a valid first character. Please try again! \n\n");
    } else if (res == 0) {
        printf("A contact with this name already exists! Please try again.\n\n");
    } else if (res == -2) {
        printf("The maximum number of allowed characters is %d! Please try again.\n\n", MAX_CHARS);
    } else if (res == -3) {
        printf("The maximum number of allowed numbers/characters is %d! Please try again.\n\n", MAX_NUMBERLEN);
    } else {
        printf("Contact successfully created!\n\n");
    }
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