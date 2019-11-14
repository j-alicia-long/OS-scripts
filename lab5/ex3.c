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
	int byteCount = 0;
	while (byteCount < totalBytesToWrite) {
		// If stream buffer is filled, call write to empty buffer		
		if (stream->index == BUFFER_SIZE-1) {
			int bytesWritten = write(stream->fd, stream->buffer, BUFFER_SIZE);
			if (bytesWritten <= 0) // Error
				break;
			stream->index = 0; // reset buffer index
		}
		// Copy buffer data to ptr
		stream->buffer[stream->index] = ((char*)ptr)[byteCount];

		// Increment index
		byteCount++;
		stream->index++;
		//printf("Bytes written count: %d\n", byteCount);
	}
	
	// Should we commit everything at the end?

	if (totalBytesToWrite > 0 && byteCount == 0)
		return MY_EOF;
	return byteCount;

}
