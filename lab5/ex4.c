/*************************************
 * Lab 5 Ex4
 * Name: 	Jennifer Long
 * Student No: 	E0446263
 * Lab Group: 	11
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

int my_fflush(MY_FILE *stream) {
	// For write mode, commit any changes
	if(stream->mode == O_WRONLY || stream->mode == O_RDWR){
		// Write when buffer is non-empty
		if(stream->bufIndex != 0) {
			int bytesCommitted = write(stream->fd, stream->buffer, stream->bufIndex);
			if (bytesCommitted <= 0)
				return MY_EOF;
			stream->bufIndex = 0; // reset buffer index
			stream->fileIndex += bytesCommitted; // shifts file index
		}
	}

	// Discard buffer
	stream->bufIndex = 0; // reset buffer index
	stream->bufferEnd = 0;

	return 0;
}

int my_fseek(MY_FILE *stream, long offset, int whence) {

	// Set file pointer using lseek
	stream->fileIndex = lseek(stream->fd, offset, whence);

	// Return if successful seek
	return stream->fileIndex;
}
