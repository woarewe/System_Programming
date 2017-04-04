#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>
#include <aio.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 512

void readFromFile(int fd, char *buffer)
{
  memset(buffer, 0, SIZE * sizeof(char));

  struct aiocb aioInfo;
  memset(&aioInfo, 0, sizeof(struct aiocb));
  aioInfo.aio_fildes = fd;
  aioInfo.aio_buf = buffer;
  aioInfo.aio_nbytes = SIZE;
  aioInfo.aio_lio_opcode = LIO_READ;

  if(aio_read(&aioInfo) == -1)
  {
    printf("Error at aio_read()\n");
    close(fd);
    exit(EXIT_FAILURE);
  }
  //printf("Read Message: %s\n",aioInfo.aio_buf);
  while(aio_error(&aioInfo) == EINPROGRESS);

}


void writeToFile(int fd, char *buffer)
{
  struct aiocb aioInfo;
  memset(&aioInfo, 0, sizeof(struct aiocb));
  aioInfo.aio_fildes = fd;
  aioInfo.aio_buf = buffer;
  aioInfo.aio_nbytes = strlen(buffer);
  aioInfo.aio_lio_opcode = LIO_WRITE;

  if(aio_write(&aioInfo) == -1)
  {
    printf("Error at aio_write()\n");
    close(fd);
    exit(EXIT_FAILURE);
  }

  while(aio_error(&aioInfo) == EINPROGRESS);

}
