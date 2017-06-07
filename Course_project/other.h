#ifndef OTHER_H
#define OTHER_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/types.h>
#include <sys/types.h>
#include <errno.h>
#include <linux/hdreg.h>
#include <stdlib.h>
#include <wait.h>

int volatile FLAG;

int check_argument(char *);
char *itoa(int);
int create_file_system(char *, int);


#endif // OTHER_H
