#include "mbr_table.h"

void check_mbr(struct device *dev)
{
    read_main_table(dev);
    dev->type = dev->pt_table[0].type_part == 0xEE ? GPT : MBR;
}

void read_main_table(struct device *dev)
{
    __u8 mbr[512];
    memset((void *)dev->pt_table, 0, 64);
    read(dev->fd, (void *)mbr, 512);
    memcpy((void *)dev->pt_table, (void *)mbr + 0x1BE, 64);
}

void read_ext_table(struct device *dev)
{

}

void show_mbr_table(struct device *dev)
{

    int pr_part_number = 1;
    int ex_part_number = 5;
    int free_space = 0;

    printf("%3s\t%10s\t%10s\t%10s\t%4s\t%8s\n","Num", "Start", "Total", "Size(MB)", "FS", "Type");

    for(int i = 0; i < 4; i++)
    {
        if(dev->pt_table[i].type_part != 0)
        {
            printf("#%d\t",pr_part_number++);
            printf("%10d\t",dev->pt_table[i].sect_before);
            printf("%10d\t",dev->pt_table[i].sect_total);
            long int size = dev->pt_table[i].sect_total;
            size = size * 512 / 1024 / 1024;
            printf("%10ld\t", size);
            switch(dev->pt_table[i].type_part)
            {
            case NTFS:
                printf("%4s\t", "NTFS");
                break;
            case FAT32:
                printf("%4s\t", "FAT32");
                break;
            case FREE:
                printf("%4s\t", "FREE");
            case EXT:
                printf("%4s\t", "EXT");
                break;
            default:
                printf("%4s\t", "UNDF");
            }

            if(dev->pt_table[i].type_part == 0x05)
            {
                printf("%8s\t", "extented");
            }
            else printf("%8s\t", "primary");
            printf("\n");
        }
        else break;
    }

}
