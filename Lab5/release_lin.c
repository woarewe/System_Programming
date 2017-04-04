#include "header.h"

char *files[5] = { "./Files/1", "./Files/2", "./Files/3", "./Files/4", "./Files/5"};

void *threadReader(void *arg)
{
  struct Info *info = (struct Info *)arg;
  void (*lib_function)(int, char *);
  *(void **) (&lib_function) = dlsym(info->library, "readFromFile");

  for(int i = 0; i < 5; i++)
  {
    while(info->flag);
    pthread_mutex_lock(&info->mutex);
    //printf("File name: %s\n", info->fileNames[i]);
    int fd = open(info->fileNames[i], O_RDONLY);
    if(fd < 0)
    {
      printf("File not found");
      exit(-1);
    }
    (*lib_function)(fd, info->buffer);
    close(fd);
    pthread_mutex_unlock(&info->mutex);
    info->size++;
    info->flag = 1;
    usleep(1);
  }
  info->threadsNumber--;
  return NULL;
}

void *threadWriter(void *arg)
{
  struct Info *info = (struct Info *)arg;
  void (*lib_function)(int, char *);
  *(void **) (&lib_function) = dlsym(info->library, "writeToFile");
  pthread_mutex_lock(&info->mutex);
  int fd = open("./Files/All", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND);
  pthread_mutex_unlock(&info->mutex);
  usleep(1);
  while(!info->flag);
  for(int i = 0; i < 5; i++)
  {
    while(!info->flag);
    pthread_mutex_lock(&info->mutex);
    //printf("Write message%s\n", info->buffer);
    (*lib_function)(fd, info->buffer);
    pthread_mutex_unlock(&info->mutex);
    info->flag = 0;
    usleep(1);
  }
  close(fd);
  info->threadsNumber--;
  return NULL;
}

void initInfo(struct Info *info)
{
  info->fileNames = files;
  info->size = 0;
  info->library = dlopen("./lib_lin.so", RTLD_LAZY);
  info->threadsNumber = 0;
  info->flag = 0;

}

void createMutex(struct Info *info)
{
  pthread_mutex_init(&info->mutex, NULL);
}

void runThreads(struct Info *info)
{
  pthread_create(&info->readThread, NULL, &threadReader, info);
  info->threadsNumber++;
  pthread_create(&info->writeThread, NULL, &threadWriter, info);
  info->threadsNumber++;
}

void waitThreads(struct Info *info)
{
  while(info->threadsNumber);
  printf("All good!\n");
  dlclose(info->library);
}
