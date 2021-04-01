#ifndef FIRSTRUN_H
	#define FIRSTRUN_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LINE_LEN 100
#define READ_LEN 81
#define LABLE_LEN 32
#define MAX_NUM 2047
#define MIN_NUM -2048
#define INT15_MAX 16383
#define INT15_MIN -16384





/*the first run will check syntax and build a symbol table,
data table,and instruction linked list which will contain all of the memory map
it will count the number of instructions and data memory lines*/
int first_run(FILE *file,IC **head,symbol **symbol_head,data **data_head);

/*readLine will get one line at a time from first run function and will check it syntax
if syntax was correct it would add the directives to the coresponding linked lists
if any error is encounter it will raise a flag and will continue to check for all the errors
in a single file*/
int readLine(char *line, int line_num,IC **head,symbol **symbol_head,data **data_head);

/*get addressing will get an argument from readline and would check if its a legal addressing mathod
if not it would print an error and will return -1 to readLine, if adressing mathod found to be legit
it would return the an intger the represent the right adressing mathod*/
int getAddressing(char *argument,int line_num);

/*if getAddressing found that both source and target argument are a registers, it would call
the get2regMask function to get a ingeter representation mask of the 3-8 bits, that mask represent the
number of the souce register in bits 6-8 and the target register in bits 3-5.*/
int get2regMask(char *dest, char *src);	

/*readLine will call this function if the addressing found to be an immidiate argument, this
functions returns an integer the represnts the bits 3-14 the will be inserted to the op word
becuase the immidiate argument is cofined to 12 bits in the compliments 2 mathod,
the integer declared must be between MIN_NUM and MAX_NUM that are define in this file*/
int getImMask(char *ImNum);

/*readLine will call this function if the addressing of source argument found to be a register argument,
this function will return a mask that represents bits 6-8 of the op word */
int getMaskSrcReg(char *reg);

/*readLine will call this function if the addressing of target argument found to be a register argument,
this function will return a mask that represents bits 3-5 of the op word */
int getMaskDestReg(char *reg);

/*isLegelAdd tests the legitimacy of the source and target arguments for the spcific intruction that
was found before arguments.
if the instruction is capble of taking the found adressing mathods the function will return 1 for positive
result and will print and error and will return 0 for failure.*/
int isLegelAdd(int op,int src,int dest,int line_num);

/*check_data function will check if data is legit if .data directive was found,
for every integer found in the same line this function will call createData to create a data node, 
that later will be printed the the ob file.
if argument was not legit the function will return 0 and will print an error.
becuase the integers arguments are cofined to 15 bits in the compliments 2 mathod,
the integer declared must be between INT15_MIN and INT15_MAX that are define in this file */
int check_data(char *str,int line_num,data **data_head);

/*check_string function will check if string is legit if .string directive was found,
for every charecter found in the string this function will call createData to create a data node, 
that later will be printed the the ob file.
if argument was not legit the function will return 0 and will print an error*/
int check_string(char *str,int line_num,data **data_head);

/*creteIC function will create a memory node for each op word that has found to be legit,
memory nodes are a linked list of IC structure objects,
this function will recieve opcode of the intruction as integer, are code as integer,
the IC_head which is the memory list head.
besides creating the node itself this function also fills the code itself for the node,
becuase of that it also recieves the source and targer masks to be filled*/
IC *createIC(int op,int src,int dest,int are,IC **IC_head);	

/*creteSym function will create a memory node for each symbol that has found to be legit,
the symbol list is a linked list of symbol structure objects,
the function will recieve the symbol name, in which memory word it had been found, if external
was declared before the symbol it would recieve 1 in the external int, same for entry.
if a addressing mathod 2 was found besides creating an IC node, it will also create a symbol node,
to later be interpreted in the second run, in thad case the functions recieves 1 int unknwen int,
line_num is pass for error purpuses.*/
symbol *createSym(char *name,int ic,int external ,int entry ,int unknowen,symbol **sym_head,int line_num);


/*this function will create the data node after check_sting or check_data functions check their 
legitimacy.*/
data *createData(int num,data **data_head);



/*printIC function will print the memory map of all instructions*/
int printIC(IC **IC_head,FILE *ob);
/*TODO delete this*/
int printSym(symbol **symbol_head);
/*printData function prints the data list of all data and strings*/
int printData(data **data_head,FILE *ob);


#endif
