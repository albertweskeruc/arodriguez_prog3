// UT El Paso EE 4374 Assignment 3
// Multi-threaded Prime Number Search
// Author: Michael McGarry

/* Macro definitions */
#define MAX_THREADS		5	// Maximum number of prime search threads

/* Data types */
typedef struct				// Prime search thread data
{
	unsigned int num;		// Prime search thread number
	unsigned int current;	// Number currently evaluating for primality
	unsigned int low;		// Low end of range to test for primality
	unsigned int high;		// High end of range to test for primality
} sPRIME_THREAD;

/* Shared global variables */
extern sPRIME_THREAD primeThreadData[MAX_THREADS];	// Prime search thread data
extern int numThreads;										// Number of prime search threads

/* Function prototypes */
int test_prime(int n);
void *prime_search(void *param);
void *mini_shell(void *param);

