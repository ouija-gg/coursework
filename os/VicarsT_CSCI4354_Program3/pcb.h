typedef enum Status { Wait, Run } Status;

typedef struct pcb {
    int arrival;
    int burst;
    char fifo_name[30];
    Status state;
} pcb;
#define timeunits 100000
