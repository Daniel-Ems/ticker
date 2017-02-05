#include "stock.h"

//inserts by tickers
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
		//Only calloc room for company if there is a company name. 
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
	  //strcasecmp is used and stored in result to sort by ticker and 
	  //reduce the number of times the function is calle. 
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

//Inserts by cost
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
      else if (cost >= root->cost)
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

//Assigns the root to the right child of it's left childs node, then moves 
//the child to root.
stock *rightRotate(stock *root)
{
	stock *temp = root->left;
	root->left = temp->right;
	temp->right = root;
	return temp;
}

//Assigns the root to the left child of it's right childs node, then moves 
//the child to root.
stock *leftRotate(stock *root)
{
	stock *temp = root->right;
	root->right = temp->left;
	temp->left = root;
	return temp;
}


stock *search(stock *root, char *ticker)
{

    //The base case
    if (root == NULL)
	{
        return root;
	}
	//our compare stored to limit number of times the function is called
	int result = strcasecmp(ticker, root->ticker);
	if (result == 0)
	{
		return root;
	}
 
    //The ticker is found in the left subtree
    if (result < 0)
    {
        //ticker is not present so we begin return
        if (root->left == NULL) 
		{
			return root;
		}
 
        //Zig-Zig (Left Left)
		result = strcasecmp(ticker, root->left->ticker);
        if (result < 0)
        {
            // First recursively bring the ticker as root of left-left
            root->left->left = search(root->left->left, ticker);
 
            // Do first rotation for root, second rotation is done after else
            root = rightRotate(root);
        }
        else if (result > 0) // Zig-Zag (Left Right)
        {
            // First recursively bring the ticker as root of left-right
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
    else //ticker lies in right subtree
    {
        //ticker is not in tree, we are done
        if (root->right == NULL)
		{
		
			return root;
		} 

		result = strcasecmp(ticker, root->right->ticker);
        // Zag-Zig (Right Left)
        if (result < 0)
        {
            // Bring the ticker as root of right-left
            root->right->left = search(root->right->left, ticker);
 
            // Do first rotation for root->right
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        else if (result > 0)// Zag-Zag (Right Right)
        {
            // Bring the ticker as root of right-right and do first rotation
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
	//Nothing was strtok'd.
	if(token == NULL)
	{
		return flag = 0;	
	}
	//The first charcter is not a letter
	if(!isalpha(token[i]))
	{
		return flag = 0;
	}
	//The ticker meets the requirements. 
	return flag = 1;
}

//Error handling  for file i/o dollars
int input_cash(char *token)
{
	int flag;
	size_t index = 0;
	size_t value = 0;

	//Nothing was strtok'd
	if(token == NULL)
	{
		return flag = 0;
	}
	//No opening stock can not be negative
	if(token[0] == '-')
	{
		return flag = 0;
	}
	if(token[0] == '+')
	{
		//move starting point for isdigit check
		value = 1;
	}
	//length check
	if(strlen(token) > MAX_DIGITS+value)
	{
		return flag = 0;
	}
	//isdigit check
	for(index = value; index < strlen(token); index++)
	{
		if(!isdigit(token[index]))
		{
			return flag = 0;
		}
	}
	//all requirements were met
	return flag = 1;
}

//Error handling for file i/o cents 
int input_cents (char *token)
{
	int flag;
	size_t index = 0;
	//Nothing was strtok'd
	if(token == NULL)
	{
		return flag = 0;
	}
	//length check
	if(strlen(token) < MAX_CENTS)
	{
		return flag = 0;
	}
	//is digit check
	for(index = 0; index < strlen(token); index++)
	{
		if(!isdigit(token[index]))
		{
			return flag = 0;
		}
	}
	//Passes all requirements
	return flag = 1;
}

//Cuts off any cents beyond two digits
char *two_cents(char *token)
{
	if(strlen(token) > MAX_CENTS)
	{
		token[MAX_CENTS] = '\0';
	}
	
	return token;
}

//Error handling or user dollar i/o
int price_check(char *token)
{
	size_t index = 0;
	size_t value = 0;
	int flag = 0;
	//Nothing was strtok'd
	if(token == NULL)
	{
		return flag = 0;
	}
	//Check if negative 
	if(token[0] == '-')
	{
		if(strlen(token) > MAX_DIGITS+1)
		{
			return flag = 0;
		}	
		//Adjust isdigit start	
		value = 1;
	}

	else
	{
		//length check
		if(strlen(token) > MAX_DIGITS)
		{
			return flag = 0;
		}	
		value = 0;
	}
	//Isdigit check
	for(index = value; index <strlen(token); index++)
	{
		if(!isdigit(token[index])) 
		{
			return flag = 0;
		}
	}
	return flag = 1;
}

//Error handling for user cents i/o
int cent_check(char *token)
{
	size_t index = 0;
	int flag = 0;
	//Nothing was strtok'd
	if(token == NULL)
	{
		return flag = 0;
	}
	//length check
	if(strlen(token) < MAX_CENTS)
	{
		return flag = 0;
	}
	//isdigit check
	for(index = 0; index <strlen(token); index++)
	{
		if(!isdigit(token[index])) 
		{
			return flag = 0;
			
		}
	}
	//All requirements were met 
	return flag = 1;
}

void maff(stock *tree, int cost)
{
	//cost is negative
	if(cost < 0)
	{
		//limit to > 0.00
		if(tree->cost + cost < 01)
		{
			printf("You have subtracted too much, operation ignored\n");
			return;
		}
		tree->cost += cost;
	}
	//cost is positive
	else 
	{	
		//limit to 1,000,000
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
	if(root==NULL)
	{
		return;
	}

	if(root->left)
	{
		print_node(root->left);
	}
	//return price to decimal value
	float divide = (float)root->cost / 100;
	printf("%s %.2f %s\n", root->ticker, divide, root->company);
	if (root -> right)
	{
		print_node(root->right);
	}
	
}

//Free's the trees
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

//Used to build out new tree 
stock *new_tree(stock *tree, stock *second,
		 stock*(*Insert_num)(stock *, int, char *, char *)) 
	{
		if(tree == NULL)
		{
			return tree;
		}
		//Insert value of current tree into new tree 
		second = Insert_num(second, tree->cost, tree->company, tree->ticker);
		//move through the current tree
		new_tree(tree->left, second, Insert_num);
		new_tree(tree->right, second, Insert_num);
		return second;
	}



 

