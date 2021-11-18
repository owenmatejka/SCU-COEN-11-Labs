/*
Lab #9
COEN 11 SCU
Programmer: Owen Matejka
Instructor: Yi Fang
Date: 18 November 2021
About: A simple phonebook where you can view, create, and delete contacts. This is a modified version of lab #8 to include recursion to reverse a linked list, multiple files to better organize code, and a makefile to make the compilation of code easier.
*/


// Main Directive
#include "lab9.h"

// Main Method
int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("No file name was supplied. Please rerun the program.");
        return 0;
    } else if (argv[2] == NULL) {
        printf("No name was supplied for the binary file. Please rerun the program.");
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
        5: Save contacts to binary file\n\
        6: Print binary file contents\n\
        7: Quit\n"
        );

        scanf("%d", &input);
        printf("\n");

        switch(input) {
            case 1 :
                manualInsert();
                save_file(argv[1]);
                write_binary(argv[2]);
                break;
            case 2 :
                deleteContact();
                break;
            case 3 :
                //displayContacts(); //-- We are printing in reverse for this lab, so this function is not called
                reverseAll();
                displayContactsInReverse();
                reverseAll();
                break;
            case 4 :
                //displayContactsByLetter();
                displayContactsByLetterInReverse();
                break;
            case 5:
                write_binary(argv[2]);
                break;
            case 6:
                read_binary(argv[2]);
                break;
            case 7 :
                save_file(argv[1]); // Save file before quitting
                write_binary(argv[2]); // Also write to the binary file
                printf("Quitting . . .");
                break;
            default :
                printf("Invalid option. Please try again!");
        }

        if (input == 7) {
            break;
        }
    }
    
}