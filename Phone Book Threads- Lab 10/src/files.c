/*
files.c
Owen Matejka

This file contains all the functions for manipulation of system files.
*/

// Main Directive
#include "lab10.h"

// Saves all contacts to specified file
void save_file(char *filename) {
    FILE *outfp;
    Contact *c;
    int i;

    outfp = fopen(filename, "w");

    if (outfp == NULL) {
        printf("Error opening file. Aborting save operation....\n");
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
        pthread_mutex_lock(&autosave_mutex);
		insertContact(Name, Number);		
        pthread_mutex_unlock(&autosave_mutex);
	} 	
}

// Writes all contacts to the specified binary file
void write_binary(char *binaryfile) {
    FILE *outfp = fopen(binaryfile, "wb");
    int i;
    
    for (i = 0; i < 26; i++) {
        Contact *c = headNodes[i];

        while (c != NULL) {
            fwrite(c, sizeof(Contact), 1, outfp); // Writes the Contact to the file
            c = c -> next;
        }
    }
    fclose(outfp);

    if (strcmp(binaryfile, AUTOSAVE_FILENAME) != 0) { // Hides save messages for background save operations
        printf("Successfully wrote to binary file!\n\n");
    }
}

// Reads the binary file and prints the contacts
void read_binary(char *binaryfile) {
    FILE *infp = fopen(binaryfile, "rb");
    Contact c; // Fread doesn't use pointers so this is a normal object

    if (infp == NULL) {
        printf("Error opening binary file!\n");
        return;
    }

    while (fread(&c, sizeof(Contact), 1, infp) > 0) { // If fread returns greater than 1, then it is fetching an object
        printf("Name: %s, Number: %s\n", c.Name, c.Number);
    }
    printf("\n");

    fclose(infp);
}

// Automatically saves data to a binary file on the specified interval
void *autosave(void *arg) {
    while (1) {
        pthread_mutex_lock(&autosave_mutex);
        write_binary(AUTOSAVE_FILENAME);
        pthread_mutex_unlock(&autosave_mutex);
        sleep(AUTOSAVE_INTERVAL);
    }
}