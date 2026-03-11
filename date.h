/* File: date.h
Aluno: Constança Fonseca
Nº de Aluno: 106251
Description: file with structs regarding dates */

#ifndef DATA_H
#define DATA_H

#include "general.h"

/* definitions of all functions declared in date.c */
void readCommandT(char *command, Date *sys_date, int flag);

Date initializeDate(int day, int month, int year);

int validDate(Date date, Date *sys_date, int flag);

int isLeapYear(int year);

Date readDate(char* word, Date date);

int compareDates(Date d1, Date d2);

#endif /* DATA_H */