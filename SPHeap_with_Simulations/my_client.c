#include "header2.h"
#include<stdio.h>
#include<stdlib.h>
#include "header1.h"

int main()
{	init_mem_system();
	printf("Enter the space seperated tuple of form(i,a_i) for numbers.\n");
	struct answer ans=evaluate();
	printf("remainder:\n");
	print_linked_list(ans.remainder);
	printf("quotient\n");
	print_linked_list(ans.quotient);
	printf("internal_fragmentation : %f\n",(float)get_internal_frag()/get_total_requested_size() );
	return 0;
}
