/*************************************
 * Lab 3 Exercise 1
 * Name: Jennifer Long
 * Student No: E0446263
 * Lab Group: 11
 *************************************/

#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  pthread_mutex_init(&(lock->rmutex), NULL);
  pthread_mutex_init(&(lock->wmutex), NULL);
  pthread_mutex_init(&(lock->roomEmpty), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->roomEmpty));

  pthread_mutex_lock(&(lock->wmutex));
  lock->writer_count++;
  pthread_mutex_unlock(&(lock->wmutex));
}

void writer_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->wmutex));
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->wmutex));

  pthread_mutex_unlock(&(lock->roomEmpty));
}

void reader_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->rmutex));
  lock->reader_count++;
  if(lock->reader_count == 1)
    pthread_mutex_lock(&(lock->roomEmpty));
  pthread_mutex_unlock(&(lock->rmutex));
}

void reader_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->rmutex));
  lock->reader_count--;
  if(lock->reader_count == 0)
    pthread_mutex_unlock(&(lock->roomEmpty));
  pthread_mutex_unlock(&(lock->rmutex));
}

void cleanup(rw_lock* lock)
{
  pthread_mutex_destroy(&(lock->rmutex));
  pthread_mutex_destroy(&(lock->wmutex));
  pthread_mutex_destroy(&(lock->roomEmpty));
}
