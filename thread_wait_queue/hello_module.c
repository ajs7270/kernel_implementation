#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h> // for thread
#include <linux/slab.h> // for kmalloc
#include <linux/delay.h>
#include <linux/wait.h> // for waitqueue

int test_thread(void *_arg)
{	
	int* arg = (int*)_arg;
	printk("argument : %d\n", *arg);
	kfree(arg);
	return 0;
}

void thread_create(void)
{
	int i;
	DECLARE_WAIT_QUEUE_HEAD(wait);
	
	/* thread create */
	for(i=0; i<10;i++){
		int* arg = (int*)kmalloc(sizeof(int),GFP_KERNEL);
		*arg = i;
		kthread_run(&test_thread,(void*)arg,"test_thread");
	} 

	if(state){
		printk("print state");
	}

	/*
         * linux doesn't have thread join function
         * so use delay function
         */
        udelay(10);
}


int __init hello_module_init(void)
{
	thread_create();
	printk(KERN_EMERG "Hello Module\n");
	return 0;
}

void __exit hello_module_cleanup(void)
{
	printk("Bye Module\n");
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);
