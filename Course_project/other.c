#include "other.h"

int check_argument(char *arg)
{
    char *str = (char *)malloc(sizeof(char) * strlen(arg));
    for(int i = 0; i < strlen(arg) - 1; ++i)
        str[i] = arg[i];
    str[strlen(arg)] = '\0';
    return strcmp(str, "/dev/sd");
}
