# define NAME_LENGTH 200
# define MAX_LINE_LENGTH 400
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// define struct
typedef struct hw_component {
	char name[NAME_LENGTH];
	int copies;
	struct hw_component *next;
}HW_component;

/*Inputs: a line that was read from the file (hw_components or actions), empty list of strings, and an indicator
Return parameters: None
Function functionality: splitting the read line into 2 or 3 different strings by "$" delimeter, depends on the indicator:
2 for hw_components and 3 for actions (and deleting \n from the end of the line if exists)*/
void split(char line[], char split_str[][NAME_LENGTH], int ind) {
	char *token = NULL;
	if(line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
	switch (ind) {
	case 2:
		token = strtok(line, "$");
		token[strlen(token) - 1] = '\0';
		strcpy(split_str[0], token);

		token = strtok(NULL, "$");
		strcpy(split_str[1], ++token);
		break;

	case 3:
		token = strtok(line, "$");
		token[strlen(token) - 1] = '\0';
		strcpy(split_str[0], token);

		token = strtok(NULL, "$");
		token++[strlen(token) - 1] = '\0';
		strcpy(split_str[1], token);

		token = strtok(NULL, "$");
		strcpy(split_str[2], ++token);
		break;
	}
}

/*Inputs: a pointer to the head of a nested list of HW_components
Return parameters: None
Function functionality: freeing the memory which was allocated to the nested list we use in this progrem */
void free_components(HW_component *head) {
	HW_component* temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

/*Inputs: 2 strings
Return parameters: int, that represents which string is first alphabethically 
Function functionality: check which one of the strings comes first alphabehtically  */
int str_sort(const char *str_a, const char *str_b) {
	int i, len_a = strlen(str_a), len_b = strlen(str_b);

	for (i = 0; i < len_a && i < len_b; i++) {
		if (str_a[i] < str_b[i])
			return 1;
		if (str_a[i] > str_b[i])
			return 0;
	}
	if (len_a >= len_b)
		return 0;
	return 0;
}

/*Inputs: a name string and a string that represents the number of copies 
Return parameters: a pointer to new HW_component object
Function functionality: create new HW_component and returns a pointer to them*/
HW_component* new_component(char *name, char *copies, HW_component *head) {
	HW_component *result = (HW_component*)malloc(sizeof(HW_component));
	if (result == NULL) {
		printf("Error: memory allocation failed\n");
		free_components(head);
		exit(1);
	}
	strcpy(result->name, name);
	result->copies = atoi(copies);
	result->next = NULL;
	return result;
}

/*Inputs: a HW_component pointer of the head of the nested list and  a name string
Return parameters: HW_component pointer that represents the component without the HW_component with that name
Function functionality: checks if a component in the nested list has this name. If there is, delete it and return a pointer to the new nested list,
if not returns the head as is*/
HW_component* del_comp(HW_component *head, char *comp_name) {
	HW_component* iter = head, *prev = NULL;
	if (head == NULL)
		return head;
	if (strcmp(head->name, comp_name) == 0) {
		iter = head->next;
		free(head);
		return iter;
	}
	while (iter->next != NULL) {
		if (strcmp(iter->name, comp_name) == 0) {
			prev->next = iter->next;
			free(iter);
			break;
		  }
		prev = iter;
		iter = iter->next;
	}
	return head;
}

/*Inputs: a HW_component pointer of the head of the nested list and a name string 
Return parameters: HW_component pointer that represent the component with that name string
Function functionality: checks if  compeonent in the nested list has this name if there is returns his pointer if not returns NULL */
HW_component* is_in(HW_component *head, char *name) {
	while (head != NULL) {
		if (strcmp(name, head->name) == 0) return head;
		head = head->next;
	}
	return NULL;
 }

/*Inputs: a HW_component pointer of the head of the nested list, a name string and a string that represents the number of copies 
Return parameters: HW_component pointer that represent the head of the nested list with the new node 
Function functionality: create new HW_component and put's him in his place in the sorted nested list and reurnes the pointer to the head of the
nested list */
HW_component* add_to_sorted_components(HW_component *head, HW_component *new_comp_to_add) {
	HW_component *iter, *prev = NULL;
	if (head == NULL) return new_comp_to_add;

	if (str_sort(new_comp_to_add->name, head->name) == 1) {
		new_comp_to_add->next = head;
		return new_comp_to_add;
	}
	iter = head;
	while (iter != NULL && str_sort(new_comp_to_add->name, iter->name) == 0) {
		prev = iter;
		iter = iter->next;
	}
	prev->next = new_comp_to_add;
	new_comp_to_add->next = iter;
	return head;
}

/*Inputs: a string that represents a file name
Return parameters: HW_component pointer that represents the head of the nested list that was made out of the arguments in the file
Function functionality: opens a file and creates out of every line a new HW_component and sorts the nested list of this HW_components, returns
a pointer to this head of the nested list*/
HW_component* init(char *components_list) {
	FILE *fp;
	char comp[MAX_LINE_LENGTH];
	HW_component *head = NULL, *new_comp_to_add;
	if (NULL == (fp = fopen(components_list, "r"))) {
		printf("Error opening file");
		exit(1);
	}
	char split_str[2][NAME_LENGTH];
	while (fgets(comp, MAX_LINE_LENGTH, fp) != NULL && comp != NULL) {
		split(comp, split_str, 2);
		new_comp_to_add = new_component(split_str[0], split_str[1], head);
		head = add_to_sorted_components(head, new_comp_to_add);
	}
	fclose(fp);
	return head;
}

/*Inputs: a HW_component pointer of the head of the nested list and destination file address
Return parameters: None
Function functionality: printing the components nested list into a file in the given destination*/
void Finalize(char *destination, HW_component *head) {
	FILE *fp;
	HW_component *temp = head;
	if (NULL == (fp = fopen(destination, "w"))) {
		printf("Error opening file");
		exit(1);
	}
	while (temp->next != NULL) {
		fprintf(fp, "%s $$$ %d\n", temp->name, temp->copies);
		temp = temp->next;
	}
	fprintf(fp, "%s $$$ %d", temp->name, temp->copies);
	fclose(fp);
}

/*Inputs: a HW_component pointer of the head of the nested list and 2 name strings: current name and a new name
Return parameters : HW_component pointer that represent the head of the nested list
Function functionality : checks if there is a component with that name if not does nothing if there is, then deletes the old one and creates
a new one with the new name */
HW_component* rename_component(char *hw_component_name, char *new_hw_component_name ,HW_component *head) {
	HW_component *temp1, *new_comp ;
	temp1 = head;
	if ((temp1 = is_in(temp1, hw_component_name)) == NULL)
		return head;
	new_comp = new_component(new_hw_component_name, "0", head);
	new_comp->copies = temp1->copies;
	head = del_comp(head, temp1->name);
	return add_to_sorted_components(head, new_comp);
}

/*Inputs: a HW_component pointer of the head of the nested list, a name string and the amount of copies string
Return parameters : HW_component pointer that represents the head of the nested list 
Function functionality : checks if there is a component with that name. if not create it, if there is, then adds the copies to
the copies field of the component */
HW_component* returned_component(char *hw_component_name, char *copies, HW_component *head) {
	HW_component *temp1, *new_comp;
	temp1 = head;
	if ((temp1 = is_in(temp1, hw_component_name)) == NULL) {
		new_comp = new_component(hw_component_name, copies, head);
		return add_to_sorted_components(head, new_comp);
	}
	temp1->copies += atoi(copies);
	return head;
}

/*Inputs: a HW_component pointer of the head of the nested list, a name string and the amount of copies string
Return parameters : HW_component pointer that represents the head of the nested list
Function functionality : checks if there is a component with that name, if not-create it. if there is, then adds the copies to
the copies field of the component*/
HW_component* production(char *hw_component_name, char *copies, HW_component *head) {
	return returned_component(hw_component_name, copies, head);	
}

/*Inputs: a HW_component pointer of the head of the nested list, a malfunctioned component name string and the amount of copies this component
Return parameters : HW_component pointer that represents the head of the nested list
Function functionality : checks if there is a component with that name, if not-do nothing. If it exists, then remove the amount of malfunctioned 
comps from the existing amount (or put 0 if that amount is bigger than the available number)*/
HW_component* fatal_malfunction(char *hw_component_name, char *copies, HW_component *head) {
	HW_component *temp = head;
	if ((temp = is_in(temp, hw_component_name)) == NULL)
		return head;
	temp->copies = temp->copies < atoi(copies) ? 0 : temp->copies - atoi(copies);
	return head;
}

/*Inputs: a HW_component pointer of the head of the nested list, a burnt component name string and the amount of copies this component
Return parameters : HW_component pointer that represents the head of the nested list
Function functionality : checks if there is a component with that name, if not-do nothing. If it exists, then remove the amount of burnt
comps from the existing amount (or put 0 if that amount is bigger than the available number)*/
HW_component* fire(char *hw_component_name, char *copies, HW_component *head) {
	return fatal_malfunction(hw_component_name, copies, head);
}

/*Inputs: a list of the files directories by this order: hw_components, actions, updated_components
Return parameters: None
Function functionality: 1. initializing the components nested list(by alphabethically order)
						2. reads each action from "actions" file and sending the components list to each action with the relevant parameters,
						3. and eventually writing the updates into a new file and freeing the allocated memory*/
void Actions(char **file_list) {
	FILE *fp;
	char action[MAX_LINE_LENGTH];
	if (NULL == (fp = fopen(file_list[2], "r"))) {
		printf("Error opening file");
		exit(1);
	}
	fgets(action, MAX_LINE_LENGTH, fp);
	HW_component *head = init(file_list[1]);
	char split_str[3][NAME_LENGTH];
	
	while (fgets(action, MAX_LINE_LENGTH, fp) != NULL && action != NULL && strcmp(action, "Finalize") != 0) {
		split(action, split_str, 3);
		if (strcmp(split_str[0], "Rename") == 0)
			head = rename_component(split_str[1], split_str[2], head);
		if (strcmp(split_str[0], "Returned_from_customer") == 0)
			head = returned_component(split_str[1], split_str[2], head);
		if (strcmp(split_str[0], "Production") == 0)
			head = production(split_str[1], split_str[2], head);
		if (strcmp(split_str[0], "Fatal_malfunction") == 0)
			head = fatal_malfunction(split_str[1], split_str[2], head);
		if (strcmp(split_str[0], "Fire") == 0)
			head = fire(split_str[1], split_str[2], head);
	}
	fclose(fp);
	Finalize(file_list[3], head);
	free_components(head);
}

int main(int argc, char* argv[]){
	Actions(argv);
	return 0;
}