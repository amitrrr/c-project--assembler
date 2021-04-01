#include "dataStructs.h"
#include "firstRun.h"
#include "functions.h"
int ic = 100;
int dc = 0;

int first_run(FILE *input,IC **IC_head,symbol **sym_head,data **data_head){
	char line[READ_LEN]; 		/*a string to contain a line from input file*/
	int len=0;					/*this integer will hold the length of a given string*/
	int file_end;				/*check the file end position*/
	int counter=1;				/*count the line numbers, mainly for error prints*/
	int sameLine=0;	
	int commant=0;			/*same line flag to indicate the line was too long and not to count 2 fgets reads as 2 diffrent lines*/
	int errf=1;					/*error in file flag*/
	ic=100;
	dc=0;



	fseek(input,0,SEEK_END);	 /*place file position to the end of the file*/
	file_end=ftell(input);		/*find length of file if value*/	

	if (file_end==0)			/*if len is 0 then file is empty, else reset file position to beginning of file*/
	{
		printf("file is empty\n");
		return 0;
	}
	else
	{
		fseek(input,0,SEEK_SET);
	}

	while(fgets(line,READ_LEN,input)!=NULL)  /*read a line from input file and send it to read_line funcion for intepratation*/
	{ 		
		len=strlen(line);	/*find length of the line that was read*/

		if (strchr(line,'\n') || file_end==ftell(input)) /*find if new line was declared or file has reached it end.*/
		{
			if (len>LINE_LEN || sameLine==1)		/*if line was too long or fgets didnt read '\n' in the last line read(which means we still reading a long line)*/
			{										/*the loop will read the line entirly, will print an error,raise a flag, and will continue to read the rest of the file*/
				if (commant==1)
				{
					counter++;
					sameLine=0;
					errf=0;
					commant=0;
				}
				else
				{
					printf("line %d :line is too long\n",counter);
					counter++;
					sameLine=0;
					errf=0;
				}
				
			}
			else
			{

				if(!readLine(line,counter,IC_head,sym_head,data_head))
				{		/*after ensuring line was in boundries we will interprate the line with read_line function*/
					errf=0;							
				}
				counter++; 							/*and will raise the line counter.*/
			}
		}

		else
		{
			if (line[0]==';')
			{
				commant=1;
			}
			sameLine=1;								/*declaring that we are stiil reading the sameline*/
			continue;
		}
	}
	
	return errf;
}

