/* File: vaccine.c
Aluno: Constança Fonseca
Nº de Aluno: 106251
Description: file with every function regarding vaccines
Adding a new vaccine batch, listing vaccines, apply a vaccine, remove a vaccine */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "vaccine.h"
#include "date.h"
#include "general.h"
#include "inoculation.h"

/**
 * Receives a command and checks if it is valid.
 * Adds a vaccine to the system.
 * @param command -> one line of input
 * @param vaccineTable -> struct with all vaccines
 * @param sys_date -> system date
 * @param flag -> program set to English (0) or Portuguese (1)
 */
void readCommandC(char *command, VaccineHashTable *vaccineTable, Date *sys_date, 
    int flag) { 
    int count_args = 0, aux = 0;
    char* word;
    Vaccine vaccine;

    memset(&vaccine, 0, sizeof(Vaccine));
    
    count_args = readWords(&command, &word, flag);

    while(count_args > 0){
        if(aux == 1) vaccine.batch = strdup(word); /* batch*/
        else if(aux == 2) vaccine.date = readDate(word, vaccine.date); /* date*/  
        else if(aux == 3) vaccine.doses = atoi(word); /* doses */
        else if(aux == 4) vaccine.name = strdup(word); /* name */
        free(word);
        aux++;
        count_args = readWords(&command, &word, flag);
    }

    if(vaccineErrors(vaccine, vaccineTable, sys_date, flag)) {
        free(vaccine.batch);
        free(vaccine.name);
        return;
    }

    /* if there are no errors, add the vaccine */
    addVaccine(vaccineTable, vaccine, flag); 
    printf("%s\n", vaccine.batch);
}

/**
 * Receives a vaccine and checks if it is valid.
 * @param vaccine -> vaccine to be added
 * @param vaccineTable -> struct with all vaccines
 * @param sys_date -> system date
 * @param flag -> program set to English (0) or Portuguese (1)
 * @return TRUE if there is an error, FALSE otherwise
 */
int vaccineErrors(Vaccine vaccine, VaccineHashTable *vaccineTable, Date *sys_date, 
    int flag) {
    if (!existsAvailability(vaccineTable)) {
        printf(flag ? "demasiadas vacinas\n" : "too many vaccines\n");
        return TRUE;
    }

    if (existsBatch(vaccineTable, vaccine.batch)) {
        printf(flag ? "número de lote duplicado\n" : "duplicate batch number\n");
        return TRUE;
    }

    if (!validBatch(vaccine.batch)) {
        printf(flag ? "lote inválido\n" : "invalid batch\n");
        return TRUE; 
    }

    if (!validVaccineName(vaccine.name)) {
        printf(flag ? "nome inválido\n" : "invalid name\n");
        return TRUE;
    }

    if (!validDate(vaccine.date, sys_date, 1)) {
        printf(flag ? "data inválida\n" : "invalid date\n");
        return TRUE;
    }

    if (!validQuantity(vaccine.doses)) {
        printf(flag ? "quantidade inválida\n" : "invalid quantity\n");
        return TRUE; 
    }
    
    return FALSE;
}

/**
 * Receives a command and checks if it is valid. If not, prints the error.
 * Lists all vaccines or a specific one.
 * @param command -> one line of input
 * @param vaccineTable -> struct with all vaccines
 * @param flag -> program set to English (0) or Portuguese (1)
 */
void readCommandL(char *command, VaccineHashTable *vaccineTable, int flag) {
    int count_words = readWordsInLine(command);
    int count_args = 0, aux = 0;

    char* word = NULL;
    count_args = readWords(&command, &word, flag);

    if(count_words == 1){ /* if input = l */
        listAllVaccines(vaccineTable);
        free(word);
        return;
    } else {
        while(count_args > 0){ /* for each vaccine name */
            if(aux != 0) {
                char *vaccine_name = strdup(word);
                listVaccine(vaccineTable, vaccine_name, flag);
                free(vaccine_name);
            }
            aux++;
            free(word);
            count_args = readWords(&command, &word, flag);
        } 
    }
}

/**
 * Receives a command and checks if it is valid. If not, prints the error.
 * Removes availability of a vaccine.
 * @param command -> one line of input
 * @param vaccineTable -> struct with all vaccines
 * @param flag -> program set to English (0) or Portuguese (1)
 */
