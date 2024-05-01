#define USR_ERR "usage: ./htable [ filename ]\n"

/* Collection of Nodes that points to the next node "Linked List */
typedef struct DataNode
{	
	unsigned char data;	
	unsigned int value;
	struct DataNode *next;
	struct DataNode *left;
	struct DataNode *right;
}DataNode;


DataNode *create_node();
DataNode *build_tree(DataNode *);
char *link_to_array(DataNode *);
char *dfs(DataNode *, char);
unsigned short dfs_traversal(DataNode *, char *, char, int);
void insert_node(unsigned char, DataNode *);
void sort_list(DataNode *);
void free_list(DataNode *);
void free_string(char *);


