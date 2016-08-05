/*
 * File: main.c
 *
 * Main entry point of program, as well as a simple demonstration
 * of stm's capabilities.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */


#include "stm.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


/*
 * th_run: Run an example transaction.
 *
 * When given an atom of an integer, this transaction
 * will set the atom's value to 1 if it is 0 and 2
 * otherwise. If race conditions were possible,
 * many transactions could set the value to 1 at once
 * while it is still 0, but with STM this is never
 * possible.
 */
void *th_run(void *at) {
    atom_t *atom = (atom_t *) at;
    int y, *z;
    StartTransaction(trans);
    ReadAtom(*atom, &y);
    z = stm_malloc(sizeof(int));
    if(y == 0)
        *z = 1;
    else
        *z = 2;
    WriteAtom(*atom, z);
    stm_free(z);
    EndTransaction(trans);
}


int main() {
    int y = 0;
    atom_t atom = atomize(&y);
    pthread_t th1, th2;
    th1 = pthread_create();
    th2 = pthread_create();
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    printf("%d\n", y);
}


