#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef __DATA_INCLUDED__
#define __DATA_INCLUDED__
#include "dataTypes.c"
#endif
#include "FCFS.c"
#include "SJF.c"
#include "RR.c"

#define PROC_FILE "processes-3"

int process_count;

PCB_Node *new_queue_head;

PCB_Node *terminated_queue_head;

void read_processes(){

    FILE *processes;
    processes = fopen(PROC_FILE, "r");

    PCB_Node *new_queue_tail = (PCB_Node*) malloc(sizeof(PCB_Node));
    new_queue_tail->PCB = (PCB*) malloc(sizeof(PCB));
    
    int id, burst, arrival;
    fscanf(processes, "%d %d %d", &id, &burst, &arrival);
    new_queue_tail->PCB->id = id;
    new_queue_tail->PCB->burst = burst;
    new_queue_tail->PCB->burst_time = burst;
    new_queue_tail->PCB->arrival_time = arrival;
    new_queue_tail->PCB->turnaround_time = 0;
    new_queue_tail->PCB->waiting_time = 0;

    new_queue_head = new_queue_tail;
    process_count = 1;


    while (fscanf(processes, "%d %d %d", &id, &burst, &arrival) != EOF) {
        new_queue_tail->next = (PCB_Node*) malloc(sizeof(PCB_Node));
        new_queue_tail = new_queue_tail->next;
        new_queue_tail->PCB = (PCB*) malloc(sizeof(PCB));
        
        new_queue_tail->PCB->id = id;
        new_queue_tail->PCB->burst = burst;
        new_queue_tail->PCB->burst_time = burst;
        new_queue_tail->PCB->arrival_time = arrival;
        new_queue_tail->PCB->turnaround_time = 0;
        new_queue_tail->PCB->waiting_time = 0;
        
        process_count++;
    }

    fclose(processes);
}

void print_results(){

    PCB_Node *node = terminated_queue_head;
    do{
        printf("%d, %lf, %lf\n", node->PCB->id, node->PCB->waiting_time, node->PCB->turnaround_time);
        node = node->next;
    }while(node != NULL);

}

int main(int argc, char **argv){
    read_processes();

    if (strcmp(argv[1], "FCFS") == 0){
        terminated_queue_head = FCFSSim(new_queue_head, process_count);
        print_results();
    } else if (strcmp(argv[1], "SJF") == 0){
        terminated_queue_head = SJFSim(new_queue_head, process_count);
        print_results();
    } else if (strcmp(argv[1], "RR") == 0){
        terminated_queue_head = RRSim(new_queue_head, process_count);
        print_results();
    } else {
        printf("Bad schedular type\n");
        return 1;
    }


    return 0;
}