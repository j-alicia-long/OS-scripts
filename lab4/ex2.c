/*************************************
 * Lab 4 Exercise 2
 * Name: 	Jennifer Long
 * Student No: 	E0446263
 * Lab Group: 	11
 *************************************/

// Goal: Allow user process to write to memory

// Compile:	gcc ex2.c runner.c -o ex2 -pthread
// Run:		./ex2 < ex2_sample1.in

#include <signal.h>
#include <stdio.h>

#include "api.h"

void os_run(int initial_num_pages, page_table *pg_table){
    // The main loop of your memory manager
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGUSR1);
    
    // create the pages in the disk first, because every page must be backed by the disk for this lab
    for (int i=0; i!=initial_num_pages; ++i) {
        disk_create(i);
    }
    

    // Mapping of frame index to page index
    int frame_to_page[1<<FRAME_BITS];
    for (int i=0; i!=(1<<FRAME_BITS); ++i) {
        frame_to_page[i] = -1;
    }
    int next_victim_frame = 0; // next frame to be swapped out

    
    while (1) {
        siginfo_t info; // What is this?
        sigwaitinfo(&signals, &info); // If signal received, page fault has occurred
        
        // retrieve the index of the page that the user program wants, or -1 if the user program has terminated
        int const requested_page = info.si_value.sival_int;
        
        if (requested_page == -1) break; //quit signal
	// Validate page index
	if (requested_page < 0 || requested_page >= initial_num_pages){
	// If out-of-range, raise a segfault
	    union sigval reply_value;
            reply_value.sival_int = 1;
            sigqueue(info.si_pid, SIGCONT, reply_value);
	    continue;
	}	

        
        // process the signal, and update the page table as necessary

	// Choose frame for replacement page
	int victim_page;
	while(1){
	    // if frame empty or not referenced, replace it
	    victim_page = frame_to_page[next_victim_frame];
	    if (victim_page == -1) // empty
	        break;
	    if (pg_table->entries[victim_page].referenced == 0){
		pg_table->entries[victim_page].valid = 0;
	        break;
	    }
	    // else check next frame in queue
	    else{
		pg_table->entries[victim_page].referenced = 0;
		next_victim_frame = (next_victim_frame+1) % (1<<FRAME_BITS);
	    }		
	}
	// Write old page to disk if dirty
	if (pg_table->entries[victim_page].dirty){
	    disk_write(next_victim_frame, victim_page);
	    pg_table->entries[victim_page].dirty = 0;
	}

	// Read in new page from disk to victim frame
	disk_read(next_victim_frame, requested_page);
	frame_to_page[next_victim_frame] = requested_page;

	// Update page table
        pg_table->entries[requested_page].valid = 1;
	pg_table->entries[requested_page].frame_index = next_victim_frame;

	// Set next victim to next frame in queue
	next_victim_frame = (next_victim_frame+1) % (1<<FRAME_BITS);




        // tell the MMU that we are done updating the page table
        union sigval reply_value;
        reply_value.sival_int = 0; // set to 0 if the page is successfully loaded, set to 1 if the page is not mapped to the user process (i.e. segfault)
        sigqueue(info.si_pid, SIGCONT, reply_value);
    }
}
    