/*this function will get a line at a time and will interprate its values,*/
/*will call the coresponding functions when a symbol/data/.external/.string/.data was met to add them to a linked list.*/
/*if line values was incorrect the function will return 0 and 1 if line was good*/
int readLine(char *line, int line_num,IC **IC_head,symbol **sym_head,data **data_head)
{
	int errl = 1; /*error flag*/
	int i=0,j=0,h=0,d=0,s=0,k=0; /*for loops integers*/
	char str[LABLE_LEN];	/*string to save the word into*/
	char check[LABLE_LEN];   /*temporary string to save extern arguments*/ 
	char destTemp[LABLE_LEN]; /*temporary string to save target arguments*/
	char srcTemp[LABLE_LEN]; /*temporary string to save source arguments*/
	int countw =0;	/*count how much words there are in the line*/
	int op=0,src=0,dest=0,are=0;/*masks for op code,source argument,target argument, and are argument*/

	/*i get the number of spaces and tabs*/
	i = space_skip(line);

	if(line[i]=='\n') /*if the line is empty (just spaces and tabs) it will return 1 and will ignore this line*/
		return errl;

	if(line[i] == ';')	/*check if the line is commant, if it is it will return 1 and will ignore this line*/		
		return errl;

	if(line[i]=='.') /*check if the first char is '.' and if it is, it will check if the word after the . is entry.*/
					 /*if the word is entry it will return 1 and will ignore this line*/
	{
		line=line+i+1;
		i=0;
		sscanf(line,"%s %s",str,check);
		line=line+strlen(str);


		if(!strcmp(str,"entry")) /*check if the word is entry, if it is leave it for second run*/
		{
			return errl;
		}
		if (!strcmp(str,"extern")) /*check if extern declaration has an argument after him*/
		{	
			if (line[i]=='\n' || line[i+space_skip(&line[i])]=='\n' || space_skip(&line[i])==strlen(line))
			{
				printf("ERROR AT LINE %d: Missing extern arguments.\n",line_num);
				errl=0;
				return errl;
			}
			if(is_label(check,line_num)) /*check if the argument is a lagit label, if legit create a node to be tested in the second pass*/
			{

				if (!createSym(check,0,1,0,0,sym_head,line_num))
				{	
					errl=0;
					return errl;
				}
			}
			else
			{/*if not print an additional error*/
				printf("\t\t  Therefore it can not be used as extern argument.\n");
				errl=0;
			}

			return errl;
		}
		line -=strlen(str)+1;
	}
	/*this while loop will read the entire line*/
	while(line[j] != '\n' && line[j]!= '\0')
	{
		/*loop that	enters the string into str and check if he is legal and call to insert into*/
		while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && i<strlen(line))   /*check if the char isnt a space/tab/enter*/
		{ 																				/*and check that doesnt exceed the length of the array*/																		   
			str[h] = line[i]; 
			h++;
			i++;
		}

		countw++; /*we have read a word so we add a count to it*/
		str[h]='\0';

		if(str[strlen(str)-1] == ':' && countw==1) 
		{
			str[strlen(str)-1]='\0';



			if(!is_label(str,line_num)) /*check if label syntax is correct*/
			{	               
				errl =0;
				memset(str,0,sizeof(str)); /*reset string*/
				h=0;
				i += space_skip(line+i);/*call a function that return the number of spaces and tabs*/
				j=i;
				continue;
			}
			else
			{
				/*check if next word is an entry or extern*/
				k+=space_skip(&line[i]);
				h=0;
				while(line[i+k] != ' ' && line[i+k] != '\t' && line[i+k] != '\n' && i+k<strlen(line))   /*check if the char isnt a space/tab/enter*/
				{ 																				/*and check that doesnt exceed the length of the array*/																		   
					check[h] = line[i+k]; 
					h++;
					k++;
				}
				check[h]='\0';
				if (!strcmp(check,".entry") || !strcmp(check,".extern"))
				{
					/*if an extern was declared here the label would be ignored*/
					printf("WARNING AT LINE %d: %s is ignored.\n",line_num,str );
					if (!strcmp(check,".extern")) /*if extern was found add it to read its argument*/
					{

						h=0;
						i=i+k;
						i+=space_skip(&line[i]);
						memset(str,0,sizeof(str));
						/*get next word*/
						while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && i<strlen(line))   /*check if the char isnt a space/tab/enter*/
						{ 																				/*and check that doesnt exceed the length of the array*/																		   
							str[h] = line[i]; 
							h++;
							i++;
						}

						/*check is its a legit label*/
						if(!is_label(str,line_num)){
							printf("\t\t  Therefore it can not be used as extern argument.\n");
							errl=0;
							return errl;
						}/*creat symbol to be checked in second run*/

						if (!createSym(str,0,1,0,0,sym_head,line_num)){
							
							errl=0;
							return errl;
						}

						return errl;
					}
					return errl;
				}
				else if(!createSym(str,ic+dc,0,0,0,sym_head,line_num)){
					
					errl=0;
					return errl;
				};
				memset(str,0,sizeof(str)); /*reset string*/
				h=0;
				i += space_skip(line+i);/*call a function that return the number of spaces and tabs*/
				j=i;
				continue;
			}
		}

		/*if the word found is an intruction, check its arguments and add them to the memory map*/
		if((op=is_action(str)) != -1)
		{
			/*ARE is alwys absoulute*/
			are=1<<2;
			/*if op is 14 or 15 check if any arguments has been given*/
			if (op==14 || op==15)
			{
				i+=space_skip(&line[i]);
				if (line[i]!='\n') /*if argument was found print an error and continue to next line*/
				{
					printf("ERROR AT LINE %d: %s action takes no arguments.\n",line_num,str );
					errl=0;
					return errl;
				}
				else{ /*else create a node*/
					ic++;
					createIC(op,0,0,are,IC_head);
					return errl;
				}
			}
			
			/*skip white spaces to get to argument*/
			i+=space_skip(&line[i]);
			/*if line has reached its end without arguments and intrusction requeires them print error*/
			
			if (line[i]=='\n')
			{
				printf("ERROR AT LINE %d: %s is missing arguments.\n",line_num,str );
				errl=0;
				return errl;
			}
			/*read next word to source variable*/
			else
			{
				while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i]!=',' && i<strlen(line))
				{
					srcTemp[s] = line[i]; 
					s++;
					i++;
				}
				srcTemp[s]='\0';
			}
			/*skip white spaces to get to next argument*/
			i+=space_skip(&line[i]);
			/*if a comma is met check if there is an argument followed by it*/
			if (line[i]==',')
			{
				i++;
				i+=space_skip(&line[i]);
				if ((op>=5 && op <=13) && line[i]!='\n')
				{	/*if there is a target argument print an error*/
					printf("ERROR AT LINE %d: instruction %s takes destination argument only. \n",line_num,str);
					errl=0;
					return errl;
				}
				else if ((op>=5 && op <=13) && line[i]=='\n')
				{	/*if there is no argument then its a exssesive comma*/
					printf("ERROR AT LINE %d: Invalid comma. \n",line_num);
				}
				else if ((op>=0 && op <=4) && line[i]=='\n')
				{	/*if the op is between 0 and 4 it requires source and targer arguments*/
					printf("ERROR AT LINE %d: %s is missing arguments.\n",line_num,str );
					errl=0;
					return errl;	
				}
				else
				{ /*read next word to target variable*/
					while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i]!=',' && i<strlen(line))
						{
							destTemp[d] = line[i]; 
							d++;
							i++;
						}
						destTemp[d]='\0';
				}

			}
			 /*if the line hasnt reached the end than there are too arguments*/
			else
			{ 
				i+=space_skip(&line[i]);
				
				if(line[i]!='\n' && line[i]!=EOF)
				{
					line[strlen(line)-1]='\0';
					printf("ERROR AT LINE %d: Invalid charecters \"%s\". \n",line_num,line+i);
					errl=0;
					return errl;
				}
				else
				{	/*if target argument is missing print an error*/
					if ((op>=0 && op <=4))
					{
						printf("ERROR AT LINE %d: %s is missing arguments.\n",line_num,str );
						errl=0;
						return errl;	
					}/*else close the string*/
					if (op>=5 && op <=13)
					{
						destTemp[0]='\0';
					}
				}

			}
			/*check if more arguments are passed*/
			i+=space_skip(&line[i]);
			if(line[i]!='\n' && i != (strlen(line)))
				{
					line[strlen(line)-1]='\0';
					printf("ERROR AT LINE %d: Invalid charecters \"%s\". \n",line_num,line+i);
					errl=0;
					return errl;
				}


			
			/*create action word*/
		
			if ((dest=getAddressing(destTemp,line_num))==-1 || (src=getAddressing(srcTemp,line_num))==-1)
			{
				errl=0;
			}
			
			
			if(dest!=-1 && src != -1)
			{
				/*check if adressing arguments match the op code*/
				
				if(!isLegelAdd(op,src,dest,line_num)){
					errl=0;

				}
				/*create action word with addressing methods*/
				
				if (op>=0 && op <=4)
				{	
					
					
					/*first create the op code for the instruction*/
					ic++;
					createIC(op,src,dest,4,IC_head);

					/*if 2 registers was found as source and targer create a single word for them*/
					if(( dest == 4 || dest == 8 ) && (src == 4 || src == 8 ))/*4 is adressing mathod 2 mask, becuase it represents 0100 binary*/
					{														/*8 is adressing mathod 3 mask, becuase it represents 1000 binary*/
						ic++;
						createIC(-1,0,get2regMask(destTemp,srcTemp),are,IC_head);		
					}
					else /*create word for each argument based on its adressing mathod first for the source*/
					{
						if(src==1) /*1 is adressing mathod 0 mask, becuase it represents 0001 binary*/
						{
							ic++; /*counting the memomy nodes*/
							createIC(-1,0,getImMask(srcTemp),are,IC_head);
						
						}
						else if (src==2)/*2 is adressing mathod 1 mask, becuase it represents 0010 binary*/
						{

							createSym(srcTemp,ic,0,0,1,sym_head,line_num);
							ic++;
							createIC(-1,0,0,0,IC_head);
						}
						else /*the adressing is 2 or 3 means the are handled the same way*/
						{

							ic++;
							createIC(-1,0,getMaskSrcReg(srcTemp),are,IC_head);
							
						}
						
						/*then exacly the same for the target argument*/
						if(dest==1)
						{
							ic++;
							createIC(-1,0,getImMask(destTemp),are,IC_head);
							
						}
						else if (dest==2)
						{

							createSym(destTemp,ic,0,0,1,sym_head,line_num);
							ic++;
							createIC(-1,0,0,0,IC_head);
							
							

						}
						else{
							ic++;
							createIC(-1,0,getMaskDestReg(destTemp),are,IC_head);
							
						}
					}				
			 
				}
							
				/*if op code is between 5 and 13 create a memory node
				and use the argument found as a target instead of source*/
				else if (op>=5 && op <=13)
				{	/*adding the node is exacly the same as before*/
					ic++;
					createIC(op,0,src,are,IC_head);
					
					if(src==1)
						{
							ic++;
							createIC(-1,0,getImMask(srcTemp),are,IC_head);
							
						}
						else if (src==2)
						{
							createSym(srcTemp,ic,0,0,1,sym_head,line_num);
							ic++;
							createIC(-1,0,0,0,IC_head);
							
						}
						else{
							ic++;
							createIC(-1,0,getMaskDestReg(srcTemp),are,IC_head);
						
						}
				
					
				}
				
			}

			/*rest the strings*/
			memset(srcTemp,0,sizeof(srcTemp));
			memset(destTemp,0,sizeof(destTemp));
			/*if any error was found return and read next line*/
			return errl;	
		}
		else 
		{


			/*if next argument is a . data or .string directive, check theme and create thair nodes*/
			if(str[0]=='.') 
			{
				if(!strcmp(str,".data"))
				{
					i+=space_skip(&line[i]);
					if (!check_data(line+i,line_num,data_head))
					{
						errl=0;
					}
					return errl;
				}
				if (!strcmp(str,".string"))
				 {
				 	i+=space_skip(&line[i]);
				 	if (!check_string(line+i,line_num,data_head))
					{
						errl=0;
					}
					return errl;
				 } 
			
			
			}
			/*if reached here than its means the word is not a symbol/label/instruction then print error*/
			printf("ERROR AT LINE %d: \"%s\" Is invalid symbol or instruction.\n",line_num,str);
			errl=0;
			return errl;
		}
		

		
		memset(str,0,sizeof(str)); /*reset string*/
		h=0;
		i += space_skip(line+i);/*call a function that return the number of spaces and tabs*/
		j=i;
	}	
	
	return errl;
}


