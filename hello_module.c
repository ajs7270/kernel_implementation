#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <asm/delay.h>
#include "mapfile.h"
#include <linux/slab.h> // for kmalloc

//persistent
#define REMAP_TABLE_SIZE 22 // (origin sector) 11 + (seq sector) 11
#define SECTOR_SIZE 11 // (unsigned long long) 10  + (null) 1
#define MAX_FREEBLOCK_LIST_SIZE 4000

struct free_blocks {
	unsigned long long start_point;
	unsigned long long end_point;
	unsigned long long used_offset; // free space start point 
};

typedef struct argument {
	int num;
}argument_t;

//file read & write
struct file *map_file;

//persistent
struct free_blocks free_block_list[MAX_FREEBLOCK_LIST_SIZE];
int free_list_size = 0;

int jisu_thread(void *arg)
{	
	argument_t* argu = (argument_t*)arg;

	printk("this thread is %d\n",argu->num);	
	printk("this thread is %d\n",argu->num);	
	printk("this thread is %d\n",argu->num);	
	printk("this thread is %d\n",argu->num);	
	printk("this thread is %d\n",argu->num);	
	printk("this thread is %d\n",argu->num);	
	printk("this thread is %d\n",argu->num);	
	printk("this thread is %d\n",argu->num);	
	printk("this thread is %d\n",argu->num);	
	printk("this thread is %d\n",argu->num);	

	return 0;
}

void thread_create(void)
{
	argument_t arg[10];
	int i = 0;

	for(i=0; i<10;i++){
		arg[i].num = i+1;
	}

	for(i=0; i<10;i++){
		kthread_run(&jisu_thread,(void*)&arg[i],"jisu_thread");
	}

	udelay(3);
}

void jisu_file_open(void)
{
	int offset = 0;
	char jisu_string[5] = "jisu";
	unsigned long long jisu_num = 100;

	map_file = (struct file *)file_open("testFile",O_RDWR|O_CREAT,S_IWUSR);
	printk("file is opened");

	printk("ajs: before write offset : %d", offset);
	file_write(map_file,offset,jisu_string,5);
	offset += 10;
	printk("ajs: after write offset : %d", offset);
}

int test_guard_metadata(void)
{
	/*
	 * 1. first free block data read
	 */

	int num_of_read_char;
	unsigned long long start_point, end_point;
	unsigned long long read_pos = 0;

	//make buffer to load start_point and end_point
        unsigned char* buffer = (unsigned char *)kmalloc(sizeof(unsigned char)*REMAP_TABLE_SIZE,GFP_USER);

	//result file have to create using dump.py
	printk("before: map_file pointer: %p\n",map_file);
	map_file = (struct file *)file_open("result",O_RDWR|O_CREAT,S_IWUSR);
	printk("after: map_file pointer: %p\n",map_file);

        //free block table open
        if (NULL != map_file)
        {
		int index = 0;
       		while(num_of_read_char = file_read(map_file, read_pos, buffer, REMAP_TABLE_SIZE)){

			printk("buffer: %s\n",buffer);

			kstrtoll(buffer, SECTOR_SIZE, &start_point);
                        kstrtoll(buffer + SECTOR_SIZE, SECTOR_SIZE, &end_point);

			printk("start_point : %llu\n",start_point);
			printk("end_point : %llu\n",end_point);

                        read_pos += REMAP_TABLE_SIZE;

			/*
			 * block size is 4KB, sector size is 512Byte
			 * start_point is block but free_block_list[index].start_point is sector
			 * so multiply start_point with 8
			 */

			free_block_list[index].start_point = start_point * 8;
			free_block_list[index].end_point = end_point * 8;
			index++;
                }
		free_list_size = index;
        }
	
	// check read list
	int i; 
	for(i = 0; i <free_list_size; i++){
		printk("free_block_list[i].startpoint: %llu\n",free_block_list[i].start_point);
		printk("free_block_list[i].endpoint: %llu\n",free_block_list[i].end_point);
	}
}


int __init hello_module_init(void)
{
	//thread_create()
	//jisu_file_open();
	test_guard_metadata();
	printk(KERN_EMERG "Hello Module~! I'm jisu \n");
	return 0;
}

void __exit hello_module_cleanup(void)
{
	printk("<0>Bye Module~!\n");
	printk("file_close: map_file pointer: %p\n",map_file);
	file_close(map_file);
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);

MODULE_LICENSE("GPL");
