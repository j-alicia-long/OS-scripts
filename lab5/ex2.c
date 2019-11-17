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

size_t my_fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {

	size_t totalBytesToRead = nmemb*size;
	
	int bytesRead = 0;

	while (bytesRead < totalBytesToRead) 
	{
		// If at end of stream buffer, call read to refill buffer	
		if (stream->bufIndex == stream->bufferEnd) {
			int bytesLoaded = read(stream->fd, stream->buffer, BUFFER_SIZE);
			if (bytesLoaded <= 0) // EOF
				break;
			stream->bufIndex = 0; // reset buffer index
			stream->bufferEnd = bytesLoaded;
			stream->fileIndex += bytesLoaded;  // shifts file index
		}

		// Copy buffer data to ptr
		memcpy(&(ptr[bytesRead]), &(stream->buffer[stream->bufIndex]), size);

		// Increment Index
		bytesRead += size;
		stream->bufIndex += size;
	}
	
	if (totalBytesToRead > 0 && bytesRead == 0)
		return MY_EOF;

	// Return number of items read
	return bytesRead/size;
}
