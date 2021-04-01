#include "functions.h"
#include "dataStructs.h"
#include "firstRun.h"

/*a function that return the number of spaces and tabs*/
int space_skip(char *line)
{
	int i = 0;
	while((line[i] == ' ' || line[i] == '\t') && line[i]!='\n' && line[i]!=EOF)/*loop until next char and count each loop into i*/
	{
		i++;
	}
	return i;
}

/*get a string and check if he is an correct action. if it is its return the number of the action at assembli and if not it will return -1*/
int is_action(char *action)
{
	
	
	int i=0;
	for(;i<NUM_OF_ACTIONS;i++)
	{
		
		if(!strcmp(action,action_type_map[i].aname))
			return i; 
	}
	return -1;
}

/*function that get a string that hold lable name, and the line num we founed the lable
  it will return 0 if the lable name is not legal and 1 if he is legal*/
int is_label(char label[],int line_num)
{

	int i=0,j=0;
	if (strlen(label)>LABLE_LEN)
	{
		printf("ERROR AT LINE %d:\"%s\" is too long for a label name.\n", line_num,label);
		return 0;
	}
	/*check if first letter is an letter, !cant be a digit or a sign!*/
	if(isalpha(label[i]))
	{
		i++;
		/*check if the rest of the string is all letters or numbers !cant be a sign! */
		for(;i<(strlen(label))-1;i++)
		{
			if(!isdigit(label[i]) && !isalpha(label[i]))
			{
				printf("ERROR AT LINE %d: Label name can contain only alphabetic or numeric charecters.\n",line_num ); 
				return 0;
			}	
		}

		for(;j<NUM_OF_ACTIONS;j++)
		{

		
			if(!strcmp(label,action_type_map[j].aname))
			{
				printf("ERROR AT LINE %d: Label name cannot be an instruction name.\n", line_num); 
				return 0;
			}
			
		}
		for(j=0;j<NUM_OF_REG;j++)
		{
		
			if(!strcmp(label,registers[j]))
			{
				printf("ERROR AT LINE %d: Label name cannot be an register name.\n",line_num ); 
				return 0;
			}
			
		}
		return 1;
	}
	else
		printf("ERROR AT LINE %d: First charecter in a label must be alphabetic.\n",line_num );
		return 0;
}






