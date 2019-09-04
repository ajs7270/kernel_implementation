#include <linux/wait.h>

extern wait_queue_head_t jisu_queue;

int test_func(wait_queue_head_t *origin){

	printk("origin : %p\n",origin); 
	printk("jisu: %p\n", &jisu_queue);
	
	return 0;
}
