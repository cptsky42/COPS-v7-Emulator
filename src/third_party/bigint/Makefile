# Makefile for bigint.

CC =		gcc -Wall
CFLAGS =	-O2
#CFLAGS =	-O2 -g
LDFLAGS =	-s
#LDFLAGS =	

all:		bi_factor bic birsa


bigint.a:		bigint_ext.o bigint_int.o
	ar rc bigint.a bigint_ext.o bigint_int.o

bigint_ext.o:		bigint_ext.c bigint.h
	$(CC) $(CFLAGS) -c bigint_ext.c

bigint_int.o:		bigint_int.c bigint.h
	$(CC) $(CFLAGS) -c bigint_int.c


bi_test:		bi_test.o bigint.a
	$(CC) $(LDFLAGS) bi_test.o bigint.a -o bi_test

bi_test.o:		bi_test.c bigint.h
	$(CC) $(CFLAGS) -c bi_test.c


bi_factor:		bi_factor.o bigint.a
	$(CC) $(LDFLAGS) bi_factor.o bigint.a -o bi_factor

bi_factor.o:		bi_factor.c low_primes.h bigint.h
	$(CC) $(CFLAGS) -c bi_factor.c


bic.c bic.h:	bic.y
	bison -d -o bic.c bic.y
bic.o:		bic.c bigint.h
	$(CC) $(CFLAGS) -c bic.c

bic_scan.o:		bic_scan.c bigint.h bic.h
	$(CC) $(CFLAGS) -c bic_scan.c

bic:			bic.o bic_scan.o bigint.a
	$(CC) $(LDFLAGS) bic.o bic_scan.o bigint.a -o bic


birsa:			birsa.o bigint.a
	$(CC) $(LDFLAGS) birsa.o bigint.a -o birsa

birsa.o:		birsa.c bigint.h
	$(CC) $(CFLAGS) -c birsa.c




clean:
	-rm -f bi_test bi_factor bic birsa *.a *.o a.out core *.core core.* bic.c bic.h
