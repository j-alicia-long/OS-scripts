/*************************************
 * Lab 3 Exercise 1
 * Name: Jennifer Long
 * Student No: E0446263
 * Lab Group: 11
 *************************************/


#include <pthread.h>

typedef struct {
  pthread_mutex_t rmutex;
  pthread_mutex_t roomEmpty;
  int reader_count;
  int writer_count;
} rw_lock;
