/*
 * zxcpyp's lib
 *
 * zxcpyp sys header
 */

#ifndef ZXCPYP_H
#define ZXCPYP_H

#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Standard library */
#include <string.h>     /* String handling functions */
#include <unistd.h>     /* Unix system calls */
#include <errno.h>      /* Error difinations */
#include <sys/types.h>  /* System data type */

#include "zxcpyp_err.h" /* Error handling functions */

/* True and false defines */
#define FALSE 0
#define TRUE 1

/* Max and min defines */
#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))

#endif // !ZXCPYP_H
