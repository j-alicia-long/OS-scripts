/*************************************
 * Lab 3 Exercise 3
 * Name: 
 * Student No:
 * Lab Group:
 *************************************/
 
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#include "traffic_synchronizer.h"


//Using extern, you can use the global variables num_of_cars and num_of_segments from ex3_runner.c in your code.
extern int num_of_cars;
extern int num_of_segments;
sem_t roundabout_max_cars_mutex; 
sem_t* segment_mutex;
sem_t* segment_queue;

void initialise()
{
    // limit total cars in roundabout
    sem_init(&roundabout_max_cars_mutex, 0, num_of_segments-1);

    // limit one car per segment
    segment_mutex = malloc (sizeof (sem_t) * num_of_segments);
    for (int i = 0; i < num_of_segments; i++) {
        sem_init(&segment_mutex[i], 0, 1);
    }

    // Add queues to prioritize roundabout movement over entry
    segment_queue = malloc (sizeof (sem_t) * num_of_segments);
    for (int i = 0; i < num_of_segments; i++) {
        sem_init(&segment_queue[i], 0, 1);
    }
}

void cleanup()
{
    sem_destroy(&roundabout_max_cars_mutex);

    for (int i = 0; i < num_of_segments; i++) {
        sem_destroy(&segment_mutex[i]);
        sem_destroy(&segment_queue[i]);
    }
}

//This function models a thread 
void* car(void* car) //Why is this void?
{
    // Cast void* to car_struct*
    car_struct* c = (struct car_struct*)car;

    // Check roundabout total car limit
    sem_wait(&roundabout_max_cars_mutex);

    // Check for cars queued to move into segment
    sem_wait(&segment_queue[c->entry_seg]);
    sem_post(&segment_queue[c->entry_seg]);
    // Check if entry segment is empty
    sem_wait(&segment_mutex[c->entry_seg]);
    
    // If coast is clear, enter roundabout
    enter_roundabout(c);

    // While not reached end segment
    while (c->current_seg != c->exit_seg) 
    {
        // Wait in queue to move to next segment
	sem_wait(&segment_queue[NEXT(c->current_seg, num_of_segments)]);
	// Check if next segment is free
        sem_wait(&segment_mutex[NEXT(c->current_seg, num_of_segments)]);

        move_to_next_segment(c);

	// Remove self from queue
	sem_post(&segment_queue[c->current_seg]);
        // Free previous segment
        sem_post(&segment_mutex[PREV(c->current_seg, num_of_segments)]);
        
    }
    
    // Exit roundabout if at end segment
    exit_roundabout(c);  
    sem_post(&segment_mutex[c->exit_seg]);  
    sem_post(&roundabout_max_cars_mutex);

    
    pthread_exit(NULL);
}
