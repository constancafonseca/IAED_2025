/* File: inoculation.c
Aluno: Constança Fonseca
Nº de Aluno: 106251
Description: file with every function regarding patients and inoculations
Vaccinate a patient, list vaccine applications, remove applications */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inoculation.h"
#include "date.h"
#include "date.h"

/**
 * Receives a command and checks if it is valid. If not, prints the error. 
 * Administers a vaccine to a patient.
 * @param command -> one line of input
 * @param inocSys -> struct with all inoculations
 * @param vaccineTable -> struct with all vaccines
 * @param sys_date -> system date
 * @param flag -> program set to English (0) or Portuguese (1)
 */
void readCommandA(char *command, InoculationtionSystem *inocSys,
    VaccineHashTable *vaccineTable, Date *sys_date, int flag) {
    int count_args = 0, aux = 0;
    char* word;
    Inoculation inoculation;

    memset(&inoculation, 0, sizeof(Inoculation));
    
    count_args = readWords(&command, &word, flag);

    while(count_args > 0){
        if(aux == 1) inoculation.name = strdup(word); /* patient name */
        else if(aux == 2) inoculation.vaccine.name = strdup(word); /* vaccine name */
        free(word);
        aux++;
        count_args = readWords(&command, &word, flag);
    }

    inoculation.date = *sys_date;

    if(inoculationErrors(vaccineTable, inoculation, inocSys, flag)) {
        return;
    }

    /* if there are no errors, inoculates the patient */
    char *batch = addInoculation(inocSys, vaccineTable, inoculation, flag);
    if (!batch) { /* if insertion fails */
        free(inoculation.name);
        free(inoculation.vaccine.name);
        return;
    }
    printf("%s\n", batch);
    if (inoculation.vaccine.name) free(inoculation.vaccine.name);
}

/**
 * Receives an inoculation and checks if it is valid. If not, prints the error.
 * @param vaccineTable -> struct with all vaccines
 * @param inoculation -> inoculation to be added
 * @param inocSys -> struct with all inoculations
 * @param flag -> program set to English (0) or Portuguese (1)
 * @return TRUE if there is an error, FALSE otherwise
 */
int inoculationErrors(VaccineHashTable *vaccineTable, Inoculation inoculation, 
    InoculationtionSystem *inocSys, int flag) {
    
    if (!existsValidVaccine(vaccineTable, inoculation.vaccine.name)) {
        free(inoculation.name);
        free(inoculation.vaccine.name);
        printf(flag ? "esgotado\n" : "no stock\n");
        return TRUE;
    }

    if (alreadyVaccinated(inocSys, inoculation)) {
        free(inoculation.name);
        free(inoculation.vaccine.name);
        printf(flag ? "já vacinado\n" : "already vaccinated\n");
        return TRUE;
    }
    
    return FALSE;
}

/**
 * Receives a command and checks if it is valid. If not, prints the error.
 * Lists all inoculations or a specific one.
 * @param command -> one line of input
 * @param inocSys -> struct with all inoculations
 * @param flag -> program set to English (0) or Portuguese (1)
 */
void readCommandU(char *command, InoculationtionSystem *inocSys, int flag) {
    int count_args = 0;
    int count_words = readWordsInLine(command);

    char *word, *patient_name = NULL;

    if(count_words == 1){  /* if input = u */
        listAllInoculations(inocSys);
        return;
    }

    count_args = readWords(&command, &word, flag);

    while(count_args > 0){ /* for patient name */
        if(patient_name) free(patient_name);
        patient_name = strdup(word);
        free(word);  
        word = NULL;
        count_args = readWords(&command, &word, flag);
    }

    if(!existsPatient(inocSys, patient_name) && patient_name != NULL) {
        printf(flag ? "%s: utente inexistente\n" : "%s: no such user\n", 
            patient_name);
        if(word) free(word);
        if(patient_name) free(patient_name); 
        return;
    }

    /* lists a specific patient's inoculations */
    listPatientInoculations(inocSys, patient_name);
    if(word) free(word);
    if(patient_name) free(patient_name);
}

/**
 * Receives a command and checks if it is valid.
 * Removes inoculations.
 * @param command -> one line of input
 * @param inocSys -> struct with all inoculations
 * @param vaccineTable -> struct with all vaccines
 * @param flag -> program set to English (0) or Portuguese (1)
 * @param sys_date -> system date
 */
