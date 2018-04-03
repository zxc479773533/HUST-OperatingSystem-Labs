/*
 * HUST OS Lab 01 - Single Pipe
 *
 * By Pan Yue
 */

#include "../zxcpyplib/zxcpyp_sys.h"
#include <signal.h>
#include <wait.h>

#define FIFO "lab01_fifo"
#define BUF_SIZE 30


int pid1, pid2;

/*
 * pipe(int *pfd)
 * pfd[0]: read
 * pfd[1]: write
 */

/*
 * sigint_handler - handle SIGINT, kill child pid
 */
void sigint_handler(int sig) {
  printf("\n");
  kill(pid1, SIGUSR1);
  kill(pid2, SIGUSR2);
}

/*
 * sigusr_handler - handle SIGUER1/2, end child pid
 */
void sigusr_handler(int sig) {
  if (sig == SIGUSR1) {
    printf("Child Process 1 is Killed by Parent!\n");
    exit(0);
  }
  else if (sig == SIGUSR2) {
    printf("Child Process 2 is Killed by Parent!\n");
    exit(0);
  }
}

int main(void) {
  int read_fd, write_fd;
  char read_buf[BUF_SIZE];
  int read_num;
  int wait_tmp;
  signal(SIGINT, sigint_handler);

  if (mkfifo(FIFO, S_IRUSR | S_IWUSR) == -1 && errno != EEXIST)
    err_exit("Pipe");
  
  switch (pid1 = fork()) {
    case -1:
      err_exit("First fork");
    
    /* Child 1 - write data */
    case 0:
      /* Ignore SIGINT */
      signal(SIGINT, SIG_IGN);
      /* Catch SIGUSR2 */
      signal(SIGUSR1, sigusr_handler);

      /* Open FIFO writter */
      if ((write_fd = open(FIFO, O_WRONLY)) == -1)
        err_exit("FIFO writter open");
      
      int count = 1;
      char write_buf[BUF_SIZE];
      for (;;) {
        sprintf(write_buf, "I send you %d times.\n", count);
        if (write(write_fd, write_buf, strlen(write_buf)) != strlen(write_buf))
          err_exit("Child 1 write");
        sleep(1);
        count++;
      }
    
    /* Parent pid */
    default:
      switch (pid2 = fork()) {
        case -1:
          err_exit("Second fork");
        
        /* Child 2 - read data */
        case 0:
          /* Ignore SIGINT */
          signal(SIGINT, SIG_IGN);
          /* Catch SIGUSR2 */
          signal(SIGUSR2, sigusr_handler);

          if ((read_fd = open(FIFO, O_RDONLY)) == -1)
            err_exit("FIFO reader open");
          
          for (;;) {
            read_num = read(read_fd, read_buf, BUF_SIZE);
            if (read_num == -1)
              err_exit("Child 2 read");
            if (read_num == 0)
              continue;
            if (write(STDOUT_FILENO, read_buf, read_num) != read_num)
              err_exit("Child 2 write");
          }

        /* Parent pid */
        default:
          /* Wait for child pid */
          
          waitpid(pid1, &wait_tmp, 0);
          waitpid(pid1, &wait_tmp, 0);
          printf("Parent Process is Killed!\n");
          exit(0);
      }
  }
}