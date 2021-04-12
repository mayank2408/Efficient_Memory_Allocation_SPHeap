#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "header1.h"

#define Memory_base_2 28
#define Memory_size ((int)1 << Memory_base_2)

#define ASL_size 56  //1 indexing
int total_requested_size;
int total_allocated_size=0;
int internal_frag=0;
int splits=0;
int recombinations=0;

int get_splits(){
    return splits;
}
int get_recombinations(){
    return recombinations;
}
struct parent_info
{
    int type;
    int k_val;
    int address;
    struct parent_info *next;
};

void init_par_list(struct parent_info **info_list){
    *info_list=NULL;
}
struct parent_info *parent_info_list=NULL;

int get_par_type(int k_val,int address){
    struct parent_info *temp=parent_info_list;
    struct parent_info *prev=NULL;
    int out=-1;
    while(1){
        if (temp->k_val==k_val && temp->address==address)
        {
            out=temp->type;
            if (prev!=NULL)
            {
                prev->next=temp->next;
            }
            else parent_info_list=(parent_info_list)->next;
            free(temp);
            break;
        }
        else{
            prev=temp;
            temp=temp->next;
        }
    }
    return out;

}
void print_l(){
    for (struct parent_info *i=parent_info_list; i !=NULL; i=i->next)
    {
        printf("address:%d  k_val:%d\t",i->address,i->k_val );
    }
    printf("\n");
}

void set_par_type(int k_val,int type,int address){
    struct parent_info *new=(struct parent_info*)malloc(sizeof(struct parent_info));
    new->k_val=k_val;
    new->type=type;
    new->address=address;
    new->next=parent_info_list;
    parent_info_list=new;
    return;
}

struct block_node {
struct block_node *prev, *next;
int address;
} ;

struct queue{
    struct block_node *front,*rear;
};

struct block_node *create_node(int address){
    struct block_node *ptr=(struct block_node *)(malloc(sizeof(struct block_node)));
    ptr->address=address;
    ptr->next=NULL;
    ptr->prev=NULL;
    return ptr;
}

struct central {
short k_val;
short type;
short tag;
} ;



struct queue ASL[ASL_size+1]; 
struct central *central_info;//[Memory_size];


// int ASL_limit;
void *base_ptr;



void enqueue(struct queue *q,int address){
    
    struct block_node *ptr=create_node(address);
    if(q->rear==NULL || q->front==NULL){
        q->front=ptr;
        q->rear=ptr;
        return;
    }
    q->rear->next=ptr;
    ptr->prev=q->rear;
    q->rear=ptr;
    return;

}
int dequeue(struct queue *q){
    if (q->front==NULL)
    {
        return -1;
    }
    int ptr=q->front->address;
    struct block_node *temp=q->front;
    q->front=q->front->next;
    // q->front->prev=NULL;
    free(temp);

    if (q->front==NULL)
    {
        q->rear=NULL;
    }
    else q->front->prev=NULL;
    return ptr;
}

void init_q(struct queue *q){
    q->front=NULL;
    q->rear=NULL;
    return ;
}

int search_remove(struct queue *q,int address){
    if (address<0){
        return -1;
    }

    int out=-1;
    struct block_node *temp=q->front;
    while (temp!=NULL){
        if (temp->address==address)
        {  

            if(temp->next != NULL) temp->next->prev = temp->prev;
            if(temp->prev != NULL) temp->prev->next = temp->next;

            if(temp->prev == NULL) q->front = temp->next;
            if(temp->next == NULL) q->rear = temp->prev;

            out=temp->address;
            free(temp);
            break;
        }
        else
        {
            temp=temp->next;
        }  
    }
return out;
}

void print_asl_list(struct block_node *l){

    struct block_node * temp=l;
    while (temp!=NULL){
        printf("%d\t",temp->address );
        temp=temp->next;
    }
    
}

void print_ASL(){
    for (int i = 0; i < ASL_size+1; ++i)
    {   
       printf("%d: ",i);
       print_asl_list((&ASL[i])->front);
       printf("NULL\n");
    }

}

