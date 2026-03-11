/* File: general.c
Aluno: Constança Fonseca
Nº de Aluno: 106251
Description: file with every function regarding general calculations */

#include "general.h"
#include "vaccine.h"
#include "date.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Receives a dimension and allocates memory for a vector.
 * @param dimension -> number of characters to allocate
 * @param flag -> program set to English (0) or Portuguese (1)
 */
char *createVecCommand(int dimension, int flag) {
    /* creates and allocates memory to a vector to use it as command */
    char *vector = (char*) malloc(dimension * sizeof(char));
    if (vector == NULL) noMemory(flag);

    return vector;
}

/**
 * Receives a command and reads it until the end of the line.
 * @param command -> command to be read
 * @param dimension -> number of characters to read
 */
void readCommand(char *command, int dimension) {
    /* reads command */
    int i = 0;
    char c;

    while ((c = getchar()) != '\n' && i < dimension - 1)
        command[i++] = c;
    command[i] = '\0';
}

/**
 * Receives a string and reads word by word.
 * @param instr -> string to be checked
 * @param word -> where to store the word read
 * @param flag -> program set to English (0) or Portuguese (1)
 * @return int -> number of characters read
 */
int readWords(char **instr, char **word, int flag) { 
    int cont = 0, i = 0, in, j, k, charac;
    char *inst = *instr;

    while (inst[i] == ' ' || inst[i] == '\t') i++;
    
    if (inst[i] == '\0') return 0; /* verifies if it's end of input */
    
    if (inst[i] == '"') {
        i++;
        in = i;
        while (inst[i++] != '"')
            cont++;
    } else {
        in = i;
        while (inst[i] != ' ' && inst[i] != '\t' && inst[i] != '\0') {
            i++;
            cont++;
        }
    }

    if(cont == 0) return 0;

    *word = (char*) malloc((cont + 1) * sizeof(char));
    if (*word == NULL) noMemory(flag);

    for (k = in, j = 0; j < cont; k++, j++) (*word)[j] = inst[k]; 
    (*word)[j] = '\0';

    charac = i;
    (*instr) += charac;

    return charac;
}

/**
 * Receives a string and counts how many words it has.
 * @param word -> string to be checked
 * @return int -> number of words in the string
 */
int readWordsInLine(char* word) {
    int wordCount = 0;
    int inWord = FALSE;
    int inQuotes = FALSE;

    for (int i = 0; word[i] != '\0'; i++) {
        if ((word[i] == ' ' || word[i] == '\t') && !inQuotes) {
            /* if we were in a word, it is the end of it */
            if (inWord) {
                inWord = FALSE;
            }
        } else if (word[i] == '"') { /* if "" then we enter words on quotes */
            inQuotes = !inQuotes;
        } else {
            if (!inWord) {
                inWord = TRUE;
                wordCount++;
            }
        }
    }
    return wordCount;
}

/**
 * Receives a flag and prints the error message for memory limit exceeded.
 * @param flag -> program set to English (0) or Portuguese (1)
 */
void noMemory(int flag) {
    printf(flag ? "sem memória\n" : "No memory\n"); 
    exit(EXIT_FAILURE); 
}