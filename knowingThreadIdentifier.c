#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 8

void* perform_work( void* argument ) {

	//A data type pthread_t is defined, the pthread_self function is invoked
	//to obtain the thread identifier and print it.
	pthread_t self_id;
	self_id = pthread_self();
	printf( "Hola	 %lu\n", self_id );

	pthread_exit(NULL);
}


int main( int argc, char** argv ) {

	pthread_t threads[ NUM_THREADS ];
	unsigned index;

 	 // create all threads one by one
	 for( index = 0; index < NUM_THREADS; ++index ) {

   	//	 printf("In main: creating thread %d\n", index);
   		 pthread_create( &threads[index], NULL, perform_work, NULL );
 	 }

 	 // wait for each thread to complete
 	 for( index = 0; index < NUM_THREADS; ++index ) {

		 // block until thread 'index' completes
   		 pthread_join( threads[ index ], NULL );
   	//	 printf( "In main: thread %d has completed\n", index );
  	 }

  	// printf( "In main: All threads completed successfully\n" );

  	 pthread_exit(NULL);
}

