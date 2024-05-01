#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "htable.h"

#define SIZE 257


int swap(DataNode *, DataNode *);
int alpha_cmp(const void *, const void *);


/*
 * Create a new DataNode will all the vairables initialiazed
 *
 * Input:
 * 	None
 *
 * 	Output:
 * 		DataNode new_node: a new single node
 * */
DataNode *create_node()
{
	DataNode *new_node;

	new_node = (DataNode *)malloc(sizeof(DataNode));
	if(new_node == NULL)
	{
		perror("Malloc");
		exit(EXIT_FAILURE);
	}
	new_node->data = '\0';
	new_node->value = 0;
	new_node->next = NULL;
	new_node->left = NULL;
	new_node->right = NULL;

	return new_node;
}


/*
 * see if the parameter character exist somewhere in the list of nodes or node
 * if it does not exit then it will create a new node and list it to the 
 * existing "list" else the node in said list's value variable will 
 * increase by 1
 *
 * Input:
 * 	unsigned char new_char: the incoming character
 * 	DataNode *list: the existing node or linked list of nodes
 * */
void insert_node(unsigned char new_char, DataNode *list)
{			
	DataNode *head;
	DataNode *current;
	DataNode *new_node;

	head = NULL;
	current = list;
	new_node = NULL;

	/* Is there a node? */
	if(current == NULL)
	{
		fprintf(stderr, "invalid argument passed in for DataNode\n");
		exit(EXIT_FAILURE);
	}

	/* Are there values in list? */	
	if(current->data != '\0')
	{			
		/* Middle */
		do
		{	
			/* Keep track of the Existing node address */
			head = current;

			/* is new_char a new char? */
			if(new_char == current->data)
			{
				/* Does Exist */			
				current->value++;
				break;
			}
			current = current->next;
		}while(current != NULL);
	
		if(current == NULL)
		{
			/* Last */
			new_node = create_node();	
			new_node->data = new_char;
			new_node->value++;

			current = new_node;
			head->next = current;
		}
	}
	/* DNE */ 	
	else
	{
		/* First */
		current->data = new_char;
		current->value++;
	}		
}


/* TODO: This sort function is basic and slow, depending of 
 * 			 performance and bandwith will improve to qucik sort
 * */

/*
 * Sort the linked list in ascending order in frequency and 
 * breaking numerical ties with character in ascending order.
 *
 * Input:
 * 	DataNode *l_list: linked list of nodes
 *
 * Output:
 * 	None
 * */
void sort_list(DataNode *l_list)
{
	DataNode *node, *temp;	
	int swapped;

	node = l_list;		
	while(node != NULL)
	{
		temp = node;
		swapped = 0;
		while(temp->next != NULL)
		{
			if(temp->value > temp->next->value)
			{
				swapped = swap(temp, temp->next);
			}
			else if(temp->value == temp->next->value)
			{
				/*
				 * Finding the difference in ASCII values
				 * if value is > 0 means switch as we are 
				 * sorting in ascending order.
				 * */
				if((temp->data - temp->next->data) > 0)
				{	
					swapped = swap(temp, temp->next);
				}
			}
			temp = temp->next;	
		}
		if(!swapped)
		{
			break;
		}
	}	
}


/* TODO: Part of function above , documenation applies to this 
 *       block too...
 * */
int swap(DataNode *node_a, DataNode *node_b)
{		
	unsigned char tdata;
	unsigned int tvalue;
	DataNode *tleft, *tright;
	int swapped;

	swapped = 1;	
	tdata = node_a->data;
	tvalue = node_a->value;
	tleft = node_a->left;
	tright = node_a->right;
	
	node_a->data = node_b->data;	
	node_a->value = node_b->value;
	node_a->left = node_b->left;
	node_a->right = node_b->right;

	node_b->data = tdata;
	node_b->value = tvalue;
	node_b->left = tleft;
	node_b->right= tright;

	tdata = '\0';
	tvalue = 0;
	tleft = NULL;
	tright = NULL;

	return swapped;
}


/*
 * Create a dynamic string that houses all the existing char in the link
 * link list.
 *
 * Input:
 * 	DataNode *link: existing linked list of nodes
 *
 * 	Output:
 * 		char *array: a sorted in ascnding order string	
 * */
