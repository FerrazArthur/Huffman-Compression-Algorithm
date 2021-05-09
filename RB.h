#include <stdio.h>
#include <stdlib.h>

enum Color {Red = 0, Black};
typedef struct Node
{
    void* info;
    enum Color color;
    struct Node* leftRB;
    struct Node* rightRB;
}Node;

//***********************************************Abstract methods:

unsigned char* getKey(Node* node);
/*
Input: Pointer to Node
Output: node->info or NULL
*/

int compareInfo(unsigned char* info1, unsigned char* info2);
/*
Expected input: two generic pointers
Expected output: 
    0 if they are the same;
    1 if info1 value is bigger than info2;
    -1 if info1 value is smaller than info2;
*/

void destroyInfo(unsigned char* info);
/*
Input: pointer to info
does free to info
output: none
*/

void printKey(Node* ptr);
/*
Input: pointer to Node
Output: print key of this node
*/
void printKey2(Node* ptr);
//***********************************************END

void destroyNodeRBTree(Node* node);
/*
input: pointer to Node
calls destroyInfo() to it's info and then free itself;
*/

void destroyRBTree(Node* node);
/*
input: pointer to root of Red Black binary tree
recursively calls destroyNodeRBTree() to every Node in the tree
*/

Node* createNodeRBTree(void* info);
/*
Input: generic type pointer to information
Output: pointer to new Node(painted red) that stores this information
*/

void changeColor(Node* node, enum Color newcolor);
/*
Input: Pointer to Node, new color
Output: void
*/

int isRed(Node* node);
/*
Input: Pointer to Node
Output: 
    0 if node is black or NULL
    1 if it is red 
*/

void printRBTree(Node* head, int level);
/*
Input: Pointer to Node root of RB Tree, zero(level must receive zero at first call)
Output: Formatted print of the RB Tree;
*/

Node* findSmallestNodeRBTree(Node* head);
/*
Input: Pointer to a Node in a RbTree
Output: Pointer to Node that has the smallest key
*/

Node* searchInfoRBTree(Node* head, unsigned char* info);
/*
Input: Pointer to root of Red-Black tree, information to be searched
Output: Pointer to Node that contains information equivalent to info or NULL otherwise
*/

int insertNodeRBTree(Node** head, Node* root, unsigned char* newInfo);
/*
Input: Pointer of pointer to Node(already in the tree),Pointer to Node that is Root of this tree, pointer to Node(to be inserted, must be red)
Output:
    1 if it has inserted
    0 if it hasnt
*/

int removeNodeRBTree(Node** head, Node* root, unsigned char* info);
/*
Input: Pointer of pointer to Node(already in the tree), information to be removed
Output:
    1 if it has been removed
    0 otherwise
*/
