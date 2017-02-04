#ifndef STOCK_H

#define STOCK_H

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


enum {LINE_SIZE = 256};//Max line I will allow
enum {MAX_CO_SIZE = 64};//Max company name
enum {MAX_TICKER_LEN = 6};//Max ticker length
enum {MAX_DIGITS = 7};//Max number of digits 
enum {MAX_CENTS = 2};//Max number of cents

/*******************************************************************************
typedef struct stock defines the charachteristics of stock nodes; 

int cost, 
char *company, 
char *ticker
and two stock pointers.
*******************************************************************************/
typedef struct stock{
    int cost;
	char *company;
	char ticker[6];
    struct stock *left;
    struct stock *right;
} stock;

/*******************************************************************************
stock *Insert(stock *root, int cost, char *company, char *ticker)

->stock *tree points to the root of the tree. 
->int cost is the stock cost being inserted.
->char *company is the company being inserted.
->char *ticker is the ticker being inserted.

char *ticker is compared to tree->word using strcasecmp. if the string is < 0,
the new node is created to the left of tree. If the string is > 0,
the new node is created to the right of tree. If the string is == 0 no node is 
created. If tree==NULL, a new node is created.

CITE: The function Insert was modified from the BSTnode exercise given during
Datastructres and Algorithims I.
*******************************************************************************/
stock *Insert(stock *root, int cost, char *company, char *ticker);

/*******************************************************************************
stock *Insert_num(stock *root, int cost, char *company, char *ticker)

->stock *root is the new tree's root
->int cost is the old tree cost
->char *company is the old tree's company
->char *ticker is the old trees ticker

This function will create a BST by comparing costs. 
*******************************************************************************/
stock *Insert_num(stock *root, int cost, char *company, char *ticker);

/*******************************************************************************
stock *rightRotate(stock *root)

->stock *root is the root node. 

rightRotate perfoms a single rotation. The root node assigns it self as the left
child of it's right childs node.  The right child then assigns it's self as 
root. This rotates the tree once to the right, raising the new root one level. 

CITE: rightRootate was taken and modified from
http://www.geeksforgeeks.org/splay-tree-set-1-insert/
*******************************************************************************/
stock *rightRotate(stock *root);

/*******************************************************************************
stock *leftRotate(stock *root)

->stock *root is the root node. 

leftRotate perfoms a single rotation. The root node assigns it self as the right
child of it's left childs node.  The left child then assigns it's self as 
root. This rotates the tree once to the left, raising the new root one level.

CITE: leftRotate was taken and modified from
http://www.geeksforgeeks.org/splay-tree-set-1-insert/
*******************************************************************************/
stock *leftRotate(stock *root);

/*******************************************************************************
stock *search(stock *root, char *ticker)

->stock *root is the root node of the tree being searched.
->char *ticker is the ticker being searced. 

the search function will look for the ticker and then splay the tree 
appropriately. The tree will compare the ticker's and the move recursively 
through out the tree until a match is found or null. Once the search has found
one of these two cases the function will either splay the matched node, or the 
would be parent of the ticker being searched. 

When splaying, the function will call right or left rotate to move the node 
being moved to the root of the tree. 

CITE: search was taken and modified from
http://www.geeksforgeeks.org/splay-tree-set-1-insert/
*******************************************************************************/
stock *search(stock *root, char *ticker);

/*******************************************************************************
int ticker_check(char *token)

->char *token is the most recent strtok token

ticker_check is used to error check the first character of the ticker is a 
letter. If the token meets all requirements, a 1 is returned. Else, a 0 is 
returned. 

Error Checking 
->token != NULL
->isalpha(token[0])
*******************************************************************************/
int ticker_check(char *token);

/*******************************************************************************
int input_cash(char *token)

->char *token is most recent strtok token.

input_cash is used to error check file input in regards to the dollar field of 
the stock price. If the token meets all requirements, a 1 is returned. Else, a 
0 is returned.

Error Checking
->token != NULL
->strlen(token) !> MAX_DIGITS
->isdigit(token[?})
->token > 0
*******************************************************************************/
int input_cash(char *token);

