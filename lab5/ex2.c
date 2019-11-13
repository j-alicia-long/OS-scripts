/*************************************
 * Lab 5 Ex2
 * Name: 	Jennifer Long
 * Student No: 	E0446263
 * Lab Group: 	11
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"
#include <stdio.h>

size_t my_fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {
	
	int count = 0;
	while (count < (nmemb*size)) {
		// If at end of stream buffer		
		if (stream->index == stream->bufferEnd) {
			// call read to refill buffer
			int bytesRead = read(stream->fd, stream->buffer, 4096);
			if (bytesRead <= 0) // EOF
				break;
			stream->index = 0; // reset buffer index
			stream->bufferEnd = bytesRead;
		}
		// Copy buffer data to ptr
		((char*)ptr)[count] = stream->buffer[stream->index];

		// Increment index
		count++;
		stream->index++;
		printf("Bytes read count: %d\n", count);
	}
	
	return count;
}
