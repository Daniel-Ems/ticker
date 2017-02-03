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
/*
void
word_check (char *string, wordnode * tree, int argv)
{

  if (tree == NULL)
    {
      return;
    }

  //strcasecmp returns -1,0,1 respectively according to the comparison between
  //string and tree->word.
  while (tree != NULL && strcasecmp (string, tree->word) != 0)
    {
      //If the result is less than the 0, the program needs to check left. 
      if (strcasecmp (string, tree->word) < 0)
	{
	  tree = tree->left;
	}
      //If the reuslt is greater than 0, then the program needs to check right. 
      else if (strcasecmp (string, tree->word) > 0)
	{
	  tree = tree->right;
	}
    }
  //Once the loop breaks, it has either found a match, or has found null, and
  //there for, no match. 
  if (tree != NULL && strcasecmp (string, tree->word) == 0)
    {
      if (tree->count < argv)
	{
	  tree->count++;
	}
      return;
    }
  else
    {
      return;
    }

}
*/

//Code Modified from BST code. 
stock *Insert(stock *root, size_t cost, char *company, char *ticker)
{
	if(root==NULL) 
	{
		root = (stock *) malloc(sizeof(stock));

		if(root == NULL)
		{
			printf("Memory Error");
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
			strncpy(root->ticker, ticker, strlen(ticker)+1);
			root->left = root->right = NULL;
		}
	}
	else
	{
		if(cost < root->cost) 
		{ 
			root->left = Insert(root->left, cost, company, ticker); 
		}
		else
		{ 
			root->right = Insert(root->right, cost, company, ticker); 
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

void
destroy_stocks (stock * tree)
{
  if (tree == NULL)
    {
      return;
    }
  //The function will recursively move to the bottom until it hits null.
  //It will then free the tree->word and then the node.  
  destroy_stocks (tree->left);
  destroy_stocks (tree->right);
  free (tree->company);
  free (tree);
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
 	char ticker[6];
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
			memset(buf, '\0', strlen(buf));
		}
		tree = Insert(tree, cost, company, ticker);
	
	}
		print_node(tree);
		destroy_stocks(tree);
		free(company);
		free(buffer);
		fclose(stockFile);
	
}
				
 	


