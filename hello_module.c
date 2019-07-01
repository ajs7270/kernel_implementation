#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static int __init hello_module_init(void)
{
	printk(KERN_EMERG "Hello Module~! I'm jisu \n");
	return 0;
}

static void __exit hello_module_cleanup(void)
{
	printk("<0>Bye Module~!\n");
}

module_init(hello_module_init);
mudule_exit(hello_module_cleanup);

MODULE_LICENSE("GPL");