void readCommandR(char *command, VaccineHashTable *vaccineTable, int flag) {
    int count_args = 0, aux = 0;
    char* word = NULL, *batch = NULL;

    count_args = readWords(&command, &word, flag);

    while(count_args > 0){
        if(aux == 1) batch = strdup(word); /* batch */
        free(word);
        aux++;
        count_args = readWords(&command, &word, flag);
    }

    if (!existsBatch(vaccineTable, batch)) {
        printf(flag ? "%s: lote inexistente\n" : "%s: no such batch\n", batch);
        free(batch);
        return;
    }

    /* in case it has been administered, just print how many applications */
    Vaccine *vaccine = findVaccineByBatch(vaccineTable, batch);
    if(vaccine->applications != 0) {
        printf("%d\n", vaccine->applications);
        vaccine->doses = 0;
        free(batch);
        return;
    }

    /* if never administered, remove availability */
    deleteBatchAvailability(vaccineTable, batch);
}

/**
 * Adds a vaccine to the hash table.
 * @param hashTable -> pointer to the hash table
 * @param vaccine -> vaccine to be added
 * @param flag -> program set to English (0) or Portuguese (1)
 */
void addVaccine(VaccineHashTable *hashTable, Vaccine vaccine, int flag) {
    int indexBatch = hashFunctionBatch(vaccine.batch);  /* index for batch */
    int indexName = hashFunctionName(vaccine.name);  /* index for name */
    VaccineNode *new_node = malloc(sizeof(VaccineNode));
    if (!new_node) noMemory(flag);

    new_node->vaccine = vaccine;  /* copies vaccine */
    new_node->vaccine.applications = 0;
    new_node->next = NULL;  /* in a dynamic array, the next one is null */

    /* inserts in the beggining of the hash */
    if (hashTable->tableBatch[indexBatch] == NULL) {
        hashTable->tableBatch[indexBatch] = new_node;
    } else {
        new_node->next = hashTable->tableBatch[indexBatch];
        hashTable->tableBatch[indexBatch] = new_node;
    }

    VaccineNode *new_node2 = malloc(sizeof(VaccineNode)); /* same logic */
    if (!new_node2) noMemory(flag);
    new_node2->vaccine = vaccine; 
    new_node2->vaccine.applications = 0; 
    new_node2->next = NULL;
    new_node2->next = hashTable->tableName[indexName];  
    hashTable->tableName[indexName] = new_node2;
}

/**
 * Lists all vaccines available.
 * @param hashTable -> pointer to the hash table
 */
void listAllVaccines(VaccineHashTable *hashTable) {
    Vaccine *array;
    int count = collect_vaccines(hashTable, &array, NULL);
    if (count == 0) return; /* no vaccines to print */

    sortingVaccines(array, 0, count - 1); /* sorting vaccines by date */

    /* print from sorted array */
    for (int i = 0; i < count; i++) {
        printf("%s %s %.2d-%.2d-%.4d %d %d\n", array[i].name, array[i].batch,
            array[i].date.day, array[i].date.month, array[i].date.year,
            array[i].doses, array[i].applications);
    }

    free(array); /* free allocated array */
}

/**
 * Lists a specific vaccine.
 * @param hashTable -> pointer to the hash table
 * @param vaccine_name -> name of the vaccine to be listed
 * @param flag -> program set to English (0) or Portuguese (1)
 */
void listVaccine(VaccineHashTable *hashTable, char *vaccine_name, int flag) {
    Vaccine *array;
    int count = collect_vaccines(hashTable, &array, vaccine_name);
    
    if (count == 0) {
        printf(flag ? "%s: vacina inexistente\n" : "%s: no such vaccine\n", 
            vaccine_name);
        return;
    }

    sortingVaccines(array, 0, count - 1); /* sorting vaccine by date */

    /* print from sorted array */
    for (int i = 0; i < count; i++) {
        printf("%s %s %.2d-%.2d-%.4d %d %d\n", array[i].name, 
            array[i].batch, array[i].date.day, 
            array[i].date.month, array[i].date.year, 
            array[i].doses, array[i].applications);
    }

    free(array); /* free allocated array */
}