int asl_index_req( int s) { 
    if (s<1 || s>256*1024*1024)
    {
        return -1;
    }
    if (s==1)
    {
        return 1;
    }
    int x = (int)ceil(log2(s) / log2(2));  
    int y = (int)ceil(log2(s) - log2(3))/log2(2);  
    return x + y + 1;
}


int index_alloc(int a){
    //use ASL here;
    if (a<1)
    {
        return -1;
    }
    int b=-1;
    for(int i=a;i<ASL_size+1;i++){
        if (ASL[i].rear!=NULL){
            b=i;
            if ((a==1) &&(b==2))
            {
                continue;
            }
            return b;
        }
    }
    if (b==ASL_size||b==-1) return -1;

    return b;

}
int get_cval_from_ind(int ind){
    if (ind%2){
        return 3;
    }
    else return 1;
}

void get_bin_3c(int *parent,int *left_c,int *right_c,int *r_add,int *k_val_l,int *k_val_r,int k_val_par,int c_val_par,int par_address){
    if ( c_val_par==3)
    {
        *parent= 2*k_val_par+3;
        *left_c= 2*k_val_par+2;
        *right_c=2*k_val_par;
        *k_val_r=k_val_par;
        *k_val_l=k_val_par+1;
        *r_add=par_address+(2<<k_val_par);
        return;
    }
    if (c_val_par==1)
    {
        *parent= 2*k_val_par;
        *left_c= 2*k_val_par-1;
        *right_c=2*k_val_par-4;
        *k_val_r=k_val_par-2;
        *k_val_l=k_val_par-2;
        *r_add=par_address+(3<<(k_val_par-2));
        return;
    }
}
int get_cval_add(int add,int k_val,int type){
    if (type==11){
        int a=1;
        if ((add>>k_val) & a){
            return 1;
        }
        else
        {   a=3;
            return a;
        }
    }
    return 1;
}

int get_memory(int b,int a){
    //use ASL here;
    if (a>b)
    {
        return -1;
    }
    int memory_add=dequeue(&ASL[b]);
    int k_val=central_info[memory_add].k_val;
    int type=central_info[memory_add].type;
    int c_val;
    int r_child;
    int l_child;
    int k_val_l;
    int k_val_r;
    int parent;
    int r_add;
    int l_add;
   
    if (a!=b)
    {
    
    while (1)
    { //  printf("splitting_index :%d\n",b );
    splits++;
    c_val=get_cval_from_ind(b);
    get_bin_3c(&parent , &l_child , &r_child , &r_add , &k_val_l , &k_val_r , k_val, c_val , memory_add);
    l_add=memory_add;
    if (c_val==1){
        set_par_type(k_val,type,memory_add);
        int rf;
        if (b==4)
        {
            rf=1;
        }
        else
            rf=b-4;
        if (a<=rf && (rf)!=2)//smaller child
        {   
            enqueue(&ASL[l_child],l_add);
            central_info[l_add].k_val=k_val_l;  //push left child
            central_info[l_add].type=11;
            central_info[l_add].tag=0;//tag=0 means free
            memory_add=r_add;
            k_val=k_val_r;
            type=11;

            b=rf;
            if (a==(b))
            {
                break;
            }
            
        }
        else
        {
            enqueue(&ASL[r_child],r_add);
            central_info[r_add].k_val=k_val_r;  //push left child
            central_info[r_add].type=11;
            central_info[r_add].tag=0;//tag=0 means free
            memory_add=l_add;
            k_val=k_val_l;
            type=11;
            b=b-1;
            if (a==b)
            {   
                break;
            }
        }
         
    }

    else if (c_val==3){
        int lf;
        if (b==3)
        {
            lf=1;
        }
        else
            lf=b-3;
        if (a<=(lf)&& lf!=2)//smaller child
        {   
            enqueue(&ASL[l_child],l_add);
            central_info[l_add].k_val=k_val_l;  //push left child
            central_info[l_add].type=01;
            central_info[l_add].tag=0;//tag=0 means free
            // int add_r_child=get_address(right);
            memory_add=r_add;
            k_val=k_val_r;
            type=10;
            b=lf;
            // if (a==(b-4))
            if (a==b)
            {   
                break;
            }
            
        }
        else
        {
            enqueue(&ASL[r_child],r_add);
            central_info[r_add].k_val=k_val_r;  //push left child
            central_info[r_add].type=10;
            central_info[r_add].tag=0;//tag=0 means free
            memory_add=l_add;
            k_val=k_val_l;
            type=01;
            b=b-1;
            if (a==b)
            {   
                break;
            }
        }
                
    }

    }}
    central_info[memory_add].k_val=k_val;
    central_info[memory_add].type=type;
    central_info[memory_add].tag=1;
 return memory_add;
}
int get_size_from_index(int a){
    if (a==1 || a==0)
    {
        return 1;
    }
    else{
        if (a%2==0)
        {
            return (1<<(a/2));
        }
        else 
        {
            return 3*(1<<((a/2)-1));
        }
    }

}
int get_internal_frag(){
    return internal_frag;
}
int get_total_requested_size(){
    return total_requested_size;
}
void * my_malloc(int x){
    if (x<=0){
        return NULL;
    }
    int a=asl_index_req(x);
    int b=index_alloc(a);
    if (b==-1)
    {
        return NULL;
    }
    total_requested_size+=x;
    // printf("requested index : %d\n",a);
    // printf("allocated index : %d\n",b );
    long unsigned int out_add=(long unsigned int)get_memory(b,a);
    internal_frag+=get_size_from_index(a)-x;
    total_allocated_size+=get_size_from_index(a);
    return (base_ptr+out_add);

}
int get_buddy_add(int address,int k_val,int type){
    int buddy_add;//change it for binary
    if (type==01)
    {   int one=1;
        buddy_add= address^(one<<k_val);
    }
    else if (type==10)
    {
        int one=2;
        buddy_add= address^(one<<k_val);
    }
    else
    {
        int one=3;
        buddy_add= address^(one<<k_val);
    }
    
    
    return buddy_add;
    
}

