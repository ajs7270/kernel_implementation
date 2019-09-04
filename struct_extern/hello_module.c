#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/wait.h>
#include "test.c"

DECLARE_WAIT_QUEUE_HEAD(jisu_queue);

int __init hello_module_init(void)
{
	test_func(&jisu_queue);
	printk(KERN_EMERG "Hello Module\n");
	return 0;
}

void __exit hello_module_cleanup(void)
{
	printk("Bye Module\n");
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);
