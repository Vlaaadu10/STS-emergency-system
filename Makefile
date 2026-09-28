# Copyright PCLP Team, 2025

# Compiler setup.
CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99 -g

# Define targets, e.g., ninel, codeinvim, vectsecv, nomogram.
TARGETS=tema1

SOURCES=main.c utils.c operations.c dll_stack_init.c

# Manually define all targets.
build: $(TARGETS)

tema1: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o tema1 -lm

# Pack the solution into a zip file.
pack:
	zip -FSr 314CA_StefanAlexandruVladut_Tema1.zip README Makefile *.c *.h

# Clean the solution.
clean:
	rm -f $(TARGETS)

.PHONY: build pack clean