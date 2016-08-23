/*
 * File: stm.h
 *
 * Main header for all software transactional memory code.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#ifndef STM_H
#define STM_H

#include <stdbool.h>
#include <pthread.h>
#include <glib.h>

/*
 * This system employs Transaction Locking 2 (TL2).
 *
 * Project contains am 'atom' type which consists of an
 * address, a lock and a version number (vlock together). Atomic typed
 * values can be altered and read within transactions without race
 * condition errors; a user can make an atom from a variable with the
 * atomize(&var_name) command.
 *
 * Globally, the the prjoect contains a version clock; whenever an
 * atom is modified, it changes its version to be that of the version
 * clock, and the clock is increased.
 *
 * Upon seeing a start_transaction("NAME") statement, the current thread will
 * enter a transaction of ID "NAME". Each time an atom is read or written
 * (using the read_atom and write_atom methods), the transaction will add
 * the read value to its read set and check that the atom's current version number
 * is less than the transaction's (taken from the version clock). If this is not
 * true, the transaction aborts. The same happens with writes, albeit with the
 * write set; atoms are not actually written to until the final commit, and
 * future reads of an altered atom will use the value in the write set rather
 * than the actual value of the atom itself.
 *
 * Once the transaction ends at the required end_transaction("NAME") statement,
 * the transaction will lock all written atoms and check their version numbers;
 * if any of them are greater than the transaction's, the transaction aborts.
 *
 * The transaction then, atom by atom, locks each read atom, checks its version
 * number, then unlocks it. If any are greater than the transaction's or are
 * locked when tried, the transaction aborts. After this, the atoms are all
 * finally written to in the transaction's commit, and the transaction ends
 * successfully.
 *
 * (If a read address that has already been validated is altered before the
 * current transaction is commited, it will have no influence on the transacion's
 * validity.)
 *
 * Any aborts should basically mean a retry; this can happen endlessly
 * or a certain number of times before a complete erroneous shutdown.
 *
 * NB: Users should not use functions with side effects within the transaction,
 * nor should they spawn multiple threads. If heap memory allocation is needed,
 * users should refer to stm_malloc, stm_realloc and stm_free. Declaring
 * variables within the transaction is also ill-advised.
 *
 * Users should also beware atomizing an address multiple times,
 * as collisions between identical atoms are never checked globally.
 */


/*
 * vlock_t: A combination of a version number and lock for an
 * atom.
 */
typedef struct {
    pthread_mutex_t lock;
    int version_number;
} vlock_t;


/*
 * atom_t: A single atomic address that will be written to and read
 * by transactions. Holds an address, a vlock and the size of the address memory space.
 */
typedef struct {
    void *address;
    vlock_t vlock;
    size_t size;
} atom_t;


atom_t atomize(void *address, size_t size);
void atom_lock(atom_t *atom);
int atom_lock_attempt(atom_t *atom);
void atom_unlock(atom_t *atom);
int atom_get_version(atom_t atom);


/*
 * _stm_global_clock: Global clock and its respective mutex.
 */
extern int _stm_global_clock;
extern pthread_mutex_t _stm_gc_lock;


// This function must be called at the start of the program.
void stm_init();
int stm_get_clock();


/*
 * read_op_t: Single read transaction.
 */
typedef struct {
    atom_t *atom;
    void *dest;
    int version_number;
} read_op_t;


read_op_t read_op_new(atom_t *atom, void *dest, int version_number);
bool read_op_validate(read_op_t *read_op);
void *read_op_read(read_op_t read_op);  // Not responsible for validation.


/*
 * write_op_t: Single write transaction.
 */
typedef struct {
    atom_t *atom;
    void *src;    // Pointer to value to write to atom.
    int version_number;
    size_t src_size;  // Size of value at src.
} write_op_t;


write_op_t write_op_new(atom_t *atom, void *src, int version_number, size_t src_size);
bool write_op_validate(write_op_t *write_op);
void write_op_write(write_op_t write_op);  // Not responsible for validation.


/*
 * readset_t: Set of read operations for an atomic code block.
 */
typedef struct {
    /*
     * read_ops stored as a llist because appending reads and
     * deleting them is common, but volatile access is not needed
     */
    GSList *read_ops;
    int num_read_ops;
} readset_t;


readset_t new_readset();
void readset_append(readset_t readset, read_op_t *read_op);
bool readset_validate_last_read(readset_t readset);
bool readset_validate_all(readset_t readset);
void readset_free_ops(readset_t readset);


/*
 * readset_t: Set of write operations for an atomic code block.
 */
