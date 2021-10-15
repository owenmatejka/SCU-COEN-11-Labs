/*
Lab #4
COEN 11 SCU
Programmer: Owen Matejka
Instructor: Yi Fang
Date: 14 October 2021
About: A simple infobook where you can view, create, and delete profiles about people.This is a semi-modified version of lab #3 using unions.
*/


// Directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Configuration
#define MAX_NAMES 20
#define MAX_CHARS 40

// Method Declarations
void displayProfiles();
int checkIfDuplicate(char Name[]);
void insertProfile();
void deleteProfile();

// Structures / Unions
typedef union Status {
    char schoolName[MAX_CHARS + 1];
    float monthlySalary;
    int retirementAge;
} Status;
typedef struct Profile {
    char Name[MAX_CHARS + 1];
    int Age;
    Status Status;
} Profile;

// Data
Profile Profiles[MAX_NAMES] = {};
int Counter = 0;

// Main Method
int main() {
    printf("Welcome to the people book!\n");

    int input;

    while (1) { // This loop repeats indefinitely unless the user enters 3
        printf("The following commands are avaliable:\n");
        printf("\
        0: Create profile\n\
        1: Delete profile\n\
        2: View all profiles\n\
        3: Quit\n"
        );

        scanf("%d", &input);
        printf("\n");

        switch(input) {
            case 0 :
                insertProfile();
                break;
            case 1 :
                deleteProfile();
                break;
            case 2 :
                displayProfiles();
                break;
            case 3 :
                printf("Quitting . . .");
                break;
            default :
                printf("Invalid option. Please try again!");
        }

        if (input == 3) {
            break;
        }
    }
    
}

// Displays all profiles
void displayProfiles() {
    int i; Profile *c = Profiles;
    for (i = 0; i < MAX_NAMES; i++) {
        if (strcmp((c + i) -> Name, "") != 0) { // Checks to see if the name field is empty so it won't display allocated, but empty rows
            printf("Name: %s\nAge: %d\n", (c + i) -> Name, (c + i) -> Age);

            if ((c + i) -> Age <= 21) {
                printf("Name of School: %s\n\n", (c + i) -> Status.schoolName);
            } else if ((c + i) -> Age < 65) {
                printf("Monthly Salary: %f\n\n", (c + i) -> Status.monthlySalary);
            } else {
               printf("Retirement Age: %d\n\n", (c + i) -> Status.retirementAge);
            }
        }
    }
    if (Counter == 0) {
        printf("There are no profiles to display.\n");
    }
    printf("\n");
}

// Checks for duplicate profiles
int checkIfDuplicate(char Name[]) {
    int i; Profile *c = Profiles;
    for (i = 0; i < MAX_NAMES; i++) {
        if (strcmp(Name, (c + i) -> Name) == 0) { // if the names match, it returns the index
            return i;
        }
    }

    return -1;
}

// Inserts a new profile
void insertProfile() {
    if (Counter == MAX_NAMES) {
        printf("The people book is full! Please delete a profile before trying to create a new one.\n\n");
        return;
    }

    char Name[MAX_CHARS + 1]; // Create a temp array so we can check the input against the array of names before adding
    
    printf("Please enter the name. **NOTE: Only enter your first name!**\n");
    scanf("%s", &Name);

    if (checkIfDuplicate(Name) != -1) {
        printf("A profile with this name already exists! Please try again.\n\n");
        return;
    } else if (strlen(Name) > MAX_CHARS) {
        printf("The maximum number of allowed characters is %d! Please try again.\n\n", MAX_CHARS);
        return;
    }

    printf("Please enter the individuals's age.\n");
    scanf("%d", &Profiles[Counter].Age);

    if (Profiles[Counter].Age <= 21) {
        printf("Please enter the school you attend.\n");
        scanf("%s", &Profiles[Counter].Status.schoolName);

    } else if (Profiles[Counter].Age < 65) {
        printf("Please enter your monthly salary.\n");
        scanf("%f", &Profiles[Counter].Status.monthlySalary);
    } else {
        printf("You inputted an age greater than 65. Autoinputting retirement age based on this info.\n");
        Profiles[Counter].Status.retirementAge = Profiles[Counter].Age - 65; // No need to do an input, just simple algebra
    }

    strcpy(Profiles[Counter].Name, Name); // Copies data to the array

    Counter += 1; // Increases counter for next profile slot to be used

    printf("Profile successfully created!\n\n");
}

// Deletes profile by supplied name
void deleteProfile(){
    char Name[MAX_CHARS + 1];

    printf("Please enter the name to delete.\n");
    scanf("%s", &Name);

    int indexToDelete = checkIfDuplicate(Name);

    if (indexToDelete == -1) {
        printf("That profile name does not exist in the people book. Please try again!\n\n");
        return;
    }

    strcpy(Profiles[indexToDelete].Name, Profiles[Counter].Name); // Copies the newest profile to the deleted slot
    Profiles[indexToDelete].Age = Profiles[Counter].Age;

    strcpy(Profiles[Counter].Name, ""); // Inserts blank data to the newest profile slot for reuse
    Profiles[Counter].Age = 0;

    printf("Profile successfully deleted!\n\n");

    Counter -= 1; // Lowers counter so the previous profile slot can be reused
}