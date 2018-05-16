/*
 * HUST OS Lab 03 - File copy
 *
 * By Pan Yue
 */

#include "filecp.h"

int main(int argc, char **argv) {
  int read_fd, write_fd;
  int shmid_head;  
  int wait_tmp;
  pid_t readpid_id, writepid_id;
  /* Check args */
  if (argc != 3)
    usage_err("./filecp <src> <dst>");
  
  /* Open files */
  read_fd = open(argv[1], O_RDONLY);
  write_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IEXEC);

  /* Create and initial 3 semaphore for user */
  if ((semid = semget(IPC_PRIVATE, 3, S_IRUSR | S_IWUSR)) == -1)
    err_exit("Get semaphore");
  if (init_sem_in_use(semid, EMPTY) == -1)
    err_exit("Initial empty");
  if (init_sem_value(semid, FULL, RING_BUF_NUM) == -1)
    err_exit("Initial full");
  if (init_sem_available(semid, MUTEX) == -1)
    err_exit("Initial mutex");
  
  /* Create a ring buffer
   * The pointer is shmid
   */
  if ((shmid_head = shmget(IPC_PRIVATE, sizeof(ring_buf), IPC_CREAT | S_IRUSR | S_IWUSR)) == -1)
    err_exit("Shared memory get");
  ring_buf *ring_buf_tmp = (ring_buf*)shmat(shmid_head, NULL, 0);
  if (ring_buf_tmp == (void*)-1)
    err_exit("Shared memory attach");
  for (int i = 0; i < RING_BUF_NUM; i++) {
    int new_shmid;
    if ((new_shmid = shmget(IPC_PRIVATE, sizeof(ring_buf), IPC_CREAT | S_IRUSR | S_IWUSR)) == -1)
      err_exit("Shared memory get");
    ring_buf_tmp->next_shmid = new_shmid;
    ring_buf_tmp->status = STATUS_AVAILAVLE;
    ring_buf_tmp = (ring_buf*)shmat(new_shmid, NULL,  0);
    if (ring_buf_tmp == (void*)-1)
      err_exit("Shared memory attach");
  }
  ring_buf_tmp->next_shmid = shmid_head;

  /* Fork reader process */
  switch(readpid_id = fork()) {
    case -1:
      err_exit("First fork");

    /* Child 1 - read data */
    case 0:
      read_buf(read_fd, shmid_head, semid);
      exit(0);
    
    /* Parent pid */
    default:
      switch(writepid_id = fork()) {
        case -1:
          err_exit("Second fork");

        /* Child 2 - write data */
        case 0:
          write_buf(write_fd, shmid_head, semid);
          exit(0);

        /* Parent pid */
        default:
          waitpid(readpid_id, &wait_tmp, 0);
          waitpid(writepid_id, &wait_tmp, 0);
          if (semctl(semid, semid, IPC_RMID, 0) == -1)
            err_exit("Delete semaphore");
          exit(0);
      }
  }
}

int read_buf(int read_fd, int shmid_tail, int semid) {
  int read_num;
  ring_buf *ring_buf_tail;
  /* Attach ring buffer tail for read in */
  if ((ring_buf_tail = (ring_buf*)shmat(shmid_tail, NULL, 0)) == (void*)-1)
    err_exit("read_buf: Shared memory attach");
  /* Reading */
  for (;;) {
    sem_p(semid, FULL);
    sem_p(semid, MUTEX);
    /* Read data */
    read_num = read(read_fd, ring_buf_tail->data, RING_BUF_LEN);
    if (read_num == -1)
      err_exit("read_buf: Read");
    /* Read finished */
    if (read_num == 0) {
      ring_buf_tail->status = STATUS_FINISH;
      ring_buf_tail->size = read_num;
      close(read_fd);
      sem_v(semid, EMPTY);
      sem_v(semid, MUTEX);      
      return 0;
    }
    ring_buf_tail->size = read_num;
    /* Attach next */
    shmid_tail = ring_buf_tail->next_shmid;
    if ((ring_buf_tail = (ring_buf*)shmat(shmid_tail, NULL, 0)) == (void*)-1)
      err_exit("read_buf: Shared memory attach");
    sem_v(semid, MUTEX);
    sem_v(semid, EMPTY);
  }
}

int write_buf(int write_fd, int shmid_head, int semid) {
  ring_buf *ring_buf_head;
  /* Attach ring buffer head for write out */
  if ((ring_buf_head = (ring_buf*)shmat(shmid_head, NULL, 0)) == (void*)-1)
    err_exit("write_buf: Shared memory attach");
  /* Writting */
  for (;;) {
    sem_p(semid, EMPTY);
    sem_p(semid, MUTEX);
    /* Write data */
    /* Write finished */
    if (ring_buf_head->status == STATUS_FINISH) {
      write(write_fd, ring_buf_head->data, ring_buf_head->size);
      close(write_fd);
      sem_v(semid, MUTEX);
      sem_v(semid, FULL);
      return 0;
    }
    write(write_fd, ring_buf_head->data, ring_buf_head->size);
    shmid_head = ring_buf_head->next_shmid;
    if ((ring_buf_head = (ring_buf*)shmat(shmid_head, NULL, 0)) == (void*)-1)
      err_exit("write_buf: Shared memory attach");
    sem_v(semid, MUTEX);
    sem_v(semid, FULL);
  }
}