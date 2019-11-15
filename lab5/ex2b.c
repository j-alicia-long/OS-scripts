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

	size_t totalBytesToRead = nmemb*size;
	size_t copySize;

	int bytesRead = 0;
	while (bytesRead < totalBytesToRead) 
	{
		printf("Index into buffer: %d\n", stream->index);

		// If at end of stream buffer, call read to refill buffer	
		if (stream->index == stream->bufferEnd) {
			int bytesLoaded = read(stream->fd, stream->buffer, BUFFER_SIZE);
			if (bytesLoaded <= 0) // EOF
				break;
			stream->index = 0; // reset buffer index
			stream->bufferEnd = bytesLoaded;
		}

		// Find size of bytes to read
		copySize = totalBytesToRead;
		if (totalBytesToRead > stream->bufferEnd+1)
			copySize = stream->bufferEnd+1;
		printf("Copy size: %d\n", copySize);

		printf("Ptr: %s\n", ptr);
		printf("buffer: %s\n", stream->buffer);

		// Copy buffer data to ptr
		memcpy(ptr, stream->buffer[stream->index], copySize);

		printf("Ptr: %s\n", ptr);
		printf("buffer: %s\n", stream->buffer);

		// Increment index
		bytesRead += copySize;
		stream->index += copySize;

		printf("Bytes read count: %d\n", bytesRead);
		printf("Bytes copied: %s\n", ptr);
	}
	
	if (totalBytesToRead > 0 && bytesRead == 0)
		return MY_EOF;

	return bytesRead/size;
}
