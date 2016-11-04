// Matthew Clark
// Producer/Consumer program

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define BUFFER_SIZE 10	//size of the storage buffer
#define MAX 500			//max number to count to
#define NUM_PRODUCERS 5	//number of producer threads
#define NUM_CONSUMERS 4	//number of consumer threads

//binary stuff for me 
#define true 1
#define false 0

int n = 0;
int buffer[BUFFER_SIZE];
int in = 0, out = 0;
sem_t empty, full, mutex;

void *producer(void *arg) {
	int isMax = false;

	do {
		sem_wait(&empty);
		sem_wait(&mutex);
		// critical section
		if (n <= MAX) {
			buffer[in] = n;
			n++;
			in = (in + 1) % BUFFER_SIZE;
		} else {
			isMax = true;
		}
		sem_post(&mutex);
		sem_post(&full);
	} while (!isMax);
}

void *consumer(void *arg) {
	int isMax = false;

	do {
		sem_wait(&full);
		sem_wait(&mutex);
		// critical section
		int o = buffer[out];
		if (o < MAX) {
			out = (out + 1) % BUFFER_SIZE;
		} else {
			isMax = true;
		}
		sem_post(&mutex);
		sem_post(&empty);

		// consume the item in nextConsumed
		printf("%i, ", o);

	} while (!isMax);
}

int main(char argc, char* argv) {

	//initialize semaphores
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1);

	//loop counter for non-C99 mode
	int i;

	//producer thread creation
	pthread_t t_producer[5];
	for (i = 0; i < NUM_PRODUCERS; i++) {
		long t_p;
		int r_p = pthread_create(&t_producer[i], NULL, producer, (void *)t_p);
		if (r_p) {
			printf("ERROR in pthread creation: %d\n", r_p);
			return(-1);
		}
	}

	//consumer thread creation
	pthread_t t_consumer[NUM_CONSUMERS];
	for (i = 0; i < NUM_CONSUMERS; i++) {
		long t_c;
		int r_c = pthread_create(&t_consumer[i], NULL, consumer, (void *)t_c);
		if (r_c) {
			printf("ERROR in pthread creation: %d\n", r_c);
			return(-1);
		}
	}

	//join all threads (wait to finish)
	for (i = 0; i < NUM_PRODUCERS; i++) {
		pthread_join(t_producer[i], NULL);
	}
	for (i = 0; i < NUM_CONSUMERS; i++) {
		pthread_join(t_consumer[i], NULL);
	}

	//print newline so console doesn't get misaligned
	printf("\n");

	return 0;
}