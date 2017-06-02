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
    for(int i = 0; i < 4; ++i)
    {
        if(dev->pt_table[i].type_part == EXT)
             //read_ext_table(dev, (__u64)dev->pt_table[i].sect_before * 512);
             my_ext_read(dev, i);
        if(dev->pt_table[i].type_part!= FREE) dev->primary_number++;
    }
    close(dev->fd);
    return;
}

void read_ext_table(struct device *dev, __u64 seek)
{
    int num = 4;

    __u8 smbr[512];

    while(1)
    {
        memset((void *)smbr, 0, 512);
        pread(dev->fd, (void *)smbr, 512, seek);

        memset((void *)&dev->pt_table[num], 0, 16 * 2);
        memcpy((void *)&dev->pt_table[num], smbr + 0x1BE, 16 * 2);

         dev->pt_table[num].sect_before += (seek / 512);

         if(dev->pt_table[num].type_part) dev->logical_number++;
         if(!(dev->pt_table[num + 1].type_part)) break;

         seek = (__u64)(dev->pt_table[num + 1].sect_before - dev->pt_table[4].sect_before  ) * 512;

         num++;
    }
}

void my_ext_read(struct device * dev,int ext_index)
{
    int num = 4;

    __u8 smbr[512];

    __u32 seek = 0;

    while(1)
    {
        memset((void *)smbr, 0, 512);
        pread(dev->fd, (void *)smbr, 512, (dev->pt_table[num].sect_before + (__u64)dev->pt_table[ext_index].sect_before) * 512);

        memset((void *)&dev->pt_table[num], 0, 16 * 2);
        memcpy((void *)&dev->pt_table[num], smbr + 0x1BE, 16 * 2);

        dev->pt_table[num].sect_before += (dev->pt_table[ext_index].sect_before + seek);

        if(dev->pt_table[num].type_part) dev->logical_number++;
        if(!(dev->pt_table[num + 1].type_part)) break;

        num++;

        seek = dev->pt_table[num].sect_before;

    }
}

void show_mbr_table(struct device *dev)
{

    if(!dev->primary_number)
    {
        printf("Device don't have partitions!!!\n");
        return;
    }

    //int free_space = 0;

    printf("%3s\t%10s\t%10s\t%10s\t%5s\t%8s\n","Num", "Start", "Total", "Size(MB)", "TFS", "Type");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    for(int i = 0; i < 4; i++)
    {
        if(dev->pt_table[i].type_part != 0)
        {
            printf("#%d\t",i + 1);
            printf("%10d\t",dev->pt_table[i].sect_before);
            printf("%10d\t",dev->pt_table[i].sect_total);
            long int size = dev->pt_table[i].sect_total;
            size = size * 512 / 1024 / 1024;
            printf("%10ld\t", size);
            switch(dev->pt_table[i].type_part)
            {
            case NTFS:
                printf("%5s\t", "NTFS");
                break;
            case FAT32:
                printf("%5s\t", "FAT32");
                break;
            case FREE:
                printf("%5s\t", "FREE");
            case EXT:
                printf("%5s\t", "EXT");
                break;
            default:
                printf("%5s\t", "UNDF");
            }

            if(dev->pt_table[i].type_part == 0x05)
            {
                printf("%8s\t\n", "extented");
                for(int j = 4; j < 4 + dev->logical_number; j++)
                {
                    if(dev->pt_table[j].type_part != 0)
                    {
                        printf("#%d\t", j + 1);
                        printf("%10d\t",dev->pt_table[j].sect_before);
                        printf("%10d\t",dev->pt_table[j].sect_total);
                        long int size = dev->pt_table[j].sect_total;
                        size = size * 512 / 1024 / 1024;
                        printf("%10ld\t", size);
                        switch(dev->pt_table[j].type_part)
                        {
                        case NTFS:
                            printf("%5s\t", "NTFS");
                            break;
                        case FAT32:
                            printf("%5s\t", "FAT32");
                            break;
                        case FREE:
                            printf("%5s\t", "FREE");
                        case EXT:
                            printf("%5s\t", "EXT");
                            break;
                        default:
                            printf("%5s\t", "UNDF");
                        }
                        printf("%8s\t", "logical");
                        printf("\n");
                    }

                }
            }
            else
            {
                printf("%8s\t", "primary");
                printf("\n");
            }
        }
    }
}