/*creteIC function will create a memory node for each op word that has found to be legit,
memory nodes are a linked list of IC structure objects,
this function will recieve opcode of the intruction as integer, are code as integer,
the IC_head which is the memory list head.
besides creating the node itself this function also fills the code itself for the node,
becuase of that it also recieves the source and targer masks to be filled*/

IC *createIC(int op,int maskS,int maskD,int are,IC **IC_head){
	IC *temp = (IC *)malloc(sizeof(IC));
	IC *p = *IC_head;

	
	/*if list is empty*/
	if (*IC_head==NULL)
	{
		*IC_head=temp;
		temp->next=NULL;

	}
	else /*if not empty*/
	{
		/*find last node and place add new node to the next of it*/
		while(p->next!=NULL)
		{
			p=p->next;
		}
		p->next=temp;
		p=*IC_head;
	}
	if(op>-1)/*it means it got an action word so we we fill its code*/
	{	
		temp->code=temp->code | (op<<8);
		temp->code=temp->code | (maskS<<4);
		temp->code=temp->code | (maskD);
		temp->are=are;
	}
	else	/*it means its an argument so we we fill its code*/
	{	
		temp->code=maskD;
		temp->are=are;
	}

	return temp;
}

/*creteSym function will create a memory node for each symbol that has found to be legit,
the symbol list is a linked list of symbol structure objects,
the function will recieve the symbol name, in which memory word it had been found, if external
was declared before the symbol it would recieve 1 in the external int, same for entry.
if a addressing mathod 2 was found besides creating an IC node, it will also create a symbol node,
to later be interpreted in the second run, in thad case the functions recieves 1 int unknwen int,
line_num is pass for error purpuses.*/
symbol *createSym(char *name,int icCounter,int external ,int entry ,int unknown,symbol **sym_head,int line_num){
	symbol *temp = (symbol *)malloc(sizeof(symbol));
	symbol *p = *sym_head;

	/*if list is empty*/
	if (*sym_head==NULL)
	{
		*sym_head=temp;
		temp->next=NULL;

	}
	else /*if not empty*/
	{
		
		/*find last node while checking for symbols with the same name that has been declared in this file*/
		while(p->next!=NULL)
		{

			if (!strcmp(name,p->symName) && p->unknown==0 && unknown==0)
			{

				
				if (external==1)
				{
					free(temp);
					return NULL;
				}/*if the same name was already declared in this file*/
				printf("ERROR AT LINE %d: Label \"%s\" was already decelared.\n",line_num,name);
				return NULL;

			}

			p=p->next;

		}/*check for the same for the last item in the list*/
		
		if (!strcmp(name,p->symName) && p->unknown==0 && unknown==0)
			{
				if (external==1)
				{
					free(temp);
					return NULL;
				}
				printf("ERROR AT LINE %d: Label \"%s\" was already decelared.\n",line_num,name);
				return NULL;
			}
			
		p->next=temp;
		p=*sym_head;

	}
	/*fill all of the symbol data to be later tested in second run*/
	strcpy(temp->symName,name);
	temp->address=icCounter;
	temp->external=external;
	temp->entry=entry;
	temp->unknown=unknown;
	temp->line_num=line_num;


	return temp;

	}

