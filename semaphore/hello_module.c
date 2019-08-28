#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h> // for thread
#include <linux/slab.h> // for kmalloc
#include <linux/semaphore.h> // for spinlock 
#include <linux/delay.h>

/* global spinlock initialize */
DEFINE_SPINLOCK(my_spinlock);

struct semaphore my_sem; 
int test_thread(void *_arg)
{	
	int* arg = (int*)_arg;

	down_interruptible(&my_sem);
	/* critical section */
	up(&my_sem);

	return 0;
}

void thread_create(void)
{
	int i;
	/* thread create */
	for(i=0; i<10;i++){
		int arg = i;
		kthread_run(&test_thread,(void*)&arg,"test_thread");
	}

	/*
         * linux doesn't have thread join function
         * so use delay function
         */
        udelay(10);


}


int __init hello_module_init(void)
{
	sema_init(&my_sem, 1);
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