void check_free_space(struct device *dev)
{
    dev->prm_free_last = 1;
    dev->primary_free_number = 1;
    dev->prm_free[0].sect_before = dev->first_sector;
    dev->prm_free[0].sect_total = dev->last_sector - dev->first_sector + 1;
    dev->prm_free[0].size = (__u64)dev->prm_free[0].sect_total * 512 / 1024 / 1024;

    struct free_memory sector;

    for(int i = 0; i < 4; i++)
    {
        if(dev->pt_table[i].type_part == EXT)
        {
            dev->log_free_last = 1;
            dev->logical_free_number = 1;
            dev->log_free[0].sect_before = dev->pt_table[i].sect_before + 2048;
            dev->log_free[0].sect_total = dev->pt_table[i].sect_total - 2048 * dev->logical_number;
            dev->log_free[0].size = (__u64)dev->log_free[0].sect_total * 512 / 1024 / 1024;

            for(int k = 4; k < 4 + dev->logical_number; k++)
            {
                if(dev->pt_table[k].type_part != FREE)
                {
                    for(int j = 0; j < dev->log_free_last; j++)
                    {
                        if(dev->pt_table[k].sect_before == dev->log_free[j].sect_before)
                        {
                            sector.sect_before = dev->pt_table[k].sect_before + dev->pt_table[k].sect_total + 2048;
                            sector.sect_total = dev->log_free[j].sect_total - dev->pt_table[k].sect_total;
                            sector.size = (__u64)sector.sect_total * 512 / 1024 / 1024;
                            if(!sector.sect_total)
                            {
                                dev->log_free[j].sect_before = 0;
                                dev->log_free[j].sect_total = 0;
                                dev->log_free[j].size = 0;
                                dev->logical_free_number--;
                            }
                            else
                                dev->log_free[j] = sector;
                        }
                        else
                        if(dev->pt_table[k].sect_before + dev->pt_table[k].sect_total == dev->log_free[j].sect_before + dev->log_free[j].sect_total)
                        {
                            sector.sect_before = dev->log_free[j].sect_before;
                            sector.sect_total = dev->log_free[j].sect_total - dev->pt_table[k].sect_total;
                            sector.size = (__u64)sector.sect_total * 512 / 1024 / 1024;
                            if(!sector.sect_total)
                            {
                                dev->log_free[j].sect_before = 0;
                                dev->log_free[j].sect_total = 0;
                                dev->log_free[j].size = 0;
                                dev->logical_free_number--;
                            }
                            else
                                dev->log_free[j] = sector;
                        }
                        else
                        if((dev->log_free[j].sect_before < dev->pt_table[k].sect_before) &&
                                (dev->pt_table[k].sect_before + dev->pt_table[k].sect_total < dev->log_free[j].sect_before + dev->log_free[j].sect_total))
                        {
                            sector.sect_before = dev->pt_table[k].sect_before + dev->pt_table[k].sect_total;
                            sector.sect_total = (dev->log_free[j].sect_before + dev->log_free[j].sect_total) -
                                    (dev->pt_table[k].sect_before + dev->pt_table[k].sect_total);
                            sector.size = (__u64)sector.sect_total * 512 / 1024 / 1024;

                            dev->log_free[j].sect_total -= (dev->pt_table[k].sect_total + sector.sect_total);
                            dev->log_free[j].size = (__u64)dev->log_free[j].sect_total * 512 / 1024 / 1024;

                            dev->log_free[dev->log_free_last] = sector;

                            dev->log_free_last++;
                            dev->logical_free_number++;
                        }

                    }
                }
            }
        }
        if(dev->pt_table[i].type_part != FREE)
        {
            for(int j = 0; j < dev->prm_free_last; j++)
            {
                if(dev->pt_table[i].sect_before == dev->prm_free[j].sect_before)
                {
                    sector.sect_before = dev->pt_table[i].sect_before + dev->pt_table[i].sect_total;
                    sector.sect_total = dev->prm_free[j].sect_total - dev->pt_table[i].sect_total;
                    sector.size = (__u64)sector.sect_total * 512 / 1024 / 1024;
                    if(!sector.sect_total)
                    {
                        dev->prm_free[j].sect_before = 0;
                        dev->prm_free[j].sect_total = 0;
                        dev->prm_free[j].size = 0;
                        dev->primary_free_number--;
                    }
                    else
                        dev->prm_free[j] = sector;
                }
                else
                if(dev->pt_table[i].sect_before + dev->pt_table[i].sect_total == dev->prm_free[j].sect_before + dev->prm_free[j].sect_total)
                {
                    sector.sect_before = dev->prm_free[j].sect_before;
                    sector.sect_total = dev->prm_free[j].sect_total - dev->pt_table[i].sect_total;
                    sector.size = (__u64)sector.sect_total * 512 / 1024 / 1024;
                    if(!sector.sect_total)
                    {
                        dev->prm_free[j].sect_before = 0;
                        dev->prm_free[j].sect_total = 0;
                        dev->prm_free[j].size = 0;
                        dev->primary_free_number--;
                    }
                    else
                        dev->prm_free[j] = sector;
                }
                else
                if((dev->prm_free[j].sect_before < dev->pt_table[i].sect_before) &&
                        (dev->pt_table[i].sect_before + dev->pt_table[i].sect_total < dev->prm_free[j].sect_before + dev->prm_free[j].sect_total))
                {
                    sector.sect_before = dev->pt_table[i].sect_before + dev->pt_table[i].sect_total;
                    sector.sect_total = (dev->prm_free[j].sect_before + dev->prm_free[j].sect_total) -
                            (dev->pt_table[i].sect_before + dev->pt_table[i].sect_total);
                    sector.size = (__u64)sector.sect_total * 512 / 1024 / 1024;

                    dev->prm_free[j].sect_total -= (dev->pt_table[i].sect_total + sector.sect_total);
                    dev->prm_free[j].size = (__u64)dev->prm_free[j].sect_total * 512 / 1024 / 1024;

                    dev->prm_free[dev->prm_free_last] = sector;

                    dev->prm_free_last++;
                    dev->primary_free_number++;
                }

            }
        }
    }

}

