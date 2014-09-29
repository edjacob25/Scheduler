/*
 * Copyright (c) 2014 Edgar Jacob Rivera Rios
 * 
 * Program: dispatcher.c
 *
 * Author:  Edgar Jacob Rivera Rios
 *			Oscar Sanchez Jimenez
 *			Diego Lopez Trevilla 
 *
 * Purpose: This program implements the sorting algorithms and the 
 *          data structures for process
 *
 * References:
 *          Based on our own code
 *
 * Restrictions:
 *          The process need 4 parameters to be created: id, arrival
 *			time, cpu burst and priority.
 *			The methods shall be provided with an ordered list by
 *			arrival time to work properly. This can be achieved by 
 *			calling the function SortProcessList of this file
 *
 * Revision history:
 *
 *          September 16 2014 - File created
 *          
 *          September 21 2014 - Added definition for add, print
 *								and order processes
 *
 *			September 22 2014 - Implemented FCFS and Non Preeemptive
 *
 *			September 29 2014 - Round Robin algorithm implemented
 *
 *			September 29 2014 - Preemptive algorithm implemented and
 *								fixed Round Robin bug
 *
 * Error handling:
 *          None
 *
 * Notes:
 *          Many methods may require refactoring, specially Preemptive
 *			and Round Robin. Also it does not handle voids in the arrival
 * 			time 
 *
 * Repository;
 *			https://github.com/edjacob25/Scheduler
 *
 */

#include <glib.h> //To use the GList

/************************************************************** 
 *             Declare the data type prototypes               * 
 **************************************************************/

struct node {
	int id;
	int arrival_time;
	int cpu_burst;
	int priority;
	int timeleft;
	int last_runned;
};

enum OPTION
{
	ARRIVALTIME,
	CPUBURST,
	PRIORITY,
	TIMELEFT
};

typedef struct node process;

/************************************************************** 
 *             Declare the function prototypes                * 
 **************************************************************/

GList * CreateProcess(GList *processList, int process_id, int arrival_time, int cpu_burst, int priority, GList *algo);
GList * SortProcessList(GList *processList, enum OPTION param);
void PrintProcessList(GList *processList);
void DestroyList(GList *processList);
void FirstCome (GList *processList);
void NonPreemptive(GList *processList,  enum OPTION param);
void Preemptive(GList *processList,  enum OPTION param);
void RoundRobin(GList *processList,  int quantum);
void PrintProcess(process *proc, int acumulatedTime);
void InitializeList(GList *processList);