#include<string.h>
#include"RB.c"

/*
this code receive a file as input, read all it's characters and store them in a Red Black Tree, counting all it's elements
*/

typedef struct codigo
{
    char caracter;
    int count;
}Code;

Code* getCode(Node* node)
{
    if(node != NULL)
        return (Code*)node->info;
    else
        return NULL;
}

void incrementCode(Node* node)
{
    if(node != NULL)
        getCode(node)->count++;
}

Code* createCode(char aux)
{
    Code* ptr = (Code*) malloc(sizeof(Code));
    if(ptr != NULL)
    {
        ptr->caracter = aux;
        ptr->count = 1;
    }
    return ptr;
}

void printKey(Node* node)
{
    if(node != NULL)
        printf("%c", getCode(node)->caracter);
}

int compareInfo(char info1, char info2)
{
   if (info1 == info2)
       return 0; 
   else if (info1 > info2)
       return 1;
   return -1;
}

char getKey(Node* node)
{
    if(node != NULL)
        return getCode(node)->caracter;
}

void destroyInfo(char info)
{
}

Node* createMap(char* fileName)
{
    FILE *fptr;
    Node* map= NULL;
    Node* temp;
    char aux = ' ';
    fptr = fopen(fileName, "r");
    if(fptr != NULL)
    {
        while((aux = getc(fptr)) != EOF)
        {
            temp = createNodeRBTree(createCode(aux));
            if((insertNodeRBTree(&map, map, temp)) == 0)//aux is already in tree
                destroyNodeRBTree(temp);
        };
        fclose(fptr);
    }
    return map;
}

int main()
{
    char fileName[50];
    Node* map;
    printf("Insira o nome do arquivo a ser comprimido:\n");
    scanf("%s", fileName);
    map = createMap(fileName);
    if(map != NULL)
        printf("%c, %d\n", getCode(map)->caracter, getCode(map)->count);//só pra testar se esta somando
    else
        printf("Arquivo não encontrado\n");
    return 0;
}
