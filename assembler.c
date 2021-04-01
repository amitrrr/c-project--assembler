		
#include "assembler.h"
#include "dataStructs.h"
#include "firstRun.h"
#include "secondRun.h"
#include "functions.h"
#define MAX_PATH 4096
#define	LINE_LEN 100

action_type action_type_map[NUM_OF_ACTIONS]={
{ 0, "mov", "0123", "123"},
{ 1, "cmp", "0123", "0123"},
{ 2, "add", "0123", "123"},
{ 3, "sub", "0123", "123"},
{ 4, "lea", "1", "123"},
{ 5, "clr", NULL, "123"},
{ 6, "not", NULL, "123"},
{ 7, "inc", NULL, "123"},
{ 8, "dec", NULL, "123"},
{ 9, "jmp", NULL, "12"},
{ 10, "bne", NULL, "12"},
{ 11, "red", NULL, "123"},
{ 12, "prn", NULL, "0123"},
{ 13, "jsr", NULL, "12"},
{ 14, "rts", NULL, NULL},
{ 15, "stop", NULL, NULL}
};


char registers[NUM_OF_REG][LEN_OF_REG]={
	{"r0"},{"r1"},{"r2"},{"r3"},{"r4"},{"r5"},{"r6"},{"r7"}
};

IC *IC_head = NULL;
symbol *sym_head = NULL;
data *data_head = NULL;



int main(int argc, char const *argv[])
{
	symbol *s1;
	FILE *input;
	FILE *ent;
	FILE *ext;
	FILE *ob;
	int i=1,countEntry=0,countExtern=0;
	char fileName[MAX_PATH];/*full name including path*/
	char *cleanName; /*name withouth path*/
	char entryFile[MAX_PATH];
	char externFile[MAX_PATH];
	char obFile[MAX_PATH];

	/*Check if command line has recieved a file to be interpreted*/
	if (argc<2)
	{
		printf("No input files was declared\n");
		exit(1);
	}
	
	/*Open files, add .as to the name of the file, and start first run.*/
	/*if file wasnt found it would print an error and will continue to next file*/
	for(;i<argc;i++)
	{	

		strcpy(fileName,argv[i]);
		strcpy(externFile,argv[i]);
		strcpy(entryFile,argv[i]);
		strcpy(obFile,argv[i]);
		strcat(fileName,".as\0"); /*add .as to file name*/
		strcat(externFile,".ext\0");		
		strcat(entryFile,".ent\0");
		strcat(obFile,".ob\0");	
		/*Get the file name without its path*/
		if((cleanName=strrchr(fileName,'/')))
		{
			cleanName=cleanName+1;
		}
		else
		{
			cleanName=fileName;
		}
		/*check if file was opened\exists*/
		if(!(input=fopen(fileName,"r")))
		{
			printf("File %s does not exists or failed to open\n",fileName);
			continue;
		}
		/*if file is opened sent it to first run for intepratation*/
		else
		{
			printf("Translating file %s\n",cleanName );
			if(!first_run(input,&IC_head,&sym_head,&data_head)) /*if first run faild throw an error with file name*/
			{
				printf("Errors was found at %s file\n",cleanName );
			}
			else
			{
				if(!secondRun(input,&IC_head,&sym_head,&data_head))
				{

					printf("Errors was found at second run on %s file \n",cleanName);
				}
				else
				{
					s1 = sym_head;
					while(s1 != NULL)
					{
						if(s1->external == 1 && s1->address != 0)
						{
							countExtern++;
							if(countExtern<2)
							{
								if(!(ext=fopen(externFile,"w")))
								{
									printf("Could not create an extern file %s.",externFile);
									exit(1);	
								}
							}
							fprintf(ext,"%d %s \n",s1->address,s1->symName);
						}
						if(s1->entry == 1)
						{
							countEntry++;
							if(countEntry<2)
							{
									if(!(ent=fopen(entryFile,"w")))
								{
									printf("Could not create an entry file %s.",entryFile);
									exit(1);
								}
								
							}
							fprintf(ent,"%d %s \n",s1->address,s1->symName);
						}
						s1=s1->next;

					}

					if(!(ob=fopen(obFile,"w")))
					{
						printf("Could not create an object file %s.",obFile);
						exit(1);
					}	

					printIC(&IC_head,ob);
					printData(&data_head,ob);
					printf("\nOutput files ware created for file %s.\n",cleanName);
				}
					
				
			}
			countExtern =0;
			countEntry=0;

			/*freeLists(sym_head,IC_head,data_head); 
			  fclose(ob);  
			  fclose(ent);
			  fclose(ext);
			we know we should use file close and freeList functions,
			but we had bug we couldnt handle, and could not understand why it crashes*/
			
			sym_head=NULL;
			IC_head=NULL;
			data_head=NULL;

		}
			
			fclose(input);
			printf("\n");
	}
	return 0;
}

/*free function disaloct all the nodes and cleans linked list for the next file*/
void freeLists(symbol *sym_h,IC* IC_h,data *data_h)
{
	
	symbol *sym_temp;
	IC *IC_temp;
	data *data_temp;
	
	while(sym_h != NULL)
	{
		sym_temp=sym_h;
		sym_h = sym_h->next;
		sym_temp = NULL;
		free(sym_temp);
		

	}
	while(IC_h != NULL)
	{
		IC_temp = IC_h;
		IC_h = IC_h->next;
		IC_temp->next=NULL;
		free(IC_temp);
		
		
	}
	while(data_h != NULL)
	{
		data_temp = data_h;
		data_h = data_h->next;
		data_temp->next=NULL;
		free(data_temp);
	}
	
}		






