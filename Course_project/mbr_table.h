#ifndef MBR_TABLE_H
#define MBR_TABLE_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/types.h>
#include <sys/types.h>
#include <errno.h>
#include <linux/hdreg.h>


#define SIGNATURE 0xAA55 //signature of MBR
#define MAX_PART 20 // maximal number of MBR sections

int ioctl(int fildes, int reaques, ...);

enum TABLE_TYPE { MBR, GPT};

enum FS_TYPE { NTFS = 0x07, FAT32 = 0x0B, FREE = 0x00, EXT = 0x05 };


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
    int fd;
    __u32 first_sector;
    __u32 last_sector;
    struct mbr_pt_struct pt_table[MAX_PART];
    struct free_memory prm_free[MAX_PART];
    struct free_memory ext_free[MAX_PART];
    struct hd_geometry geo;
    enum TABLE_TYPE type;
    int primary_number;
    int logical_number;
    int primary_free_number, prm_free_last;
    int logical_free_number, log_free_last;
};

void check_free_space(struct device *);
void read_main_table(struct device *);
void read_ext_table(struct device *, __u64);
void show_mbr_table(struct device *);
void check_mbr(struct device *);
void get_geometry(const char *, struct device *);
void show_mbr_free_space(struct device *);


#endif // MBR_TABLE_H