/**
 * Checks if there is availability for adding more vaccines.
 * @param hashTable -> pointer to the hash table
 * @return TRUE if there is availability, FALSE otherwise
 */
int existsAvailability(VaccineHashTable *hashTable) {
    int count = 0;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        VaccineNode *current = hashTable->tableBatch[i];

        while (current != NULL) {
            count++;
            if (count >= MAX_BATCH) {
                return FALSE; /* exceeded limit */
            }
            current = current->next;
        }
    }

    return TRUE; /* there is still availability */
}

/**
 * Checks if a certain batch exists in the hash table.
 * @param hashTable -> pointer to the hash table
 * @param batch -> batch to be checked
 * @return TRUE if the batch exists, FALSE otherwise
 */
int existsBatch(VaccineHashTable *hashTable, char *batch) {
    int index = hashFunctionBatch(batch); /* index for batch */
    VaccineNode *current = hashTable->tableBatch[index];

    while (current != NULL) {
        if (strcmp(current->vaccine.batch, batch) == 0) {
            return TRUE; /* batch exists */
        }
        current = current->next;
    }

    return FALSE; /* batch not found */
}

/**
 * Checks if a batch is valid.
 * @param batch -> batch to be checked
 * @return TRUE if the batch is valid, FALSE otherwise
 */