void readCommandD(char *command, InoculationtionSystem *inocSys, 
    VaccineHashTable *vaccineTable, int flag, Date *sys_date) {
    int count_words = readWordsInLine(command);
    int count_args = 0, aux = 0;
    char *word = NULL, *patient_name = NULL, *batch = NULL;
    Date new_date;
    memset(&new_date, 0, sizeof(Date));

    count_args = readWords(&command, &word, flag);

    while(count_args > 0){ /* for each argument */
        if(aux == 1) patient_name = strdup(word); /* patient name*/
        else if(aux == 2) new_date = readDate(word, new_date); /* application date */
        else if(aux == 3) batch = strdup(word); /* date */
        free(word);
        aux++;  
        count_args = readWords(&command, &word, flag);
    }

    if(deleteInoculationErrors(inocSys, vaccineTable, patient_name, new_date, 
        batch, count_words, flag, sys_date)) {
        return;
    }

    deleteInoculation(inocSys, patient_name, new_date, batch, count_words);
}

/**
 * Receives an inoculation system and checks if it is valid. If not, prints the error.
 * @param inocSys -> struct with all inoculations
 * @param vaccineTable -> struct with all vaccines
 * @param patient_name -> name of the patient
 * @param new_date -> date of the application
 * @param batch -> batch of the vaccine
 * @param count_words -> number of words in the command
 * @param flag -> program set to English (0) or Portuguese (1)
 * @param sys_date -> system date
 */
