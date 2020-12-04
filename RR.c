#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUANTUM 2
#define CONTEXT_SWITCH 0.1


PCB_Node *RRSim(PCB_Node *new_queue_head, int proc_count){
    PCB_Node *ready_queue_head = NULL;
    PCB_Node *ready_queue_tail = NULL;

    PCB_Node *ready_queue_switched = NULL;

    PCB_Node *terminated_queue_head = NULL;

    double clock = 0;
    int terminated_count = 0;
    int qLength = 0;

    PCB_Node *running = NULL;

    while(terminated_count < proc_count){
        PCB_Node* last_PCB = NULL;
        PCB_Node* next_PCB = new_queue_head;

        if(new_queue_head != NULL){
            do{
                if(next_PCB->PCB->arrival_time == (int)clock){
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

        //Add last to be switched back to end of ready queue
        if(ready_queue_switched != NULL){
            ready_queue_tail->next = ready_queue_switched;
            ready_queue_tail = ready_queue_switched;

            if(ready_queue_head == NULL)
                ready_queue_head = ready_queue_tail;

            ready_queue_switched = NULL;
        }

        if(running == NULL && ready_queue_head != NULL){
            //Scheduler dispatch

            running = ready_queue_head;
            ready_queue_head = ready_queue_head->next;

            running->next = NULL;
        }


        //Cycle clock
        if(running != NULL){
            qLength++;
            if(running->PCB->burst_time <= 1 - CONTEXT_SWITCH){
                //Context Switch overhead
                clock += running->PCB->burst_time + CONTEXT_SWITCH;
                qLength = 0;
                
                //exit

                //Data logging
                running->PCB->turnaround_time = clock - running->PCB->arrival_time;
                running->PCB->waiting_time = running->PCB->turnaround_time - running->PCB->burst;

                //Add to terminated queue
                running->next = terminated_queue_head;
                terminated_queue_head = running;
                terminated_count++;
                
                running = NULL;
            }else{
                clock += 1;
                if(qLength == QUANTUM){
                    //Context Switch
                    qLength = 0;
                    running->PCB->burst_time -= 1 - CONTEXT_SWITCH;

                    //Add back to end of ready queue
                    running->next = NULL;
                    ready_queue_switched = running;

                    running = NULL;
                } else {
                    running->PCB->burst_time -= 1;
                }
            }
        } else {
            clock += 1;
        }
    }

    return terminated_queue_head;
}