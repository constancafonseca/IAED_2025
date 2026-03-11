/* File: general.h
Aluno: Constança Fonseca
Nº de Aluno: 106251
Description: file with general structs */

#ifndef GENERAL_H
#define GENERAL_H

#define EXIT_FAILURE 1
#define FALSE 0
#define TRUE 1

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char *name;
    char *batch;
    Date date;
    int doses;
    int applications;
} Vaccine;

typedef struct inoculation {
    char* name;
    Vaccine vaccine;
    Date date;
} Inoculation;

/* definitions of all functions declared in general.c */
char *createVecCommand(int dimension, int flag);

void readCommand(char *command, int dimension);

int readWords(char **command, char **word, int flag);

int readWordsInLine(char *command);

void noMemory(int flag);

#endif /* GENERAL_H */