/*******************************************************************************
int input_cents(char *token)

->char *token is most recent strtok token.

input_cents is used to error check file input in regards to the cents field of 
the stock price. If the token meets all requirements, a 1 is returned. Else, a 
0 is returned.

Error Checking
->token != NULL
->strlen(token) !> MAX_DIGITS
->isdigit(token[?})
*******************************************************************************/
int input_cents (char *token);

/*******************************************************************************
char *two_check(char *token)

->char *token is most recent strtok token.

cent_check is used to remove fractions of user input for the cents field. If 
fractions are present, they are replace with a null byte. 

Error Checking 
->strlen(token) == MAX_DIGITS
*******************************************************************************/
char *two_cents(char *token);

/*******************************************************************************
int cent_check(char *token)

->char *token is most recent strtok token.

centt_check is used to error check user input in regards to the cents field of 
the stock price. If the token meets all requirements, a 1 is returned. Else, a 
0 is returned.


Error Checking 
->token != NULL;
->strlen(token) == MAX_CENTS
->isdidgit(token[?])
*******************************************************************************/
int cent_check(char *token);

/*******************************************************************************
int price_check(char *token)

->char *token is most recent strtok token.

price_check is used to error check user input in regards to the dollar field of 
the stock price. If the token meets all requirements, a 1 is returned. Else, a 
0 is returned.

Error Checking
->token != NULL
->strlen(token) !> MAX_DIGITS
->isdigit(token[?})
->if token is negative, adjust error handling accordingly. 
*******************************************************************************/
int price_check(char *token);

/*******************************************************************************
void maff(stock *tree, int cost)

->stock *tree points to the root of the tree.
->int cost is the value of the stock being entered.

The function maff will check the cost for certain parameters to determine how 
the value should update the curret stock. if the value is negative, the value is 
subtracted from the current value. if the value is positive, it is added. 

Error Checking
->tree->cost >= 0.01
->tree->cost <= 1,000,000 
*******************************************************************************/
void maff(stock *tree, int cost);

/******************************************************************************
void print_node(stock *tree) 

->The stock *tree, points to the node of the tree to be printed. 

The function print_node recursively moves left, until it hits null, and then it 
will print the stock. The function will then recursively move right and repeat
the print process.

CITE: The function print_node was modified from the BSTnode exercise handed out 
during Datastructures and Algorithims I.
*******************************************************************************/
void print_node(stock *root);

/*******************************************************************************
size_t tree_size(stock *root)

->stock *tree points to the root of the tree.

tree_size will recursively call it's incrementing a counter with each node it
finds. The function the returns the counter to determine how many nodes are in 
the tree.

CITE: The function tree_size was modified from the BSTnode exercise handed out 
during Datastructures and Algorithims I.
*******************************************************************************/
size_t tree_size(stock *root);

/*******************************************************************************
void destroy_stocks(stock *tree)

->stock *tree points to the node of the tree. 

the function call destroy_stocks until a null is found, and then
will recursvely free tree->company, and tree. 

CITE: The function destroy_stock was modified from the BSTnode exercise 
handed out during Datastructures and Algorithims I.
*******************************************************************************/
void destroy_stocks (stock * tree);

/*******************************************************************************
stock * new_tree(stock *tree, stock *second, 
		 stock*(*Insert_num)(stock *, int, char *,char *))

->stock *tree is root to the old tree
->stock *second is the root to the new tree
->Insert_num is a function pointer to Insert_num

The function passes the tree already built to the new insert function along with
the new node, creates the node, and then traverses the old tree building out 
the new tree one by one.

CITE: The development of this prototype was assisted by John Haulbrich. 
*******************************************************************************/
stock * new_tree(stock *tree, stock *second, 
		 stock*(*Insert_num)(stock *, int, char *,char *));


#endif
