//Stefan Alexandru Vladut 314CA
#include "operations.h"
#include "dll_stack_init.h"
#include "utils.h"

void add_incident(doubly_linked_list_t *list, doubly_linked_list_t *queue_high,
				  doubly_linked_list_t *queue_medium,
				  doubly_linked_list_t *queue_low,
				  int id, char *priority, char *description)
{
	struct incident nou;
	nou.id = id;
	strcpy(nou.priority, priority);
	strcpy(nou.status, "queued");
	nou.description = malloc(strlen(description) + 1);
	DIE(!nou.description, "Malloc failed()!\n");
	strcpy(nou.description, description);

	dll_node_t *new = malloc(sizeof(dll_node_t));
	DIE(!new, "Malloc failed()!\n");
	new->data = malloc(sizeof(struct incident));
	DIE(!new->data, "Malloc failed()!\n");
	memcpy(new->data, &nou, sizeof(struct incident));
	//se face adaguarea concrecta folosindu-ne de 2 pointeri: santinela si last
	dll_node_t *santinela = list->head;
	dll_node_t *last = santinela->prev;
	new->next = santinela;
	new->prev = last;
	last->next = new;
	santinela->prev = new;
	list->size++;
	//in functie de gradul de prioritate se adauga la coada specifica
	if (strcmp(nou.priority, "high") == 0) {
		dll_add_queue(queue_high, &new);
	} else if (strcmp(nou.priority, "medium") == 0) {
		dll_add_queue(queue_medium, &new);
	} else
		dll_add_queue(queue_low, &new);
}

//afisarea numarului de echipaje disponibile
void check_units_availability(FILE *out, doubly_linked_list_t *list)
{
	fprintf(out, "Number of available units: %u\n", list->size);
}

void dispatch(doubly_linked_list_t *queue_available_units,
			  doubly_linked_list_t *interventions_list,
			  doubly_linked_list_t *queue_high,
			  doubly_linked_list_t *queue_medium,
			  doubly_linked_list_t *queue_low,
			  stack_t *istoric, FILE *out)
{
	doubly_linked_list_t *priority_queue = NULL;
	if (queue_high->size > 0)
		priority_queue = queue_high;
	else if (queue_medium->size > 0)
		priority_queue = queue_medium;
	else if (queue_low->size > 0)
		priority_queue = queue_low;
	if (!priority_queue || queue_available_units->size == 0) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	/*extragem incidentul si unitatea din coada de asteptare,
	respectiv lista de unitati disponibile*/
	dll_node_t *incident = remove_first(priority_queue);
	dll_node_t *unit = remove_first(queue_available_units);
	//preluam pointerii pe care ii retin si setam statusul ca fiind "intervened"
	dll_node_t *leg_incident = *(dll_node_t **)incident->data;
	struct incident *i = (struct incident *)leg_incident->data;
	struct unit *u = *(struct unit **)unit->data;
	strcpy(i->status, "intervened");
	u->availability = 0;
	//creem interventia
	struct intervention new_intervention;
	new_intervention.incident = i;
	new_intervention.unit = u;
	dll_node_t *intervention = malloc(sizeof(dll_node_t));
	DIE(!intervention, "Malloc failed()!\n");
	intervention->data = malloc(sizeof(struct intervention));
	DIE(!intervention->data, "Malloc failed()!\n");
	memcpy(intervention->data, &new_intervention, sizeof(struct intervention));
	//facem adaugarea concreta folosindu-ne de 2 pointeri santinela si last
	dll_node_t *santinela = interventions_list->head;
	dll_node_t *last = santinela->prev;
	intervention->next = santinela;
	intervention->prev = last;
	last->next = intervention;
	santinela->prev = intervention;
	interventions_list->size++;
	//adaugam in istoricul interventiilor 
	st_push(istoric, &intervention);
	//dealocarea memoriei
	free(incident->data);
	free(incident);
	free(unit->data);
	free(unit);
}

