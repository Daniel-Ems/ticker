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

//Code modified from BST code 
typedef struct stock{
    size_t cost;
	char *company;
	char ticker[6];
    struct stock *left;
    struct stock *right;
} stock;



// Code taken from intersector, used to organize tree alphabetically.
stock *Insert(stock *root, size_t cost, char *company, char *ticker)
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
	
	  	root->cost = cost;
		if(company == NULL)
		{
			root -> company = NULL;
		}
		else 
		{
			root->company = calloc(1, MAX_CO_SIZE);
			strncpy(root->company, company, MAX_CO_SIZE);
		}
		strncpy(root->ticker, ticker, strlen(ticker));
		root->left = root->right = NULL;
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
	printf("%s %zd ", root->ticker, root->cost);
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

// This function brings the key at root if key is present in tree.
// If key is not present, then it brings the last accessed item at
// root.  This function modifies the tree and returns the new root
stock *splay(stock *root, stock *test)
{
	int result = strcasecmp(test->ticker, root->ticker);

    // Base cases: root is NULL or key is present at root **
    if (root == NULL || result == 0)
        return root;
 
    // Key lies in left subtree **
    if (result < 0)
    {
        // Key is not in tree, we are done
        if (root->left == NULL) 
		{
			root->left = test;
			test->right = test->left = NULL;
			rightRotate(root);
			return test;
		}
 
        // Zig-Zig (Left Left)
		result = strcasecmp(test->ticker, root->left->ticker);
        if (result < 0)
        {
            // First recursively bring the key as root of left-left
            root->left->left = splay(root->left->left, test);
 
            // Do first rotation for root, second rotation is done after else
            root = rightRotate(root);
        }
        else if (result > 0) // Zig-Zag (Left Right)
        {
            // First recursively bring the key as root of left-right
            root->left->right = splay(root->left->right, test);
 
            // Do first rotation for root->left
            if (root->left->right != NULL)
                root->left = rightRotate(root->left);
        }
 
        // Do second rotation for root
        return (root->left == NULL)? root: rightRotate(root);
    }
    else // Key lies in right subtree
    {
		
        // Key is not in tree, we are done
        if (root->right == NULL)
		{
			root->right = test;
			test->right = test->left = NULL;
			leftRotate(root);
			return test;
		} 
		result = strcasecmp(test->ticker, root->right->ticker);
        // Zag-Zig (Right Left)
        if (result < 0)
        {
            // Bring the key as root of right-left
            root->right->left = splay(root->right->left, test);
 
            // Do first rotation for root->right
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        else if (result > 0)// Zag-Zag (Right Right)
        {
            // Bring the key as root of right-right and do first rotation
            root->right->right = splay(root->right->right, test);
            root = leftRotate(root);
        }
 
        // Do second rotation for root
        return (root->right == NULL)? root: leftRotate(root);
    }
}
 
// The search function for Splay tree.  Note that this function
// returns the new root of Splay Tree.  If key is present in tree
// then, it is moved to root.
stock *search(stock *root, stock *test)
{
    return splay(root, test);
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
	size_t cost;
	char *company = calloc(1, MAX_CO_SIZE+1);
	

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
		print_node(tree);

		stock *test = calloc(1,sizeof(stock));
		char *term = calloc(1,6);
		size_t thing = 1234;
		strncpy(term, "FFF\0", 6);
		strncpy(test->ticker, term, strlen(term));
		test->company = NULL;
		test->cost = thing;
		test->right = test->left = NULL;

		tree = search(tree, test);

		puts(" ");
		printf("%s %zd \n", tree->ticker, tree->cost);
		puts(" ");

		free(term);
		print_node(tree);
		destroy_stocks(tree);
		free(test);
		free(company);
		free(buffer);
		fclose(stockFile);
	
}
				
 	


