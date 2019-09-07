#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h> // for kmalloc

struct my_node {
	struct list_head entry;
	int data; 
};

void struct_exmaple(void)
{
	struct list_head my_list;
	 
	/* initialize list */
	INIT_LIST_HEAD(&my_list);

	/* list element add */
	int i;
	for (i = 0; i<10;i++){
		struct my_node *new =  kmalloc(sizeof(struct my_node),GFP_KERNEL);
		new->data = i;
		list_add(&new->entry,&my_list);
	}

	struct my_node *current_node = NULL;
	/* check list */
	list_for_each_entry(current_node,&my_list,entry){
		printk("current value : %d\n", current_node->data);
	}

	printk("---------------------------\n");

	/* check list reverse*/
	list_for_each_entry_reverse(current_node,&my_list,entry){
		printk("current value : %d\n", current_node->data);
	}
	
	printk("---------------------------\n");

	/* list element delete */
	struct my_node *tmp;
	list_for_each_entry_safe(current_node, tmp, &my_list,entry){
		if(current_node->data == 2){
			printk("current node value : %d\n", current_node->data);
			list_del(&current_node->entry);
			kfree(current_node);
		}
	}

	printk("---------------------------\n");

	/* check list */
	list_for_each_entry(current_node,&my_list,entry){
		printk("current value : %d\n", current_node->data);
	}

	printk("---------------------------\n");
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
