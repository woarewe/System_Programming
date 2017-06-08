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

int check_argument(char *);
char *itoa(int);
int create_file_system(char *, int);

#define SIGNATURE 0xAA55 //signature of MBR
#define MAX_PART 20 // maximal number of MBR sections
#define LAST_CHS_SECTOR 16418815
#define GPT_MAXPART_NUMBER 128

#define TYPE_GUID1_NTFS 4914476015546310818
#define TYPE_GUID2_NTFS -4064174618243972985

int ioctl(int fildes, int reaques, ...);

enum TABLE_TYPE { MBR, GPT};

enum FS_TYPE { NTFS = 0x07, FAT32 = 0x0B, FREE = 0x00, EXT = 0x05, EXT4 = 0x83 };


struct mbr_pt_struct
{
    __u8 bootable;
    __u8 start_part[3];
    __u8 type_part;
    __u8 end_part[3];
    __u32 sect_before;
    __u32 sect_total;
};

struct free_memory
{
    __u32 sect_before;
    __u32 sect_total;
    __u32 size;
};



struct gpt_table_header
{
    __u8 signature[8];
    __u32 revision;
    __u32 headerSize;
    __u32 headerCRC32;
    __u32 reserved;
    __u64 primaryLBA;
    __u64 backupLBA;
    __u64 firstUsableLBA;
    __u64 lastUsableLBA;
    __u64 uniqueGUID1;
    __u64 uniqueGUID2;
    __u64 firstEntryLBA;
    __u32 numberOfEntries;
    __u32 sizeOfEntry;
    __u32 entriesCRC32;
};

struct gpt_table_partition
{
    __u64 typeGUID1;
    __u64 typeGUID2;
    __u64 uniqueGUID1;
    __u64 uniqueGUID2;
    __u64 startingLBA;
    __u64 endingLBA;
    __u64 atributes;
    __u16 typeName[36];

};

struct device
{
    char *path;
    int fd;
    __u32 first_sector;
    __u32 last_sector;

    //MBR_DATA_BEGIN
    struct mbr_pt_struct pt_table[MAX_PART];
    struct free_memory prm_free[MAX_PART];
    struct free_memory log_free[MAX_PART];
    struct hd_geometry geo;
    enum TABLE_TYPE type;
    int primary_number;
    int logical_number;
    int primary_free_number, prm_free_last;
    int logical_free_number, log_free_last;
    //MBR_DATA_END

    //GPT_DATA_BEGIN

    struct gpt_table_header gpt_head, gpt_second_head;
    struct gpt_table_partition lba_table[GPT_MAXPART_NUMBER];
    int lba_number;
    struct free_memory gpt_free[GPT_MAXPART_NUMBER];
    int gpt_free_number, gpt_free_last;


    //GPT_DATA_END

};

struct __bit_8
{
    __u8 bit_0 : 1;
    __u8 bit_1 : 1;
    __u8 bit_2 : 1;
    __u8 bit_3 : 1;
    __u8 bit_4 : 1;
    __u8 bit_5 : 1;
    __u8 bit_6 : 1;
    __u8 bit_7 : 1;
};

struct __bit_16
{
    __u16 bit_0 : 1;
    __u16 bit_1 : 1;
    __u16 bit_2 : 1;
    __u16 bit_3 : 1;
    __u16 bit_4 : 1;
    __u16 bit_5 : 1;
    __u16 bit_6 : 1;
    __u16 bit_7 : 1;
    __u16 bit_8 : 1;
    __u16 bit_9 : 1;
    __u16 bit_10 : 1;
    __u16 bit_11 : 1;
    __u16 bit_12 : 1;
    __u16 bit_13 : 1;
    __u16 bit_14 : 1;
    __u16 bit_15 : 1;
};


//CRC32_DATA

void crc32_init(__u32 *, __u32 *);
void get_crc32(void *, int , __u32 *, __u32 *);




#endif // OTHER_H
