#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <asm/delay.h>
#include <linux/slab.h> // for kmalloc
#include <linux/mutex.h>

//mutex
// global mutex 



// struct mutex
typedef struct argument {
	int value;
	struct mutex test_mutex;
}argument_t;



struct free_blocks {
	unsigned long long start_point;
	unsigned long long end_point;
	unsigned long long used_offset; // free space start point 
};


int test_thread(void *arg)
{	
	
	argument_t* argu = (argument_t*)arg;
	mutex_lock(&argu->test_mutex);
	printk("this thread is %d\n",argu->value);
	mutex_unlock(&argu->test_mutex);
	return 0;
}

void thread_create(void)
{
	argument_t arg[10];
	int i = 0;

	for(i=0; i<10;i++){
		arg[i].value = i+1;
		mutex_init(&arg[i].test_mutex);	
	}

	for(i=0; i<10;i++){
		kthread_run(&test_thread,(void*)&arg[i],"test_thread");
	}

	udelay(3);
}


int __init hello_module_init(void)
{
	thread_create();
	printk(KERN_EMERG "Hello Module~! I'm jisu \n");
	return 0;
}

void __exit hello_module_cleanup(void)
{
	printk("<0>Bye Module~!\n");
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);

//MODULE_LICENSE(;

