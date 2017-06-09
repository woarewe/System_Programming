#include "gpt_table.h"

int gpt_read_table(struct device *dev)
{
    memset((void *)&dev->gpt_head, 0, sizeof(struct gpt_table_header));
    memset((void *)dev->lba_table, 0, sizeof(struct gpt_table_partition) * GPT_MAXPART_NUMBER);

    dev->fd = open(dev->path, O_RDONLY);

    pread(dev->fd, (void *)&dev->gpt_head, sizeof(struct gpt_table_header), 512);

    __u64 offset = 1024;

    for(int i = 0; i < GPT_MAXPART_NUMBER; i++)
    {
        pread(dev->fd, (void *)&dev->lba_table[i], sizeof(struct gpt_table_partition), offset);
        offset += 128;
        if(dev->lba_table[i].typeGUID1) dev->lba_number++;
    }

    offset = dev->gpt_head.backupLBA;
    offset *= 512;

    pread(dev->fd, (void *)&dev->gpt_second_head, sizeof(struct gpt_table_header), offset);

    close(dev->fd);

    gpt_check_free_space(dev);

    return 0;
}


void gpt_show_table(struct device *dev)
{
    if(!dev->lba_number)
    {
        printf("Device don't have partitions!!!\n");
        return;
    }

    printf("%3s\t%10s\t%10s\t%10s\t%5s\t%8s\n","Num", "Start", "Total", "Size(MB)", "TFS", "Type");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    for(int i = 0; i < GPT_MAXPART_NUMBER; i++)
    {
        if(dev->lba_table[i].typeGUID1)
        {
            printf("#%d\t",i + 1);
            printf("%10lld\t",dev->lba_table[i].startingLBA);
            printf("%10lld\t",dev->lba_table[i].endingLBA - dev->lba_table[i].startingLBA + 1);
            long int size = dev->lba_table[i].endingLBA - dev->lba_table[i].startingLBA + 1;
            size = size * 512 / 1024 / 1024;
            printf("%10ld\t", size);
            if(dev->lba_table[i].typeGUID1 == TYPE_GUID1_NTFS && (long long int)dev->lba_table[i].typeGUID2 == TYPE_GUID2_NTFS)
                printf("%5s\t", "NTFS");
            else  printf("%5s\t", "UNDF");

            printf("%8s\t", "primary");
            printf("\n");
        }
    }
}

void gpt_check_free_space(struct device *dev)
{
    dev->gpt_free_last = 1;
    dev->gpt_free_number = 1;
    dev->gpt_free[0].sect_before = dev->first_sector;
    dev->gpt_free[0].sect_total = dev->last_sector - 2048 - dev->first_sector + 1;
    dev->gpt_free[0].size = (__u64)dev->gpt_free[0].sect_total * 512 / 1024 / 1024;

    struct free_memory sector;
    memset((void *)&sector, 0, sizeof(struct free_memory));

    for(int i = 0; i < GPT_MAXPART_NUMBER; i++)
    if(dev->lba_table[i].typeGUID1)
    {
        for(int j = 0; j < dev->gpt_free_last; j++)
        {
            if(dev->lba_table[i].startingLBA == dev->gpt_free[j].sect_before)
            {
                sector.sect_before = dev->lba_table[i].startingLBA + ( dev->lba_table[i].endingLBA - dev->lba_table[i].startingLBA + 1);
                sector.sect_total = dev->gpt_free[j].sect_total - ( dev->lba_table[i].endingLBA - dev->lba_table[i].startingLBA + 1);
                sector.size = (__u64)sector.sect_total * 512 / 1024 / 1024;
                if(!sector.sect_total)
                {
                    dev->gpt_free[j].sect_before = 0;
                    dev->gpt_free[j].sect_total = 0;
                    dev->gpt_free[j].size = 0;
                    dev->gpt_free_number--;
                }
                else
                    dev->gpt_free[j] = sector;
            }
            else
            if(dev->lba_table[i].endingLBA == dev->gpt_free[j].sect_before + dev->gpt_free[j].sect_total - 1)
            {
                sector.sect_before = dev->gpt_free[j].sect_before;
                sector.sect_total = dev->gpt_free[j].sect_total - ( dev->lba_table[i].endingLBA - dev->lba_table[i].startingLBA + 1);
                sector.size = (__u64)sector.sect_total * 512 / 1024 / 1024;
                if(!sector.sect_total)
                {
                    dev->gpt_free[j].sect_before = 0;
                    dev->gpt_free[j].sect_total = 0;
                    dev->gpt_free[j].size = 0;
                    dev->gpt_free_number--;
                }
                else
                    dev->gpt_free[j] = sector;
            }
            else
            if((dev->gpt_free[j].sect_before < dev->lba_table[i].startingLBA) &&
                    (dev->lba_table[i].endingLBA < dev->gpt_free[j].sect_before + dev->gpt_free[j].sect_total - 1))
            {
                sector.sect_before = dev->lba_table[i].startingLBA + ( dev->lba_table[i].endingLBA - dev->lba_table[i].startingLBA + 1);
                sector.sect_total = (dev->gpt_free[j].sect_before + dev->gpt_free[j].sect_total) - (dev->lba_table[i].endingLBA + 1);
                sector.size = (__u64)sector.sect_total * 512 / 1024 / 1024;

                dev->gpt_free[j].sect_total -= (( dev->lba_table[i].endingLBA - dev->lba_table[i].startingLBA + 1) + sector.sect_total);
                dev->gpt_free[j].size = (__u64)dev->gpt_free[j].sect_total * 512 / 1024 / 1024;

                dev->gpt_free[dev->gpt_free_last] = sector;

                dev->gpt_free_last++;
                dev->gpt_free_number++;
            }

        }
    }
}