int deleteInoculationErrors(InoculationtionSystem *inocSys, VaccineHashTable 
    *vaccineTable, char *patient_name, Date new_date, char *batch, 
    int count_words, int flag, Date *sys_date) {
    if(count_words == 2){
        if(!existsPatient(inocSys, patient_name) && patient_name != NULL) {
            printf(flag ? "%s: utente inexistente\n" : "%s: no such user\n", 
                patient_name);
            free(patient_name);
            free(batch);
            return TRUE;
        }
    } else if(count_words == 3){
        if(!validDate(new_date, sys_date, 0)){
            printf(flag ? "data inválida\n" : "invalid date\n");
            free(patient_name);
            free(batch);
            return TRUE;
        }
    } else if(count_words == 4){
        if(!existsBatch(vaccineTable, batch)) {
            printf(flag ? "%s: lote inexistente\n" : "%s: no such batch\n", 
                batch);
            free(patient_name);
            free(batch);
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Checks if this vaccine is available.
 * @param vaccineTable -> pointer to the hash table
 * @param vaccine_name -> name of the vaccine to be checked
 * @return TRUE if there is availability, FALSE otherwise
 */
int existsValidVaccine(VaccineHashTable *vaccineTable, char *vaccine_name) {
    for (int i = 0; i < TABLE_SIZE; i++) { 
        VaccineNode *current = vaccineTable->tableBatch[i];  
        while (current != NULL) {
            if (strcmp(current->vaccine.name, vaccine_name) == 0 && 
            current->vaccine.doses > 0) {
                return TRUE;
            }
            current = current->next;
        }
    }
    return FALSE;
}

/**
 * Checks if the patient has already been vaccinated with the same vaccine
 * and date.
 * @param inocSys -> struct with all inoculations
 * @param inoculation -> inoculation to be checked
 * @return TRUE if already vaccinated, FALSE otherwise
 */
int alreadyVaccinated(InoculationtionSystem *inocSys, Inoculation inoculation) {
    unsigned int index = hashFunctionName(inoculation.name);
    InoculationNode *node = inocSys->tablePatient[index];
    
    while (node) {
        if (strcmp(node->inoculation.name, inoculation.name) == 0 &&
            strcmp(node->inoculation.vaccine.name, inoculation.vaccine.name) ==
            0 && compareDates(node->inoculation.date, inoculation.date) == 0) {
            return TRUE; 
        }
        node = node->next;
    }

    return FALSE; 
}

/**
 * Checks if the patient exists in the system.
 * @param inocSys -> struct with all inoculations
 * @param patient_name -> name of the patient
 * @return TRUE if exists, FALSE otherwise
 */
int existsPatient(InoculationtionSystem *inocSystem, char *patient_name) {
    int index = hashFunctionName(patient_name);

    InoculationNode *node = inocSystem->tablePatient[index];
    while (node) {
        if (strcmp(node->inoculation.name, patient_name) == 0) {
            return TRUE;
        }
        node = node->next;
    }

    return FALSE;
}

/**
 * Adds an inoculation to the system.
 * @param inocSys -> struct with all inoculations
 * @param vaccineTable -> struct with all vaccines
 * @param inoculation -> inoculation to be added
 * @param flag -> program set to English (0) or Portuguese (1)
 * @return char* -> batch of the vaccine used
 */
char* addInoculation(InoculationtionSystem *inocSys, VaccineHashTable 
    *vaccineTable, Inoculation inoculation, int flag) {
    
    VaccineNode* vaccine_node = findValidVaccine(vaccineTable, 
        inoculation.vaccine.name);
    if (!vaccine_node) return NULL;
    
    vaccine_node->vaccine.applications++;
    vaccine_node->vaccine.doses--;
    inoculation.vaccine = vaccine_node->vaccine;

    inocSys->list = realloc(inocSys->list, (inocSys->count + 1) * 
    sizeof(Inoculation));

    if (!inocSys->list) {
        noMemory(flag);
        return NULL;
    }
    
    /* adds new inoculation at the end of the array */
    inocSys->list[inocSys->count] = inoculation;
    inocSys->count++;

    unsigned int index = hashFunctionName(inoculation.name);
    InoculationNode *new_hash_node = malloc(sizeof(InoculationNode));
    if (!new_hash_node) {
        noMemory(flag);
        return NULL;
    }

    new_hash_node->inoculation = inoculation;
    new_hash_node->next = inocSys->tablePatient[index];
    inocSys->tablePatient[index] = new_hash_node;

    return inoculation.vaccine.batch;
}

/**
 * Finds a valid vaccine in the hash table.
 * @param vaccineTable -> pointer to the hash table
 * @param vaccine_name -> name of the vaccine to be found
 * @return VaccineNode* -> pointer to the found vaccine node
 */
VaccineNode* findValidVaccine(VaccineHashTable *vaccineTable, char* vaccine_name) {
    Vaccine *matchingVaccines = NULL;
    int count = collect_vaccines(vaccineTable, &matchingVaccines, vaccine_name);
    if (!count) return NULL; /* if no vaccines found */
    
    sortingVaccines(matchingVaccines, 0, count - 1);

    int validIndex = -1;
    /* finds the first vaccine with valid doses */
    for (int i = 0; i < count; i++) {
        if (matchingVaccines[i].doses > 0) {
            validIndex = i;
            break;
        }
    }

    if (validIndex == -1) { /* if no vaccines with doses left are available */
        free(matchingVaccines);
        return NULL;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        VaccineNode *node = vaccineTable->tableBatch[i];
        while (node) {
            if (strcmp(node->vaccine.batch, matchingVaccines[validIndex].batch) == 0) {
                free(matchingVaccines);  
                return node;
            }
            node = node->next;
        }
    }

    free(matchingVaccines);
    return NULL;
}

/**
 * Lists all inoculations in the system.
 * @param inocSys -> struct with all inoculations
 */
void listAllInoculations(InoculationtionSystem *inocSys) {
    int count = inocSys->count;
    Inoculation *list = inocSys->list;
    
    if (count == 0) return;

    for (int i = 0; i < count; i++) { /* iterate the entire array */
        printf("%s %s %.2d-%.2d-%.4d\n", list[i].name, 
            list[i].vaccine.batch, list[i].date.day,
            list[i].date.month, list[i].date.year);
    }
}

/**
 * Lists all inoculations of a specific patient.
 * @param inoculationSystem -> struct with all inoculations
 * @param patient_name -> name of the patient
 */
void listPatientInoculations(InoculationtionSystem *inoculationSystem, 
    char *patient_name) {
    Inoculation *inoculations = NULL;
    int count = 0;

    collectPatientInoculations(inoculationSystem, patient_name, &inoculations, 
        &count);

    if (count == 0) return;

    for (int i = count - 1; i >= 0; i--) { /* iterate the entire array */
        printf("%s %s %.2d-%.2d-%.4d\n", inoculations[i].name, 
            inoculations[i].vaccine.batch, inoculations[i].date.day,
            inoculations[i].date.month, inoculations[i].date.year);
    }

    free(inoculations);
    inoculations = NULL;
}

/**
 * Collects all inoculations of a specific patient.
 * @param inoculationSystem -> struct with all inoculations
 * @param patient_name -> name of the patient
 * @param inoculations -> pointer to the array of inoculations
 * @param count -> pointer to the number of inoculations
 */
void collectPatientInoculations(InoculationtionSystem *inoculationSystem, 
    char *patient_name, Inoculation **inoculations, int *count) {
    int index = hashFunctionName(patient_name);
    InoculationNode *node = inoculationSystem->tablePatient[index]; 

    *count = 0; 
    while (node) {
    if (strcmp(node->inoculation.name, patient_name) == 0) {
        Inoculation *temp = realloc(*inoculations, sizeof(Inoculation) * (*count + 1));
        *inoculations = temp;
        (*inoculations)[*count] = node->inoculation;
        (*count)++;
    }
    node = node->next;
    }
}

/**
 * Frees the memory allocated for the inoculation system.
 * @param inocSys -> struct with all inoculations
 */
void deleteInoculation(InoculationtionSystem *inocSys, char *patient_name, 
    Date new_date, char *batch, int count_words) {
    unsigned int index = hashFunctionName(patient_name);
    InoculationNode *node = inocSys->tablePatient[index];
    InoculationNode *prev = NULL;
    int num_deleted = 0;

    while (node) {
        int match = strcmp(node->inoculation.name, patient_name) == 0;

        /* if input has date too, include date in match condition */
        if (count_words == 3) 
            match = match && compareDates(node->inoculation.date, new_date) == 0;
            /* If input has date and batch, include both in match condition */
        else if (count_words > 3)
            match = match && compareDates(node->inoculation.date, new_date) == 0 
            && strcmp(node->inoculation.vaccine.batch, batch) == 0;

        if (match) {
            InoculationNode *temp = node;
            node = node->next;
            if (prev == NULL) inocSys->tablePatient[index] = node;
            else prev->next = node;
            free(temp);
            num_deleted++;
        } else {
            prev = node;
            node = node->next;
        }
    }

    printf("%d\n", num_deleted); /* how many records were deleted */
    free(patient_name);
    free(batch);
}

/**
 * Hash function for the patient name.
 * @param key -> patient name
 * @return int -> hash value
 */
int hashFunctionName(char *key) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return (int)(hash % TABLE_SIZE);
}

/**
 * Compares two inoculations.
 * @param a -> first inoculation
 * @param b -> second inoculation
 * @return int -> comparison result
 */
int compareInoculation(Inoculation a, Inoculation b) {
    int date_cmp = compareDates(a.date, b.date);
    if (date_cmp != 0) return date_cmp;

    int name_cmp = strcmp(a.name, b.name);
    if (name_cmp != 0) return name_cmp;

    return strcmp(a.vaccine.name, b.vaccine.name);
}

/**
 * Swaps two inoculations.
 * @param a -> first inoculation
 * @param b -> second inoculation
 */
void swapInoc(Inoculation *a, Inoculation *b) {
    Inoculation temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Partitions the array for a sorting algorithm.
 * @param arr -> array of Inoculation structures
 * @param low -> starting index
 * @param high -> ending index
 * @return int -> partition index
 */
int partitionInoc(Inoculation *arr, int low, int high) {
    Inoculation pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (compareInoculation(arr[j], pivot) < 0) {
            swapInoc(&arr[++i], &arr[j]);
        }
    }
    swapInoc(&arr[i + 1], &arr[high]);
    return i + 1;
}

/**
 * Sorts an array of Inoculation structures using a sorting algorithm.
 * @param arr -> array of Inoculation structures
 * @param low -> starting index
 * @param high -> ending index
 */
void sortInoculations(Inoculation *arr, int low, int high) {
    if (low < high) {
        int pi = partitionInoc(arr, low, high);
        sortInoculations(arr, low, pi - 1);
        sortInoculations(arr, pi + 1, high);
    }
}

/**
 * Creates a hash table and an array for storing inoculations.
 * @param flag -> program set to English (0) or Portuguese (1)
 * @return InoculationtionSystem* -> pointer to the created hash table and array
 */
InoculationtionSystem* createInoculationSystem(int flag) {
    InoculationtionSystem *inoculationSystem = 
    (InoculationtionSystem *)malloc(sizeof(InoculationtionSystem));
    if (!inoculationSystem) { /* if memory allocation fails */
        noMemory(flag);
        return NULL;
    }
    
    inoculationSystem->list = malloc(sizeof(Inoculation) * 10);
    if (!inoculationSystem->list) {
        noMemory(flag);
        return NULL;
    }
    inoculationSystem->count = 0;

    /* initialize positions in the hash table to NULL */
    for (int i = 0; i < TABLE_SIZE; i++) {
        inoculationSystem->tablePatient[i] = NULL;
    }
    
    return inoculationSystem;
}

/**
 * Frees the memory allocated for the hash table / array and its contents.
 * @param inocSys -> pointer to the hash table and array to be freed
 */
void freeInoculationSystem(InoculationtionSystem *inocSys){
    int count = inocSys->count;
    if (!inocSys) return;

    /* free all information in array */
    if (inocSys->list) {
        for (int i = 0; i < count; i++) {
            free((inocSys->list)[i].name);
        }
        free(inocSys->list); 
        inocSys->list = NULL;
    }

    InoculationNode *temp, *current;

    /* free all information in hash table indexed by name */
    for (int i = 0; i < TABLE_SIZE; i++) {
        current = inocSys->tablePatient[i];
        while (current) {
            temp = current;
            current = current->next;
            if(temp) free(temp);
        }
    }
    free(inocSys);  /* frees struct */
}