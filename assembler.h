#ifndef ASSEMBLER_H
	#define ASSEMBLER_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "dataStructs.h"
#define MAX_PATH 4096
#define LINE_LEN 100
#define READ_LEN 81





/*the second run will read the file again while checking for syntax errors,*/
/*will call a function that converts the line to an binary OPCODE*/
/*will call a function that will converts the binary code into octal code*/
/*if successful it would call functions to create files and free all linked lists */
int second_run(FILE *file);
void freeLists(symbol *sym_head,IC* IC_head,data *data_head);
/*
void freeIcList(IC **IC_head);

void freeSymList(symbol **sym_head);
*/
#endif
