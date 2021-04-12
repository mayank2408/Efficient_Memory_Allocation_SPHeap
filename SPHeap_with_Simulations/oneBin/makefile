a.out: poly_client.o poly_impl.o one_bin_impl.o
	gcc poly_client.o poly_impl.o one_bin_impl.o 

poly_client.o: poly_impl.c poly_head.h one_bin_head.h
	gcc -c poly_client.c

poly_impl.o: one_bin_impl.c poly_head.h one_bin_head.h
	gcc -c poly_impl.c
one_bin_impl.o: one_bin_impl.c one_bin_head.h
	gcc -c one_bin_impl.c 
