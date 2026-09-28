//Stefan Alexandru Vladut 314CA
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "declarations.h"

//declararea celor 8 functionalitati ale sistemului STS
void add_incident(doubly_linked_list_t *list, doubly_linked_list_t *queue_high,
				  doubly_linked_list_t *queue_medium,
				  doubly_linked_list_t *queue_low,
				  int id, char *priority, char *description);

void check_units_availability(FILE *out, doubly_linked_list_t *list);

void dispatch(doubly_linked_list_t *queue_available_units,
			  doubly_linked_list_t *interventions_list,
			  doubly_linked_list_t *queue_high,
			  doubly_linked_list_t *queue_medium,
			  doubly_linked_list_t *queue_low,
			  stack_t *istoric, FILE *out);


void undo_last_dispatch(doubly_linked_list_t *incidents_list,
						doubly_linked_list_t *queue_available_units,
						doubly_linked_list_t *interventions_list,
						doubly_linked_list_t *queue_high,
						doubly_linked_list_t *queue_medium,
						doubly_linked_list_t *queue_low,
						stack_t *istoric, FILE *out);

void show_unit(struct unit units[], int id_unit, int nr, FILE *out);
void show_incident(doubly_linked_list_t *incidents, int id_incident, FILE *out);
void show_interventions(doubly_linked_list_t *interventions, FILE *out);
void solved_incident(doubly_linked_list_t *interventions, int id_incident,
					 doubly_linked_list_t *queue_available_units,
					 FILE *out);
                     
#endif
