#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h> // for thread
#include <linux/slab.h> // for kmalloc
#include <linux/spinlock.h> // for spinlock 

/* global spinlock initialize */
DEFINE_SPINLOCK(my_spinlock);

int test_thread(void *_arg)
{	
	int* arg = (int*)_arg;

	spin_lock(&my_spinlock);
	/* critical section */
	spin_unlock(&my_spinlock);

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
