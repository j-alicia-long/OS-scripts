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
	// Write when buffer is non-empty
	if((O_WRONLY || O_RDWR) && stream->bufIndex != 0) {
		int bytesCommitted = write(stream->fd, stream->buffer, stream->bufIndex+1);
		if (bytesCommitted <= 0) // Error
			return MY_EOF;
		stream->bufIndex = 0; // reset buffer index
		stream->fileIndex += bytesCommitted; // shifts file index
	}

	// Discard read buffer
	if (O_RDONLY)
		stream->bufferEnd = 0;

	return 0;
}

int my_fseek(MY_FILE *stream, long offset, int whence) {

/*
	// No need to do this manually, really...
	switch (whence) {
	case SEEK_SET: // Rel to start
		stream->fileIndex = offset;
		break;
	case SEEK_CUR:
		stream->fileIndex += offset;
		break;
	case SEEK_END:
		stream->index = BUFFER_SIZE + offset;
		break;
	default:
		// Didn't receive a valid whence
		return MY_EOF;
	}
*/

	// Set file pointer using lseek
	if (lseek(stream->fd, offset, whence) < 0)
		return MY_EOF;

	// Get new pointer location
	stream->fileIndex = ftell(stream->fd);

	// Return if successful seek
	return 0;
}
