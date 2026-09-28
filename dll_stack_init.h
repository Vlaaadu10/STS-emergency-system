//Stefan Alexandru Vladut 314CA
#ifndef DLL_STACK_INIT_H
#define DLL_STACK_INIT_H
#include "declarations.h"

//declararea functiilor esentiale de la laborator pentru dll
doubly_linked_list_t *dll_create(unsigned int data_size);
dll_node_t *get_nth_node(doubly_linked_list_t *list, unsigned int n);
void ll_add_nth_node(doubly_linked_list_t *list, unsigned int n,
				     const void *new_data);
dll_node_t *ll_remove_nth_node(doubly_linked_list_t *list, unsigned int n);

//declararea functiilor esentiale de la laborator pentru stiva folosind dll
stack_t *st_create(unsigned int data_size);
unsigned int st_is_empty(stack_t *st);
void *st_peek(stack_t *st);
void st_push(stack_t *st, void *new_data);
void st_pop(stack_t *st);

//declararea functiilor esentiale pentru coada folosind dll
void dll_add_queue(doubly_linked_list_t *queue, const void *new_data);
void dll_add_front(doubly_linked_list_t *queue, const void *new_data);
dll_node_t *remove_first(doubly_linked_list_t *list);
#endif
