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
        sleep(5);
        if(0 > execlp(program, "-Q" , "-Q", "-v", "-F", "-F", file_path, NULL)) printf ("Erorr!!!!\n");
        fclose(fd);
        exit(0);
    }
    else waitpid(pd, NULL, 0);


    printf("\n");

    return 0;
}

//CRC32_DATA

void crc32_init(__u32 *m_crc32, __u32 *table)
{
    __u32 CRC_POLY = 0xEDB88320;
    __u32 i, j, r;
    for (i = 0; i < 256; i++)
    {
        for (r = i, j = 8; j; j--)
            r = r & 1? (r >> 1) ^ CRC_POLY: r >> 1;
        table[i] = r;
    }
    *m_crc32 = 0;
}

void get_crc32(void *pData, int nLen, __u32 *m_crc32, __u32 *table)
{
    __u32 CRC_MASK = 0xD202EF8D;
    register __u8* pdata = (__u8 *)pData;
    register __u32 crc = *m_crc32;
         while (nLen--)
         {
                crc = table[(__u8)crc ^ *pdata++] ^ crc >> 8;
                crc ^= CRC_MASK;
         }
         *m_crc32 = crc;
}
