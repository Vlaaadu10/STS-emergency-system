//Stefan Alexandru Vladut 314CA
#include "dll_stack_init.h"

//definirea functiilor pentru a crea dll, respectiva stiva
doubly_linked_list_t*
dll_create(unsigned int data_size)
{
	doubly_linked_list_t *list = malloc(sizeof(doubly_linked_list_t));
	DIE(!list, "Malloc failed()!\n");
	list->head = NULL;
	list->data_size = data_size;
	list->size = 0;
	return list;
}

stack_t *
st_create(unsigned int data_size)
{
	struct stack_t *stiva = malloc(sizeof(struct stack_t));
	DIE(!stiva, "Malloc failed()!\n");
	stiva->list = dll_create(data_size);
	return stiva;
}

//cele 3 functii esentiale(adaugare, obtinere, eliminare) intr-un dll
dll_node_t *get_nth_node(doubly_linked_list_t *list, unsigned int n)
{
	if (!list) {
		return NULL;
	}

	unsigned int len = list->size - 1;
	unsigned int i;
	dll_node_t *node = list->head;

	n = MIN(n, len);

	for (i = 0; i < n; ++i) {
		node = node->next;
	}

	return node;
}

void
ll_add_nth_node(doubly_linked_list_t *list, unsigned int n,
				const void *new_data)
{
	dll_node_t *prev, *curr;
	dll_node_t *new_node;

	if (!list) {
		return;
	}

	if (n > list->size) {
		n = list->size;
	}

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	new_node = malloc(sizeof(*new_node));
	DIE(!new_node, "new_node malloc");
	new_node->data = malloc(list->data_size);
	DIE(!new_node->data, "new_node->data malloc");
	memcpy(new_node->data, new_data, list->data_size);

	new_node->next = curr;
	new_node->prev = prev;
	if (!prev) {
		list->head = new_node;
	} else {
		prev->next = new_node;
	}
	if (curr) {
		curr->prev = new_node;
	}
	list->size++;
}

dll_node_t *
ll_remove_nth_node(doubly_linked_list_t *list, unsigned int n)
{
	dll_node_t *prev, *curr;
	if (!list || !list->head) {
		return NULL;
	}

	if (n > list->size - 1) {
		n = list->size - 1;
	}

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	if (!prev) {
		list->head = curr->next;
		if (list->head) {
			list->head->prev = NULL;
		}
	} else {
		prev->next = curr->next;
		if (curr->next) {
			curr->next->prev = prev;
		}
	}

	list->size--;
	return curr;
}

//cele 4 functii esentiale(peek, push, pop, isEmpty) ale unei stive
void *
st_peek(stack_t *st)
{
	if (!st || st_is_empty(st))
		return NULL;
	return get_nth_node(st->list, st->list->size - 1)->data;
}

void
st_push(stack_t *st, void *new_data)
{
	if (!st)
		return;
	return ll_add_nth_node(st->list, st->list->size, new_data);
}

void
st_pop(stack_t *st)
{
	if (!st || st_is_empty(st) == 1)
		return;
	dll_node_t *remove = ll_remove_nth_node(st->list, st->list->size - 1);
	free(remove->data);
	free(remove);
}

unsigned int
st_is_empty(stack_t *st)
{
	if (!st || st->list->size == 0) {
		return 1;
	} else
		return 0;
}

//functiile de adaugare in coada, respectiv adaugare la inceput a unui nou element
void dll_add_queue(doubly_linked_list_t *queue, const void *new_data)
{
	dll_node_t *new = malloc(sizeof(dll_node_t));
	DIE(!new, "Malloc failed()!\n");
	new->data = malloc(queue->data_size);
	DIE(!new->data, "Malloc failed()!\n");
	memcpy(new->data, new_data, queue->data_size);

	if (!queue->head) {
		queue->head = new;
		new->next = NULL;
		new->prev = NULL;
	} else {
		dll_node_t *curr = queue->head;
		for (unsigned int i = 0 ; i < queue->size && curr->next; i++) {
			curr = curr->next;
		}
		curr->next = new;
		new->prev = curr;
		new->next = NULL;
	}
	queue->size++;
}

void dll_add_front(doubly_linked_list_t *queue, const void *new_data)
{
	dll_node_t *new = malloc(sizeof(dll_node_t));
	DIE(!new, "Malloc failed()!\n");
	new->data = malloc(queue->data_size);
	memcpy(new->data, new_data, queue->data_size);
	if (!queue->head) {
		queue->head = new;
		queue->head->prev = NULL;
		queue->head->next = NULL;
	} else {
		new->next = queue->head;
		queue->head->prev = new;
		queue->head = new;
		new->prev = NULL;
	}
	queue->size++;
}

//functie pentru eliminarea primului element dintr-un dll
dll_node_t *remove_first(doubly_linked_list_t *list)
{
	if (!list || list->size == 0)
		return NULL;
	dll_node_t *remove = list->head;
	if (list->size == 1) {
		list->head = NULL;
	} else {
		list->head = list->head->next;
		list->head->prev = NULL;
	}
	list->size--;
	return remove;
}