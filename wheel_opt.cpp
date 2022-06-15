/* Sieve of Pritchard */
/* optimized version with lazy deletions */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define N 1000000

#define Insert(w) {\
    s[maxS] = w;  s[w-1] = maxS;  maxS = w; /* Insert w into W; */\
}

#define Extend(length, n) {\
    uint32_t w, x;\
    w = 1;\
    x = length+1;\
    while (x <= n) {\
        Insert (x);\
        w = s[w]; /* w := next(W,w); */\
        x = length + w;\
	}\
    length = n;\
}

#define Delete(p) {\
    uint32_t w, pw, wbound, temp2;\
    w = p;\
    while (p*w <= length)\
        w = s[w]; /* w := next(W,w); */\
	w = s[w-1];\
	pw = p*w;\
	if (length == N) {\
	    wbound = w;\
	    while (pw > wbound) {\
	        /* printf("Lazy deleting %d\n", pw); fflush(stdout);*/\
            s[pw]++; /* Remove p*w from W; */\
            w = s[w-1]; /* w := prev(W,w); */\
		    pw = p*w;\
	    }\
	}\
	while (pw > 1) {\
 	    /*printf("Deleting %d\n", pw); fflush(stdout);*/\
        temp2 = s[pw-1]; s[temp2] = s[pw]; s[s[pw]-1] = temp2; /* Remove p*w from W; */\
        w = s[w-1]; /* w := prev(W,w); */\
		pw = p*w;\
	}\
}

int main (int argc, char *argv[]) {
    static uint32_t s[N+2]; /* in 1..N+3 */
	uint32_t maxS;
    uint32_t k, length, p, w;
	uint32_t nrPrimes;
	printf("N=%d\n",N);
    k = 1;
    maxS = 1; /* W = {1}; */
    length = 2;
    p = 3;
	/* printf("%d\n", 2); */
    nrPrimes = 1; /* Pr = {2}; */
    /* invariant: p = p_(k+1) and W = W_k inter {1,...,N} and length = min(P_k,N) and Pr = the primes up to p_k */
    while (p*p <= N) {
	    /*printf("p=%d\n", p);*/
		nrPrimes++;
        if (length < N) {
			uint32_t temp = p*length;
			if (N < temp) temp = N;
            Extend (length, temp); /* Extend W,length to minimum of p*length,N; */
			if (length == N) Insert (N+3); /* sentinel */
		}
        Delete (p); 
        /* Insert p into Pr; */
        k++; 
        p = s[1]; /* p = next(W, 1); */
	}
    if (length < N) {
        Extend (length, N);
		Insert (N+3); /* sentinel */
	}
    /* print primes */
	w = p;
	while (w <= N) {
		if (s[w] % 2 == 0) {
			w = s[w]-1;
		} else {
		    /*printf("%d\n", w);*/
		    nrPrimes++;
			w = s[w];
		}
	}
	printf("%d primes found\n", nrPrimes);
}