/*check_data function will check if data is legit if .data directive was found,
for every integer found in the same line this function will call createData to create a data node, 
that later will be printed the the ob file.
if argument was not legit the function will return 0 and will print an error.
becuase the integers arguments are cofined to 15 bits in the compliments 2 mathod,
the integer declared must be between INT15_MIN and INT15_MAX that are define in this file */
	int check_data(char *line,int line_num,data **data_head){
	char temp[LABLE_LEN];
	int num;
	int i=0,j=0;
	int sign=0;

	while(line[i] != '\n' && line[i]!= '\0')
	{
		/*while loop the traverses the entire line*/
		while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i]!=',' && i<strlen(line))
			{
				/*check if a minus sign was found*/		
				if (line[i]=='-' && sign==0 )
				{
					sign=1;
					temp[j]=line[i];
					i++;
					j++;
					if (line[i]==' ' || line[i] == '\t')/*check whitespace between sign and operand*/
					{
						printf("ERROR AT LINE %d: No whitespace allowed after a sign definition.\n", line_num);
						return 0;
					}
					else if (line[i]==',' || i==strlen(line) || line[i] == '\n'){
						printf("ERROR AT LINE %d: \"-\" is defined without a integer.\n",line_num);
						return 0;
					}
					continue;

				}/*check multiple minus sign*/
				else if (line[i]=='-' && sign==1)
				{
					printf("ERROR AT LINE %d: Multipale \"-\" sign definition.\n", line_num);
					return 0;
				}
				else if (line[i]=='+' && sign==0)
				{/*skip plus sign if found and check for whitespaces*/
					sign=1;
					i++;
					if (line[i]==' ' || line[i] == '\t')
					{
						printf("ERROR AT LINE %d: No whitespace allowed after a sign definition.\n", line_num);
						return 0;
					}
					else if (line[i]==',' || i==strlen(line) || line[i] == '\n'){
						printf("ERROR AT LINE %d: \"+\" is defined without a integer.\n",line_num);
						return 0;
					}
					continue;
				}/*check multuple plus signs */
				else if (line[i]=='+' && sign==1)
				{
					printf("ERROR AT LINE %d: Multipale \"+\" sign definition.\n", line_num);
					return 0;
				}

				if(!isdigit(line[i]))/*check that there are no charcters that are not digits*/
				{
					printf("ERROR AT LINE %d: Only integers allowed in data definition.\n",line_num );
					return 0;
				}
				else
				{ /*every thing was fine so we add it to number temp variable*/
					temp[j]=line[i];
					i++;
					j++;
				}

			}
			/*check if there is a ',' between arguments */
			i+=space_skip(&line[i]);
			if (line[i]!=',' && line[i] != '\n' && i!=strlen(line))
			{
				printf("ERROR AT LINE %d: Invalid integer.\n",line_num);
				return 0;
			}
			
			/*convert the string to an integer*/
			num=atoi(temp);
			if(num>(INT15_MAX) || num <(INT15_MIN)){
				printf("ERROR AT LINE %d: Invalid integer, number should be between %d and %d.\n",line_num,(INT15_MIN),(INT15_MAX));
				return 0;
			}
			if (num<0)
			{
				num=num | 1<<14;/* code */
			}
			dc++; /*add to the data node coounter and create the node*/
			createData(num,data_head);


			if (line[i]==',')
			{
				i++;
				j=0;
				sign=0;
				/*check for multiple commas or exssesive comma in the end of the integer*/
				if (line[i+space_skip(&line[i])]==',')
				{
					printf("ERROR AT LINE %d: Multiple commas.\n",line_num);
					return 0;
				}
				else if(line[i+space_skip(&line[i])]=='\n' || (i+space_skip(&line[i]))==strlen(line))
				{
					printf("ERROR AT LINE %d: A comma was declared without next argument.\n",line_num);
					return 0;
				}
				i+=space_skip(&line[i]); /*skip spaces to the next integet or the line end*/
				memset(temp,0,sizeof(temp)); /*rest the string*/
			}		

	}

					
