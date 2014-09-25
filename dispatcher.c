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
#include <stdio.h>					        	         /* For use printf 	*/
#include "FileIO.h"					    	         /* For error handling  */
#include "dispatcher.h"	   /* Definitions of functions and data structures	*/

GList * CreateProcess(GList *processList, int process_id, int arrival_time, int cpu_burst, int priority, GList *algo) {
	process *temp;
	temp = malloc(sizeof(process));
	temp->id = process_id;
	temp->arrival_time = arrival_time;
	temp->cpu_burst = cpu_burst;
	temp->priority = priority;
	temp->timeleft = cpu_burst;

	processList = g_list_prepend(processList, temp);
	return processList;
}

GList * SortProcessList(GList *processList, enum OPTION param) {
	GList *orderedList = NULL, *i, *j;
	process *temp, *temp2;
	int elements = 0;

	for (i = processList; i != NULL; i = i->next) { // Iterates over the linked list
		temp = i->data;
		//Choses the criteria to sort 
		int criteria;
		if (param == 0 ) {
			criteria = temp->arrival_time;
		}
		else if (param == 1) {
			criteria = temp->cpu_burst;
		}
		else if (param == 2) {
			criteria = temp->priority;
		}


		int added = 0;
		if (elements>0)	{ // Is there a element in the new list yet?

			for (j = orderedList; j != NULL; j = j->next) { // Iterate the ordered list
				temp2 = j->data;
				int comparison;
				if (param == 0 ) {
					comparison = temp2->arrival_time;
				}
				else if (param == 1) {
					comparison = temp2->cpu_burst;
				}
				else if (param == 2) {
					comparison = temp2->priority;
				}

				if ((criteria < comparison) && (added == 0)) { 
					// If your parameter is smaller than the next one, you found your place 
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
		printf("Unordered list\n");
		PrintProcessList(processList);
		printf("\nordered list\n");
		PrintProcessList(orderedList);
		printf("\n");
	#endif

	//g_list_free_timeSummation(processList);
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
	g_list_free(processList);
}

void FirstCome (GList *processList) {
	GList *i;
	process *temp;
	int acumulatedTime = 0;
	float timeSummation = 0;
	float numberProcess = 0;
	float average;
	for (i = processList; i != NULL; i=i->next)	{
		temp = i->data;
		temp->last_runned = acumulatedTime;
		acumulatedTime = acumulatedTime + temp->cpu_burst;
		timeSummation = timeSummation + (temp->last_runned - temp->arrival_time);
		numberProcess++;
	}
	average = timeSummation/numberProcess;

	printf("FirstCome\n");
	printf("Average time = %2f\n",average);
	#ifdef DEBUG
		printf("%f %f\n",timeSummation,numberProcess);
	#endif
}

void NonPreemptive(GList *processList,  enum OPTION param) {
	int acumulatedTime = 0;
	float timeSummation = 0;
	float numberProcess = 0;
	float average;
	process *temp;
	GList *i = g_list_copy(processList);
	
	//First member;
	temp = i->data;
	acumulatedTime = temp->arrival_time;
	temp->last_runned = acumulatedTime;
	acumulatedTime = acumulatedTime + temp->cpu_burst;
	timeSummation = timeSummation + (temp->last_runned - temp->arrival_time);
	numberProcess++;
	i = g_list_delete_link(i, i);
	
	i = SortProcessList(i, param);
	// Rest of the elements
	for (i = i; i != NULL; i=i->next)	{
		temp = i->data;
		temp->last_runned = acumulatedTime;
		acumulatedTime = acumulatedTime + temp->cpu_burst;
		timeSummation = timeSummation + (temp->last_runned - temp->arrival_time);
		numberProcess++;
	}

	average = timeSummation/numberProcess;
	g_list_free(i);
	printf("Non Preemptive %d\n", param);
	printf("Average time = %2f\n",average);
	#ifdef DEBUG
		printf("%f %f\n",timeSummation,numberProcess);
	#endif
}

void Preemptive(GList *processList,  enum OPTION param) {

}

void RoundRobin(GList *processList, int quantum){
	int acumulatedTime = 0;
	float timeSummation = 0;
	float numberProcess = 0;
	float average;
	GList *list = g_list_copy(processList),*i;
	process *temp = list->data;
	acumulatedTime = temp->arrival_time;
	for (i= list; i != NULL; i=i->next) {
		temp = i->data;
		if (temp->timeleft > 0){
			if (temp->timeleft > quantum) {
				temp->timeleft = temp->timeleft - quantum;
				acumulatedTime = acumulatedTime + quantum;
				i = g_list_append(i, temp);
			}
			else {
				temp->last_runned = acumulatedTime;
				timeSummation = timeSummation + (temp->last_runned - temp->arrival_time  - (temp->cpu_burst - temp->timeleft));
				#ifdef DEBUG
					PrintProcess(temp);
				#endif
				acumulatedTime = acumulatedTime + temp->timeleft;
				temp->timeleft = 0;
				numberProcess++;
			}
		}
	}
	average = timeSummation/numberProcess;
	g_list_free(i);
	printf("Round Robin\n");
	printf("Average time = %2f\n",average);
	#ifdef DEBUG
		printf("%f %f\n",timeSummation,numberProcess);
	#endif
}

void PrintProcess(process *proc) {
	printf("Process %d\n",proc->id);
	printf("Last Runned: %d    ",proc->last_runned);
	printf("Arrival: %d    ",proc->arrival_time);
	printf("Burst: %d    ",proc->cpu_burst);
	printf("Left: %d    \n",proc->timeleft);
}
