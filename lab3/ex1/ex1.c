
/*************************************
 * Lab 3 Exercise 1
 * Name:
 * Student No:
 * Lab Group:
 *************************************/
#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  pthread_mutex_init(&(lock->mutex), NULL);
  pthread_mutex_init(&(lock->roomEmpty), NULL);
  lock->reader_count = 0;
  // lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->roomEmpty));
  // lock->writer_count++;
  // pthread_mutex_unlock(&(lock->roomEmpty));
}

void writer_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->roomEmpty));
  // lock->writer_count--;
  // pthread_mutex_unlock(&(lock->roomEmpty));
}

void reader_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex));
  lock->reader_count++;
  if(reader_count == 1)
    pthread_mutex_lock(&(lock->roomEmpty));
  pthread_mutex_unlock(&(lock->mutex));
}

void reader_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex));
  lock->reader_count--;
  if(reader_count == 0)
    pthread_mutex_unlock(&(lock->roomEmpty));
  pthread_mutex_unlock(&(lock->mutex));
}

void cleanup(rw_lock* lock)
{
  pthread_mutex_destroy(&(lock->mutex));
}
