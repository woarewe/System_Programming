#include "other.h"
#define INT_DIGITS 19		/* enough for 64 bit integer */
#define LEN 50

int check_argument(char *arg)
{
    char *str = (char *)malloc(sizeof(char) * strlen(arg));
    for(int i = 0; i < (signed int)strlen(arg) - 1; ++i)
        str[i] = arg[i];
    str[strlen(arg)] = '\0';
    return strcmp(str, "/dev/sd");
}



char *itoa(int i)
{
  /* Room for INT_DIGITS digits, - and '\0' */
  static char buf[INT_DIGITS + 2];
  char *p = buf + INT_DIGITS + 1;	/* points to terminating '\0' */
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  }
  else {			/* i < 0 */
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}

int create_file_system(char *device_name, int part_number)
{
    char *program = "/usr/bin/mkntfs";


    char *file_path = (char *)malloc(1);
    strcpy(file_path, device_name);
    strcat(file_path, itoa(part_number));


    pid_t pd = fork();
    if(!pd)
    {
        printf("Creating filesystem....");
        FILE *fd = freopen("/dev/null", "w", stdout);
        sleep(3);
        if(0 > execlp(program, "-Q" , "-Q", "-v", "-F", "-F", file_path, NULL)) printf ("Erorr!!!!\n");
        fclose(fd);
        exit(0);
    }
    else waitpid(pd, NULL, 0);


    printf("\n");

    return 0;
}
