#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/hashtable.h>
#include <linux/slab.h> // for kmalloc

#define MY_HASH_BITS 2

struct my_node {
	u32 key;
	int value; 
	struct hlist_node hnode;
};

void struct_exmaple(void)
{
	DEFINE_HASHTABLE(my_hash, MY_HASH_BITS);
	hash_init(my_hash);

	/* (key,value) insert */
	int i;
	for(i=0;i<10;i++){
		struct my_node *new = kmalloc(sizeof(struct my_node), GFP_KERNEL);
		new->value = i * 10;
		new->key = i;
		memset(&new->hnode, 0, sizeof(struct hlist_node));
		hash_add(my_hash,&new->hnode,new->key);
	}

	/* circuit every bucket */
	int bkt;
	struct my_node *cur;
	hash_for_each(my_hash,bkt,cur,hnode){
		printk("key,value : %d,%d is in bucket %d \n", cur->key, cur->value, bkt);
	}
	printk("--------------------------\n");

	/* 3th bucket search */
	hlist_for_each_entry(cur, &my_hash[3], hnode){
		printk("key,value : %d,%d in the bucket\n", cur->key, cur->value);
	}
	printk("--------------------------\n");

	/* 3th bucket delete */
	struct hlist_node* tmp;
	hlist_for_each_entry_safe(cur,tmp, &my_hash[3], hnode){
		hash_del(&cur->hnode);
	}
	printk("--------------------------\n");
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