typedef struct {
    GSList *write_ops;  // See readset_t for reason to use llist.
    int num_write_ops;
} writeset_t;


writeset_t new_writeset();
void writeset_append(writeset_t writeset, write_op_t *write_op);
int writeset_lock(writeset_t writeset);           // To be used just before commit.
void writeset_unlock(writeset_t writeset);         // ^
bool writeset_validate_all(writeset_t writeset);    // ^^
bool writeset_validate_last_write(writeset_t writeset);
void writeset_commit(writeset_t writeset);
void writeset_free_ops(writeset_t writeset);


/*
 * transaction_t: State of a currently operating transaction.
 */
typedef struct {
    readset_t readset;
    writeset_t writeset;
    void **malloc_pnts;  // Pointers to malloc'd memory locations during transaction
    char *buf_name;     // Name of transaction and setjmp buffer at transaction's start.
    int version_number;
} transaction_t;


transaction_t transaction_new(char *name);
void transaction_add_read(transaction_t transaction, read_op_t *read_op);
void *transaction_get_read(transaction_t transaction, atom_t *atom);
bool transaction_validate_last_read(transaction_t transaction);  // Returns nonzero if invalid
void transaction_add_write(transaction_t transaction, write_op_t *write_op);
bool transaction_validate_last_write(transaction_t transaction);  // Returns nonzero if invalid
void *transaction_add_malloc(transaction_t transaction, size_t size);
void transaction_add_free(transaction_t transaction, void *pnt);
void transaction_abort(transaction_t transaction);
int transaction_commit(transaction_t transaction);     // Returns nonzero if commit failed


// Utility macro for transaction name.
#define _Trans(TRANS_NAME) __trans_ ## TRANS_NAME  ## __

// Utility macro for buffer name.
#define _Buf(TRANS_NAME) __buf_ ## TRANS_NAME  ## __

// Called by other macros to return to start of transaction.
#define _Abort(TRANS_NAME) longjmp(_Buf(TRANS_NAME))


/*
 * StartTransaction: Begin a transaction with name TRANS_NAME (not a string).
 *
 * Two transactions can have the same name without conflict, as long as they are
 * never in the same scope.
 *
 * Transactions must not be nested, and transactions in the same scope should have different names.
 *
 * This macro must be called on a separate line.
 */
#define StartTransaction(TRANS_NAME) do { \
    transaction_t _Trans(TRANS_NAME) = transaction_new(#TRANS_NAME); \
    jmp_buf _Buf(TRANS_NAME); \
    if(!setjmp(_Buf(TRANS_NAME))) \
        transaction_abort(_Trans(TRANS_NAME)); \
    } while(0)


/*
 * EndTransaction: Ends a transaction with name TRANS_NAME.
 *
 * Must be called in the same code block as its respective StartTransaction.
 *
 * This macro must be called on a separate line.
 */
#define EndTransaction(TRANS_NAME) do { \
    if(transaction_commit(_Trans(TRANS_NAME))) \
        _Abort(TRANS_NAME); \
    } while(0)


/*
 * ReadAtom: Read the value of an atom into an address.
 *
 * Must be called on a separate line. Should be used with caution if called in a separate function,
 * as it uses a longjmp() to abort if need be.
 */
#define ReadAtom(atom, dest, dest_type, TRANS_NAME) do { \
    transaction_add_read(_Trans(TRANS_NAME), read_op_new(atom, (void *) dest, stm_get_clock())); \
    if(!transaction_validate_last_read(_Trans(TRANS_NAME))) \
        _Abort(TRANS_NAME); \
    *(dest) = * (dest_type *) transaction_get_read(_Trans(TRANS_NAME), &(atom));\
    } while(0)


/*
 * WriteAtom: Write a value to an atom from an address.
 *
 * Must be called on a separate line. Should be used with caution if called in a separate function,
 * as it uses a longjmp() to abort if need be.
 */
#define WriteAtom(atom, src, src_type, TRANS_NAME) do { \
    transaction_add_write(_Trans(TRANS_NAME), write_op_new(atom, src, stm_get_clock(), sizeof(src_type))); \
    if(!transaction_validate_last_write(_Trans(TRANS_NAME))) \
        _Abort(TRANS_NAME); \
    } while(0)


// These will simply log the memory allocations performed for cleanup at abort.

#define stm_malloc(size, TRANS_NAME) transaction_add_malloc(_Trans(TRANS_NAME), size)
#define stm_free(pnt, TRANS_NAME) transaction_add_free(_Trans(TRANS_NAME), pnt)


#endif // STM_H

