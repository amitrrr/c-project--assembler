#ifndef DATASTRUCTS_H
	#define DATASTRUCTS_H
#define LABLE_LEN 32
#define NUM_OF_REG 8
#define LEN_OF_REG 3
#define NUM_OF_ACTIONS 16


/*TODO CHANGE STRUCT*/
typedef struct IC{
	unsigned int code:12;
	unsigned int are:3;
	struct IC *next;
}IC;


typedef struct symbol{
	
	unsigned int address;
	unsigned int external:1;
	unsigned int entry:1;
	unsigned int unknown:1;
	unsigned int line_num;
	char symName[LABLE_LEN];
	struct symbol *next;
}symbol;


typedef struct data{
	unsigned int code:15;
	struct data *next;
}data;


/*A struct thet represents the OPCODE structure and legal action the can be prefomed on it, it name,and the OPCODE itself.*/
typedef struct action_type{
	unsigned int acode:4;
	char *aname;
	char *legal_src;
	char *legal_dest;
}action_type;


/*OPCODE table of legal source and destintion values */
extern action_type action_type_map[NUM_OF_ACTIONS];


/*Avaliable registers*/
extern char registers[NUM_OF_REG][LEN_OF_REG];

/*temp->address =0;
	memset(temp->symName,0,sizeof(temp->symName));
	temp->external=0;
	temp->entry=0;
	temp->unknown=0;
	temp->line_num =0;
	temp->next=NULL;
	symbol *p = *sym_head;*/


#endif	