return 1;
}

/*check_string function will check if string is legit if .string directive was found,
for every charecter found in the string this function will call createData to create a data node, 
that later will be printed the the ob file.
if argument was not legit the function will return 0 and will print an error*/
int check_string(char *line,int line_num,data **data_head){
	int charecter;
	int i=0;

	/*check if word begins with quates*/
	if (line[i]=='\"')
		{
			i++;
		}
		else{
			printf("ERROR AT LINE %d: String definition need to start with quetes.\n",line_num);
			return 0;
		}
	/*read word and add data nodes*/
	while(line[i] != '\n' && line[i]!= '\0'&& line[i]!= '\"' && i<strlen(line))
	{

			charecter=line[i];
			dc++;
			i++;
			createData(charecter,data_head);	

	}
	if (line[i]!='\"')/*check if string end with quates*/
	{
			printf("ERROR AT LINE %d: String definition need to end with quetes.\n",line_num);
			return 0;
	}
	charecter='\0';
	dc++;
	createData(charecter,data_head);/*add the finishing \0 to string end*/
	if (line[i]=='\"' && line[1+i+(space_skip(&line[(i+1)]))]!='\n') /*check if any other charecter was declared after quates*/
	{
		line[strlen(line)-1]='\0';
		printf("ERROR AT LINE %d: invalid charecters \"%s\".\n",line_num,line+i+1);
	}	
return 1;
}


