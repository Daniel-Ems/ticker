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


enum {LINE_SIZE = 256};
enum {MAX_CO_SIZE = 64};
enum {MAX_TICKER_LEN = 6};
enum {MAX_DIGITS = 7};
enum {MAX_CENTS = 2};

//Code modified from BST code 
typedef struct stock{
    int cost;
	char *company;
	char ticker[6];

    struct stock *left;
    struct stock *right;
} stock;

stock *Insert(stock *root, int cost, char *company, char *ticker);
stock *rightRotate(stock *root);
stock *leftRotate(stock *root);
stock *search(stock *root, char *ticker);
stock *splay(stock *root, char *ticker);
int input_cash(char *token);
int input_cents (char *token);
int cent_check(char *token);
int price_check(char *token);
void maff(stock *tree, int cost);
void print_node(stock *root);
void destroy_stocks (stock * tree);
#endif
