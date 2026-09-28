//Stefan Alexandru Vladut 314CA
#include "utils.h"

//adaugarea nodului santinela la inceputului listei de incidente
void add_santinela_incident(doubly_linked_list_t *list)
{
	struct incident fictiv;
	fictiv.id = 0;
	strcpy(fictiv.priority, "low");
	fictiv.description = strdup("test incident");
	strcpy(fictiv.status, "solved");

	dll_node_t *curr =  malloc(sizeof(dll_node_t));
	DIE(!curr, "Malloc failed()!\n");
	curr->data = malloc(list->data_size);
	DIE(!curr->data, "Malloc failed()!\n");
	memcpy(curr->data, &fictiv, list->data_size);
	curr->next = curr;
	curr->prev = curr;
	list->head = curr;
	list->size = 1;
}

//adaugarea nodului santinela la inceputului listei de interventii
void add_santinela_interventie(doubly_linked_list_t *list)
{
	struct intervention fictiv;
	fictiv.incident = NULL;
	fictiv.unit = NULL;

	dll_node_t *curr = malloc(sizeof(dll_node_t));
	DIE(!curr, "Malloc failed()!\n");
	curr->data = malloc(list->data_size);
	DIE(!curr->data, "Malloc failed()!\n");
	memcpy(curr->data, &fictiv, list->data_size);
	curr->next = curr;
	curr->prev = curr;
	list->head = curr;
	list->size = 1;
}

/*cu flag-ul ok verific daca e lista de incidente
pentru a da free si la campul description*/
void
dll_free(doubly_linked_list_t **pp_list, int ok)
{
	if (!pp_list || !*pp_list)
		return;
	dll_node_t *new;
	new = (*pp_list)->head;
	for (unsigned int i = 0; i < (*pp_list)->size; i++) {
		dll_node_t *tmp = new;
		new = new->next;
		if (ok == 1) {
			struct incident *aux = (struct incident *)tmp->data;
			if (aux->description) {
				free(aux->description);
			}
		}
		free(tmp->data);
		free(tmp);
	}
	free(*pp_list);
	*pp_list = NULL;
}

//free pentru stiva folosindu-ne de functia de free pentru dll
void
st_free(stack_t **st)
{
	dll_free(&(*st)->list, 0);
	free(*st);
	*st = NULL;
}
