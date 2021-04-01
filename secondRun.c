#include "dataStructs.h"
#include "secondRun.h"
#include "functions.h"

/*secondRun function are the second pass on the source code recieved.*/
/*it would add entry values to entry linked list which was ignored in first run,*/
/*fill the missing machine code the could not be done on the first pass,*/
int secondRun(FILE *input,IC **IC_head,symbol **sym_head,data **data_head)
{

	char line[READ_LEN]; 		/*a string to contain a line from input file*/
	int counter=1;				/*count the line numbers, mainly for error prints*/
	int errf=1;					/*error in file flag*/		

	fseek(input,0,SEEK_SET);
	while(fgets(line,READ_LEN,input)!=NULL)  /*read a line from input file and send it to checkEntry funcion for intepratation*/
	{ 	

			if(!checkEntry(line,counter,sym_head))/*check if there is entry action at the line and will return 1 if evrithing good,if not it change the errf into 0*/
			{
				errf = 0;
			}
		counter++;	/*upgrade by 1 the line counter*/
	}

	if(!checkUnknown(IC_head,sym_head))	/*call the function checkunkown to check if there is an ic that the are or code is unkown from the first run, if we fined 1 we check*/
										/*if we can fill him from the symbol linked list , if we cant it mean that he isnt defined at file and we return 0 as error*/
	{

		errf=0;
		return errf;
	}
	
	return errf;	
}

/*this function will get each line from the file at a time and will search for .entry action*/
/*if the function will fined .entry at the file it will check that the label is exist at the symbol table, anf if not it will return 0*/
/*the function get an address to line, int line number, and address to the synbol head linked list*/
int checkEntry(char *line, int line_num,symbol **symbol_head)
{
	int i=0,h=0,j=0; /*for loop integers*/
	int errl = 1;	/*error in line flag*/
	char str[LABLE_LEN]; /*string to save the word into*/
	int countw =0;	/*count how much words there are in the line*/
	
	i = space_skip(line);	/*call a function that return the number of spaces and tabs*/
	if(line[i]=='\n')	/*if the line is empty (just spaces and tabs) it will return 1 and will ignore this line*/
		return errl;
	if(line[i] == ';')	/*check if the line is commant, if it is it will return 1 and will ignore this line*/		
		return errl;
	while(line[j] != '\n' && line[j] != '\0' )	/*this while loop will read the entire line*/
	{
		while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && i<strlen(line))   /*check if the char isnt a space/tab/enter*/
		{ 																		  	    /*and check that doesnt exceed the length of the array*/																		   
			str[h] = line[i]; 
			h++;
			i++;
		}
		countw++; /*we have read a word so we add a count to it*/
		str[h]='\0';
		if(str[0] == '.')
		{

			if(!strcmp(str,".entry")) /*check if the ward is .entry*/
			{
				if(countw >2)	/*check if we founed .entry after the seconed word at line*/
				{
					printf(".entry can be defined only at the start of line or after a label at line : %d \n ",line_num);
					errl =0;
					return errl;
				}
				i += space_skip(line+i); /*call a function that return the number of spaces and tabs*/
				if(!(isProperEntry(line+i, symbol_head, line_num)))/*check if the rest of the line is only 1 label and if it is it will check that the label*/
													       	  	  /*is in the symbol table. if not it will return 0*/
				{
					errl =0;
					return errl;
				}
				else
				{
					return errl;
				}
			}
			else
			{
				memset(str,0,sizeof(str));	/*reset string*/
				i += space_skip(line+i);  /*call a function that return the number of spaces and tabs*/
				j=i;
				h=0;
			}
		}
		else
		{
			memset(str,0,sizeof(str));	/*reset string*/
			i += space_skip(line+i);	/*call a function that return the number of spaces and tabs*/
			j=i;
			h=0;
		}
	}

	return errl;
}

