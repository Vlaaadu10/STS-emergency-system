//Stefan Alexandru Vladut 314CA
#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*Macro-uri utile pentru aflarea minimului, respectiv cazul in care
malloc nu reuseste*/
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

//definirea structurilor necesare implementarii sistemului
typedef struct dll_node_t {
	void *data; /* Pentru ca datele stocate sa poata avea orice tip, folosim un
					pointer la void. */
	struct dll_node_t *prev, *next;
} dll_node_t;

typedef struct doubly_linked_list_t {
	dll_node_t *head;
	unsigned int data_size;
	unsigned int size;
} doubly_linked_list_t;

struct unit {
	int id;
	char type;
	int availability;
};

struct incident {
	int id;
	char priority[7];
	char *description;
	char status[11];
};

struct intervention {
	struct incident *incident;
	struct unit *unit;
};

struct system {
	struct unit *units;
	struct incident *incidents;
	struct intervention *interventions;
};

typedef struct stack_t stack_t;
struct stack_t {
	struct doubly_linked_list_t *list;
};

#endif