/*this function will create the data node after check_sting or check_data functions check their 
legitimacy.*/
data *createData(int num,data **data_head){
	data *temp = (data *)malloc(sizeof(data));
	data *p = *data_head;
	
	/*if list is empty*/
	if (*data_head==NULL)
	{
		*data_head=temp;
		temp->next=NULL;

	}
	else
	{
		/*if list is not empty search the last node and and new one to the next of it*/
		while(p->next!=NULL)
		{
			p=p->next;
		}
		p->next=temp;
		p=*data_head;
	}	
		/*fill the code of the node*/
		temp->code=num;
	
	return temp;
}



/*this function prints the memory list to the ob file*/
int printIC(IC **IC_head,FILE *ob)
{
	IC *p = *IC_head;
	int i=100;
	fprintf(ob,"%4d %d\n",ic,dc );
	while(p!=NULL)
		{
			
			fprintf(ob,"%04d %04o%o\n",i,p->code,p->are );
			p=p->next;
			i++;
		}
		return 0;
}

int printSym(symbol **symbol_head)
{
	symbol *p = *symbol_head;
	int i=0;
	while(p!=NULL)
		{
			
			printf("%p %d %d %s\n",(void *)p,i,p->address,p->symName);
			p=p->next;
			i++;
		}
		return 0;
}

int printData(data **data_head,FILE *ob)
{
	data *p = *data_head;
	int i=ic;
	while(p!=NULL)
		{
			
			fprintf(ob,"%04d %05o\n",i,p->code );
			p=p->next;
			i++;
		}
		return 0;

}


