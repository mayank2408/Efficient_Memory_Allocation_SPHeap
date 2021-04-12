#include <stdio.h>
#include<stdlib.h>
#include"header2.h"
#include"header1.h"
#define epsilon 0.002


void print_linked_list(struct Node* header){
	for (struct Node *ptr = header; ptr!=NULL; ptr = ptr->Next){
		printf("(%d,%.2f)  ", ptr -> power,ptr->value);
		// printf("\n");
	}
printf("\n");
}
void add_element(struct Node** h, int power,float value){
	struct Node* ptr;
	ptr = (struct Node *)my_malloc(sizeof(struct Node));
	ptr->Next = *h;
	(*h)->Prev=ptr;
	ptr->power = power;
	ptr->value=value; 
	*h = ptr;
}
void add_element_opposite(struct Node** h, int power,float value){
	struct Node* ptr;
	ptr = (struct Node *)my_malloc(sizeof(struct Node));
	(*h)->Next = ptr;
	ptr->power = power;
	ptr->value=value;
	ptr->Prev=*h;
	ptr->Next=NULL;
	*h=ptr; 
}
void delete(struct Node *a){
	struct Node *temp;
	while(a!=NULL){
		temp=a->Next;
		my_dealloc(a);
		a=temp;

	}

}



struct answer divide(struct Node *a,struct Node *b){
	struct answer ans;
	struct Node* quotient=(struct Node*)my_malloc(sizeof(struct Node)) ;
	struct Node* quotient_final=quotient ;
	struct Node* temp1=a ;
	struct Node* remainder=a;
	struct Node* temp2=b ;	
	struct Node* remainder_temp;
	// struct Node* remainder_temp_next ;
	// struct Node* temp1;
	// struct Node* temp3=NULL;
	float multiplier;
	int power_diff;
	int m=0;
	
	while((remainder->power)>=(b->power)){
		temp2=b;
		temp1=remainder;
		// print_linked_list(remainder);
		power_diff=(remainder->power)-(b->power);
		multiplier=(remainder->value)/(b->value);
		int power=0;
		float value=0;
		int d1=0;
		int d2=0;
		struct Node* temp3=(struct Node*)my_malloc(sizeof(struct Node)) ;
		temp3->Next=NULL;
		remainder_temp=temp3;
		int n=0;
		while((temp1!=NULL)|(temp2!=NULL)){
			if (temp1==NULL) d1=1;
			if (temp2==NULL) d2=1;
			if((!d1)&(!d2)){

				if ((temp1->power)==(temp2->power+power_diff)){
					value=temp1->value-multiplier*temp2->value;
					power=temp1->power;
					// printf("val:%f\n",value );
					if ((value>epsilon)|(value<-epsilon)){
						if (n==0){temp3->power=power;
							temp3->value=value;
							n=1;
						}
						else add_element_opposite(&temp3,power,value);
						// printf("yo3\n");
					}
					temp1=temp1->Next;
					// printf("hi\n");
					temp2=temp2->Next;
					// printf("hi2\n");
				}

				else if ((temp1->power)>(temp2->power+power_diff)){
					value = temp1->value;
					power = temp1->power;
					if ((value>epsilon)|(value<(-epsilon))){
						if (n==0){temp3->power=power;
							temp3->value=value;
							n=1;
						}
						else add_element_opposite(&temp3,power,value);
					}
					temp1=temp1->Next;
				}
				else if ((temp1->power)<(temp2->power+power_diff)){
					value = -multiplier*temp2->value;
					power = temp2->power+power_diff;
					if ((value>epsilon)|(value<(-epsilon))){
						if (n==0){temp3->power=power;
							temp3->value=value;
							n=1;
						}
						else add_element_opposite(&temp3,power,value);
					}
					temp2=temp2->Next;
				}




		}
			

		else if ((!d1)&(d2)){
			value = temp1->value;
			power = temp1->power;
			if ((value>epsilon)|(value<(-epsilon))){
				if (n==0){temp3->power=power;
					temp3->value=value;
					n=1;
				}
				else add_element_opposite(&temp3,power,value);
			}
			temp1=temp1->Next;
		}
		else if ((!d2)&(d1)){
			value = -multiplier*temp2->value;
			power = temp2->power+power_diff;
			if ((value>epsilon)|(value<(-epsilon))){
				if (n==0){temp3->power=power;
					temp3->value=value;
					n=1;
				}
				else add_element_opposite(&temp3,power,value);
			}
			temp2=temp2->Next;
			}
			


		}
		// printf("hello\n");
		// print_linked_list(quotient);
		// printf("ho\n");
		delete(remainder);
		remainder=remainder_temp;
		temp3=NULL;
		if (m==0){quotient->power=power_diff;
				quotient->value=multiplier;
				m=1;
				// printf("pow:%d\n",power_diff);
				// printf("mul:%f\n",multiplier );
			}
			
			else {
				// printf("pow:%d\n",power_diff);
				// printf("mul:%f\n",multiplier );
				add_element_opposite(&quotient,power_diff,multiplier);
		}
	}
	ans.remainder=remainder;
	ans.quotient=quotient_final;
	// print_linked_list(quotient_final);
	// printf("rem:\n");
	// print_linked_list(remainder);
	return ans;




}
struct Node* get_number(){
	struct Node *output=(struct Node*)my_malloc(sizeof(struct Node));
	output->Next=NULL;
	int power;
	float value;
	char c='t';
	int n=0;
	struct Node *temp=output;
	int temp_power=999999;
	while(c!=10){
		scanf("(%d,%f)%c",&power,&value,&c);
		if(temp_power>power){
			if (n==0){temp->power=power;
					temp->value=value;
					n=1;
				}
			else add_element_opposite(&temp,power,value);
			temp_power=power;
		}
		else {printf("wrong input\n");
		exit(0);}
	}
	return output;

}
struct answer evaluate(){
	struct Node* a;
	struct Node* b;
	a=get_number();
	// print_linked_list(a);
	b=get_number();
	// print_linked_list(b);
	struct answer output=divide(a,b);
	return output;
}
