/*
 * HUST OS Lab 02 - Thread synchronization
 *
 * By Pan Yue
 */

#include "../zxcpyplib/zxcpyp_sys.h"
#include "../zxcpyplib/zxcpyp_sem.h"
#include <pthread.h>

#define COMPUTE 0
#define PRINT 1

/* The global sum */
int sum = 0;
/* Semaphores */
int semid;

void* compute(void *arg) {
  for (int i = 1; i <= 100; i++) {
    if (sem_p(semid, COMPUTE) == -1)
      err_exit("P compute");

    sum += i;

    if (sem_v(semid, PRINT) == -1)
      err_exit("V print");
  }
  return NULL;
}

void* print(void *arg) {
  for (int i = 1; i <= 100; i++) {
    if (sem_p(semid, PRINT) == -1)
      err_exit("P print");

    printf("The sum is: %d\n", sum);

    if (sem_v(semid, COMPUTE) == -1)
      err_exit("V copmute");
  }
  return NULL;
}

int main(void) {
  /* Thread 0: Computr, Thread 1: Print */
  pthread_t thread[2];
  void *thread_result;
  /* Create and inital 2 semaphore for user */
  semid = semget(IPC_PRIVATE, 2, S_IRUSR | S_IWUSR);
  if (init_sem_available(semid, COMPUTE) == -1)
    err_exit("Initial compute semaphore");
  if (init_sem_in_use(semid, PRINT) == -1)
    err_exit("Initial print semaphore");

  /* Create compute thread */
  if (pthread_create(&thread[0], NULL, compute, NULL) != 0)
    err_exit("Compute thread create");
  
  /* Create print thread */
  if (pthread_create(&thread[1], NULL, print, NULL) != 0)
    err_exit("Print thread create");

  pthread_join(thread[0], &thread_result);
  pthread_join(thread[1], &thread_result);
  if (semctl(semid, IPC_RMID, 0) == -1)
    err_exit("Delete semaphore");
  return 0;
}