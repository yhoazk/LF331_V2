/*
 * Copyright (C) 2005, Ingo Molnar
 *
 * tsc-dump.c: print out TSC values for one minute
 *
 * Compile with: gcc -Wall -O2 -o tsc-dump tsc-dump.c
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned long long cycles_t;

#define rdtscll()					\
({							\
	cycles_t __t;					\
	__asm__ __volatile__("rdtsc" : "=A" (__t));	\
	__t;						\
})

int main(int argc, char **argv)
{
	int i;
	cycles_t t0, t0_prev = 0, delta = 0;

	printf("dumping TSC values for 1 minute:\n");

	for (i = 0; i < 60; i++) {
		t0 = rdtscll();
		if (t0_prev)
			delta = t0 - t0_prev;
		t0_prev = t0;

		printf("%20Ld / %016Lx [delta: %Ld]\n", t0, t0, delta);
		sleep(1);
	}

	return 0;
}
