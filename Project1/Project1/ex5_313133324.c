# define NAME_LENGTH 200
# define MAX_LINE_LENGTH 400
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// define struct
typedef struct hw_component
{
	char name[NAME_LENGTH];
	int copies;
	struct hw_component *next;
}HW_component;

/*Inputs: a string
Return parameters: int that represnts the value of the string
Function functionallity: convert a strimg that represents a number into an int*/
int char_to_int(char *number) {
	int new_number = 0;
	int i = 0;
	while (number[i] >= '0' && number[i] <= '9')
	{
		new_number *= 10;
		new_number += number[i] - '0';
		i++;
	}
	return new_number;
}
/* a test function sould be delated*/
void print_components(HW_component *head) {
	if (head == NULL)
		printf("\n");
	else {
		printf("%s%d \n", head->name, head->copies);
		print_components(head->next);
	}

}
/*Inputs: a pointer to the head of a nested list of HW_components
Return parameters: None
Function functionallity: freeing the memory which was allocated to the nested list we use in this progrem */
void free_components(HW_component *head) {
	HW_component* temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}

}
/*Inputs: 2 strings
Return parameters: int , that represnts which string is first alphabetically 
Function functionallity: check which one of the strings comes first alphabetically  */
int str_sort(const char *str_a, const char *str_b) {
	int i , j = 0 ,len_a = strlen(str_a) , len_b = strlen(str_b);
	for (i = 0, j = 0; i < len_a && j < len_b; i++, j++) {
		if (str_a[i] < str_b[j]) {
			return 1;
		}
		if (str_a[i] > str_b[j]) {
			return 0;
		}
	}
	if (len_a >= len_b) {
		return 0;
	}
	
	return 0;

}
/*Inputs: a name string and a string that represents the number of copies 
Return parameters: a pointer to new HW_component object
Function functionallity: create new HW_component and returnes a pointer to them  */
HW_component *new_comp(char *name, char *copies) {
	HW_component *result = (HW_component*)malloc(sizeof(HW_component));
	if (result == NULL) {
		printf("Error: memory allocation failed\n");
		exit(1);
	}
	strcpy(result->name, name);
	result->copies = char_to_int(copies);
	result->next = NULL;
	return result;
}

/*Inputs: a HW_component pointer of the head of the nested list  a name string and a string that represents the number of copies 
Return parameters: HW_component pointer that represent the head of the nested list with the new node 
Function functionallity: create new HW_component and put's him in his place in the sorted nested list and reurnes the pointer to the head of the
nested list */
HW_component * add_sorted_components(HW_component *head, char *name, char *copies) {
	HW_component* iter, *prev = NULL;
	HW_component* new_comp_to_add = new_comp(name, copies);
	if (head == NULL) {
		return new_comp_to_add;
	}
	if (str_sort(new_comp_to_add->name, head->name) == 1) {
		new_comp_to_add->next = head;
		return new_comp_to_add;
	}
	iter = head;
	while (iter != NULL  && str_sort(new_comp_to_add ->name ,iter ->name) == 0  ){
		prev = iter;
		iter = iter->next;
	}
	prev->next = new_comp_to_add;
	new_comp_to_add->next = iter;
	return head;
}


/*Inputs: a string that represents a file name
Return parameters: HW_component pointer that represent the head of the nested list that was made out of the arguments in the file
Function functionallity: opens a file and make out of every line a new HW_component and sortes the nested list of this HW_components , returns
a pointer to this head of the nested list*/
HW_component * init(char * componnets_list) {
	FILE *fp;
	char comp[MAX_LINE_LENGTH], name[NAME_LENGTH], copies[NAME_LENGTH];
	int counter = 0, i = 0, len_comp , j = 0 , k = 0;
	HW_component  *head = NULL ;
	if (NULL == (fp = fopen(componnets_list, "r"))) {
		printf("Error opening file");
		exit(1);
	}
	while (fgets(comp, MAX_LINE_LENGTH, fp) != NULL) {
		if (comp == NULL)
			break;
		len_comp = strlen(comp);
		for (i = 0, k = 0, j = 0; i < len_comp && comp[i] != '\n'; i++) {
			if (comp[i] == '$') {
				counter += 1;
				continue;
			}
			if (counter == 3) {
				if (comp[i] != ' ') {
					copies[k] = comp[i];
					k++;
				}
				continue;
			}
			if (counter < 3) {
				name[j] = comp[i];
				j++;
			}
		}
		counter = 0;
		name[j] = '\0';
		copies[k] = '\0';
		head = add_sorted_components(head, name, copies);
	}
	return head;
}

void rename_componnent(char*  hw_component_name, char*  new_hw_component_name) {

}



void returned_component(char*  hw_component_name, int copies) {

}


void production(char*  hw_component_name, int copies) {

}


void fatal_malfunction(char*  hw_component_name, int copies) {

}


void fire(char*  hw_component_name, int copies) {

}


int main(){
	HW_component *head ,*temp;
	head = init("hw_components.txt");
	temp = head;
	print_components(temp);
	free_components(head);
	return 0;

}