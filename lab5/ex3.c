/*************************************
 * Lab 5 Ex3
 * Name: 	Jennifer Long
 * Student No: 	E0446263
 * Lab Group: 	11
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {
	
	size_t totalBytesToWrite = nmemb*size;

	// Note: In a+ mode we need to shift the "seek" to the end of the file
	
	// Write the bytes
	int bytesWritten = 0;
	while (bytesWritten < totalBytesToWrite) {
		// If stream buffer is filled, call write to empty buffer		
		if (stream->bufIndex == BUFFER_SIZE-1) {
			int bytesCommitted = write(stream->fd, stream->buffer, BUFFER_SIZE);
			if (bytesCommitted <= 0) // Error
				break;
			stream->bufIndex = 0; // reset buffer index
			stream->fileIndex += bytesCommitted; // shifts file index
		}
		// Copy ptr data to buffer
		//stream->buffer[stream->bufIndex] = ((char*)ptr)[bytesWritten];
		memcpy(&(stream->buffer[stream->bufIndex]), &(ptr[bytesWritten]), size);

		// Increment Index
		bytesWritten += size;
		stream->bufIndex += size;
	}
	
	// Should we commit everything at the end?

	if (totalBytesToWrite > 0 && bytesWritten == 0)
		return MY_EOF;

	// Return number of items written
	return bytesWritten/size;

}
