/*************************************
 * Lab 4 Exercise 3
 * Name: 	Jennifer Long
 * Student No: 	E0446263
 * Lab Group: 	11
 *************************************/

// Goal: Allow user process to alloc/de-alloc memory

// Compile:	gcc ex3.c runner.c -o ex3 -pthread
// Run:		./ex3 < ex3_sample1.in

#include <signal.h>
#include <stdio.h>

#include "api.h"

void os_run(int initial_num_pages, page_table *pg_table){
    // The main loop of your memory manager
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGUSR1);
    sigaddset(&signals, SIGUSR2);
    
    // create the pages in the disk first, because every page must be backed by the disk for this lab
    int is_page_allocated[(1<<PAGE_BITS)];

    for (int i=0; i!=initial_num_pages; ++i) {
        disk_create(i);
	is_page_allocated[i] = 1;
    }
    

    // Inverted page table: Mapping of frame index to page index
    int frame_to_page[1<<FRAME_BITS];
    for (int i=0; i!=(1<<FRAME_BITS); ++i) {
        frame_to_page[i] = -1;
    }
    int next_victim_frame = 0; // next frame to be swapped out

    
    while (1) {
        siginfo_t info; // What is this?
        sigwaitinfo(&signals, &info); // If signal received, page fault has occurred


	if (info.si_signo == SIGUSR1) {

	    // retrieve the index of the page that the user program wants
            int const requested_page = info.si_value.sival_int;
        
            if (requested_page == -1) break; //quit signal
	    
	    // If page not allocated, raise a segfault
	    if (is_page_allocated[requested_page] == 0) {
		printf("Requested page %d is not allocated\n", requested_page);
	        union sigval reply_value;
                reply_value.sival_int = 1; // set to 1 if the page is not mapped to the user process (i.e. segfault)
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

	}
	else if (info.si_signo == SIGUSR2){

	    int const requested_page = info.si_value.sival_int;

	    // MMAP
	    if (requested_page == -1) {
		// Choose available page for mapping
    	        int available_page;
	        int next_page_number = initial_num_pages; 
		for (int p=0; p<(1<<PAGE_BITS); p++) {
	            // Find unmapped page (invalid)
	            if (is_page_allocated[p] == 0){
			available_page = p;
	                break;
	            }
	        }

		// Call disk_create to create new page on disk
		disk_create(available_page);
		is_page_allocated[available_page] = 1;

		// Return chosen page to MMU
		union sigval reply_value;
		reply_value.sival_int = available_page; // set to new page 
		sigqueue(info.si_pid, SIGCONT, reply_value);
		continue;		
	    }
	    // MUNMAP
	    else {
		// retrieve the index of the page that the user program wants
                int const requested_page = info.si_value.sival_int;

		// Check if delete memory requested is within range
		if (requested_page >= 0 && requested_page < (1<<PAGE_BITS)) {
		    // Check if delete memory requested is actually allocated
		    if (is_page_allocated[requested_page] == 1) {
			// Call disk_delete to delete page on disk
			pg_table->entries[requested_page].valid = 0;
			is_page_allocated[requested_page] = 0;
			disk_delete(requested_page);
		    }		
		}
	    }
	}


        // tell the MMU that we are done updating the page table
        union sigval reply_value;
        reply_value.sival_int = 0; // set to 0 if the page is successfully loaded, 
        sigqueue(info.si_pid, SIGCONT, reply_value);
    }
}
    
