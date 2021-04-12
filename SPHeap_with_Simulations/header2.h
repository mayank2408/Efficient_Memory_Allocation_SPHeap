#pragma once

struct Node { 
	float value;
	int power ;
	struct Node* Next;
	struct Node* Prev;
	 };

struct answer{
	struct Node *remainder;
	struct Node *quotient;

};

void print_linked_list(struct Node* header);
void add_element(struct Node** h, int power,float value);
void add_element_opposite(struct Node** h, int power,float value);
void delete(struct Node *a);
struct answer divide(struct Node *a,struct Node *b);
struct answer evaluate();
