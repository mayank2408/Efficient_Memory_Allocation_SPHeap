#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "header1.h"
int current_time=0;

int main(int argc, char const *argv[])
{   
    init_mem_system();
    struct simulation_node *head=NULL;
    int request_size;
    int time;
    char *ptr=NULL;
    char *address=NULL;
    for (int i = 0; 	i < 50000; ++i)
    {
        
    printf("t:  %d\n",i );
    current_time++;
    request_size=uniform(100,8000);
    time=uniform(1,200);
    printf("time  %d\n",time );
    printf("space  %d\n",request_size );
    ptr=delete_node_s(current_time,&head);
    while(ptr!=NULL){
        my_dealloc(ptr);
        ptr=delete_node_s(current_time,&head);

    }

    address = (char *)my_malloc(request_size);

    if (address!=NULL){
        *address='6';
        // total_requested_size+=request_size;
        struct simulation_node *new=create_node_s(time+current_time,address);
        sorted_insert(&head,new);
    }
    else{
        //external fragmentation track
        printf("external fragmentation %f\n",get_external_frag() );
	exit(0);
    }
    }
    printf("internal_fragmentation : %f\n",(float)get_internal_frag()/get_total_requested_size() );

    for (int i = 0; i < 201; ++i)
    {
         ptr=delete_node_s(current_time+i,&head);
     while(ptr!=NULL){
        my_dealloc(ptr);
        ptr=delete_node_s(current_time+i,&head);

    }

    }
    printf("recombinations:%d\n",get_recombinations() );
    printf("splits:%d\n",get_splits() );
    print_ASL();
    return 0;
}
