
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
		strncpy(ticker, token, MAX_TICKER_LEN);
		
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

		if(token != NULL)
		{
			result = strcasecmp(tree->ticker, ticker);
		}

		while(token != NULL)
		{
			token = strtok(NULL, ".\t\n");
		
			cost = strtol(token, NULL, 10);
			
			cost *= 100;
			
			token = strtok(NULL, " \n");
			cost -= strtol(token, NULL, 10);
			
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
				
 	


