#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


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

void add_tail(struct node* new, int data)
{
	new->data = data;
	new->gc_next = NULL;
	new->gc_prev = NULL;
	new->next = NULL;
	new->prev = __sync_val_compare_and_swap(&tail, tail, new); 

	if(new->prev ==NULL){
		head = new;
	}else{
		new->prev->next = new;
	}
}

void delete(struct node* selected) 
{
	//logical delete
	__sync_lock_test_and_set(&selected->del, true);

	// add gc list
	selected->gc_prev = __sync_val_compare_and_swap(&gc_tail, gc_tail, selected);
	if(selected->prev == NULL){
		gc_head = selected;
	}else{
		if(selected->gc_prev){
			selected->gc_prev->gc_next = selected;
		}

	}
}

void delete_node(int value)
{
	struct node * traverse_head = head;	
	
	do{
		if(!traverse_head){
			break;
		}

		if(__sync_bool_compare_and_swap(&traverse_head->del,false,false)){
			//traverse
			if(traverse_head->data == value){
				delete(traverse_head);
			}
		}
	}while(__sync_val_compare_and_swap(&traverse_head, traverse_head, traverse_head->next));
}


void traverse()
{
	struct node * traverse_head = head;	
	
	do{
		if(!traverse_head){
			break;
		}

		if(__sync_bool_compare_and_swap(&traverse_head->del,false,false)){
			//traverse
			printf("value : %d\n", traverse_head->data);
		}
	}while(__sync_val_compare_and_swap(&traverse_head, traverse_head, traverse_head->next));
}

void free_gc_list()
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
		free(cur);
	}while(gc_head);
}

void main() {
	for(int i = 0; i< 10; i++){
		struct node* new = (struct node*)malloc(sizeof(struct node));
		add_tail(new, i);
	}

	traverse();
	delete_node(5);
	traverse();
}
