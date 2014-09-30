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

#include <glib.h>				          /* For use linked lists in GList 	*/
#include <stdio.h>					        	         /* For use printf 	*/
#include "FileIO.h"					    	         /* For error handling  */
#include "dispatcher.h"	   /* Definitions of functions and data structures	*/

/*
 *
 *  Function: CreateProcess
 *
 *  Purpose: Adds a process to the list which is going to be used
 *      
 *  Parameters:
 *            input    The numbers correspondant with the parameters os the process
 *
 *            output   A Glist pointer to the start of the new list
 *
 */
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

/*
 *
 *  Function: SortProcessList
 *
 *  Purpose: Sorts a given Glist of processess by the giver parameter
 *      
 *  Parameters:
 *            input    	A pointer to the start of the list and a paramenter
 *					 	of how its going to be sorted
 *
 *            output   	A Glist pointer to the start of the ordered list
 *
 */
GList * SortProcessList(GList *processList, enum OPTION param) {
	GList *orderedList = NULL, *i, *j;
	process *temp, *tempNext;
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
		else if (param == 3) {
			criteria = temp->timeleft;
		}


		int added = 0;
		if (elements>0)	{ // Is there a element in the new list yet?

			for (j = orderedList; j != NULL; j = j->next) { // Iterate the ordered list
				tempNext = j->data;
				int comparison;
				if (param == 0 ) {
					comparison = tempNext->arrival_time;
				}
				else if (param == 1) {
					comparison = tempNext->cpu_burst;
				}
				else if (param == 2) {
					comparison = tempNext->priority;
				}
				else if (param == 3) {
					comparison= tempNext->timeleft;
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
		printf("\nordered list by %d\n",param);
		PrintProcessList(orderedList);
		printf("\n");
	#endif

	//g_list_free_timeSummation(processList);
	return orderedList;
}

/*
 *
 *  Function: PrintProcessList
 *
 *  Purpose: Prints the list of processes with its attributes
 *      
 *  Parameters:
 *            input    	A pointer to the start of the list
 *
 *            output   	Prints the attributes of all processes in the list 
 *
 */
void PrintProcessList(GList *processList) {
	process *temp;
	GList *i;
	for (i = processList; i != NULL; i = i->next) { // Iterates over the linked list
		temp = i->data;
		//Assigns the variables to a node in the list
		printf("ID: %d ", temp->id);
		printf("AR: %d ", temp->arrival_time);
		printf("CPU: %d ", temp->cpu_burst);
		printf("PRI: %d ", temp->priority);
		printf("LEF: %d\n",temp->timeleft );
	}
}

/*
 *
 *  Function: DestroyList
 *
 *  Purpose: Sorts a given Glist of processess by the giver parameter
 *      
 *  Parameters:
 *            input    	A pointer to the start of the list
 *
 *            output   	None
 *
 */
void DestroyList(GList *processList) {
	g_list_free(processList);
}

/*
 *
 *  Function: FirstCome
 *
 *  Purpose: 	Implements the First Come First Served (FCFS) algorithm and
 *				then prints the Average Waiting Time (AWS) of the processess
 *				in the list
 *      
 *  Parameters:
 *            input    	A pointer to the start of the list
 *
 *            output   	Prints the average waiting time for the process list
 *
 */
void FirstCome (GList *processList) {
	GList *i;
	process *temp;
	int acumulatedTime = 0;
	float timeSummation = 0;
	float numberProcess = 0;
	float average;
	for (i = processList; i != NULL; i=i->next)	{ // Iterates over the list
		temp = i->data;
		if (numberProcess==0)
		{
			acumulatedTime = temp->arrival_time;
		}
		temp->last_runned = acumulatedTime;
		acumulatedTime = acumulatedTime + temp->cpu_burst; // Auments the acumulated time 
		timeSummation = timeSummation + (temp->last_runned - temp->arrival_time); // Adds the calculated tume for the furmula
		numberProcess++;
	}
	average = timeSummation/numberProcess;

	printf("\nFirstCome\n");
	printf("Average time = %2f\n",average);
	#ifdef DEBUG
		printf("%f %f\n",timeSummation,numberProcess);
	#endif
}

/*
 *
 *  Function: NonPreemptive
 *
 *  Purpose: 	Implements the non preemptive algorithm for dispatching and
 *				then prints the Average Waiting Time (AWS) of the processess
 *				in the list
 *      
 *  Parameters:
 *            input    	A pointer to the start of the list and the parameter
 *						which it will use to apply the algorithm
 *
 *            output   	Prints the average waiting time for the process list
 *
 */
void NonPreemptive(GList *processList,  enum OPTION param) {
	int acumulatedTime = 0;
	float timeSummation = 0;
	float numberProcess = 0;
	float average;
	process *temp;
	GList *i = g_list_copy(processList);
	
	// This section calculates the things for the first member and then deletes the link
	temp = i->data;
	acumulatedTime = temp->arrival_time;
	temp->last_runned = acumulatedTime;
	acumulatedTime = acumulatedTime + temp->cpu_burst;
	timeSummation = timeSummation + (temp->last_runned - temp->arrival_time);
	numberProcess++;
	i = g_list_delete_link(i, i);
	
	i = SortProcessList(i, param); // Organizes the left elements by the parameter required
	// The rest of the elements are processed
	for (i = i; i != NULL; i=i->next)	{
		temp = i->data;
		temp->last_runned = acumulatedTime;
		acumulatedTime = acumulatedTime + temp->cpu_burst;
		timeSummation = timeSummation + (temp->last_runned - temp->arrival_time);
		numberProcess++;
	}

	average = timeSummation/numberProcess;
	g_list_free(i);
	if (param==1) {
		printf("\nNon Preemptive cpu_burst\n");
	}
	else if (param==2) {
		printf("\nNon Preemptive priority\n");
	}
	printf("Average time = %2f\n",average);
	#ifdef DEBUG
		printf("%f %f\n",timeSummation,numberProcess);
	#endif
}

/*
 *
 *  Function: Preemptive
 *
 *  Purpose: 	Implements the preemptive algorithm for dispatching and
 *				then prints the Average Waiting Time (AWS) of the processess
 *				in the list
 *      
 *  Parameters:
 *            input    	A pointer to the start of the list and the parameter
 *						which it will use to apply the algorithm
 *
 *            output   	Prints the average waiting time for the process list
 *
 */
void Preemptive(GList *processList,  enum OPTION param) {
	int acumulatedTime = 0;
	float timeSummation = 0;
	float numberProcess = 0;
	float average;
	GList *arr; // List of arrivals
	GList *i = NULL; // Running list
	process *temp, *tempNext;
	
	arr = g_list_copy(processList);
	InitializeList(arr);
	do{
		if (arr!=NULL) { //Is a process spected to arrive?
			temp = arr->data;
			if (i==NULL) {
				acumulatedTime = temp->arrival_time;
			}
			i = g_list_append(i,temp); // Add the process and then order the running list
			
			if (param == 1)
				i = SortProcessList(i,3);
			else
				i = SortProcessList(i,param);

			arr = arr->next; // Advance the list of arrivals to the next item
		}
		if (arr!=NULL) { // Is a process spected to arrive next?
			temp = i->data; // For modify current item
			tempNext = arr->data; // To check when does the new process arrives
			if (temp->timeleft > 0){ // Is this process finished?
				if (temp->timeleft > (tempNext->arrival_time - acumulatedTime )) { // Will the actual process finish before the next process arrives?
					temp->timeleft =temp->timeleft  - (tempNext->arrival_time - acumulatedTime); // If not finish before the new arrival, reduce the time still have to run
					acumulatedTime = acumulatedTime + (tempNext->arrival_time - acumulatedTime); // Aument the acumulated time
					i = g_list_append(i,temp); //append the process to be runned again
				}
				else { // If finishes before the new arrival 
					temp->last_runned = acumulatedTime; // Last time the process runned
					timeSummation = timeSummation + (temp->last_runned - temp->arrival_time  - (temp->cpu_burst - temp->timeleft)); //Add to the counter of the formula
					#ifdef DEBUG
						PrintProcess(temp, acumulatedTime);
					#endif
					acumulatedTime = acumulatedTime + temp->timeleft; // Aument the acumulated time
					temp->timeleft = 0; // Finish the process
					numberProcess++; // Aument the number of processess
				}
			}
		}
		else{ // No more arrivals are expected
			temp = i->data; // To transform the current item
			if (temp->timeleft > 0){ // Is this process finished?
				temp->last_runned = acumulatedTime; // Last time the process runned
				timeSummation = timeSummation + (temp->last_runned - temp->arrival_time  - (temp->cpu_burst - temp->timeleft));
				#ifdef DEBUG
					PrintProcess(temp, acumulatedTime);
				#endif
				acumulatedTime = acumulatedTime + temp->timeleft; //Aument the acumulated time
				temp->timeleft = 0; //Finish the process
				numberProcess++; // Aument the numer of processess
			}
		}
		i = i->next; // Go to the next process in the list

	}while(i!=NULL || arr!= NULL); // Do it until the list is finished
	
	average = timeSummation/numberProcess;  // Calculate the average
	g_list_free(arr); // Destroy the copy of the original list
	g_list_free(i);
	if (param==1) {
		printf("\nPreemptive cpu_burst\n");
	}
	else if (param==2) {
		printf("\nPreemptive priority\n");
	}
	printf("Average time = %2f\n",average);
	#ifdef DEBUG
		printf("%f %f\n",timeSummation,numberProcess);
	#endif
}

/*
 *
 *  Function: RoundRobin
 *
 *  Purpose: 	Implements the Round Robin algorithm for dipatching and
 *				then prints the Average Waiting Time (AWS) of the processess
 *				in the list
 *      
 *  Parameters:
 *            input    	A pointer to the start of the list and the quantum of
 *						which is intented to use
 *
 *            output   	Prints the average waiting time for the process list
 *
 */
void RoundRobin(GList *processList, int quantum){
	int acumulatedTime = 0;
	float timeSummation = 0;
	float numberProcess = 0;
	float average;
	GList *list = g_list_copy(processList),*i,*j;
	InitializeList(list);
	process *temp = list->data, *tempNext;
	acumulatedTime = temp->arrival_time;
	int reins;
	for (i= list; i != NULL; i=i->next) {
		temp = i->data;
		if (temp->timeleft > 0){ // Is the process finished?
			if (temp->timeleft > quantum) { // Is the process going to finish before the quantum expires?
				reins =0;
				temp->timeleft = temp->timeleft - quantum;
				acumulatedTime = acumulatedTime + quantum;

				// This section checks where the process should be reinserted (In case not all the process have arrived)
				for (j=i; j !=NULL; j = j->next) {
					tempNext= j->data;
					if ((tempNext->arrival_time > acumulatedTime)&& (reins==0) ) {
						i = g_list_insert_before(i,j->prev,temp);
						reins++;
						break;
					}
				}
				if (reins==0) {
					i = g_list_append(i,temp);
				}
				// End of section
			}
			else { // Calculates the operations to obtain the data for the formula
				temp->last_runned = acumulatedTime;
				timeSummation = timeSummation + (temp->last_runned - temp->arrival_time  - (temp->cpu_burst - temp->timeleft));
				#ifdef DEBUG
					PrintProcess(temp, acumulatedTime);
				#endif
				acumulatedTime = acumulatedTime + temp->timeleft;
				temp->timeleft = 0;
				numberProcess++;
			}
		}
	}
	average = timeSummation/numberProcess;

	// Destroy the copy of the original list and the iterators
	g_list_free(list); 
	g_list_free(i); 
	g_list_free(j);

	printf("\nRound Robin\n");
	printf("Average time = %2f\n",average);
	#ifdef DEBUG
		printf("%f %f\n",timeSummation,numberProcess);
	#endif
}

/*
 *
 *  Function: PrintProcess
 *
 *  Purpose: 	Prints an indiviual process from the list, displaying most
 *				of its attributes, and also the acumulated time at the moment
 *      
 *  Parameters:
 *            input    	A pointer to the process data and the acumulated time
 *
 *            output   	Prints the attributes of a process
 *
 */
void PrintProcess(process *proc, int acumulatedTime) {
	printf("Process %d\n",proc->id);
	printf("Last Runned: %d    ",proc->last_runned);
	printf("Arrival: %d    ",proc->arrival_time);
	printf("Burst: %d    ",proc->cpu_burst);
	printf("Priority: %d    ",proc->priority);
	printf("Left: %d    ",proc->timeleft);
	printf("Acum: %d\n",acumulatedTime );
}

/*
 *
 *  Function: InitializeList
 *
 *  Purpose: 	Initializes the value of time left in each process of the list
 *				by assigning the cpu burst to it. Used by algorithms such as
 *				preemptive and Round Robin 
 *      
 *  Parameters:
 *            input    	A pointer to the process data and the acumulated time
 *
 *            output   	Prints the attributes of a process
 *
 */
void InitializeList(GList *processList){
	GList *i;
	process *temp;
	for (i = processList; i!=NULL; i=i->next)
	{
		temp = i->data;
		temp->timeleft = temp->cpu_burst;
	}
}