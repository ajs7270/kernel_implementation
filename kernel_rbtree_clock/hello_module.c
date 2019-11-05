#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/rbtree.h>
#include <linux/slab.h> // for kmalloc

#define FALSE 0
#define TRUE 1

#define BILLION 1000000000

struct my_type {
	struct rb_node node;
	int key;
	int value;
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


struct my_type *rb_search(struct rb_root *root, int key)
{
	struct rb_node *node = root->rb_node;

	while (node) {
		struct my_type *data = container_of(node, struct my_type, node);

		if(data->key > key)	
			node = node->rb_left;
		else if(data->key < key)	
			node = node->rb_right;
		else
			return data;
	}
	return NULL;
}

int rb_insert(struct rb_root *root, struct my_type *data)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;

	/* Figure out "where" to put new node */
	while (*new) {
		struct my_type *this = container_of(*new, struct my_type, node);
		parent = *new;
		if (this->key > data->key)
			new = &((*new)->rb_left);
		else if (this->key < data->key)
			new = &((*new)->rb_right);
		else
			return FALSE;
	}

	rb_link_node(&data->node, parent, new); 	/*relinking*/
	rb_insert_color(&data->node, root);	 /*recoloring & rebalancing*/

	return TRUE;
}


int rb_delete(struct rb_root *mytree, int key)
{
	struct my_type *data = rb_search(mytree, key);

	if (data) {
		rb_erase(&data->node, mytree);
		kfree(data);
	}
}


void struct_exmaple(void)
{
	struct rb_root my_tree[3] = {RB_ROOT, RB_ROOT, RB_ROOT};
	int index[3] = {1000,10000,100000};
	unsigned long long insert_time[3];
	unsigned long long search_time[3];
	unsigned long long del_time[3];
	struct timespec temp[2];
	int i = 0,j= 0, ret;


	/* rb_node create and insert */	
	for(i =0; i< 3; i++){
		getrawmonotonic(&temp[0]);
		for(j = 0; j <index[i]; j++){
			struct my_type *new = kmalloc(sizeof(struct my_type),GFP_KERNEL);
			if(!new){
				return NULL;
			}
			new->value = j;
			new->key =j;

			ret = rb_insert(&my_tree[i],new);
		}
		getrawmonotonic(&temp[1]);
		insert_time[i] = calculate(temp);
	}


	/* rb_tree find node */
	for(i = 0; i<3; i++){
		getrawmonotonic(&temp[0]);
		struct my_type *find_node = rb_search(&my_tree[i],8);
		getrawmonotonic(&temp[1]);
		search_time[i] = calculate(temp);
	}

	/* rb_tree delete node */
	for(i = 0; i<3; i++){
		getrawmonotonic(&temp[0]);
		rb_delete(&my_tree[i],0);
		getrawmonotonic(&temp[1]);
		del_time[i] = calculate(temp);
	}

	printk("---------------------------\n");

	for(i = 0; i<3;i++){
		printk("[%d] total time \n",index[i]); 
		printk("insert time : %lld\n",insert_time[i]);
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
