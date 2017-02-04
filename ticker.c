 #include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sysexits.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>


enum {LINE_SIZE = 256};
enum {MAX_CO_SIZE = 64};
enum {MAX_TICKER_SIZE = 6};

//Code modified from BST code 
typedef struct stock{
    int cost;
	char *company;
	char ticker[6];

    struct stock *left;
    struct stock *right;
} stock;



// Code taken from intersector, used to organize tree alphabetically.
stock *Insert(stock *root, int cost, char *company, char *ticker)
{
  int result;

  if (root == NULL)
    {

      root = calloc (1,sizeof (stock));
      if (root == NULL)
	{
	  printf ("Memory Error");
	  return NULL;
	}
      else
	{
	
		if(company == NULL)
		{
			root -> company = NULL;
		}
		else 
		{
			root->company = calloc(1, MAX_CO_SIZE);
			strncpy(root->company, company, MAX_CO_SIZE);
		}
		
	  	root->cost = cost;
		root->left = root->right = NULL;
		strncpy(root->ticker, ticker, strlen(ticker));
	}
    }

  else
    {
      result = strcasecmp (ticker, root->ticker);
      if (result < 0)
	{
	  root->left = Insert (root->left, cost, company, ticker);
	}
      else if (result > 0)
	{
	  root->right = Insert (root->right, cost, company, ticker);
	}
      else
	{
	  return root;
	}

    }
  return root;
}


void print_node(stock *root)
{

	if(root->left)
	{
		print_node(root->left);
	}
	printf("%s %d ", root->ticker, root->cost);
	if(root->company != NULL)
	{
		printf("%s\n", root->company);
	}
	else 
	{
		puts(" ");
	}
	if (root -> right)
	{
		print_node(root->right);
	}
	
}
stock *rightRotate(stock *root)
{
	stock *temp = root->left;
	root->left = temp->right;
	temp->right = root;
	return temp;
}

stock *leftRotate(stock *root)
{
	stock *temp = root->right;
	root->right = temp->left;
	temp->left = root;
	return temp;
}
void
destroy_stocks (stock * tree)
{
  if (tree == NULL)
    {
      return;
    }
  destroy_stocks (tree->left);
  destroy_stocks (tree->right);
  free (tree->company);
  free (tree);
}

