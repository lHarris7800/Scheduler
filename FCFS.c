#ifndef __DATA_INCLUDED__
#define __DATA_INCLUDED__
#include "dataTypes.c"
#endif

PCB_Node *FCFSSim(PCB_Node *new_queue_head, int proc_count){
    PCB_Node *ready_queue_head = NULL;
    PCB_Node *ready_queue_tail = NULL;

    PCB_Node *terminated_queue_head = NULL;

    int clock = 0;
    int terminated_count = 0;

    PCB_Node *running = NULL;


    while(terminated_count < proc_count){
        PCB_Node* last_PCB = NULL;
        PCB_Node* next_PCB = new_queue_head;

        if(new_queue_head != NULL){
            do{
                if(next_PCB->PCB->arrival_time == clock){
                    //Admite PCB into ready queue
                    
                    //Remove process from new queue
                    if(last_PCB != NULL)
                        last_PCB->next = next_PCB->next;
                    else
                        new_queue_head = new_queue_head->next;    

                    //Add process to ready queue
                    if(ready_queue_head == NULL){
                        ready_queue_head = next_PCB;
                        ready_queue_tail = next_PCB;
                    }else{
                        ready_queue_tail->next = next_PCB;
                        ready_queue_tail = ready_queue_tail->next;
                    }

                    next_PCB = next_PCB->next;
                    ready_queue_tail->next = NULL;

                } else {
                    last_PCB = next_PCB;
                    next_PCB = next_PCB->next;
                }
            } while(next_PCB != NULL);
        }

        if(running == NULL && ready_queue_head != NULL){
            //Scheduler dispatch

            running = ready_queue_head;
            ready_queue_head = ready_queue_head->next;

            running->next = NULL;

            //Perfomance data
            running->PCB->waiting_time = clock - running->PCB->arrival_time;
            running->PCB->turnaround_time = running->PCB->burst_time + running->PCB->waiting_time;
        }

        if(running != NULL)
            running->PCB->burst_time--;

        if(running->PCB->burst_time == 0){
            //exit

            //Add to terminated queue
            running->next = terminated_queue_head;
            terminated_queue_head = running;
            terminated_count++;
            
            running = NULL;
        }

        //Cycle clock
        clock++;
    }

    return terminated_queue_head;
}