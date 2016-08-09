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


/*
 * new_readset: Creates an empty read set.
 */
readset_t new_readset() {
    //
}


/*
 * readset_append: Append a read operation to the readset.
 *
 * Will not validate this read.
 */
void readset_append(readset_t readset, read_op_t read_op) {
    //
}


/*
 * readset_validate_last_read: Checks whether most recent read is valid.
 * Returns True if so, False otherwise.
 *
 * Is to be used right after appending a read operation.
 */
bool readset_validate_last_read(readset_t readset) {
    //
}


/*
 * readset_validate_all: Check whether all reads are valid.
 * Returns True if all are valid, False otherwise.
 *
 * Is to be used at commit of transaction.
 */
bool readset_validate_all(readset_t readset) {
    //
}


/*
 * readset_free_ops: Frees memory of all read operations in set.
 * The read set can then be freed in the standard free(set) fashion.
 */
void readset_free_ops(readset_t readset) {
    //
}


// writeset functions


/*
 * new_writeset: Create a new empty write set.
 */
writeset_t new_writeset() {
    //
}


/*
 * writeset_append: Add a new write operation to the write set.
 *
 * Does not validate this operation.
 */
void writeset_append(writeset_t writeset, write_op_t write_op) {
    //
}


/*
 * writeset_validate_last_write: Checks whether most recent write is valid.
 * Returns True if so, False otherwise.
 *
 * Is to be used right after appending a write operation.
 */
bool writeset_validate_last_write(writeset_t writeset) {
    //
}


/*
 * writeset_lock: Locks all atoms to be written to by set's write operations.
 * If any locks fail, a nonzero value is returned rather than retrying.
 *
 * To be used at commit of transaction.
 */
int writeset_lock(writeset_t *writeset) {
    //
}


/*
 * writeset_unlock: Unlocks all atoms to be written to by set's write operations.
 */
void writeset_unlock(writeset_t *writeset) {
    //
}


/*
 * writeset_validate_all: Validates every write set operation.
 *
 * Assumes writeset is already locked. To be used at commit.
 */
bool writeset_validate_all(writeset_t writeset) {
    //
}


/*
 * writeset_commit: Commits the write operations to all written atoms.
 *
 * Assumes write set has already been locked.
 */
void writeset_commit(writeset_t writeset) {
    //
}


/*
 * writeset_free_ops: Frees all write operations in the write set.
 *
 * After this, the write set itself can be freed with free(set) if need be.
 */
void writeset_free_ops(writeset_t writeset) {
    //
}


// transaction functions


/*
 * transaction_new: Create a new empty transaction.
 */
transaction_t transaction_new(char *name) {
    //
}


/*
 * transaction_add_read: Adds a new read operation to transaction.
 *
 * Does not validate this read.
 */
void transaction_add_read(transaction_t transaction, read_op_t read_op) {
    //
}


/*
 * transaction_validate_last_read: Validates the most recent read operation in the transaction.
 *
 * Returns nonzero values if invalid.
 */
int transaction_validate_last_read(transaction_t transaction) {
    //
}


/*
 * transaction_add_write: Adds a new write operation to transaction.
 *
 * Does not validate this write.
 */
void transaction_add_write(transaction_t transaction, write_op_t write_op) {
    //
}


/*
 * transaction_validate_last_write: Validates the most recent write operation in the transaction.
 *
 * Returns nonzero values if invalid.
 */
int transaction_validate_last_write(transaction_t transaction) {
    //
}


/*
 * transaction_add_malloc: Malloc memory space of a size, add a record of the
 * transaction using stm_malloc, and return the address of the malloc'd memory space.
 * This allows the transaction to free the allocated memory on abortion.
 */
void *transaction_add_malloc(transaction_t transaction, size_t size) {
    //
}


/*
 * transaction_add_free: Free pointer and remove record of memory being allocated within
 * transaction.
 *
 * Does not work with memory allocated outside of transaction.
 */
void transaction_add_free(transaction_t transaction, void *pnt) {
    //
}


/*
 * transaction_abort: Abort transaction, doing all cleanup needed.
 * This includes freeing read and write operations, along with stm_malloc'd memory.
 *
 * Is not responsible for returning to start of transaction.
 */
void transaction_abort(transaction_t transaction) {
    //
}


/*
 * transaction_commit: Commit all the writes of the transaction.
 *
 * Will do all locking, validating, commiting and unlocking of write set along with read set.
 * Returns a nonzero value if transaction commit fails.
 */
int transaction_commit(transaction_t transaction) {
    //
}

