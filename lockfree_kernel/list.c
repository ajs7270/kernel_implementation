#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h> // for kmalloc
#include <linux/kthread.h> // for thread
#include <linux/time.h> // for time calculate
#include <linux/delay.h>
#include <stdbool.h>

#define NUM_OF_THREAD 1
#define BILLION 1000000000

unsigned long long insert_time = 0;
unsigned long long delete_time = 0;
unsigned long long traverse_time = 0;

struct node {
	struct node* next;
	struct node* prev;
	struct node* gc_next;
	struct node* gc_prev;

	bool del; // false : 정상, true: 지워질 예정
	int data;
};

struct node* head;
struct node* tail;
struct node* gc_head;
struct node* gc_tail;

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

void add_tail(struct node* new, int data)
{
	new->data = data;
	new->gc_next = NULL;
	new->gc_prev = NULL;
	new->next = NULL;

	new->prev = __sync_lock_test_and_set(&tail, new);

	if(new->prev ==NULL){
		__sync_lock_test_and_set(&head, new);
	}else{
		__sync_lock_test_and_set(&new->prev->next, new);
	}
}

void delete(struct node* selected) 
{
	//logical delete
	__sync_lock_test_and_set(&selected->del, true);

	// add gc list
	selected->gc_prev = __sync_lock_test_and_set(&gc_tail, selected);
	if(selected->gc_prev == NULL){
		gc_head = selected;
	}else{
		selected->gc_prev->gc_next = selected;
	}
}

void delete_node(int value)
{
	struct timespec temp[2];
	struct node * traverse_head = head;	
	
	getrawmonotonic(&temp[0]);
	do{
		if(!traverse_head){
			break;
		}

		if(__sync_bool_compare_and_swap(&traverse_head->del,false,false)){
			//traverse
			if(traverse_head->data == value){
				delete(traverse_head);
				break;
			}
		}
	}while(__sync_lock_test_and_set(&traverse_head, traverse_head->next));
	getrawmonotonic(&temp[1]);
	__sync_fetch_and_add(&delete_time,calculate(temp));
}

int traverse(void *arg)
{
	struct timespec temp[2];
	struct node * traverse_head = head;	
	
	getrawmonotonic(&temp[0]);
	do{
		if(!traverse_head){
			break;
		}

		if(__sync_bool_compare_and_swap(&traverse_head->del,false,false)){
			//traverse
			//printk("value : %d\n", traverse_head->data);
		}
	}while(__sync_lock_test_and_set(&traverse_head, traverse_head->next));

	getrawmonotonic(&temp[1]);
	__sync_fetch_and_add(&traverse_time,calculate(temp));

	return 0;
}

void free_gc_list(void)
{
	struct node * next;
	struct node * cur;

	do{	
		if(gc_head)
			break;
		// linking 
		gc_head->prev->next = gc_head->next;
		gc_head->next->prev = gc_head->prev;
 
		// free currnet node
		cur = __sync_val_compare_and_swap(&gc_head, gc_head, gc_head->next);
		kfree(cur);
	}while(gc_head);

	__sync_lock_test_and_set(&gc_tail, NULL);
}

int insert_thread(void * arg){
	int i;
	struct timespec temp[2];
	getrawmonotonic(&temp[0]);
	for(i = 0; i< 320000; i++){
		struct node* new = (struct node*)kmalloc(sizeof(struct node),GFP_KERNEL);
		add_tail(new,i);
	}
	getrawmonotonic(&temp[1]);
	__sync_fetch_and_add(&insert_time,calculate(temp));

	return 0;
}


int struct_example(void) {
	int i;
	for(i = 0;i < NUM_OF_THREAD; i++){
		kthread_run(&insert_thread,NULL,"child_thread");
	}

	msleep(5000);
	printk("finish:\n");
	delete_node(5);

	printk("------------\n");
	for(i = 0;i < NUM_OF_THREAD; i++){
		kthread_run(&traverse,NULL,"child_thread");
	}
	msleep(5000);

	//free_gc_list();
	printk("------------\n");

	return 0;
}

int __init hello_module_init(void)
{

	struct_example();
	printk("module init\n");
	return 0;
}

void __exit hello_module_cleanup(void)
{
	printk("add time : %lld\n",insert_time);
	printk("search time : %lld\n",traverse_time);
	printk("del time : %lld\n",delete_time);

	printk("Bye Module\n");
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);
