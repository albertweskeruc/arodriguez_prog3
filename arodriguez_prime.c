#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "arodriguez_prime.h"

/* Global variables that are shared */
sPRIME_THREAD primeThreadData[MAX_THREADS];
int	numThreads;


//OBJECTIVE2 - complete 
void *prime_search(void *param)
{
  sPRIME_THREAD* PThread = (sPRIME_THREAD *)param; // The structure passed needs to reference
  FILE *PrimesFound;                              //as structure type pointer in order to be able  
                                                 //access its data we will need for the prime seach
  //this only works for 2 search threads
  //creates file primes1 for thread1
  if(PThread->num == 1){
    PrimesFound = fopen("primes1","w");
  } 
  else{
    //creates file primes2 for thread2
    PrimesFound = fopen("primes2","w");
  }
  
  //values of struct num, low, high, current
  //go through the threads range given and start finding and writing all primes found
  for(PThread->current = PThread->low; PThread->current < PThread->high; PThread->current++){
    //use test_prime() and if it returns 1, write value into the file
    if(test_prime(PThread->current) == 1){
      fprintf(PrimesFound, "%d\n", PThread->current);
    }
  }
  
  fclose(PrimesFound);
  pthread_exit(NULL);
}



void *mini_shell(void *param)
{
	int inputSize, i;
	int threadNum;
	char buff[128];		// Character buffer

	while(1)
	{
		// Request a line of input
		write(1, " -> ", 4);
		// Read the line
		inputSize = read(0, buff, 128);
		// Strip off carriage return
		buff[inputSize-1] = '\0';
		
		if((buff[0] >= '1') && (buff[0] <= '9'))
		{
			// An integer was entered
			threadNum = buff[0] - '0';
			if(threadNum <= numThreads) //numThreads value is in main()
			{
       	printf("Thread %d progress: %d\n", threadNum, primeThreadData[threadNum-1].current);
			}
			else
			{
				printf("Thread %d does not exist\n", threadNum);
			}			
		}
		else
		{		
			if(buff[0] == 'a')
			{
				// Print status for all threads
				for(i = 0; i < numThreads; i++)
				{
			       printf("Thread %d progress: %d\n", i+1, primeThreadData[i].current);
				}
			}
			else
			{
	    printf("Sorry,I only take a single Integer or a lowercase letter A as valid input\n");
			}
		}

		printf("\n");
		fflush(NULL);
	}

	pthread_exit(0);
}






//OBJECTIVE1 -complete
int test_prime(int n)
  
{
  int i;
  
  //start dividing by 2 through half the value of int n
  for(i = 2; i <= n/2; i++){

    //return 0 if the remainder % is 0 meaning NOT prime 
    if(n % i == 0){
      return 0;
    }
  }
  
  //return 1 for Prime if no remainder of 0 solution was found
  return 1;
}
