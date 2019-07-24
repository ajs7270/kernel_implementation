#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <asm/delay.h>

typedef struct argument {
	int num;
}argument;

int jisu_thread(void *arg)
{	
	argument* argu = (argument*)arg;

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

int __init hello_module_init(void)
{
	argument arg[10];
	int i = 0;

	for(i=0; i<10;i++){
		arg[i].num = i+1;
	}

	for(i=0; i<10;i++){
		kthread_run(&jisu_thread,(void*)&arg[i],"jisu_thread");
	}

	udelay(3);
	printk(KERN_EMERG "Hello Module~! I'm jisu \n");
	return 0;
}

void __exit hello_module_cleanup(void)
{
	printk("<0>Bye Module~!\n");
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);

MODULE_LICENSE("GPL");