int max_first(int a1,int a2){
    if (a1>a2)
    {       
        return 1;
    }
    else return 0;
}
int get_k_val(int k_val,int address){

    return get_par_type(k_val+2,address);
}
int get_buddy_kval(int k_val,int type){
    if (type==11)
    {
        return k_val;
    }
    else if (type==01)
    {
        return k_val-1;
    }
    else {
        return k_val+1;
    }

}

int get_buddy_type(int type){
    if (type==11)
    {
        return 11;
    }
    else if (type==01)
    {
        return 10;
    }
    else {
        return 1;
    }

}
float get_external_frag(){
    return 1-(float)(total_allocated_size)/(1<<28);
}

void my_dealloc(void *ptr){
    int k_val;
    int type;
    int buddy_add;
    int temp_c;
    int buddy_type;
    if (ptr==NULL){
        return;
    }
    int address=((unsigned long int)(ptr)-(unsigned long int)(base_ptr));
    int buddy_kval;
    if (address<0)
    {	
        exit(0);
    }
    int c_val_for_external_frag= get_cval_add(address,central_info[address].k_val,central_info[address].type);
    total_allocated_size-=c_val_for_external_frag*(1<<central_info[address].k_val);

    while (1)
    {
        k_val=central_info[address].k_val;
        type=central_info[address].type;
        buddy_add=get_buddy_add(address,k_val,type);
        buddy_kval=get_buddy_kval(k_val,type);
        buddy_type=get_buddy_type(type);
        if ((!central_info[buddy_add].tag)&&(buddy_kval==central_info[buddy_add].k_val)&&(buddy_type==central_info[buddy_add].type) &&(k_val!=28))//tag=0 means free
        {   recombinations++;
            if (central_info[buddy_add].type==11)
            {
                temp_c= get_cval_add(buddy_add,central_info[buddy_add].k_val,central_info[buddy_add].type); 
                if(temp_c==1){
                    search_remove(&ASL[2*central_info[buddy_add].k_val],buddy_add);//remove buddy
                    central_info[address].k_val=k_val+2;
                    int one=1;
                    
                    type=get_k_val(k_val,address);
                    central_info[address].type=type;
                    central_info[address].tag=0;
                    
                }
                else 
                {   
                    search_remove(&ASL[2*(central_info[buddy_add].k_val)+3],buddy_add);//remove buddy
                    address=buddy_add;
                    central_info[address].k_val=k_val+2;
                    int one=1;  
                    type=get_k_val(k_val,address);
                    central_info[address].type=type;
                    central_info[address].tag=0;
                }

            }
            else{
                    search_remove(&ASL[2*central_info[buddy_add].k_val],buddy_add);//remove buddy
                    if (max_first(k_val,central_info[buddy_add].k_val))
                    {
                        central_info[address].k_val=k_val-1; 
                    }
                    else
                    {
                        address=buddy_add;
                        central_info[address].k_val=k_val; 
                    }
                    central_info[address].type=11;
                    central_info[address].tag=0;
            }

        }

        else
        {   
            temp_c= get_cval_add(address,k_val,type);
            int index;
            if (temp_c==1)
            {
                index=2*k_val;
            }
            else
            {
                index=2*k_val+3;
            }
            central_info[address].tag=0; 
            enqueue(&ASL[index],address);
            break;
        }
            
    }
        
}
void init_mem_system(){
central_info=(struct central *)malloc(sizeof(struct central)*1024*1024*256);
base_ptr=malloc(sizeof(char)*1024*1024*256);
for (int i = 0; i < (ASL_size+1); i++)
{   
    init_q(&ASL[i]);
}

enqueue(&ASL[ASL_size],0);
central_info[0].tag=0;
central_info[0].type=1;
central_info[0].k_val=28;
}



