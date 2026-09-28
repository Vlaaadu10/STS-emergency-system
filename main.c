//Stefan Alexandru Vladut 314CA
#include "utils.h"
#include "operations.h"
#include "dll_stack_init.h"
#include "declarations.h"

int main(void)
{
	struct unit units[50];
	doubly_linked_list_t *incidents_list = dll_create(sizeof(struct incident));
	add_santinela_incident(incidents_list);
	doubly_linked_list_t *interventions_list = dll_create(sizeof(struct intervention));
	add_santinela_interventie(interventions_list);
	doubly_linked_list_t *queue_high = dll_create(sizeof(dll_node_t *));
	doubly_linked_list_t *queue_medium = dll_create(sizeof(dll_node_t *));
	doubly_linked_list_t *queue_low = dll_create(sizeof(dll_node_t *));
	doubly_linked_list_t *queue_available_units = dll_create(sizeof(struct unit *));
	stack_t *istoric = st_create(sizeof(dll_node_t *));
	FILE *in = fopen("tema1.in", "r");
	FILE *out = fopen("tema1.out", "w");
	DIE(!in, "Couldn't open input file");
	DIE(!out, "Couldn't open output file");
	int nr_echipaje;
	fscanf(in, "%d", &nr_echipaje);
	for (int i = 0; i < nr_echipaje; i++) {
		fscanf(in, "%d %c", &units[i].id, &units[i].type);
		units[i].availability = 1;
		struct unit *tmp = &units[i];
		dll_add_queue(queue_available_units, &tmp);
	}
	int n;
	fscanf(in, "%d", &n);
	//optiunea aleasa duce la apelarea functiei potrivite
	for (int i = 0; i < n; i++) {
		char comanda[100];
		fscanf(in, "%s", comanda);
		if (strcmp(comanda, "ADD_INCIDENT") == 0) {
			int id;
			char description[255], priority[7];
			fscanf(in, "%d %s \"", &id, priority);
			fscanf(in, "%[^\"]\"", description);
			add_incident(incidents_list, queue_high, queue_medium, queue_low, id, priority, description);
		} else if (strcmp(comanda, "CHECK_UNITS_AVAILABILITY") == 0) {
			check_units_availability(out, queue_available_units);
		} else if (strcmp(comanda, "DISPATCH") == 0) {
			dispatch(queue_available_units, interventions_list, queue_high, queue_medium, queue_low, istoric, out);
		} else if (strcmp(comanda, "UNDO_LAST_DISPATCH") == 0) {
			undo_last_dispatch(incidents_list, queue_available_units, interventions_list, queue_high, queue_medium, queue_low, istoric, out);
		} else if (strcmp(comanda, "SOLVED_INCIDENT") == 0) {
			int id;
			fscanf(in, "%d", &id);
			solved_incident(interventions_list, id, queue_available_units, out);
		} else if (strcmp(comanda, "SHOW_UNIT") == 0) {
			int id;
			fscanf(in, "%d", &id);
			show_unit(units, id, nr_echipaje, out);
		} else if (strcmp(comanda, "SHOW_INCIDENT") == 0) {
			int id;
			fscanf(in, "%d", &id);
			show_incident(incidents_list, id, out);
		} else if (strcmp(comanda, "SHOW_INTERVENTIONS") == 0) {
			show_interventions(interventions_list, out);
		} else {
			fprintf(out, "INVALID OPERATION! ERROR 404");
			return 0;
		}
	}
	//dealocarea memoriei pentru a evita memory leaks + inchidere fisiere
	st_free(&istoric);
	dll_free(&incidents_list, 1);
	dll_free(&interventions_list, 0);
	dll_free(&queue_high, 0);
	dll_free(&queue_medium, 0);
	dll_free(&queue_low, 0);
	dll_free(&queue_available_units, 0);
	fclose(in);
	fclose(out);
	return 0;
}
