/*************************************
 * Lab 5 my_stdio.h
 * Name: 	Jennifer Long
 * Student No: 	E0446263
 * Lab Group: 	11
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

/*
 * CS2106 my_stdio Library
 */

#ifndef MY_STDIO_H
#define MY_STDIO_H

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define MY_EOF (-1)
#define BUFFER_SIZE (4096)

typedef struct {
	int fd;
	// TODO: Define the members of your structure

	// The buffer size (capacity) is 4096 bytes
	char* buffer;
	// Should I use a char pointer or char array? - Dynamic mem is better
	// I shouldn't malloc buffer within struct, right - Yes

	int bufIndex; // Marks current index in buffer (read or write)
	int bufferEnd; // Marks end of valid data (for fread)
	
	int fileIndex; // Marks current index into file

	// Consider adding flags for lastRead/lastWrite
	
} MY_FILE; 

MY_FILE *my_fopen(const char *pathname, const char *mode);
int my_fclose(MY_FILE *stream);

size_t my_fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream);
size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MY_FILE *stream);
int my_fflush(MY_FILE *stream);
int my_fseek(MY_FILE *stream, long offset, int whence);

#endif /* MY_STDIO_H */