/*get addressing will get an argument from readline and would check if its a legal addressing mathod
if not it would print an error and will return -1 to readLine, if adressing mathod found to be legit
it would return the an intger the represent the right adressing mathod*/
int getAddressing(char *argument, int line_num){
	int i=0;
	int flag=0;
	char *p;
	int num;

	/*check if empty string*/
	p=argument;
	if(argument[0]=='\n')
	{
		printf("ERROR AT LINE %d: Missing Operands\n",line_num );
		return -1;
	}
	/*Check if immediate Addressing*/
	if(argument[i]=='#')
	{ /*skip # sign*/
		i++;
		if (argument[i]=='-' || argument[i]=='+') /*skip - or + sign*/
		{
			i++;
			for (; i < strlen(argument); i++)
			{ 
				if(!isdigit(argument[i])) /*check if all charecters are integers*/
				{
					printf("ERROR AT LINE %d: Immidiate argument must be a integer.\n",line_num);
					return -1;
				}
			}
			num=atoi(argument+1); /*check if argument is within boundries*/
			if(num>MAX_NUM || num<MIN_NUM)
			{
				printf("ERROR AT LINE %d: Value %d is invalid, immidiate argument must be between %d and %d.\n",line_num,num,MIN_NUM,MAX_NUM );
				return -1;
			}
			return 1;
		}
		else
		{/*check if all charecters are integers*/
			for (; i < strlen(argument); i++)
			{
				if(!isdigit(argument[i]))
				{
					printf("ERROR AT LINE %d: Immidiate argument must be a integer.\n",line_num);
					return -1;			
				}
			}
			num=atoi(argument+1);/*check if argument is within boundries*/
			if(num>MAX_NUM || num<MIN_NUM){
				printf("ERROR AT LINE %d: Value %d is invalid, immidiate argument must be between %d and %d.\n", line_num,num,MIN_NUM,MAX_NUM);
				return -1;
			}
		}
		return 1;
	}
	i=0;
	/*Check if indirect register addressing*/
	if (argument[i]=='*')
	{
		p++;
		for (; i < NUM_OF_REG; i++)
		{
			if (strcmp(p,registers[i])==0)
			{
				flag=1;
			}
		}
		if (flag)
		{
			return 1<<2;
		}
		else
		{
			printf("ERROR AT LINE %d: Register %s does not exists. \n",line_num,p );
			return -1;
		}
	}
	flag=0;
	i=0;

	/*check if direct register addressing*/
	for (; i < NUM_OF_REG; i++)
		{
			if (strcmp(p,registers[i])==0)
			{
				flag=1;
			}
		}
		if (flag)
		{
			return (1<<3);
		}
	i=0;
	/*check if label*/
	if(argument[i]=='\0' || argument[i]=='\t' || argument[i]==' '){
		return 0;
	}
	if(!is_label(p,line_num))
	{	/*if not the it couldnt possibly be an argument*/
		printf("\t\t  Therefore it can not be used as argument.\n");
		printf("ERROR AT LINE %d: Argument \"%s\" invalid, enter \"#\" before immidiate argument. \n", line_num,p);
		return -1;
	}
	else
		return 1<<1;
	}

/*if getAddressing found that both source and target argument are a registers, it would call
the get2regMask function to get a ingeter representation mask of the 3-8 bits, that mask represent the
number of the souce register in bits 6-8 and the target register in bits 3-5.*/
int get2regMask(char *dest, char *src){
	int maskSD;
	int i=0;
	int srcRegNum;
	int destRegNum;

	/*check if it direct adressing, if so skip the * sign becuase they are handled the same way*/
	if (dest[i]=='*')
	{
		dest++;
	}
	if (src[i]=='*')
	{
		src++;
	}
	for (; i < NUM_OF_REG; i++)/*find number of register for bot source and target registers*/
		{
			if (strcmp(src,registers[i])==0)
			{
				srcRegNum=i;
			};
			if (strcmp(dest,registers[i])==0)
			{
				destRegNum=i;
			};
		}
	/*bitwise or operation to create a mask to be implemented in the instruction code*/
	maskSD=(destRegNum) | (srcRegNum<<3);
	return maskSD;

}

