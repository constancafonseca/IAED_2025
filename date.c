/* File: date.c
Aluno: Constança Fonseca
Nº de Aluno: 106251
Description: file with every function regarding dates and command T
Managing the system date, comparing and validating dates. */

#include "date.h"
#include "general.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Receives a command and checks if it is valid. If not, prints the error. 
 * Changes the system date.
 * @param command -> one line of input
 * @param sys_date -> system date
 * @param flag -> program set to English (0) or Portuguese (1)
 */
void readCommandT(char *command, Date *sys_date, int flag) {
    int count_args = 0;
    int count_words = readWordsInLine(command);

    char* word;
    Date new_date;

    memset(&new_date, 0, sizeof(Date));

    count_args = readWords(&command, &word, flag);

    if(count_words == 1){ /* if input = t */
        printf("%.2d-%.2d-%.4d\n", sys_date->day, sys_date->month, sys_date->year);
        free(word);
        return;
    }

    while(count_args > 0){ /* for date argument */
        new_date = readDate(word, new_date);
        free(word);  
        word = NULL;
        count_args = readWords(&command, &word, flag);
    } 

    if(!validDate(new_date, sys_date, 1)) {
        printf(flag ? "data inválida\n" : "invalid date\n");
        if(word) free(word);  
        return;
    }

    *sys_date = new_date; /* sets new system date */
    printf("%.2d-%.2d-%.4d\n", new_date.day, new_date.month, new_date.year);
    if(word) free(word);  
}

/**
 * Initializes a date structure with the given day, month, and year.
 * @param day -> day of the date
 * @param month -> month of the date
 * @param year -> year of the date
 * @return Date structure initialized with the given values
 */
Date initializeDate(int day, int month, int year) {
    Date date;
    date.day = day;
    date.month = month;
    date.year = year;
    return date;
}

/**
 * Checks if a date is valid.
 * @param date -> date to be checked
 * @param sys_date -> system date
 * @param flag -> program set to English (0) or Portuguese (1)
 * @return TRUE if the date is valid, FALSE otherwise
 */
int validDate(Date date, Date *sys_date, int flag) {
    if (date.month < 1 || date.month > 12 || date.day < 1)  return FALSE;
    
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (isLeapYear(date.year)) daysInMonth[1] = 29;
    
    if (date.day > daysInMonth[date.month - 1]) return FALSE;
    
    int cmp = compareDates(date, *sys_date);
    /* if date is before or after the system's date */
    if ((flag && cmp < 0) || (!flag && cmp > 0)) return FALSE;
    
    return TRUE; /* valid date */
}

/**
 * Checks if a year is a leap year.
 * @param year -> year to be checked
 * @return TRUE if the year is a leap year, FALSE otherwise
 */
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/**
 * Reads a word from the command and builds a date.
 * @param word -> word to be read
 * @param date -> date to be built
 * @return Date structure with the read values
 */
Date readDate(char* word, Date date){
    sscanf(word, "%d-%d-%d", &date.day, &date.month, &date.year);
    return date;
}

/**
 * Compares two dates.
 * @param d1 -> first date
 * @param d2 -> second date
 * @return negative if d1 < d2, positive if d1 > d2, 0 if equal
 */
int compareDates(Date d1, Date d2) {
    if (d1.year != d2.year) {
        return d1.year - d2.year;
    }
    if (d1.month != d2.month) {
        return d1.month - d2.month; 
    }
    return d1.day - d2.day;
}