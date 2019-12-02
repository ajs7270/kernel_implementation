#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h> // for kmalloc
#include <linux/time.h> // for time calculate
#include <linux/kthread.h> // for thread
#include <linux/delay.h>

#define BILLION 1000000000
#define NUM_OF_THREAD 1

DEFINE_SPINLOCK(list_lock);

unsigned long long insert_time = 0; // 1000, 10000, 10000
unsigned long long search_time = 0; 
unsigned long long del_time = 0; 

struct list_head my_list;

struct my_node {
	struct list_head entry;
	int data; 
};

unsigned long long calculate(struct timespec* localtimes)
{
	unsigned long long total_time = 0, time_delay = 0, temp, temp_n;

	if(localtimes[1].tv_nsec >= localtimes[0].tv_nsec){
		temp = localtimes[1].tv_sec - localtimes[0].tv_sec;
		temp_n = localtimes[1].tv_nsec - localtimes[0].tv_nsec;
	}else{
		temp = localtimes[1].tv_sec - localtimes[0].tv_sec - 1;
		temp_n = BILLION + localtimes[1].tv_nsec - localtimes[0].tv_nsec;
	}

	time_delay = BILLION * temp + temp_n;

	__sync_fetch_and_add(&total_time, time_delay);

	return total_time;
}

int insert_thread(void * arg){
	int i;
	struct timespec temp[2];
	getrawmonotonic(&temp[0]);
	for(i = 0; i< 320000; i++){
		struct my_node* new = (struct my_node*)kmalloc(sizeof(struct my_node),GFP_KERNEL);
		spin_lock(&list_lock);
		list_add(&new->entry,&my_list);
		spin_unlock(&list_lock);
	}
	getrawmonotonic(&temp[1]);
	__sync_fetch_and_add(&insert_time,calculate(temp));

	return 0;
}

int traverse(void * arg){
	int i;
	struct timespec temp[2];
	struct my_node *current_node = NULL;


	getrawmonotonic(&temp[0]);
	spin_lock(&list_lock);
	list_for_each_entry(current_node,&my_list,entry){
		// list for each entry 
	}
	spin_unlock(&list_lock);
	getrawmonotonic(&temp[1]);
	__sync_fetch_and_add(&search_time,calculate(temp));
}

void struct_exmaple(void)
{
	struct timespec temp[2];
	int i;
	 
	/* initialize list */
	INIT_LIST_HEAD(&my_list);
	
	/* list element add */
	for(i = 0;i < NUM_OF_THREAD; i++){
		kthread_run(&insert_thread,NULL,"child_thread");
	}

	msleep(10000);

	/* search list */
	for(i = 0;i < NUM_OF_THREAD; i++){
		kthread_run(&traverse,NULL,"child_thread");
	}
	msleep(10000);

	printk("---------------------------\n");

	printk("add time : %lld\n",insert_time);
	printk("search time : %lld\n",search_time);

}

int __init hello_module_init(void)
{

	struct_exmaple();
	printk("module init\n");
	return 0;
}

void __exit hello_module_cleanup(void)
{
	printk("Bye Module\n");
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);