char *link_to_array(DataNode *link)
{	
	char *array;
	unsigned int index;
	DataNode *head;

	array = NULL;
	index = 0;
	head = link;

	/* We can cheat by creating a string that is 0x00 - 0xFF instead
   * of being dynamically creatling a string since we know only one
   * of those hexadecimal will occupy only one space on this string*/
	array = (char *)calloc(SIZE, sizeof(char));
	if(array == NULL)
	{
		perror("Calloc");
		exit(EXIT_FAILURE);
	}

	index = 0;
	while(head != NULL)
	{	
		array[index] = head->data;
		head = head->next;
		index++;
	}
	/* The +1 for size is to avoid sorting the null terminator */
	qsort(array, (index), sizeof(char), alpha_cmp);
	return array;
}


/*
 * Sorting function for the function above
 *
 * Input:
 * 	Takes in 2 values from link_to_array's array
 *
 * 	Output:
 * 		int value: a int value that determines if the two values a and b
 * 		           need sorting
 * */
int alpha_cmp(const void *a, const void *b)
{
	return (*(char *)a) - (*(char *)b);
}


/*
 * Build a tree from the left most nodes to the right, reinsert the new
 * node into the list in sorted order, repeat this proceed untill there exist
 * one node that is linked too all other nodes.
 *
 * Input:
 * 	DataNode *link: incoming linked list
 *
 * Output:
 * 	DataNode *link: the pointer pointing to the top of the tree i.e. head 
 * */
DataNode *build_tree(DataNode *link)
{
	DataNode *node, *left, *right;

	node = NULL;
	left = NULL;
	right = NULL;

	if(link != NULL)
	{
		while(link->next != NULL)
		{
			node = create_node();
			left = link;	
			right = left->next;	

			/* Get sum of left and right node */
			node->value = (left->value) + (right->value);
			node->left = left;
			node->right = right;
			
			/* Point to the next node after the targeted two */
			node->next = right->next;
		
			left->next = NULL;
			right->next = NULL;
		
			link = node;	
			sort_list(link);
		}
	}
	return link;
}


/*
 * Depth first search... find the target character and return the path
 * that it needs to get there
 *
 * Input:
 * 	DataNode *tree: incoming tree
 * 	char c: char it is looking for 
 *
 * Ouput:
 *	Path in the tree to get there 0 iare left paths and 1 are right 	
 * */
char *dfs(DataNode *tree, char c)
{
	char *bin_path;
	int index;
	unsigned short found;

	/* Knowing max height is 255 in relation to all the valid ASCII */	
	bin_path = (char *)calloc(SIZE, sizeof(char));	
	index = -1;

	found = dfs_traversal(tree, bin_path, c, index);	
	if(!found)
	{
		fprintf(stderr, "Cannot find path of Node\n");
		exit(EXIT_FAILURE);
	}
	return bin_path;
}


/*
 * As stated by the doc string above this function is too find the path
 * recursively.
 *
 * Input:
 * 	DataNode *tree: linked nodes tree
 * 	char *r_str: string to populate with the path
 * 	char target: char we want to find in the tree
 * 	int index: keep track of the index of the empty string
 *
 * 	Ouput:
 * 		int found: 0 means we did not find the path after 
 * 		           traversing through every node else 1
 * */
unsigned short dfs_traversal(DataNode *tree,char *r_str,char target,int index)
{
	DataNode *cur_node, *left, *right;
	unsigned short found;
	
	cur_node = tree;
	left = NULL;
	right = NULL;
	found = 0;
	index++; /* Creating current index for path */

	if(cur_node != NULL)
	{	
		/* Found the Node, return path */
		if((cur_node->data - target) == 0)
		{
			found = 1;	
			return found;
		}

		/* Go left */
		if(cur_node->left != NULL)
		{
			r_str[index] = '0';	/* Encoding */
			left = cur_node->left;
			found = dfs_traversal(left, r_str, target, index);
			if(!found)
			{
				r_str[index] = '\0';
			}
		}
		
		/* Go right if left no good */
		if(cur_node->right != NULL && !found)
		{
			right = cur_node->right;
			found = dfs_traversal(right, r_str, target, index);
			if(found)
			{
				r_str[index] = '1'; /* Encoding */
			}
		}
	}
	return found;
}


/*
 * Free nodes in tree recursively
 *
 * Input:
 * 	DataNode *tree: tree
 *
 * 	Output:
 * 		None
 * */
void free_list(DataNode *tree)
{		
	if(tree->left == NULL && tree->right == NULL)
	{
		free(tree);
		tree = NULL;
		return;
	}
	else
	{	
		if(tree->left)
		{
			free_list(tree->left);
		}

		if(tree->right)
		{
			free_list(tree->right);
		}
	}
	/* Free entry node */
	free(tree);
}


/*
 * Free string
 *
 * Input:
 * 	char *str: incoming string
 *
 * Ouput:
 * 	None
 * */
void free_string(char *str)
{
	free(str);
}