void sorted_insert(struct simulation_node** head, struct simulation_node* new_node) 
{ 
    if (head==NULL || new_node==NULL){
        printf("wrong_input");
        exit(0);
    }
    struct simulation_node* current=NULL; 
    if ((*head == NULL )|| ((*head)->time >= new_node->time))
    { 
        new_node->next = *head; 
        *head = new_node; 
    } 
    else
    {   
        
        current = *head; 
        while ((current->next!=NULL) && 
               (current->next->time < new_node->time)) 
        { 
            current = current->next; 
        } 
        new_node->next = current->next; 
        current->next = new_node; 
    } 
} 
struct simulation_node *create_node_s(int time,char *address) 
{   
    if (address==NULL){
        printf("wrong_input");
        exit(0);
    }
    struct simulation_node* new_node = (struct simulation_node*) malloc(sizeof(struct simulation_node)); 
    new_node->time  = time; 

    new_node->address=address;
    new_node->next =  NULL; 
    return new_node; 
} 
char *delete_node_s(int time ,struct simulation_node **head ){
    if (head==NULL){
        printf("wrong_input");
        exit(0);
    }
    if (*head==NULL){
        return NULL;
    }
    else
    {
        if (((*head)->time)>time){
            return NULL;
        }
        else
        {
            char *add=(char *)((*head)->address);
            struct simulation_node *current=*head;
            *head=(*head)->next;
            free(current);
            return add;
        }
        
    }

} 
void printList(struct simulation_node *head) 
{ 
    struct simulation_node *temp = head;
    int i=0; 
    while(temp != NULL) 
    {   
        printf(" %d  %p   \n",i, temp->address); 
        temp = temp->next; 
        i++;
    } 
} 

int uniform(int low,int high){
    int out;
    out=(rand()%(high-low+1))+low;
    return out;
}
int exponential_trun(int low,int high,int mean){

    float u=(float)rand()/RAND_MAX;
    float p=low+(-1*mean*log(1-u));
    if (p>high)
    {
        return high;
    }
    else{
        return p;
    }

}
struct simulation_node* initialize(){
    struct simulation_node *head= (struct simulation_node *)(malloc(sizeof(struct simulation_node)));
    return head;
}    
