/* File: inoculation.h
Aluno: Constança Fonseca
Nº de Aluno: 106251
Description: file with structs regarding patients and inoculations */

#ifndef PATIENT_H
#define PATIENT_H

#include "vaccine.h"

typedef struct InoculationNode {
    Inoculation inoculation;
    struct InoculationNode *next;
} InoculationNode;

typedef struct {
    Inoculation *list;
    int count;
    InoculationNode *tablePatient[TABLE_SIZE];
} InoculationtionSystem;

/* definitions of all functions declared in inoculation.c */
void readCommandA(char *command, InoculationtionSystem *inoculationSystem,
    VaccineHashTable *vaccineTable, Date *sys_date, int flag);

int inoculationErrors(VaccineHashTable *vaccineTable, Inoculation inoculation, 
    InoculationtionSystem *inoculationSystem, int flag);
    
void readCommandU(char *command, InoculationtionSystem *inoculationSystem, 
    int flag);

void readCommandD(char *command, InoculationtionSystem *inocSys, VaccineHashTable 
    *vaccineTable, int flag, Date *sys_date);

int deleteInoculationErrors(InoculationtionSystem *inocSys, VaccineHashTable 
    *vaccineTable, char *patient_name, Date new_date, char *batch, 
    int count_words, int flag, Date *sys_date);

int existsValidVaccine(VaccineHashTable *vaccineTable, char *vaccine_name);

int alreadyVaccinated(InoculationtionSystem *inocSys, Inoculation inoculation);

int existsPatient(InoculationtionSystem *inoculationSystem, char *patient_name);

char* addInoculation(InoculationtionSystem *inoculationSystem, 
    VaccineHashTable *vaccineTable, Inoculation inoculation, int flag);

VaccineNode* findValidVaccine(VaccineHashTable *vaccineTable, char* vaccine_name);

void listAllInoculations(InoculationtionSystem *inoculationSystem);

void listPatientInoculations(InoculationtionSystem *inoculationSystem, char 
    *patient_name);

void collectPatientInoculations(InoculationtionSystem *inoculationSystem, 
    char *patient_name, Inoculation **inoculations, int *count);

void deleteInoculation(InoculationtionSystem *inocSys, char *patient_name, 
    Date new_date, char *batch, int count_words);

int hashFunctionName(char *key);

int compareInoculation(Inoculation a, Inoculation b);

void swapInoc(Inoculation *a, Inoculation *b);

int partitionInoc(Inoculation *arr, int low, int high);

void sortInoculations(Inoculation *arr, int low, int high);

InoculationtionSystem* createInoculationSystem(int flag);

void freeInoculationSystem(InoculationtionSystem *inoculationSystem);

#endif /* INOCULATION_H */