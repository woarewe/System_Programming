#include "gpt_table.h"
#include "mbr_table.h"

int main(int argc, char **argv)
{
    if(argc != 2 && !check_argument(argv[1]))
    {
        printf("Example to run the program: program_name /dev/sd*");
        return 1;
    }

    int choise;

    struct device dev;

    printf("Partition table creator 1.1.0");

    while(1)
    {
        if( mbr_init(&dev, argv[1]) < 0) return -1;

        if(dev.type == GPT)
            gpt_read_table(&dev);

        printf("\n");
        printf("1. Get informaition about partition table.\n");
        printf("2. Add new section\n");
        printf("3. Delete section\n");
        printf("0. Exit\n");
        printf("::> ");
        scanf("%d", &choise);
        printf("\n");
        switch(choise)
        {
        case 1:
        {
            printf("Type of partiton table --> ");
            if(dev.type == GPT) printf("GPT\n");
                    else printf("MBR\n");
            printf("\n");
            if(dev.type == MBR)
            {
                show_mbr_table(&dev);
                show_mbr_free_space(&dev);
            }
            else
            {
                gpt_show_table(&dev);
                gpt_show_free_space(&dev);
            }
        }
            break;
        case 2:
        {
            if(dev.type == MBR)
               mbr_create_new_partition(&dev);
            else
            {
                int size;
                printf("Enter please size of new partition: ");
                scanf("%d", &size);
                printf("\n");
                gpt_create_partition(&dev, size);
            }
        }
            break;
        case 3:
        {

            int number;
            printf("Enter please partition number: ");
            scanf("%d", &number);
            if(dev.type == MBR)
            {
                dev.fd = open(argv[1], O_WRONLY);
                mbr_delete_partition(&dev, number);
                close(dev.fd);
            }
            else gpt_delete_partition(&dev, number - 1);
        }
            break;
        case 0:
        {
            return 0;
            break;
        }
        default:
            printf("Try again!!!\n");
            break;
        }
    }

    return 0;
}
