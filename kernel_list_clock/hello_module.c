#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h> // for kmalloc
#include <linux/time.h> // for time calculate

#define BILLION 1000000000

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


void struct_exmaple(void)
{
	struct list_head my_list[3];
	int index[3] = {1000,10000,100000};
	unsigned long long add_time[3]; // 1000, 10000, 10000
	unsigned long long search_time[3]; 
	unsigned long long del_time[3]; 
	struct timespec temp[2];
	int i,j;
	 
	/* initialize list */
	INIT_LIST_HEAD(&my_list[0]);
	INIT_LIST_HEAD(&my_list[1]);
	INIT_LIST_HEAD(&my_list[2]);
	
	/* list element add */
	for(i = 0; i<3;i++){
		getrawmonotonic(&temp[0]);
		for (j = 0; j<index[i];j++){
			struct my_node *new =  kmalloc(sizeof(struct my_node),GFP_KERNEL);
			new->data = j;
			list_add(&new->entry,&my_list[i]);
		}
		getrawmonotonic(&temp[1]);
		add_time[i] = calculate(temp);
	}

	/* search list */
	struct my_node *current_node = NULL;
	for(i = 0; i<3;i++){
		getrawmonotonic(&temp[0]);
		list_for_each_entry(current_node,&my_list[i],entry){
			// list for each entry 
		}
		getrawmonotonic(&temp[1]);
		search_time[i] = calculate(temp);
	}

	/* list element delete */
	struct my_node *tmp;
	for(i = 0; i<3;i++){
		getrawmonotonic(&temp[0]);
		list_for_each_entry_safe(current_node, tmp, &my_list[i] ,entry){
			list_del(&current_node->entry);
			kfree(current_node);
		}
		getrawmonotonic(&temp[1]);
		del_time[i] = calculate(temp);
	}
	printk("---------------------------\n");

	for(i = 0; i<3;i++){
		printk("%d :  total time \n",index[i]); 
		printk("add time : %lld\n",add_time[i]);
		printk("search time : %lld\n",search_time[i]);
		printk("del time : %lld\n",del_time[i]);
	}

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
