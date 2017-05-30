#include <stdio.h>
#include <linux/types.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 50

struct mem_sector
{
    __u8 **pointer;
    __u8 before;
    __u8 total;
};

struct free_block
{
    __u8 before;
    __u8 total;
    __u8 size;
};


struct global_buffer
{
    struct mem_sector sectors[MAX_SIZE];
    __u8 flags[MAX_SIZE];
    __u8 buffer[MAX_SIZE];
    int sector_number;
} global_memory;


void init()
{
    memset((void *)&global_memory, 0, sizeof(struct global_buffer));
}

__u8 *getMemory(__u8 **pointer, size_t size)
{
    int start = -1;
    for(int i = 0; i < MAX_SIZE; i++)
    {
        if(!global_memory.flags[i])
        {
            int j = i;
            size_t block = 0;
            while(1)
            {
                if(!global_memory.flags[j]) block++;
                else break;
                if(block == size) break;
                j++;
            }
            if (size == block)
            {
                start = i;
                break;
            }
        }
    }
    if(start >= 0)
    {
        struct mem_sector sector;
        sector.pointer = pointer;
        sector.before = start;
        sector.total = size;

        memset((void *)(global_memory.flags + start), 1, size);

        global_memory.sectors[global_memory.sector_number++] = sector;

        return &global_memory.buffer[start];
    }
    else return NULL;
}

void free_pointer(__u8 *pointer)
{
    if(pointer == NULL)
    {
        printf("Null pointer exception\n");
        exit(1);
    }
    int position = -1;
    for(int i = 0; i < global_memory.sector_number; i++)
        if(global_memory.sectors[i].pointer)
        if(*global_memory.sectors[i].pointer == pointer)
        {
            position = i;
            break;
        }
    if(position < 0)
    {
        printf("Pointer undefined\n");
        exit(1);
    }

    memset((void *)&global_memory.flags[global_memory.sectors[position].before], 0, global_memory.sectors[position].total);
    global_memory.sectors[position].before = 0;
    global_memory.sectors[position].total = 0;
    *global_memory.sectors[position].pointer = NULL;
    global_memory.sectors[position].pointer = NULL;
}

void defragmentation()
{
    int start, position = 0;
    for(int i = 0; i < MAX_SIZE; i++)
    {

        if(!global_memory.flags[i])
        {
            start = i;
            while(!global_memory.flags[start] && start < MAX_SIZE) start++;
            if(start == MAX_SIZE) break;

            for(; position < global_memory.sector_number; position ++)
                if(global_memory.sectors[position].before == start) break;

            int k = i;
            memcpy((void *)&global_memory.buffer[k], (void *)&global_memory.buffer[start],  global_memory.sectors[position].total );
            memset((void *)&global_memory.flags[start], 0, global_memory.sectors[position].total);
            memset((void *)&global_memory.flags[i], 1, global_memory.sectors[position].total);
            global_memory.sectors[position].before = i;
            *global_memory.sectors[position].pointer = &global_memory.buffer[i];
        }
    }
}

__u8 *resizePointer(__u8 *pointer, size_t size)
{
    int position = -1;
    for(int i = 0; i < global_memory.sector_number; i++)
        if(global_memory.sectors[i].pointer)
        if(*global_memory.sectors[i].pointer == pointer)
        {
            position = i;
            break;
        }
    if(position < 0)
    {
        printf("Pointer undefined\n");
        exit(1);
    }

    int extra = 0;

    for(int i = global_memory.sectors[position].before +  global_memory.sectors[position].total; i < MAX_SIZE; i++)
    {
        if(!global_memory.flags[i]) extra++;
        else break;
        if(global_memory.sectors[position].total + extra == size) break;
    }

    if(global_memory.sectors[position].total + extra == size)
    {
        global_memory.sectors[position].total = size;
        memset((void *)&global_memory.flags[global_memory.sectors[position].before], 1, global_memory.sectors[position].total);
        return *global_memory.sectors[position].pointer;
    }
    else
    {
        free_pointer(pointer);
        __u8 *ptr;
        ptr = getMemory(&ptr, size);
        return ptr;
    }
}

void showMemory()
{
    for(int i = 0; i < MAX_SIZE; i++)
        if(global_memory.flags[i]) printf("%c", '*');
        else printf("%c", '_');
    printf("\n");
}



int main()
{
    init();

    __u8 *a, *b, *c;

    a = getMemory(&a, sizeof(__u8));

    b = getMemory(&b, sizeof(__u8));

    c = getMemory(&c, sizeof(__u8));

    *a = 34;

    *b = 34;

    *c = 111;

    showMemory();

    c = resizePointer(c, sizeof(__u8) * 3);
    for(int i = 0; i < 3; i++)
      *(c + i) = i;



    showMemory();


    __u8 *d, *e;
    d = getMemory(&d, sizeof(__u8));
    e = getMemory(&e, sizeof(__u8) * 3);

    for(int i = 0; i < 3; i++)
      *(e + i) = i + 3;
    showMemory();

    free_pointer(b);
    free_pointer(d);

    showMemory();

    printf("%d\n", *a);

    for(int i = 0; i < 3 ; i++)
        printf("%d\n", c[i]);

    for(int i = 0; i < 3 ; i++)
        printf("%d\n", e[i]);

    defragmentation();

    showMemory();

    printf("%d\n", *a);

    for(int i = 0; i < 3 ; i++)
        printf("%d\n", c[i]);


    for(int i = 0; i < 3 ; i++)
        printf("%d\n", e[i]);


    return 0;
}
