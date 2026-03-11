/* Ficheiro: main.c
Aluno: Constança Fonseca
Nº de Aluno: 106251
Description: main function to read commands, initialize and end the system. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "general.h"
#include "vaccine.h"
#include "date.h"
#include "inoculation.h"

#define MAX_COMMAND 65535

/**
 * Function to free the memory allocated for the system.
 * @param vaccineTable -> The hash table of vaccines.
 * @param inoculationSystem -> The inoculation system.
 */
void freeSystem(VaccineHashTable *vaccineTable, InoculationtionSystem 
    *inoculationSystem) {
    freeVaccineHashTable(vaccineTable);
    freeInoculationSystem(inoculationSystem);
}

/**
 * Function to process the command read from the user.
 * @param command -> The command read from the user.
 * @param vaccineTable -> The hash table of vaccines.
 * @param inoculationSystem -> The inoculation system.
 * @param sys_date -> The current date of the system.
 * @param flag -> program set to English (0) or Portuguese (1)
 */
void processCommand(char *command, VaccineHashTable *vaccineTable, 
    InoculationtionSystem *inoculationSystem, Date *sys_date, int flag) {
    switch (command[0]) {
        case 'c': /* Add a new batch of a vaccine */
            readCommandC(command, vaccineTable, sys_date, flag);
            break;
        case 'l': /* List available vaccines */
            readCommandL(command, vaccineTable, flag);
            break;
        case 'a': /* Administer a vaccine dose to a patient */
            readCommandA(command, inoculationSystem, vaccineTable, sys_date, flag);
            break;
        case 'r': /* Remove a vaccine from availability */
            readCommandR(command, vaccineTable, flag);
            break;
        case 'd': /* Delete vaccine application records */
            readCommandD(command, inoculationSystem, vaccineTable, flag, sys_date);
            break;
        case 'u': /* List a patient's vaccine applications */
            readCommandU(command, inoculationSystem, flag);
            break;
        case 't': /* Advance the simulated time */
            readCommandT(command, sys_date, flag);
            break;
        case 'q': /* Quit the program */
            free(command);
            freeSystem(vaccineTable, inoculationSystem);
            exit(0);
    }
}

int main (int argc, char *argv[]) {
    /* if there's an arg then program is set to Portuguese */
    int flag = (argc > 1) ? 1 : 0;
    (void) argv;
    char *command = createVecCommand(MAX_COMMAND, flag); 

    VaccineHashTable *vaccineTable = createVaccineHashTable(flag);
    InoculationtionSystem *inoculationSystem = createInoculationSystem(flag);
    Date sys_date = initializeDate(1, 1, 2025);
    
    while (1) {
        readCommand(command, MAX_COMMAND);
        processCommand(command, vaccineTable, inoculationSystem, &sys_date, flag);
    }

    free(command);
    freeSystem(vaccineTable, inoculationSystem);
    return 0;
}