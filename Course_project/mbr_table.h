#ifndef MBR_TABLE_H
#define MBR_TABLE_H

#include "other.h"

#define SIGNATURE 0xAA55 //signature of MBR
#define MAX_PART 20 // maximal number of MBR sections
#define LAST_CHS_SECTOR 16418815


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

struct device
{
    char *path;
    int fd;
    __u32 first_sector;
    __u32 last_sector;
    struct mbr_pt_struct pt_table[MAX_PART];
    struct free_memory prm_free[MAX_PART];
    struct free_memory log_free[MAX_PART];
    struct hd_geometry geo;
    enum TABLE_TYPE type;
    int primary_number;
    int logical_number;
    int primary_free_number, prm_free_last;
    int logical_free_number, log_free_last;
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

int mbr_init(struct device *, const char *);
void check_free_space(struct device *);
void read_main_table(struct device *);
void read_ext_table(struct device *, __u64);
void show_mbr_table(struct device *);
void check_mbr(struct device *);
void get_geometry(const char *, struct device *);
void show_mbr_free_space(struct device *);
void my_ext_read(struct device *, int);
int mbr_delete_partition(struct device *, int);
int mbr_create_primary(struct device *, size_t);
int mbr_create_extended(struct device *, size_t, int *, int, int);
int mbr_create_new_partition(struct device *);
void lba_to_chs(__u8 *, __u16, struct hd_geometry);

#endif // MBR_TABLE_H