void gpt_show_free_space(struct device *dev)
{
    printf("\n");
    if(!dev->gpt_free_number)
    {
        printf("Device don't have free space!!!\n");
        return;
    }
    else printf("############################FREE SPACE##################################\n\n");

    printf("%3s\t%10s\t%10s\t%10s\t%5s\t%8s\n","Num", "Start", "Total", "Size(MB)", "TFS", "Type");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    for(int i = 0; i < dev->gpt_free_last; i++)
        if(dev->gpt_free[i].sect_total)
        {
            printf("#%d\t", i + 1);
            printf("%10d\t", dev->gpt_free[i].sect_before);
            printf("%10d\t", dev->gpt_free[i].sect_total);
            printf("%10d\t", dev->gpt_free[i].size);
            printf("%5s\t", "FREE");
            printf("%8s\t", "primary");
            printf("\n");
        }
}

int gpt_delete_partition(struct device *dev, int number)
{
    if(!dev->lba_table[number].typeGUID1)
    {
        printf("Cannot delete empty LBA\n");
        return -1;
    }

    memset((void *)&dev->lba_table[number], 0, sizeof(struct gpt_table_partition));

    __u32 table_crc32;
    __u32 table[256];

    memset((void *)table, 0, 256);

    crc32_init(&table_crc32, table);

    int pos = 0;
    while(pos < 128)
    {
        get_crc32((void *)&dev->lba_table[pos], 256, &table_crc32, table);
        pos += 2;
    }

    dev->gpt_head.entriesCRC32 = table_crc32;

    __u32 head_crc32;

    memset((void *)table, 0, 256);

    crc32_init(&head_crc32, table);

    dev->gpt_head.headerCRC32 = 0;

    get_crc32((void *)&dev->gpt_head, dev->gpt_head.headerSize, &head_crc32, table);

    dev->gpt_head.headerCRC32 = head_crc32;

    dev->fd = open(dev->path, O_WRONLY);

     pwrite(dev->fd, (void *)&dev->gpt_head, sizeof(struct gpt_table_header), 512);

    __u64 offset = 1024;

    for(int i = 0; i < GPT_MAXPART_NUMBER; i++)
    {
        pwrite(dev->fd, (void *)&dev->lba_table[i], sizeof(struct gpt_table_partition), offset);
        offset += 128;
    }

    close(dev->fd);

    return 0;
}

int gpt_create_partition(struct device *dev, int size)
{
    if(size < 2)
    {
        printf("Miminum size = 2 MB\n");
        return -1;
    }

    int i = 0;
    for(i = 0; i < dev->gpt_free_last; i++)
        if(dev->gpt_free[i].size >= (__u32)size) break;

    if(i == dev->gpt_free_last)
    {
        printf("Not enough free space\n");
        return -1;
    }

    int number = 0;
    while(dev->lba_table[number].typeGUID1) number++;

    if(number == GPT_MAXPART_NUMBER)
    {
        printf("Limit of partitions number\n");
        return -1;
    }

    struct gpt_table_partition new_part;
    memset((void *)&new_part, 0, sizeof(struct gpt_table_partition));

    __u32 sect_size = (__u64)size * 1024 * 1024 / 512;

    new_part.startingLBA = dev->gpt_free[i].sect_before;
    new_part.endingLBA = dev->gpt_free[i].sect_before + sect_size - 1;
    new_part.typeGUID1 = TYPE_GUID1_NTFS;
    new_part.typeGUID2 = TYPE_GUID2_NTFS;
    new_part.uniqueGUID1 = new_part.startingLBA ^ new_part.endingLBA;
    new_part.uniqueGUID2 = ~new_part.uniqueGUID1;

    dev->lba_table[number] = new_part;

    __u32 table_crc32;
    __u32 table[256];

    memset((void *)table, 0, 256);

    crc32_init(&table_crc32, table);

    int pos = 0;
    while(pos < 128)
    {
        get_crc32((void *)&dev->lba_table[pos], 256, &table_crc32, table);
        pos += 2;
    }

    dev->gpt_head.entriesCRC32 = table_crc32;

    __u32 head_crc32;

    memset((void *)table, 0, 256);

    crc32_init(&head_crc32, table);

    dev->gpt_head.headerCRC32 = 0;

    get_crc32((void *)&dev->gpt_head, dev->gpt_head.headerSize, &head_crc32, table);

    dev->gpt_head.headerCRC32 = head_crc32;

    dev->fd = open(dev->path, O_WRONLY);

     pwrite(dev->fd, (void *)&dev->gpt_head, sizeof(struct gpt_table_header), 512);

    __u64 offset = 1024;

    for(int i = 0; i < GPT_MAXPART_NUMBER; i++)
    {
        pwrite(dev->fd, (void *)&dev->lba_table[i], sizeof(struct gpt_table_partition), offset);
        offset += 128;
    }

    close(dev->fd);

    create_file_system(dev->path, number + 1);

    return 0;
}
