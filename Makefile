arodriguez_prog3 : arodriguez_prog3.c arodriguez_prime.c arodriguez_prime.h
	gcc -o arodriguez_prog3 arodriguez_prog3.c arodriguez_prime.h arodriguez_prime.c -lpthread
clean :
	rm arodriguez_prog3.o arodriguez_prime.o