/*readLine will call this function if the addressing found to be an immidiate argument, this
functions returns an integer the represnts the bits 3-14 the will be inserted to the op word
becuase the immidiate argument is cofined to 12 bits in the compliments 2 mathod,
the integer declared must be between MIN_NUM and MAX_NUM that are define in this file*/
int getImMask(char *imNum){
		int imMask;
		/*skip the # sign*/
		imNum++;
		imMask=atoi(imNum);
		if(imMask<0)/*becuase we use the 2 comliment mathod number below 1 need to have 1 in the last bit*/
		{
			imMask=imMask | 1<<13;
		}	
	return imMask;
}

/*readLine will call this function if the addressing of source argument found to be a register argument,
this function will return a mask that represents bits 6-8 of the op word */
int getMaskSrcReg(char *reg){
	int i=0;
	int srcRegNum;
	/*skip the * sign */
	if (reg[i]=='*')
	{
		reg++;
	}
	for (; i < NUM_OF_REG; i++)/*find the number of register*/
	{
		if (strcmp(reg,registers[i])==0)
			{
				srcRegNum=i;
			};

	}
	/*bit move use to creat a mast the will cover the 3-5 bits*/
	return srcRegNum<<3;
}

/*readLine will call this function if the addressing of target argument found to be a register argument,
this function will return a mask that represents bits 3-5 of the op word */
int getMaskDestReg(char *reg){
	int i=0;
	int destRegNum;
	/*skip the * sign */
	if (reg[i]=='*')
	{
		reg++;
	}
	for (; i < NUM_OF_REG; i++)/*find the number of register*/
	{
		if (strcmp(reg,registers[i])==0)
			{
				destRegNum=i;
			};
	}
	/*no need to move becuase the 3-5 bit are at the beginning of the code varible*/
	return destRegNum;
}


/*isLegelAdd tests the legitimacy of the source and target arguments for the spcific intruction that
was found before arguments.
if the instruction is capble of taking the found adressing mathods the function will return 1 for positive
result and will print and error and will return 0 for failure.*/
int isLegelAdd(int op,int src,int dest,int line_num){
	int i=0;
	int mask=0;
	char intToChar;

	/*check if the are source and target arguments for op codes 0-4*/
	if((src==0 || dest==0) && (op>=0 && op <=4))
	{
		printf("ERROR AT LINE %d: Actions 0-4 must have a source and destination argument.\n",line_num );
		return 0;
	} 
	if (op>=0 && op <=4)
	{
		mask=1;
		while(mask!=src)
		{
			mask = mask<<1;
			i++;
		}	
		intToChar=i + '0';
		if (!strchr(action_type_map[op].legal_src,intToChar)) /*check if the addressing mathod appears in the allowed adressing mathods in the action type map*/
		{
			printf("ERROR IN LINE %d: Wrong source adressing type for action %s.\n",line_num,action_type_map[op].aname );
			return 0;
		}

		mask=1;
		i=0;
		while(mask!=dest)
		{
			mask = mask<<1;
			i++;
		}
		intToChar=i + '0';
		if (!strchr(action_type_map[op].legal_dest,intToChar)) /*check if the addressing mathod appears in the allowed adressing mathods in the action type map*/
		{
			printf("ERROR IN LINE %d: Wrong destination adressing type for action %s.\n",line_num,action_type_map[op].aname );
			return 0;
		}
	}
	else if(src==0 && (op>=5 && op <=13)) /*check if there are no arguments for intstuctions 5-13*/
	{
		printf("ERROR AT LINE %d: Actions 5-13 must have a destination argument.\n",line_num );
		return 0;
	}
	else if(dest!=0 && (op>=5 && op <=13)) /*check if there are 2 arguements for instructions 5-13*/
	{
		printf("ERROR AT LINE %d: Actions 5-13 must have a destination argument only!\n",line_num );
		return 0;
	}
	else
	{
		mask=1;

		while(mask!=src)
		{
			mask = mask<<1;
			i++;

		}	
		intToChar=i + '0';
		if (!strchr(action_type_map[op].legal_dest,intToChar))/*check if the addressing mathod appears in the allowed adressing mathods in the action type map*/
		{
			
			printf("ERROR IN LINE %d: Wrong destination adressing type for action %s.\n",line_num,action_type_map[op].aname );
			return 0;
		}
	}
	
	return 1;
}

