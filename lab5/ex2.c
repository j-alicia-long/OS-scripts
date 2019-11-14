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
	
	int byteCount = 0;
	while (byteCount < totalBytesToRead) {
		// If at end of stream buffer, call read to refill buffer	
		if (stream->index == stream->bufferEnd) {
			int bytesRead = read(stream->fd, stream->buffer, BUFFER_SIZE);
			if (bytesRead <= 0) // EOF
				break;
			stream->index = 0; // reset buffer index
			stream->bufferEnd = bytesRead;
		}
		// Copy buffer data to ptr
		((char*)ptr)[byteCount] = stream->buffer[stream->index];

		// Increment index
		byteCount++;
		stream->index++;
		//printf("Bytes read count: %d\n", byteCount);
	}
	
	if (totalBytesToRead > 0 && byteCount == 0)
		return MY_EOF;
	return byteCount;
}