/*this function will get the entire line after the .entry word, and will check if there is only 1 label after the .entry and, the the label is defined on the first run*/
/*on the symbole linked list. if not it will return 0 as error or 1*/
/*the function get an address to line, an adress of symbol head of linked list, and integer line number*/
int isProperEntry(char *line,symbol **sym_head,int line_num)
{
	symbol *s = *sym_head;
	int countw=0;	/*an integers to count how much words there after .entry action*/
	int i =0,j=0,h=0;	/*integers for loop*/
	int entry=0;	/*turn entry flag if we found a defined label with the same name from entry and the symbol linked list */
	int erre = 1;	/*error at line with .entry action flag*/
	char str[LABLE_LEN];	/*string to save the word into*/
	
	if(line[i] == '\n') /*if there is nothing after .entry its an error*/
	{
		erre=0;
		printf("missing lable for entry action at line %d\n",line_num);
		return erre;
	}
	else 
	{
		while(line[j] != '\n' && line[j] != '\0' && j<strlen(line))	/*this while loop will read the entire line*/
		{
			while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && i<strlen(line)) /*check if the rest of the line is only 1 label and if it is it will check that the label*/
													       	  							  /*is in the symbol table. if not it will return 0*/
			{
				str[h] = line[i];
				h++;
				i++;
			}
			countw++;	/*we have read a word so we add a count to it*/
			str[h] = '\0';
			if(countw > 1) /*if there is more then 1 word its an error and will return 0*/
			{
				erre=0;
				printf("to much parameters for entry at line %d,%s \n",line_num,line+i); 
				return erre;
			}
			while(s != NULL )
			{
				if(!(strcmp(str,s->symName)) && s->unknown == 0) /*check if the label for entry is on the symbol linked list and he defined as a label*/
															     /*unknown = 0 mean that its a defined label and not a label for an action*/
				{
					if(s->external == 1)/*check if we try to do entry for external label*/
					{
						erre =0;
						printf("you cant do entry for an external label %s at line : %d\n",s->symName,line_num);

					}
					entry =1; /*turn entry flag if we found a defined label with the same name from entry and the symbol linked list*/
					s->entry=1; /*turn on the flag that the label is entry*/
				}
				s=s->next;
			}
			i += space_skip(line+i); /*call a function that return the number of spaces and tabs*/
			j=i;
		}
		if(!entry) /*check if we didnt founed the label from the entry at symbol linked list*/
		{
			printf("the label %s you try to do entry for is not defined in this file at line %d\n",str,line_num);
			return entry;
		}
		else
			return entry; /*return the place of '\n' at the line if the line is correct and entry=1*/
	}
		

}

/*the function checkunkown check if there is an ic that the are or code is unkown from the first run, if we fined 1 we check*/
/*if we can fill him from the symbol linked list , if we cant it mean that he isnt defined at file and we return 0 as error*/
/*the function get an adress to the ic head of linked list, and an address to the symbol head of linked list*/
int checkUnknown(IC **IC_head,symbol **sym_head)
{
	unsigned int i;
	symbol *s1 = *sym_head;
	symbol *s2 = *sym_head;
	int errc =1;	/*error for unkown flag*/

	/*loop to run on symbole linked list until the last stract*/
	while(s1 != NULL)
	{
		if(s1->unknown == 0)	/*search on symbol linked list for an starct that is known*/
		{
			
			while(s2 != NULL)	/*loop to run on symbole linked list until the last stract*/
			{
				if(!(strcmp(s1->symName,s2->symName)) && s2->unknown == 1)	/*check if the symbol name that we fount as known and the s2 symbol name are the same*/
																			/*and if it is' its check that s2 is unkown(mean unkown = 1)*/
				{
					if(s1->external == 1)
					{
						s2->external=1;
					}
					i = (s2->address)-100;	/*check i for the loop number we need to do on ic to get to the right ic, sub 100 cus the ic start from 100*/
					fillIcLinked(IC_head,s1,i);	/*call a function fillIcLinked to fill the unkown ic that we founed.*/												
					s2->unknown=0;	/*after we filled the unkown ic we change the unkown now to 0 cus he is known now*/
				}
				s2=s2->next;
			}
			s2 = *sym_head;
		}
		s1 = s1->next;
	}
	s1= *sym_head;

	/*loop to run on symbole linked list until the last stract*/
	while(s1 != NULL)
	{
		if(s1->unknown ==1) /*if after i filled all the unkown lables (that defined on files) there is still uknown ic so there is an error and return 0*/
		{
			printf("you try to use undefined label %s as oppernd at line %d\n",s1->symName,s1->line_num);
			errc=0;
		}
		s1=s1->next;
	}

	return errc;
}

/*a void function fillIcLinked that fill the unkown ic that we founed*/
/*the function get an adders to ic head linked list, the known symbol to fill the ic from , and integer for loop on ic*/
void fillIcLinked(IC **IC_head,symbol *s1,int i)
{
	
	IC *ic1 = *IC_head;

	/*loop until the ic we need to fill*/
	for(;i>0;i--)
	{
		ic1=ic1->next;
	}

	if(s1->external == 1) /*check if the ic we need to fill is an external symbol*/
	{
		ic1->are=1; /*fill the ic are to be 1(e)*/
	}
	else
	{
		ic1->code = s1->address;	/*fill in ic code the address of the label*/
		ic1->are=1<<1;	/*fill in ic are to be 2(r)*/
	}

}





