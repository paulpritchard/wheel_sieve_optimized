/* Sieve of Pritchard */
/* Optimized version with original indexed linked-list */
/* 1. When deleting with length=N, do fast deletions for values p*f that cannot be factors f in later deletions, */
/*    by just incrementing s[p*f]. */
/*    When gathering the primes, these deleted elements are recognized by being even even and skipped. */
/* 2. Avoid a forward search for the largest factor maxf involved in deleting multiples of p, by */
/*    (a) if length < N, setting maxf = the previous length -1 */
/*    (b) otherwise, do a forward search the first time, then search backward from the previous value,*/
/*        by first skipping deleted values, then skipping values that are too big  */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define N 100

#define Insert(w) {\
    s[maxS] = w;  s[w-1] = maxS;  maxS = w; /* Insert w into W; */\
}\

static uint32_t s[N+2]; /* in 1..N+3 */
uint32_t maxS;
uint32_t length, prev_length, maxf;

void Extend (uint32_t &length, uint32_t n) {
    uint32_t w, x;
    w = 1;
    x = length+1;
    while (x <= n) {
        Insert (x);
        w = s[w]; /* w := next(W,w); */
        x = length + w;
    }
    length = n;
}

void Delete (uint32_t p) {
    uint32_t f, pf, fbound, temp;
    if (length == N) {
        if (maxf == 0) {
            maxf = p;
            while (p*maxf <= length)
                maxf = s[maxf]; /* maxf := next(W,maxf); */
            maxf = s[maxf-1];
        } else {
            temp = s[maxf-1];
            while (s[temp] > maxf) {
                maxf = temp;
                temp = s[maxf-1];
            }
            while (p*maxf > length) {
                maxf = s[maxf-1];
            }
        }
        f = maxf;
        pf = p*f;
        fbound = f;
        while (pf > fbound) {
            /*printf("Lazy deleting %d\n", pf);*/
            s[pf]++; /* Remove p*f from W; */
            f = s[f-1]; /* f := prev(W,f); */
            pf = p*f;
        }
    } else {
        f = prev_length-1;
        pf = p*f;
    }
    while (pf > 1) {
        /*printf("Deleting %d\n", pf);*/
        temp = s[pf-1]; s[temp] = s[pf]; s[s[pf]-1] = temp; /* Remove p*f from W; */
        f = s[f-1]; /* f := prev(W,f); */
        pf = p*f;
    }
}

int main (int argc, char *argv[]) {
    uint32_t k, p, w;
    uint32_t nrPrimes;
    bool printPrimes = (argc == 2 ? 1 : 0);
    printf("N=%d\n",N);
    k = 1;
    maxS = 1; /* W = {1}; */
    length = 2;
    p = 3;
    maxf = 0;
    /* printf("%d\n", 2); */
    nrPrimes = 1; /* Pr = {2}; */
    /* invariant: p = p_(k+1) and W = W_k inter {1,...,N} and length = min(P_k,N) and Pr = the primes up to p_k */
    while (p*p <= N) {
        if (length < N) {
            prev_length = length;
            uint32_t temp = p*length;
            if (N < temp) temp = N;
            Extend (length, temp); /* Extend W,length to minimum of p*length,N; */
            if (length == N) Insert (N+3); /* sentinel */
        }
        Delete (p); 
        ; /* Insert p into Pr; */
        k++; 
        p = s[1]; /* p = next(W, 1); */
    }
    if (length < N) {
        Extend (length, N);
        Insert (N+3); /* sentinel */
    }
    /* count primes */
    w = 2;
    if (printPrimes) printf("%d\n", w);
    nrPrimes = 1;
    w = 3;
    while (w <= N) {
        if (s[w] % 2 == 0) {
            w = s[w]-1;
        } else {
            if (printPrimes) printf("%d\n", w);
            nrPrimes++;
            w = s[w];
        }
    }
    printf("%d primes found\n", nrPrimes);
}
