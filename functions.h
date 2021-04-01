#ifndef FUNCTIONS_H
	#define FUNCTIONS_H
#include <ctype.h>



/*a function that get a pointer to a line and will return the number of white spaces*/
int space_skip(char *line);

/*get a string and check if he is an correct action. if it is its return the number of the action at assembli and if not it will return -1*/
int is_action(char action[]);

/*function that get a string that hold lable name, and the line num we founed the lable
  it will return 0 if the lable name is not legal and 1 if he is legal*/
int is_label(char *label,int line_num);

#endif
