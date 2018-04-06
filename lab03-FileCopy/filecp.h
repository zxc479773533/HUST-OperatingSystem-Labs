/*
 * HUST OS Lab 03 - File copy
 *
 * By Pan Yue
 */

#include "../zxcpyplib/zxcpyp_sys.h"
#include "../zxcpyplib/zxcpyp_sem.h"
#include "../zxcpyplib/ring_buf.h"
#include <wait.h>

/* Seamphores defines */
#define EMPTY 0
#define FULL 1
#define MUTEX 2


/* Semphores */
int semid;

/* Functions */

/*
 * read_buf - copy buffer from src to ringbuf
 */
int read_buf(int read_fd, int shmid_head, int shmid_tail, int semid);

/*
 * write_buf - copy buffer from ring buf to dst
 */
int write_buf(int write_fd, int shmid_head, int shmid_tail, int semid);
