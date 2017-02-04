#include "stock.h"


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
			root->company = calloc(1, MAX_CO_SIZE+1);
			strncpy(root->company, company, MAX_CO_SIZE);
		}
		
	  	root->cost = cost;
		root->left = root->right = NULL;
		strncpy(root->ticker, ticker, MAX_TICKER_LEN);
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


stock *search(stock *root, char *ticker)
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
            root->left->left = search(root->left->left, ticker);
 
            // Do first rotation for root, second rotation is done after else
            root = rightRotate(root);
        }
        else if (result > 0) // Zig-Zag (Left Right)
        {
            // First recursively bring the key as root of left-right
            root->left->right = search(root->left->right, ticker);
 
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
            root->right->left = search(root->right->left, ticker);
 
            // Do first rotation for root->right
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        else if (result > 0)// Zag-Zag (Right Right)
        {
            // Bring the key as root of right-right and do first rotation
            root->right->right = search(root->right->right, ticker);
            root = leftRotate(root);
        }
 
        // Do second rotation for root
        return (root->right == NULL) ? root: leftRotate(root);
    }
}

int ticker_check(char *token)
{
	int i=0;
	int flag;
	if(token == NULL)
	{
		printf("Bad ticker\n");
		return flag = 0;	
	}
	if(!isalpha(token[i]))
	{
		return flag = 0;
	}
	
	return flag = 1;
}

int input_cash(char *token)
{
	int flag;
	size_t index = 0;
	if(token == NULL)
	{
		printf("%s - stock ignored\n", token);
		return flag = 0;
	}
	if(token[0] == '-')
	{
		printf("%s - stock ignored\n", token);
		return flag = 0;
	}
	if(strlen(token) > MAX_DIGITS)
	{
		printf("%s - stock ignored\n", token);
		return flag = 0;
	}
	for(index = 0; index < strlen(token); index++)
	{
		if(!isdigit(token[index]))
		{
			printf("%s - stock ignored\n", token);
			return flag = 0;
		}
	}
	return flag = 1;
}

int input_cents (char *token)
{
	int flag;
	size_t index = 0;
	if(token == NULL)
	{
		printf("%s - stock ignored\n", token);
		return flag = 0;
	}
	if(strlen(token) < MAX_CENTS)
	{
		printf("NOT enough sense\n");
		return flag = 0;
	}
	for(index = 0; index < strlen(token); index++)
	{
		if(!isdigit(token[index]))
		{
			printf("%s - stock ignored\n", token);
			return flag = 0;
		}
	}
	return flag = 1;
}

char *two_cents(char *token)
{
	if(strlen(token) > MAX_CENTS)
	{
		token[MAX_CENTS] = '\0';
	}
	
	return token;
}

int price_check(char *token)
{
	size_t index = 0;
	size_t value = 0;
	int flag = 0;
	if(token == NULL)
	{
		printf("It's not difficult, just get it right.\n");
		return flag = 0;
	}

	if(token[0] == '-')
	{
		if(strlen(token) > MAX_DIGITS+1)
		{
			printf("The cost can only be 7 digits\n");
			return flag = 0;
		}		
		value = 1;
	}

	else
	{
		if(strlen(token) > MAX_DIGITS)
		{
			printf("The cost can only be 7 digits\n");
			return flag = 0;
		}	
		value = 0;
	}

	for(index = value; index <strlen(token); index++)
	{
		if(!isdigit(token[index])) 
		{
			printf("You messed up\n");
			return flag = 0;
		}
	}
	return flag = 1;
}

int cent_check(char *token)
{
	size_t index = 0;
	int flag = 0;

	if(token == NULL)
	{
		printf("It's not difficult, just get it right.\n");
		return flag = 0;
	}
	if(strlen(token) < MAX_CENTS)
	{
		printf("NOT ENOUGH CENTS\n");
		return flag = 0;
	}
	for(index = 0; index <strlen(token); index++)
	{
		if(!isdigit(token[index])) 
		{
			printf("So close! But close is only good for horse shoes "
					"and hand grenades, try again\n");
			return flag = 0;
			
		}
	}
	return flag = 1;
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

void print_node(stock *root)
{

	if(root->left)
	{
		print_node(root->left);
	}
	float divide = (float)root->cost / 100;
	printf("%s %.2f %s\n", root->ticker, divide, root->company);
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
  destroy_stocks (tree->left);
  destroy_stocks (tree->right);
  free (tree->company);
  free (tree);
}

	
size_t tree_size(stock *root)
{
	size_t count = 1;
	if(!root)
	{
		return 0;
	}
	count += tree_size(root->left);
	count += tree_size(root->right);
	
	return count;
}

void traverse(stock *tree)
{
	if(tree == NULL)
	{
		return;
	}
	traverse(tree->left);
	traverse(tree->right);
}
stock *new_tree(stock *tree, stock *second, void(*traverse)(stock*),
		 stock*(*Insert_num)(stock *, int, char *, char *)) 
	{
		if(tree == NULL)
		{
			return second;
		}
		traverse(tree);
		second = Insert_num(second, tree->cost, tree->company, tree->ticker);
		traverse(tree);
		return second;
		
	}

stock *Insert_num(stock *root, int cost, char *company, char *ticker)
{
  

  if (root == NULL)
    {
      root = calloc (1, sizeof (stock));
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
			root->company = calloc(1, MAX_CO_SIZE+1);
			strncpy(root->company, company, MAX_CO_SIZE);
		}
		
	  	root->cost = cost;
		root->left = root->right = NULL;
		strncpy(root->ticker, ticker, MAX_TICKER_LEN);
	}
    }

  else
    {
    
      if (cost < root->cost)
	{
	  root->left = Insert_num(root->left, cost, company, ticker);
	}
      else if (cost > root->cost)
	{
	  root->right = Insert_num(root->right, cost, company, ticker);
	}
      else
	{
	  return root;
	}

    }
  return root;
}
 

