CC = gcc
CFLAGS  = -g -Wall 
 
default:test

btree_mgr.o:btree_mgr.c btree_mgr.h
	$(CC) $(CFLAGS) -c btree_mgr.c 

record_mgr.o:record_mgr.c record_mgr.h
	$(CC) $(CFLAGS) -c record_mgr.c 

buffer_mgr.o: buffer_mgr.c buffer_mgr.h
	$(CC) $(CFLAGS) -c buffer_mgr.c 

buffer_mgr_stat.o: buffer_mgr_stat.c buffer_mgr_stat.h
	$(CC) $(CFLAGS) -c buffer_mgr_stat.c 

storage_mgr.o: storage_mgr.c storage_mgr.h 
	$(CC) $(CFLAGS) -c storage_mgr.c 

dberror.o: dberror.c dberror.h 
	$(CC) $(CFLAGS) -c dberror.c

expr.o: expr.c expr.h 
	$(CC) $(CFLAGS) -c expr.c

test_expr.o:
	$(CC) $(CFLAGS) -o test_expr.o -c test_expr.c

test_assign4_1.o: test_assign4_1.c dberror.h expr.h storage_mgr.h buffer_mgr.h record_mgr.h btree_mgr.h tables.h buffer_mgr_stat.h test_helper.h rm_serializer.c 
	$(CC) $(CFLAGS) -c test_assign4_1.c 

test: test_assign4_1.o storage_mgr.o dberror.o buffer_mgr.o buffer_mgr_stat.o record_mgr.o btree_mgr.o expr.o test_expr.o rm_serializer.c
	$(CC) $(CFLAGS) -o test_assign4_1 test_assign4_1.o storage_mgr.o dberror.o buffer_mgr.o buffer_mgr_stat.o record_mgr.o btree_mgr.o expr.o rm_serializer.c -lm

clean: 
	$(RM) -r test_assign4_1 *.o *~ test_expr *.dSYM


run_test:	
	./test_assign4_1



	

