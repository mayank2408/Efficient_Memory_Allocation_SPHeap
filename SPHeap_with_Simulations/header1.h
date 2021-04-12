#include <stdio.h>
// static int total_requested_size=0;
struct simulation_node{
    int time;
    char * address;
    struct simulation_node *next;
};
int get_total_requested_size();
void print_ASL();
int get_splits();
int get_recombinations();	
int get_internal_frag();
float get_external_frag();
void * my_malloc(int x);
void my_dealloc(void *ptr);
void init_mem_system();
void sorted_insert(struct simulation_node** head, struct simulation_node* new_node);
struct simulation_node *create_node_s(int time,char *address);
char *delete_node_s(int time ,struct simulation_node **head );
int uniform(int low,int high);
int exponential_trun(int low,int high,int mean);
struct simulation_node* initialize();
