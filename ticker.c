#include "stock.h"

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
	  free (buffer);
	  return EX_USAGE;
	}
    }

  //This will check to be sure more than one file was passed to the program.
  //If this is true, then the first file will be assigned to a file pointer.
  if (argc < 2)
    {
      printf ("Please pass a file\n");
      free (buffer);
      return EX_USAGE;
    }
  else
    {
      stockFile = fopen (argv[1], "r");

      if (!stockFile)
	{
	  free (buffer);
	  return EX_USAGE;
	}

    }

  //This temporary buffer is used to bring in words. The pointer is calloced
  //to the size of MAX_WORD_SIZE, an enumed value of 256, the maximum word
  //size the program will see.
  stock *tree = NULL;

  char buf[LINE_SIZE];

  char *token;			//strtok token
  char ticker[MAX_TICKER_LEN] = { 0 };
  int cost;			//dollars and total cost of stock.
  char *company = calloc (1, MAX_CO_SIZE + 1);
  int flag = 0;			//error flag
  int result = 0;		//strcasecmp result
  int cents = 0;		//stock price cents

  //file i/o
  while (fgets (buf, LINE_SIZE, stockFile))
    {
      token = strtok (buf, " \n\t");
      flag = ticker_check (token);	//Error handling for ticker
      if (flag == 0)
	{
	  token = NULL;		//Skip to next line of file
	}
      else
	{
	  strncpy (ticker, token, MAX_TICKER_LEN);	//copy token to ticker           
	}
      while (token != NULL)
	{
	  token = strtok (NULL, ".\t\n");
	  flag = input_cash (token);	//Error handling for dollars
	  if (flag == 0)
	    {
	      token = NULL;	//skip to next line of file
	      continue;
	    }

	  cost = strtol (token, NULL, 10);	//turn into int
	  cost *= 100;		//turn dollars to cents

	  token = strtok (NULL, " \n");
	  flag = input_cents (token);	//Error Handling for cents
	  if (flag == 0)
	    {
	      token = NULL;	//Skip to next line of file
	      continue;
	    }
	  token = two_cents (token);	//chop off cent fractions

	  cents = strtol (token, NULL, 10);	//turn into int


	  cost += cents;	//add dollars/100 and cents

	  token = strtok (NULL, "\n");
	  if (token == NULL)
	    {
	      memset (company, '\0', MAX_CO_SIZE);
	      token = NULL;
	    }
	  else
	    {
	      //Company can be called anything
	      strncpy (company, token, MAX_CO_SIZE);
	      token = NULL;
	    }

	}
      //No errors found
      if (flag == 1)
	{
	  tree = Insert (tree, cost, company, ticker);
	}
      //clear buffers for next line if file.
      memset (buf, '\0', LINE_SIZE);
      memset (company, '\0', MAX_CO_SIZE);
      memset (ticker, '\0', MAX_TICKER_LEN);

    }

  //User i/o
  while (fgets (buf, LINE_SIZE, stdin))
    {
      token = strtok (buf, " \n\t");
      flag = ticker_check (token);	//Error handlign for ticker
      if (flag == 0)
	{
	  token = NULL;
	}
      else
	{
	  strncpy (ticker, token, MAX_TICKER_LEN);

	  tree = search (tree, ticker);	//move ticker or it's parent to root

	  result = strcasecmp (tree->ticker, ticker);	//is ticker present   
	}
      while (token != NULL)
	{

	  token = strtok (NULL, ".\t\n");

	  flag = price_check (token);	//Error handling for dollars

	  if (flag == 0)
	    {
	      token = NULL;
	      continue;
	    }

	  cost = strtol (token, NULL, 10);

	  cost *= 100;

	  token = strtok (NULL, " \n");

	  flag = cent_check (token);	//Error handing for cents
	  if (flag == 0)
	    {
	      token = NULL;
	      continue;
	    }

	  token = two_cents (token);
	  cents = strtol (token, NULL, 10);

	  if (cost < 0)		//adding or subtracting cents
	    {
	      cost -= cents;
	    }
	  else
	    {
	      cost += cents;
	    }

	  token = strtok (NULL, "\n");

	  if (token == NULL)
	    {
	      memset (company, '\0', strlen (company));
	      token = NULL;
	    }
	  else
	    {
	      strncpy (company, token, MAX_CO_SIZE);
	      token = NULL;
	    }

	  token = NULL;
	}
      //No Errors found
      if (flag == 1)
	{
	  if (result == 0)
	    {
	      maff (tree, cost);	//Adjust stock price accordingly
	    }
	  else if (cost < 01)
	    {
	      printf ("New companies can not start below $00.01\n");
	    }
	  else if (cost > 100000000)
	    {
	      printf ("Max starting price is $1,000,000");
	    }
	  else
	    {
	      //Insert new ticker and starting price
	      tree = Insert (tree, cost, company, ticker);
	    }
	}

      memset (buf, '\0', strlen (buf));
      memset (company, '\0', MAX_CO_SIZE);
      memset (ticker, '\0', MAX_TICKER_LEN);
    }
  //Build new tree to print numerically
  stock *new = NULL;
  new = new_tree (tree, new, Insert_num);

  print_node (new);

  free (company);
  free (buffer);
  destroy_stocks (new);
  destroy_stocks (tree);
  fclose (stockFile);

}
