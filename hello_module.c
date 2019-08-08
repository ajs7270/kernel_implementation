#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <asm/delay.h>
#include "mapfile.h"

typedef struct argument {
	int num;
}argument_t;

struct file *map_file;

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
/*
	*
	* jisu thread create
	*
*/
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

int __init hello_module_init(void)
{
	//thread_create()
	jisu_file_open();
	printk(KERN_EMERG "Hello Module~! I'm jisu \n");
	return 0;
}

void __exit hello_module_cleanup(void)
{
	printk("<0>Bye Module~!\n");
	file_close(map_file);
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);

MODULE_LICENSE("GPL");
