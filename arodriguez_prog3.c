#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "arodriguez_prime.h"

#define BUFFER_SIZE	1000000

unsigned char buffer[BUFFER_SIZE+1];
unsigned char fileName[100];
FILE *primeFile;
FILE *primeThreadFile;
sPRIME_THREAD primeThreadData[MAX_THREADS];

int main(int argc, char *argv[])
{
  int i, bytesRead, bytesWritten;
  pthread_t tid[MAX_THREADS]; 
  pthread_t tidshell;
  pthread_attr_t attr;
  time_t before, after;

  /* Record time at start */
  before = time(NULL);

  /* Setup threads to find prime numbers */
  pthread_attr_init(&attr); //you need this for the pthread_create()
  numThreads = 2;
  int Prime = 500000;
  int midpoint = (Prime / 10) * 7 ;// manipulate midpoint at will 

  /*Results to consider
     first half to thread 1 and the other half to thread 2 =       17 second
     first 60% th thread 1 and the other 40% to thread 2 =         14 second
     first 70% of range thread 1 and the last 30% thread 2 =       11 second (BEST RESULT)
    first 75% of range thread 1 and last 25% of range to thread2 = 14 second
    first 80% of range thread 1 and last 20% of range to thread2 = 18 seconds

*/

  
  //thread1 for prime search
  primeThreadData[0].num = 1;
  primeThreadData[0].low = 0;
  primeThreadData[0].high = midpoint;
  
  //thread2 for prime search  
  primeThreadData[1].num = 2;
  primeThreadData[1].low = midpoint + 1;  
  primeThreadData[1].high = Prime;

  

  //begin ZE SEARCH FOR PRIMES!! Sending struct to each prime_search to have their set of values to
  //work with indepdently
  
  pthread_create(&tid[0], &attr, prime_search,(void *)&primeThreadData[0]);  
  pthread_create(&tid[1], &attr, prime_search,(void *)&primeThreadData[1]);
  

  /* Setup a mini shell thread to provide interactivity with the user */
  pthread_create(&tidshell,&attr,mini_shell,NULL);
  
  // Create primes output file, this file will combine al the primes found in prime search 1 and 2
  primeFile = fopen("primest","w");
  fclose(primeFile);

  /* Wait for the prime search threads to complete and combine their data */
  for(i = 0; i < numThreads; i++)
  {
  	/* Wait for the next thread to complete */
  	pthread_join(tid[i],NULL);
  	/* On thread completion, write its data to "primest" */
    fileName[0] = '\0';
    sprintf(fileName, "primes%d", i+1);		       		// Open the thread's data file
    if((primeThreadFile = fopen(fileName,"r")) == NULL) // checks if file empty AND gives fileName 
      {                                                // to primeThreadFile to now reference later
    	printf("Failed to open file: %s\n", fileName);
    }
    else
    {
    	if((primeFile = fopen("primest","a")) == NULL)	// Open "primest" and gives that file
	  {                                             // to primeFile to now reference later
    		printf("Failed to open file: primest\n");
    	}
    	else
    	{
    		while(feof(primeThreadFile) == 0)
			{
				/* Read from the thread's data file */
				bytesRead = fread(buffer,1,BUFFER_SIZE,primeThreadFile);
				/* Write to the combined file */
				bytesWritten = fwrite(buffer,1,bytesRead,primeFile);
    		}
			fclose(primeFile);
    	}
		fclose(primeThreadFile);
	}
  }
  
  /* Record execution time */
  after = time(NULL);
  printf("\nPrime search done after %ld seconds\n", after-before);


  sleep(6);
  
  /* Lastly, kill the interaction thread */
  pthread_kill(tidshell, SIGKILL); 
  return 0;

}