int validBatch(char *batch) {
    if(strlen(batch) > MAX_NAME_BATCH) {
        return FALSE;
    }

    /* if it has non hexadecimal capital numbers */
    for (int i = 0; i < (int)strlen(batch); i++) {
        if (!isxdigit(batch[i]) || (isalpha(batch[i]) && !isupper(batch[i]))) {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * Checks if a vaccine name is valid.
 * @param name -> name to be checked
 * @return TRUE if the name is valid, FALSE otherwise
 */
int validVaccineName(char *name) {
    if(strlen(name) > MAX_NAME_VACCINE) {
        return FALSE;
    }

    /* no blank spaces */
    for (int i = 0; i < (int)strlen(name); i++) {
        if (name[i] == ' ' || name[i] == '\n' || name[i] == '\t') {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * Checks if a quantity is valid.
 * @param quantity -> quantity to be checked
 * @return TRUE if the quantity is valid, FALSE otherwise
 */
int validQuantity(int quantity) {
    if(quantity < 0) {
        return FALSE;
    }
    return TRUE;
}

/**
 * Deletes a batch from the hash table.
 * @param hashTable -> pointer to the hash table
 * @param batch -> batch to be deleted
 */
void deleteBatchAvailability(VaccineHashTable *hashTable, char *batch) {
    for (int i = 0; i < TABLE_SIZE; i++) { /* deletes in hash by batch */
        deleteFromTable(&hashTable->tableBatch[i], batch, 0);
    }

    for (int i = 0; i < TABLE_SIZE; i++) { /* deletes in hash by name */
        deleteFromTable(&hashTable->tableName[i], batch, 1);
    }

    free(batch);
}

/**
 * Deletes a node from the linked list in the hash table.
 * @param head -> pointer to the head of the linked list
 * @param batch -> batch to be deleted
 * @param freeFields -> 0 if not to free fields, 1 if to free fields
 */
void deleteFromTable(VaccineNode **head, char *batch, int freeFields) {
    VaccineNode *current = *head, *prev = NULL, *temp;
    while (current) {
        if (strcmp(current->vaccine.batch, batch) == 0) {
            /* prints before it's freed */
            if (!freeFields) printf("%d\n", current->vaccine.applications);
            temp = current;
            current = current->next;
            if (prev == NULL) *head = current;
            else prev->next = current;

            if (freeFields) {
                free(temp->vaccine.batch);
                free(temp->vaccine.name);
            }
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

/**
 * Finds a vaccine by its batch number.
 * @param hashTable -> pointer to the hash table
 * @param batch -> batch number to be searched
 * @return Vaccine* -> pointer to the found vaccine, or NULL if not found
 */
Vaccine* findVaccineByBatch(VaccineHashTable *hashTable, char *batch) {
    VaccineNode *current;
    
    /* iterate through the hash table indexed by batch */
    for (int i = 0; i < TABLE_SIZE; i++) {
        current = hashTable->tableBatch[i];
        
        while (current) {
            if (strcmp(current->vaccine.batch, batch) == 0) {
                return &current->vaccine;
            }
            current = current->next;
        }
    }
    
    /* if no vaccine found */
    return NULL;
}

/**
 * Hash function for batch numbers.
 * @param batch -> batch number to be hashed
 * @return int -> hash value for the batch number
 */
int hashFunctionBatch(char *batch) {
    unsigned long hash = 0;
    
    while (*batch) {
        hash = (hash * 31) + (*batch);
        batch++;
    }

    return hash % TABLE_SIZE;
}

/**
 * Collects vaccines from the hash table into an array.
 * @param ht -> pointer to the hash table
 * @param array -> pointer to the array where vaccines will be stored
 * @return int -> number of vaccines collected
 */
int collect_vaccines(VaccineHashTable *ht, Vaccine **array, char *vaccine_name) {
    int count = 0;
    Vaccine *temp_array = malloc(TABLE_SIZE * sizeof(Vaccine));
    if (!temp_array) return 0;
    
    /* single pass: collect all vaccines */
    for (int i = 0; i < TABLE_SIZE; i++) {
        VaccineNode *node = ht->tableBatch[i];
        while (node) {
            if (!vaccine_name || strcmp(node->vaccine.name, vaccine_name) == 0) {
                temp_array[count++] = node->vaccine;
            }
            node = node->next;
        }
    }
    
    if (count == 0) { /* there is no vaccines */
        free(temp_array);
        return 0;
    }
    
    /* resize array to exact match count */
    *array = realloc(temp_array, count * sizeof(Vaccine));
    if (!*array) {
        free(temp_array);
        return 0;
    }
    
    return count;
}

/**
 * Swaps two vaccines.
 * @param a -> first vaccine
 * @param b -> second vaccine
 */
void swap(Vaccine *a, Vaccine *b) {
    Vaccine temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Partitions the array for a sorting algorithm.
 * @param arr -> array of Vaccine structures
 * @param low -> starting index
 * @param high -> ending index
 * @return int -> partition index
 */
int partition(Vaccine *arr, int low, int high) {
    Vaccine pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        int dateComparison = compareDates(arr[j].date, pivot.date);
        if (dateComparison < 0 || (dateComparison == 0 && strcmp(arr[j].batch, 
            pivot.batch) < 0)) {
            swap(&arr[++i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

/**
 * Sorts an array of Vaccine structures using a sorting algorithm.
 * @param arr -> array of Vaccine structures
 * @param low -> starting index
 * @param high -> ending index
 */
void sortingVaccines(Vaccine *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        sortingVaccines(arr, low, pi - 1);
        sortingVaccines(arr, pi + 1, high);
    }
}

/**
 * Creates a hash table for storing vaccines.
 * @param flag -> program set to English (0) or Portuguese (1)
 * @return VaccineHashTable* -> pointer to the created hash table
 */
VaccineHashTable* createVaccineHashTable(int flag) {
    VaccineHashTable* table = (VaccineHashTable*)malloc(sizeof(VaccineHashTable));

    if (!table) { /* if memory allocation fails */
        noMemory(flag);
    }

    /* initialize positions in the hash table to NULL */
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->tableBatch[i] = NULL;
        table->tableName[i] = NULL;
    }

    return table;
}

/**
 * Frees the memory allocated for the hash table and its contents.
 * @param hashTable -> pointer to the hash table to be freed
 */
void freeVaccineHashTable(VaccineHashTable *hashTable) {
    VaccineNode *current, *temp;

    /* free all information in hash table indexed by batch */
    for (int i = 0; i < TABLE_SIZE; i++) {
        current = hashTable->tableBatch[i];
        while (current) {
            temp = current;
            current = current->next;
            if (temp->vaccine.batch) free(temp->vaccine.batch);
            if (temp->vaccine.name) free(temp->vaccine.name);
            if(temp) free(temp);
        }
    }

    /* free all information in hash table indexed by name */
    for (int i = 0; i < TABLE_SIZE; i++) {
        current = hashTable->tableName[i];
        while (current) {
            temp = current;
            current = current->next;
            /* other data has already been freed in the other table */
            if(temp) free(temp);
        }
    }

    free(hashTable); /* frees struct */
}