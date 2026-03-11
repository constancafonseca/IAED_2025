/* File: vaccine.h
Aluno: Constança Fonseca
Nº de Aluno: 106251
Description: file with structs regarding vaccines */

#ifndef VACCINE_H
#define VACCINE_H

#include "general.h"

#define MAX_NAME_VACCINE 50
#define MAX_NAME_PATIENT 200
#define MAX_NAME_BATCH 20

#define MAX_BATCH 1000 /* number of batches that can be added */

#define TABLE_SIZE 1001

typedef struct VaccineNode {
    Vaccine vaccine;
    struct VaccineNode *next;
} VaccineNode;

typedef struct {
    VaccineNode *tableBatch[TABLE_SIZE];
    VaccineNode *tableName[TABLE_SIZE];
} VaccineHashTable;

/* definitions of all functions declared in vaccine.c */
void readCommandC(char *command, VaccineHashTable *vaccineTable, Date *sys_date, 
    int flag);

int vaccineErrors(Vaccine vaccine, VaccineHashTable *vaccineTable, Date *sys_date, 
    int flag);

void readCommandL(char *command, VaccineHashTable *vaccineTable, int flag);

void readCommandR(char *command, VaccineHashTable *vaccineTable, int flag);

void addVaccine(VaccineHashTable *vaccineTable, Vaccine vaccine, int flag);

void listAllVaccines(VaccineHashTable *vaccineTable);

void listVaccine(VaccineHashTable *vaccineTable, char *vaccine_name, int flag);

int existsAvailability(VaccineHashTable *vaccineTable);

int existsBatch(VaccineHashTable *vaccineTable, char* batch);

int validBatch(char* batch);

int validVaccineName(char *name);

int validQuantity(int quantity);

void deleteBatchAvailability(VaccineHashTable *hashTable, char *batch);

void deleteFromTable(VaccineNode **head, char *batch, int freeFields);

Vaccine* findVaccineByBatch(VaccineHashTable *hashTable, char *batch);

int hashFunctionBatch(char *batch);

int collect_vaccines(VaccineHashTable *ht, Vaccine **array, char *vaccine_name);

void swap(Vaccine *a, Vaccine *b);

int partition(Vaccine *arr, int low, int high);

void sortingVaccines(Vaccine *arr, int low, int high);

VaccineHashTable* createVaccineHashTable(int flag);

void freeVaccineHashTable(VaccineHashTable* table);

#endif /* VACCINE_H */
