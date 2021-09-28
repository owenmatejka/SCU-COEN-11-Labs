/*
Lab #1
COEN 11 SCU
Programmer: Owen Matejka
Instructor: Yi Fang
About: A simple math game where you answer basic division questions.
*/


// Directives
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Configuration
const int TOTAL_QUESTIONS = 10;

// Method Declarations
int divide(int Divisor, int Quotient);

// Main Method
int main() {
    srand ((int) time (NULL)); // Seeds numbers to ensure randomness

    int Divisor;
    int Quotient;
    int Correct = 0;
    size_t i;

    for (i = 0; i < TOTAL_QUESTIONS; i++){
        printf("Generating Numbers...\n");

        Divisor = rand() % 12 + 1;
        Quotient = rand() % 13;

        Correct += divide(Divisor, Quotient); // Since it returns 0 or 1 based on correctness, we just add whatever int is returned
    }

    float totalCorrect = ((float) Correct / TOTAL_QUESTIONS) * 100; // Converts to a float for precision and gets it out of 100
    printf("You got %d out of %d correct resulting in a score of %.2f%%\n", Correct, TOTAL_QUESTIONS, totalCorrect);

    return 0;
}

// Divide Method
int divide(int Divisor, int Quotient) {   
    int Dividend = Divisor * Quotient;

    printf("Your dividend is: %d\n", Dividend);
    printf("Your divisor is: %d\n", Divisor);
    printf("Please enter the answer to %d / %d:\n", Dividend, Divisor);
    
    int Answer;
    scanf("%d", &Answer);

    if (Answer == Quotient) {
        printf("Correct! Nice work!\n\n");
        return 1;
    } else {
        printf("\nIncorrect answer! The answer was: %d.\n\n", Quotient);
        return 0;
    }
}