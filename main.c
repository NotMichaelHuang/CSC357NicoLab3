#include <stdio.h>
#include <stdlib.h>
#include "htable.h"


/*
 * Driver code, is temparay. Will be replace in 
 * time for Assginment 3 submission and all funcitons here
 * will be semi/fully integrated to said assignment.
 *
 * Input:
 * 	CLT input of a file name
 *
 * Output:
 *	Return EXIT_SUCCESS if everything passed and EXIT_FAILURE if any
 *	errors occurs
 * */
int main(int argc, char **argv)
{
	FILE *fd;
	int byte;
	unsigned int c;
	unsigned int iterate;
	char *sorted_string;
	char *p_path;
	DataNode *head;

	fd = NULL;
	byte = 0;
	c = '\0';
	iterate = 0;
	sorted_string = '\0';
	p_path = '\0';
	head = NULL;

	/* Only max of 2 arguments stdin and stdout */
	if(argc > 3)
	{
		fprintf(stderr, USR_ERR);
		exit(EXIT_FAILURE);
	}
	
	fd = fopen(argv[1], "rb");
	if(fd == NULL)
	{
		fprintf(stderr, "Unable to open file\n");
		exit(EXIT_FAILURE);
	}

	/* 
 	 * TODO: - change to unbuffered for Asgn3 
	 * */	
	head = create_node();	
	while((byte = fgetc(fd)) != EOF)
	{	
		/* Create List */
		c = byte;
		insert_node(c, head);	
	}	
	
	sort_list(head);	
	sorted_string = link_to_array(head);	
	head = build_tree(head);

	iterate = 0;
	while(sorted_string[iterate] != '\0')
	{
		p_path = dfs(head, sorted_string[iterate]);
		printf("%#04x: %s\n", sorted_string[iterate], p_path);
		free(p_path);
		iterate++;
	}

	/* Clean up and do our shares! */
	free_list(head);
	free_string(sorted_string);
	fclose(fd);

	return EXIT_SUCCESS;
}


