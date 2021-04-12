#include <stdio.h>
#include "one_bin_head.h"
#include <stdlib.h>

#define m_pow_2 28
void* base_address;
int* tag_array;
int size_block=1;
int flag=0;
int number=0;
int last_alloc_index=-1;
void init_mem(int size)
{
    if(size>(1<<m_pow_2) || size<=0)
    {
        printf("size of block should be less than %d\n",(1<<m_pow_2));
        exit(1);
    }
    size_block=size;
    number=(1<<m_pow_2)/size;
    tag_array=(int*)calloc(number,sizeof(int));
    base_address=malloc(1<<m_pow_2);
    flag=1;
}
void my_dealloc(void* address)
{   if (address==NULL){
        return;
    }
    int add_offset=(char*)address-(char*)base_address;
    if((add_offset)%size_block!=0 || tag_array[add_offset/size_block]==0)
    {
        printf("memory address is invalid or not allocated\n");
        exit(0);
    }
    tag_array[add_offset/size_block]=0;
    return ;
}


void* my_malloc()
{
    if(flag==0)
    {
        printf("memory is not initialized\n");
        exit(1);
    }
    int t=0;
    int i=last_alloc_index;
    while (i<number-1)
    {
        if (tag_array[++i]==0)
        {   t=1;
            tag_array[i]=1;
            last_alloc_index=i;
		break;
        }
           
    }
    if (i==number && t==0)
    {   i=0;
        while (i<last_alloc_index-1)
    {
        if (tag_array[++i]==0)
        {   t=1;
            tag_array[i]=1;
            last_alloc_index=i;
		break;
        }
           
    }   
    }
    if(t==0)
    {
        printf("memory not available\n");
        return NULL;
    }
    void* output=(char*)base_address + i*size_block;
    return output;
}

