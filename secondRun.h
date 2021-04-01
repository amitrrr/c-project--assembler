#ifndef SECONDRUN_H
	#define SECONDRUN_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_LEN 100
#define READ_LEN 81
#define LABLE_LEN 32
#define MAX_NUM 2047
#define MIN_NUM -2048

/*secondRun function are the second pass on the source code recieved.*/
/*it would add entry values to entry linked list which was ignored in first run,*/
/*fill the missing machine code the could not be done on the first pass,*/
int secondRun(FILE *input,IC **IC_head,symbol **sym_head,data **data_head);

/*this function will get each line from the file at a time and will search for .entry action*/
/*if the function will fined .entry at the file it will check that the label is exist at the symbol table, anf if not it will return 0*/
/*the function get an address to line, int line number, and address to the synbol head linked list*/
int checkEntry(char *line, int line_num, symbol **symbol_head);

/*this function will get the entire line after the .entry word, and will check if there is only 1 label after the .entry and, the the label is defined on the first run*/
/*on the symbole linked list. if not it will return 0 as error or 1*/
/*the function get an address to line, an adress of symbol head of linked list, and integer line number*/
int isProperEntry(char *line, symbol **sym_head,int line_num);

/*the function checkunkown check if there is an ic that the are or code is unkown from the first run, if we fined 1 we check*/
/*if we can fill him from the symbol linked list , if we cant it mean that he isnt defined at file and we return 0 as error*/
/*the function get an adress to the ic head of linked list, and an address to the symbol head of linked list*/
int checkUnknown(IC **IC_head,symbol **sym_head);


/*a void function fillIcLinked that fill the unkown ic that we founed*/
/*the function get an adders to ic head linked list, the known symbol to fill the ic from , and integer for loop on ic*/
void fillIcLinked(IC **IC_head,symbol *s1,int i);


#endif