void maff(stock *tree, int cost)
{
	if(cost < 0)
	{
		if(tree->cost + cost < 01)
		{
			printf("You have subtracted too much, operation ignored\n");
			return;
		}
		tree->cost += cost;
	}
	else 
	{
		if(tree->cost + cost > 100000000)
		{
			printf("You have added too much, operation ignored\n");
			return;
		}
		tree->cost += cost;
	}
}
		
	
// This function brings the key at root if key is present in tree.
// If key is not present, then it brings the last accessed item at
// root.  This function modifies the tree and returns the new root
stock *splay(stock *root, char *ticker)
{

    // Base cases: root is NULL or key is present at root **
    if (root == NULL)
	{
        return root;
	}
	
	int result = strcasecmp(ticker, root->ticker);
	if (result == 0)
	{
		return root;
	}
 
    // Key lies in left subtree **
    if (result < 0)
    {
        // Key is not in tree, we are done
        if (root->left == NULL) 
		{
			return root;
		}
 
        // Zig-Zig (Left Left)
		result = strcasecmp(ticker, root->left->ticker);
        if (result < 0)
        {
            // First recursively bring the key as root of left-left
            root->left->left = splay(root->left->left, ticker);
 
            // Do first rotation for root, second rotation is done after else
            root = rightRotate(root);
        }
        else if (result > 0) // Zig-Zag (Left Right)
        {
            // First recursively bring the key as root of left-right
            root->left->right = splay(root->left->right, ticker);
 
            // Do first rotation for root->left
            if (root->left->right != NULL)
				{
                root->left = leftRotate(root->left);
				}
        }
 
        // Do second rotation for root
        return (root->left == NULL) ? root: rightRotate(root);
    }
    else // Key lies in right subtree
    {
        // Key is not in tree, we are done
        if (root->right == NULL)
		{
		
			return root;
		} 

		result = strcasecmp(ticker, root->right->ticker);
        // Zag-Zig (Right Left)
        if (result < 0)
        {
            // Bring the key as root of right-left
            root->right->left = splay(root->right->left, ticker);
 
            // Do first rotation for root->right
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        else if (result > 0)// Zag-Zag (Right Right)
        {
            // Bring the key as root of right-right and do first rotation
            root->right->right = splay(root->right->right, ticker);
            root = leftRotate(root);
        }
 
        // Do second rotation for root
        return (root->right == NULL) ? root: leftRotate(root);
    }
}
 
// The search function for Splay tree.  Note that this function
// returns the new root of Splay Tree.  If key is present in tree
// then, it is moved to root.
stock *search(stock *root, char *ticker)
{
    return splay(root, ticker);
}

//TODO : rotate-left;
//TODO : rotate-right;
//TODO : Zig-Zig;
//TODO : Zig;
//TODO : Finf;
//TODO : Delete;

int
main (int argc, char *argv[])
{
  FILE *stockFile;

  //Struct stat is a built in structure that gives you multiple file 
  //properties. It is being used here for the size of file. 
  struct stat *buffer = malloc (sizeof (*buffer));
  int i = 1;
  off_t fileEnd;

  //The for loop iterates through all files passed through the command line.
  //The for loop then checks to make sure that none of the files are empty.
  for (i = 1; i < argc; i++)
    {
      stat (argv[i], buffer);
      fileEnd = buffer->st_size;
      if (fileEnd == 0)
	{
	  printf ("Empty Files not allowed\n");
      free(buffer);
	  return EX_USAGE;
	}
    }

  //This will check to be sure more than one file was passed to the program.
  //If this is true, then the first file will be assigned to a file pointer.
  if (argc < 2)
    {
      printf ("Please pass a file\n");
      return EX_USAGE;
    }
  else
    {
      stockFile = fopen (argv[1], "r");

      if (!stockFile)
	{
	  return EX_USAGE;
	}

    }

  //This temporary buffer is used to bring in words. The pointer is calloced
  //to the size of MAX_WORD_SIZE, an enumed value of 256, the maximum word
  //size the program will see.
  stock *tree = NULL; 

	char buf[LINE_SIZE];
		

 	char *token;
 	char ticker[6] = {0};
	int cost;
	char *company = calloc(1, MAX_CO_SIZE);
	

 	while(fgets(buf, LINE_SIZE, stockFile))
 	{
		token = strtok(buf, " \n\t");
		strncpy(ticker, token, strlen(token));
		
		while(token != NULL)
		{
			token = strtok(NULL, ".\t\n");
			cost = strtol(token, NULL, 10);
			cost *= 100;
			token = strtok(NULL, " \n");
			cost += strtol(token, NULL, 10);
			token = strtok(NULL, "\n");
			if(token == NULL)
			{
				memset(company, '\0', strlen(company));
				token = NULL;
			}
			else 
			{
			    strncpy(company, token, MAX_CO_SIZE);
				token = NULL;
			}
			
		}
		tree = Insert(tree, cost, company, ticker);
		memset(buf, '\0', strlen(buf));
		memset(company, '\0', strlen(company));
		memset(ticker, '\0', strlen(ticker));
	
	}

	free(company);
	print_node(tree);

	int result;

	while(fgets(buf, LINE_SIZE, stdin))
 	{
		result = 0;
		token = strtok(buf, " \n\t");
		strncpy(ticker, token, strlen(token));

		tree = search(tree, ticker);

		result = strcasecmp(tree->ticker, ticker);
		while(token != NULL)
		{
			token = strtok(NULL, ".\t\n");
		
			cost = strtol(token, NULL, 10);
			printf("%d\n", cost);
			cost *= 100;
			printf("%d\n", cost);
			token = strtok(NULL, " \n");
			cost -= strtol(token, NULL, 10);
			printf("%d\n", cost);
			token = NULL;
			
			
		}
		if(result == 0)
		{
			maff(tree, cost);
			printf("%d\n", tree->cost);
		}
		else
		{
			company = NULL;
			tree = Insert(tree, cost, company, ticker);
		}
		memset(buf, '\0', strlen(buf));
		memset(ticker, '\0', strlen(ticker));
	
	}
		
		print_node(tree);

		//free(company);
		free(buffer);
		destroy_stocks(tree);
		fclose(stockFile);
	
}
				
 	


