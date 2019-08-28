#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h> // for thread
#include <linux/slab.h> // for kmalloc
#include <linux/mutex.h> // for mutex 
#include <linux/delay.h> // for delay 

/* global mutex initialize */
typedef struct argument {
	int value;
	struct mutex my_mutex;
}argument_t;

int test_thread(void *arg)
{	
	argument_t* argu = (argument_t*)arg;

	mutex_lock(&argu->my_mutex);
	/* critical section */
	mutex_unlock(&argu->my_mutex);

	return 0;
}

void thread_create(void)
{
	argument_t arg[10];

	int i;
	/* struct mutex initialize */
	for(i=0; i<10;i++){
		arg[i].value = i+1;
		mutex_init(&arg[i].my_mutex);	
	}

	/* thread create */
	for(i=0; i<10;i++){
		kthread_run(&test_thread,(void*)&arg[i],"test_thread");
	}

	/*
	 * linux doesn't have thread join function
	 * so use delay function
	 */
	udelay(3);
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
