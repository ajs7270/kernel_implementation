#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/rbtree.h>
#include <linux/slab.h> // for kmalloc

struct my_node {
	struct rb_node node;
	// 100, 300, 200
	// 100, 108, 116 //sequntialize
	int key; // rbtree를 정렬하기 위한 기준
       	int value1;
	int value2;
	int value3;
/*
           (200,116)
	   /       \
    (100,100)     (300,108)
    /       \     /       \
    null    null  null   null
*/
};

struct my_node *rb_search(struct rb_root *root, int key)
{
	struct rb_node *node = root->rb_node;

	while (node) {
		struct my_node *data = container_of(node, struct my_node, node)

	}

	return NULL;
}

void struct_exmaple(void)
{
	struct my_node *root;
	struct my_node *hmm;
	struct rb_node **link = &root.node;
	struct rb_node *parent = NULL;
	
	struct rb_root my_tree = RB_ROOT;


	while(*link){
		parent = *link;
		entry = rb_entry(parent, struct my_node,node );
		if(root->data < hmm->data)
			parent = &(*parent)->rb_left;
		else if()
	
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
