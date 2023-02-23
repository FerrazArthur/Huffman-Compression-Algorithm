/*
Projeto: Árvore binária balanceada rubro-negra
Autor: Arthur Ferraz
Propriedades:
-Todos os nós da Árvore Rubro-Negra são vermelhos ou pretos
-A Raiz é sempre preta
-Todo nó folha é preto
-Se um nó é vermelho, então seus filhos são pretos(não existem nós vermelhos consecutivos)
-Para cada nó, todos os caminhos até as folhas possuem o mesmo número de nós pretos
*/
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
//These functions have to be written by the one who knows the type of info it's going to be used by the redBlackTree, as this a generic implementation
//***********************************************Métodos abstratos:
//Essas funções devem ser implementadas considerando o tipo de dado que será armazenado na arvore, pois essa é uma implementação genérica.
void* getKey(Node* node);
/*
Input: Pointer to Node ~ Ponteiro para Node
Output: Node->info or NULL
Behaviour: Return a pointer to the variable that is going to be the key parameter.
Comportamento: retorna um ponteiro para a variável que será o parâmetro chave(key) das comparações.
*/

int compareInfo(void* info1, void* info2);
/*
Input: Two generic pointers ~ Dois ponteiros genericos
Output: Int
Behaviour: User define a method to compare the keys, given by getKey method, and the output should be as follows
    0 if they have the same weight;
    1 if info1 weight is bigger than info2;
    -1 if info1 weight is smaller than info2.
Comportamento: Compara as duas chaves retornadas pelo método getKey e o output deve ser o seguinte:
    0 se as chaves possuírem o mesmo valor;
    1 se info1 tiver valor maior que info2;
    -1 se info1 tiver um valor menor que info2.
*/

void destroyInfo(void* info);
/*
Input: Pointer to info that the user has defined ~ Ponteiro para o tipo info 
Output: Void
Behaviour: Releases memory previous allocatted.
Comportamento: Libera memória alocada para info, caso tenha sido alocada.
*/

void printKey(Node* ptr);
/*
Input: Pointer to Node ~ Ponteiro para tipo Node
Output: Void
Behaviour: Print the key in node
Comportamento: Imprime o valor chave no nó
*/

//***********************************************END
//As funções abaixo ja estão implementadas no arquivo RB.c

void destroyNodeRBTree(Node* node);
/*
Input: Pointer to Node
Output: Void
Behaviour: Calls destroyInfo() then unallocate Node;
*/

void destroyRBTree(Node* node);
/*
Input: Pointer to root of Red Black binary tree
Output: Void
Behaviour: Recursively calls destroyNodeRBTree() to every Node in the tree
*/

Node* createNodeRBTree(void* info);
/*
Input: Generic type pointer to information
Output: Pointer to new Node(painted red) that stores this information
Behaviur: Allocate memory for new Node and store info in it
*/

void changeColor(Node* node, enum Color newcolor);
/*
Input: Pointer to Node, new color
Output: Void
Behaviour: Change the color in Node to newcolor
*/

int isRed(Node* node);
/*
Input: Pointer to Node
Output: Verify color of Node and return
    0 if node is black or NULL
    1 if it is red 
*/

void printRBTree(Node* head,void (*printKey) (Node*), int level);
/*
Input: Pointer to Node root of RB Tree, pointer to function that print info key, zero(level must receive zero at first call)
Output: Void.
Behaviour: Recursivelly print the formatted RB Tree;
*/

Node* findSmallestNodeRBTree(Node* head);
/*
Input: Pointer to a Node in a RbTree
Output: Pointer to Node that has the smallest key
Behaviour: Searches the current brach of tree for the smallest element
*/

Node* searchInfoRBTree(Node* head, void* key);
/*
Input: Pointer to root of Red-Black tree, pointer to information to be searched
Output: Pointer to Node that contains information equivalent to info or NULL otherwise
Behaviour: Searches the tree for the element that holds the same weight than key's
*/

int insertNodeRBTree(Node** head, Node* root, Node* newNode);
/*
Input: Pointer of pointer to Node(already in the tree),Pointer to Node that is Root of this tree, pointer to Node(to be inserted, must be red)
Output: Int
Behaviour: recursivelly searches the tree until find a NULL node, this case it's inserted, or find a node with the same weight. returns
    1 or 6 if it has inserted new Node
    0 if it has failed for unknow error
    2 if it has failed because newNode weight is already in tree
*/

int removeNodeRBTree(Node** head, Node* root, void* info);
/*
Input: Pointer of pointer to Node(already in the tree), information to be removed
Output: Int
Behaviour: Recursivelly searches the tree until find the node with the same weight than info or find NULL. returns
    1 if it find info weigth and it has been removed
    0 otherwise
*/
