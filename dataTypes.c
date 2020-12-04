typedef struct {
    int id;
    int burst;
    double burst_time;
    int arrival_time;
    double waiting_time;
    double turnaround_time;
} PCB;

typedef struct PCB_Node{
    PCB *PCB;
    struct PCB_Node *next;
} PCB_Node;