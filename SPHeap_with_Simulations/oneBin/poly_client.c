#include "one_bin_head.h"
#include <stdio.h>
#include <stdlib.h>
#include "poly_head.h"

int main()
{	init_mem(sizeof(struct Node));
	printf("Enter the space seperated tuple of form(i,a_i) for numbers.\n");
	struct answer ans=evaluate();
	printf("remainder:\n");
	print_linked_list(ans.remainder);
	printf("quotient\n");
	print_linked_list(ans.quotient);
	return 0;
}
