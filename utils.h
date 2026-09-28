//Stefan Alexandru Vladut 314CA
#ifndef UTILS_H
#define UTILS_H
#include "declarations.h"

//declararea celor 2 functii de adaugare a santinelei si cele 2 functii de free
void add_santinela_incident(doubly_linked_list_t *list);
void add_santinela_interventie(doubly_linked_list_t *list);
void dll_free(doubly_linked_list_t **pp_list, int ok);
void st_free(stack_t **st);
#endif
