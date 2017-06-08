#ifndef MBR_TABLE_H
#define MBR_TABLE_H

#include "other.h"


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
