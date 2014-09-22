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

#include <glib.h>				          /* For use linked lists in Glist 	*/
#include <stdio.h>					        	         /* For use Printf 	*/
#include "FileIO.h"					    	         /* For error handling  */
#include "dispatcher.h"	   /* Definitions of functions and data structures	*/

GList * CreateProcess(GList *processList, int process_id, int arrival_time, int cpu_burst, int priority, GList *algo) {
	process *temp;
	temp = malloc(sizeof(process));
	temp->id = process_id;
	temp->arrival_time = arrival_time;
	temp->cpu_burst = cpu_burst;
	temp->priority = priority;

	processList = g_list_prepend(processList, temp);
	return processList;
}

GList * SortProcessList(GList *processList, char * param) {
	GList *orderedList = NULL, *i, *j;
	process *temp, *temp2;
	int elements = 0;

	for (i = processList; i != NULL; i = i->next) { // Iterates over the linked list
		temp = i->data;
		int firstArrival = temp->arrival_time;
		int added = 0;
		if (elements>0)	{ // Is there a element in the new list yet?

			for (j = orderedList; j != NULL; j = j->next) { // Iterate the ordered list
				temp2 = j->data;
				if ((firstArrival < temp2->arrival_time) && (added == 0)) { 
					// If your arrival time is smaller than the next one, you found your place 
					orderedList = g_list_insert_before(orderedList, j, temp);
					added = 1;
				}
			}

			if (added == 0)	{
				// If you are the biggest one, add in th end 
				orderedList = g_list_append(orderedList, temp);
			}
		}
		else { // Add first element in the new list
			orderedList = g_list_append(orderedList, temp);
		}
		elements++;
	}

	#ifdef DEBUG
		PrintProcessList(processList);
		PrintProcessList(orderedList);
	#endif

	//g_list_free_full(processList);
	return orderedList;
}

void PrintProcessList(GList *processList) {
	process *temp;
	GList *i;
	for (i = processList; i != NULL; i = i->next) { // Iterates over the linked list
		temp = i->data;
		printf("%d ", temp->id);
		printf("%d ", temp->arrival_time);
		printf("%d ", temp->cpu_burst);
		printf("%d\n", temp->priority);
	}
}

void DestroyList(GList *processList) {

}
void FirstCome (GList *processList) {

}
void NonPreemptive(GList *processList,  char * param) {
	
}
void Preemptive(GList *processList,  char * param) {

}
void RoundRobin(GList *processList,  char * param){

}
