#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/rbtree.h>
#include <linux/slab.h> // for kmalloc

#define FALSE 0
#define TRUE 1

struct my_type {
	struct rb_node node;
	int key;
	int value;
};

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
	struct rb_root my_tree = RB_ROOT;
	int i = 0, ret;

	/* rb_node create and insert */
	for(;i<20;i++){
		struct my_type *new =kmalloc(sizeof(struct my_type),GFP_KERNEL);
		if(!new)
			return NULL;

		new->value = i*10;
		new->key = i;

		ret = rb_insert(&my_tree,new);
	}

	/* rb_tree traversal using iterator */
	struct rb_node *node; 
	for (node = rb_first(&my_tree); node; node = rb_next(node)) 
		printk("(key,value)=(%d.%d)\n", rb_entry(node, struct my_type, node)->key,rb_entry(node, struct my_type, node)->value); 


	/* rb_tree find node */
	struct my_type *find_node = rb_search(&my_tree,8);
	if(!find_node){
		return NULL;
	}	
	printk("find : (key,value)=(%d.%d)\n", find_node->key,find_node->value); 


	/* rb_tree delete node */
	rb_delete(&my_tree,0);
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
