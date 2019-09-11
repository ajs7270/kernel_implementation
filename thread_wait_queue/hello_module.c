#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h> // for thread
#include <linux/slab.h> // for kmalloc
#include <linux/delay.h>
#include <linux/wait.h> // for waitqueue

/* static declaration */
DECLARE_WAIT_QUEUE_HEAD(wait);
/* define condition and set false */
bool status = false;

int test_thread(void *_arg)
{	
	int* arg = (int*)_arg;
        udelay(100);
	kfree(arg);

	/* parent thread wake up */
	status = true;
	wake_up(&wait);
	
	return 0;
}

void thread_create(void)
{
	
	/* thread create */
	int* arg = (int*)kmalloc(sizeof(int),GFP_KERNEL);
	*arg = 10;
	kthread_run(&test_thread,(void*)arg,"test_thread");

	/* parent thread sleep */
	wait_event_interruptible(wait,status);
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
