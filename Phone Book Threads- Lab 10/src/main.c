/*
Lab #10
COEN 11 SCU
Programmer: Owen Matejka
Instructor: Yi Fang
Date: 2 December 2021
About: A simple phonebook where you can view, create, and delete contacts. This is a modified version of lab #9 to include autosaving using a thread.
*/


// Main Directive
#include "lab10.h"

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

    pthread_t autosave_thread;
    pthread_mutex_init(&autosave_mutex, NULL); // Initializes the mutex lock
    pthread_create(&autosave_thread, NULL, autosave, NULL); // Creates and executes the thread

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
                pthread_mutex_lock(&autosave_mutex);
                manualInsert();
                save_file(argv[1]);
                write_binary(argv[2]);
                pthread_mutex_unlock(&autosave_mutex);
                break;
            case 2 :
                pthread_mutex_lock(&autosave_mutex);
                deleteContact();
                pthread_mutex_unlock(&autosave_mutex);
                break;
            case 3 :
                //displayContacts(); //-- We are printing in reverse for this lab, so this function is not called
                pthread_mutex_lock(&autosave_mutex);
                reverseAll();
                displayContactsInReverse();
                reverseAll();
                pthread_mutex_unlock(&autosave_mutex);
                break;
            case 4 :
                //displayContactsByLetter();
                pthread_mutex_lock(&autosave_mutex);
                displayContactsByLetterInReverse();
                pthread_mutex_unlock(&autosave_mutex);
                break;
            case 5:
                pthread_mutex_lock(&autosave_mutex);
                write_binary(argv[2]);
                pthread_mutex_unlock(&autosave_mutex);
                break;
            case 6:
                read_binary(argv[2]);
                break;
            case 7 :
                pthread_mutex_lock(&autosave_mutex);
                pthread_cancel(autosave_thread); // Cancels the autosave thread
                pthread_mutex_unlock(&autosave_mutex);
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