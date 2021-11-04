/*
Lab #7
COEN 11 SCU
Programmer: Owen Matejka
Instructor: Yi Fang
Date: 4 November 2021
About: A simple phonebook where you can view, create, and delete contacts. This is a modified version of lab #6 using file i/o to allow the importanting and persistant date storage of contacts
*/

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
int insertContact(char *Name, char *Number);
void manualInsert();
void deleteContact();
void save_file(char *filename);
void read_file(char *filename);

// Structures
typedef struct Contact {
    char Name[MAX_CHARS + 1];
    char Number[MAX_NUMBERLEN + 1];
    struct Contact *next;
} Contact;

// Data
Contact *headNodes[26];

// Main Method
int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("No file name was supplied. Please rerun the program.");
        return 0;
    }

    read_file(argv[1]); // Reads and imports file if it exists

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
                manualInsert();
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
                save_file(argv[1]); // Save file before quitting
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

// Saves all contacts to specified file
void save_file(char *filename) {
    FILE *outfp;
    Contact *c;
    int i;

    outfp = fopen(filename, "w");

    if (outfp == NULL) {
        printf("Error opening file. Aborting save operation....");
        return;
    }

    fprintf(outfp, HEADER);

    for (i = 0; i < 26; i++) {
		c = headNodes[i];
		while(c != NULL) {
			fprintf(outfp, "%s\t%s\n", c -> Name, c -> Number);
			c = c -> next;
		}
	}

	fclose(outfp);
}

// Reads the specified file if it exists and imports any contact data
void read_file(char *filename) {
    FILE *infp;

    char Name[MAX_CHARS + 1];
	char Number[MAX_NUMBERLEN + 1];

	infp = fopen(filename, "r");
	
	if (infp == NULL) {
		return;
	}

	fseek(infp, strlen(HEADER), SEEK_SET); // Our header is 51 chars so we skip over it

	while(fscanf(infp, "%s %s\n", Name, Number) == 2) { // As long as fseek can successfully fetch 2 values for the book, it will keep iterating through
		insertContact(Name, Number);		
	} 	
}