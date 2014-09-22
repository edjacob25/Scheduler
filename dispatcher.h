/*
 * Copyright (c) 2014 Edgar Jacob Rivera Rios
 * 
 * Program: dispatcher.c
 *
 * Author:  Edgar Jacob Rivera Rios
 *
 * Purpose: This program implements the sorting algorithms and the 
 *          data structures for process
 *
 * References:
 *          Based on our own code
 *
 * Restrictions:
 *          The process need 4 parameters to be created: id, arrival
 *			time, cpu burst and priority 
 *
 * Revision history:
 *
 *          September 16 2014 - File created
 *          
 *          September 21 2014 - Added definition for add, print
 *								and order processes
 *
 * Error handling:
 *          On any unrecoverable error, the program exits
 *
 * Notes:
 *          Stiil incomplete
 *
 * 
 */


#include <glib.h> //To use the GList

struct node {
	int id;
	int arrival_time;
	int cpu_burst;
	int priority;
};

typedef struct node process;

GList * CreateProcess(GList *processList, int process_id, int arrival_time, int cpu_burst, int priority, GList *algo);
GList * SortProcessList(GList *processList, char * param);
void PrintProcessList(GList *processList);
void DestroyList(GList *processList);
void FirstCome (GList *processList);
void NonPreemptive(GList *processList,  char * param);
void NonPreemptive(GList *processList,  char * param);
void Preemptive(GList *processList,  char * param);
void Preemptive(GList *processList,  char * param);
void RoundRobin(GList *processList,  char * param);
