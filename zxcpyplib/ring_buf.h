/*
 * zxcpyp's lib
 *
 * zxcpyp ring buffer header
 */

#ifndef ZXCPYP_RINGBUF_H
#define ZXCPYP_RINGBUF_H

#include "zxcpyp_sys.h"

/* Buffer status */
#define STATUS_AVAILAVLE 0
#define STATUS_FINISH 1

/* Ring buffer blocks num */
#define RING_BUF_NUM 10

/* The ring buffer length */
#define RING_BUF_LEN 1024

typedef struct ring_buf {
  int status;               /* This buffer status */
  int size;                 /* This buffer size */
  int next_shmid;           /* The semid of next buffer */
  char data[RING_BUF_LEN];  /* Data stored */
} ring_buf;



#endif // !ZXCPYP_RINGBUF_H