void undo_last_dispatch(doubly_linked_list_t *incidents_list,
						doubly_linked_list_t *queue_available_units,
						doubly_linked_list_t *interventions_list,
						doubly_linked_list_t *queue_high,
						doubly_linked_list_t *queue_medium,
						doubly_linked_list_t *queue_low,
						stack_t *istoric, FILE *out)
{
	if (st_is_empty(istoric)) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	// eliminam din stiva toate incidentele care au fost solutionate
	while (!st_is_empty(istoric)) {
		dll_node_t **p_interventie = (dll_node_t **)st_peek(istoric);
		dll_node_t *interventie = *p_interventie;
		struct intervention *i = (struct intervention *)interventie->data;
		if (strcmp(i->incident->status, "solved") == 0) {
		    st_pop(istoric);
		    continue;
		}
		//realizarea operatiei de undo pe primul incident posibil
		strcpy(i->incident->status, "queued");
		i->unit->availability = 1;
		//echipajul devine din nou disponibil, iar incidentul in asteptare
		dll_add_queue(queue_available_units, &i->unit);
		dll_node_t *curr = incidents_list->head->next;
		dll_node_t *nod_incident = NULL;
		int ok = 0;
		while (curr != incidents_list->head && ok == 0) {
		    if (curr->data == i->incident) {
				nod_incident = curr;
				ok = 1;
			}
			curr = curr->next;
		}
		//in functie de prioritatea lui este adaugat in coada destinata gradului
		if (nod_incident) {
		    if (strcmp(i->incident->priority, "high") == 0) {
				dll_add_front(queue_high, &nod_incident);
		    } else if (strcmp(i->incident->priority, "medium") == 0) {
				dll_add_front(queue_medium, &nod_incident);
			} else
				dll_add_front(queue_low, &nod_incident);
		}
		interventie->prev->next = interventie->next;
		interventie->next->prev = interventie->prev;
		interventions_list->size--;
		free(interventie->data);
		free(interventie);
		st_pop(istoric);
		return;
	}
}

void show_unit(struct unit units[], int id_unit, int nr, FILE *out)
{
	int gasit = 0;
	for (int i = 0; i < nr && gasit == 0; i++) {
		//echipajul exista
		if (units[i].id == id_unit) {
			//echipaj disponibil sau nu
			if (units[i].availability == 1) {
				fprintf(out, "Unit %d is type %c and is available\n",
						units[i].id, units[i].type);
			} else {
				fprintf(out, "Unit %d is type %c and is unavailable\n",
						units[i].id, units[i].type);
			}
			gasit = 1;
		}
	}
	//cazul in care id-ul nu e valid
	if (!gasit) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
	}
}

void show_incident(doubly_linked_list_t *incidents, int id_incident, FILE *out)
{
	int gasit = 0;
	dll_node_t *curr = incidents->head->next;
	while (curr != incidents->head && gasit == 0) {
		struct incident *tmp = curr->data;
		//incidentul exista
		if (tmp->id == id_incident) {
			fprintf(out, "Incident %d has %s priority, the following description: \"%s\" and is %s\n", id_incident, tmp->priority, tmp->description, tmp->status);
			gasit = 1;
		}
		curr = curr->next;
	}
	//cazul in care id incident nu e valid
	if (!gasit) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
	}
}

void show_interventions(doubly_linked_list_t *interventions, FILE *out)
{
	if (interventions->size <= 1) {
		fprintf(out, "No intervention has been initiated\n");
		return;
	}
	//exista o interventie activa sau rezolvata in trecut
	dll_node_t *curr = interventions->head->next;
	while (curr != interventions->head) {
		struct intervention *tmp = curr->data;
		fprintf(out, "Incident %d was assigned to unit %d, and has the following status: \"%s\"\n", tmp->incident->id, tmp->unit->id, tmp->incident->status);
		curr = curr->next;
	}
}

void solved_incident(doubly_linked_list_t *interventions, int id_incident,
					 doubly_linked_list_t *queue_available_units,
					 FILE *out)
{
	//id invalid
	if (id_incident <= 0) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	dll_node_t *curr = interventions->head->next;
	int ok = 0;
	while (curr != interventions->head && ok == 0) {
		struct intervention *tmp = curr->data;
		//id-ul incidentului exista si acesta inca nu e marcat ca fiind rezolvat
		if (tmp->incident->id == id_incident && strcmp(tmp->incident->status, "intervened") == 0) {
			strcpy(tmp->incident->status, "solved");
			tmp->unit->availability = 1;
			struct unit *aux = tmp->unit;
			dll_add_queue(queue_available_units, &aux);
			ok = 1;
		}
		curr = curr->next;
	}
	//incidentul a fost deja marcat ca fiind solutionat
	if (ok == 0) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
}
