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
    return splay(root, ticker);
}

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

void maff(stock *tree, int cost)
{
	if(cost < 0)
	{
		if(tree->cost + cost < 01)
		{
			printf("You have subtracted too much, operation ignored\n");
			return;
		}
		tree->cost = cost;
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
