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


    if(dev.type == GPT)
    {
        // ...
    }

    printf("Partition table creator 1.1.0");

    while(1)
    {
        if( mbr_init(&dev, argv[1]) < 0) return -1;

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
        }
            break;
        case 2:
        {
            if(dev.type == MBR)
               mbr_create_new_partition(&dev);
        }
            break;
        case 3:
        {

            int number;
            printf("Enter please partition number: ");
            scanf("%d", &number);
            dev.fd = open(argv[1], O_WRONLY);
            if(dev.type == MBR)
                mbr_delete_partition(&dev, number);
            close(dev.fd);
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
