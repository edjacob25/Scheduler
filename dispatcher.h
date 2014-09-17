#include <glib.h>

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
