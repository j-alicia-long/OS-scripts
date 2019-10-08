/*************************************
 * Lab 3 Exercise 2
 * Name: Jennifer Long
 * Student No: E0446263
 * Lab Group: 11
 *************************************/


#include <pthread.h>

typedef struct {
  pthread_mutex_t rmutex;
  pthread_mutex_t wmutex;
  pthread_mutex_t readGate;
  pthread_mutex_t roomEmpty;
  int reader_count;
  int writer_count;
} rw_lock;
