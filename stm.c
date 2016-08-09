/*
 * File: stm.c
 *
 * Currently only source file. Will be split up later.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */


#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <glib.h>
#include "stm.h"


int _stm_global_clock;
pthread_mutex_t _stm_gc_lock;


// Atom functions


/*
 * atomize: Takes an address and produces an atom to wrap
 * around it for transactions.
 */
atom_t atomize(void *address) {
    //
}


/*
 * atom_lock: Locks an atom within a thread.
 * Will block until atom is locked.
 */
void atom_lock(atom_t *atom) {
    //
}


/*
 * atom_lock_attempt: Locks an atom within a thread.
 * Unlike atom_lock, if this lock fails on its first try,
 * it will report an error rather than try again.
 *
 * Returns a nonzero value if failed.
 */
int atom_lock_attempt(atom_t *atom) {
    //
}


/*
 * atom_unlock: Unlock a previously locked atom.
 * Should only use if atom has been locked in this thread.
 */
void atom_unlock(atom_t *atom) {
    //
}


/*
 * atom_get_version: Get the version number of an atom.
 */
int atom_get_version(atom_t atom) {
    //
}


// read_op functions


/*
 * read_op_new: Creates a read operation.
 */
read_op_t read_op_new(atom_t *atom, void *dest) {
    //
}


/*
 * read_op_validate: Checks if read operation is still valid.
 * Returns True if so, False otherwise.
 */
bool read_op_validate(read_op_t *read_op) {
    //
}


/*
 * read_op_read: Read the value pointed to by a read operation
 * into its respective atom.
 *
 * Will not validate the read.
 */
void *read_op_read(read_op_t read_op) {
    //
}


// write_op functions


/*
 * write_op_new: Creates a new write operation.
 */
write_op_t write_op_new(atom_t *atom, void *src) {
    //
}


/*
 * write_op_validate: Checks if a write operation is still valid.
 * Returns True if so, False otherwise.
 */
bool write_op_validate(write_op_t *write_op) {
    //
}


/*
 * write_op_write: Writes the operation's value from an atom to a destination.
 *
 * Does not validate the write operation.
 */
void write_op_write(write_op_t write_op) {
    //
}


// readset functions


readset_t new_readset() {
    //
}


void readset_append(readset_t readset, read_op_t read_op) {
    //
}


bool readset_validate_all(readset_t readset) {
    //
}


void readset_free_ops(readset_t readset) {
    //
}


// writeset functions


writeset_t new_writeset() {
    //
}


void writeset_append(writeset_t writeset, write_op_t write_op) {
    //
}


void writeset_lock(writeset_t *writeset) {
    //
}


void writeset_unlock(writeset_t *writeset) {
    //
}


bool writeset_validate_all(writeset_t writeset) {
    //
}


void writeset_commit(writeset_t writeset) {
    //
}


void writeset_free_ops(writeset_t writeset) {
    //
}


// transaction functions


transaction_t transaction_new(char *name) {
    //
}


void transaction_add_read(transaction_t transaction, read_op_t read_op) {
    //
}


int transaction_validate_last_read(transaction_t transaction) {
    //
}


void transaction_add_write(transaction_t transaction, write_op_t write_op) {
    //
}


int transaction_validate_last_write(transaction_t transaction) {
    //
}


void transaction_add_malloc(transaction_t transaction, void *pnt) {
    //
}


void transaction_abort(transaction_t transaction) {
    //
}


int transaction_commit(transaction_t transaction) {
    //
}


// utility functions


void *stm_malloc(size_t size) {
    //
}


void *stm_realloc(void *pos, size_t size) {
    //
}


void stm_free(void *pos) {
    //
}