void get_geometry(const char *path, struct device *dev)
{
    int hdd = open(path, O_RDONLY);
    ioctl(hdd, HDIO_GETGEO, &dev->geo);
    close(hdd);
    dev->first_sector = 2048;
    dev->last_sector = dev->geo.cylinders * dev->geo.heads * dev->geo.sectors - 1;
}

void show_mbr_free_space(struct device *dev)
{
    printf("\n");
    if(!dev->primary_free_number && !dev->logical_free_number)
    {
        printf("Device don't have free space!!!\n");
        return;
    }
    else printf("############################FREE SPACE##################################\n\n");

    printf("%3s\t%10s\t%10s\t%10s\t%5s\t%8s\n","Num", "Start", "Total", "Size(MB)", "TFS", "Type");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    if(dev->primary_free_number)
    for(int i = 0; i < dev->prm_free_last; i++)
        if(dev->prm_free[i].sect_total)
        {
            printf("#%d\t", i + 1);
            printf("%10d\t", dev->prm_free[i].sect_before);
            printf("%10d\t", dev->prm_free[i].sect_total);
            printf("%10d\t", dev->prm_free[i].size);
            printf("%5s\t", "FREE");
            printf("%8s\t", "primary");
            printf("\n");
        }

    if(dev->logical_free_number)
    for(int i = 0; i < dev->log_free_last; i++)
        if(dev->log_free[i].sect_total)
        {
            printf("#%d\t", i + dev->primary_free_number + 1);
            printf("%10d\t", dev->log_free[i].sect_before);
            printf("%10d\t", dev->log_free[i].sect_total);
            printf("%10d\t", dev->log_free[i].size);
            printf("%5s\t", "FREE");
            printf("%8s\t", "logical");
            printf("\n");
        }


}

int delete_partition(struct device *dev, int number)
{
    if(number <= 0)
    {
        printf("Error: negative number of partition\n");
        return -1;
    }

    if(number > 3)
    {
        printf("Can't delete logical partition.\n");
        return -1;
    }

    if(dev->pt_table[number - 1].type_part == FREE)
    {
        printf("Can't delete empty partition\n");
        return -1;
    }

    memset((void *)&dev->pt_table[number - 1], 0, sizeof(struct mbr_pt_struct));
    pwrite(dev->fd, dev->pt_table, 64, 446);

    return 0;
}

int init(struct device *dev, const char * path)
{
    memset((void *)dev, 0, sizeof(struct device));
    get_geometry(path, dev);
    dev->fd = open(path, O_RDONLY);
    if(!dev->fd)
    {
        printf("Cannot open file %s \n", path);
        return - 1;
    }
    check_mbr(dev);
    check_free_space(dev);
    return 0;
}
