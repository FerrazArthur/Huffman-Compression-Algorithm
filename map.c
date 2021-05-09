#include<string.h>
#include<limits.h>
#include<math.h>
#include"RB.c"

/*
this code receive a file as input, read all it's characters and store them in a Red Black Tree, counting all it's elements
*/

typedef struct code
{
    unsigned char* character;
    int count;
    unsigned char* codeName;
    int sizeCodeName;
    struct code* left;
    struct code* right;
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

void* createCode(unsigned char* aux)
{
    Code* ptr = (Code*) malloc(sizeof(Code));
    if(ptr != NULL)
    {
        ptr->character = aux;
        ptr->count = 1;
        ptr->codeName = NULL;
        ptr->sizeCodeName = 0;
    }
    return ptr;
}

void printKey(Node* node)
{
    if(node != NULL)
        printf("%c", *getCode(node)->character);
}

int compareInfo(unsigned char* info1, unsigned char* info2)
{
   if (*info1 == *info2)
       return 0; 
   else if (*info1 > *info2)
       return 1;
   return -1;
}

unsigned char* getKey(Node* node)
{
    if(node != NULL)
        return getCode(node)->character;
}

void destroyInfo(unsigned char* info)
{
    free(info);
}

//

typedef struct queue
{
    Code* info;
    struct queue* next;
}Queue;

Queue* createQueue(Code* info)
{
    Queue* ptr = (Queue*) malloc(sizeof(Queue));
    if(ptr != NULL)
    {
        ptr->info = info;
        ptr->next = NULL;
    }
    return ptr;
}

void getQueue(Node* RB, Queue** queue)
{//insert RB content to queue in order
    Queue* ptr = NULL;
    if(RB != NULL)
    {
        if(RB->leftRB != NULL)
            getQueue(RB->leftRB, queue);
        ptr = createQueue(RB->info);
        ptr->next = *queue;
        *queue = ptr;
        if(RB->rightRB != NULL)
            getQueue(RB->rightRB, queue);
    }
}

Queue* insertionSort(Node* RB)
{
    Queue* queue = NULL;
    Queue* output = NULL;
    Queue* minPtr = NULL;
    Queue* pPtr = NULL;
    Queue* ptr = NULL;
    getQueue(RB, &queue);
    if(queue == NULL)
        return NULL;
    int count = 0;
    //counting how many distint characters there are
    ptr = queue;
    while(ptr != NULL)
    {
        count++;
        ptr = ptr->next;
    }
    //popping elements from queue into output until they are in decrescent order
    for(int i = 0; i < count; i++)
    {
        minPtr = ptr = queue;
        pPtr = NULL;
        while(ptr != NULL && ptr->next != NULL)
        {
            if(ptr->next->info->count < minPtr->info->count)
            {
                minPtr = ptr->next;
                pPtr = ptr;//keep track of previous ptr so we can remove minPtr from queue
            }
            ptr = ptr->next;
        }
        if(pPtr != NULL)//smallest isnt the first
            pPtr->next = minPtr->next;//isolate minPtr
        else//smallest is the first
            queue = queue->next;
        minPtr->next = output;//insert minPtr as first element in output
        output = minPtr;//update output's head
    }
    ptr = output;
    /*while(ptr != NULL)
    {
        printf("%c = %d\n", *ptr->info->character, ptr->info->count);
        ptr = ptr->next;
    }*/
    return output;
}

Code* createHuffmanTree(Queue* minPriority)
{
    Queue* ptr1;
    Queue* ptr2;
    Queue* pPtr;
    Queue* ptr;
    Code* sum = NULL;
    while(minPriority != NULL)
    {
        //take the 2 smallest
        ptr1 = minPriority;
        ptr2 = ptr1->next;
        if(ptr2 == NULL)//base case
            break;
        minPriority = ptr2->next;
        sum = createCode(NULL);
        if(sum == NULL)
            return NULL;
        //create a sum Code with the 2 smallest
        sum->count = ptr1->info->count + ptr2->info->count;
        if(ptr1->info->count > ptr2->info->count)
        {
            sum->left = ptr1->info;
            sum->right = ptr2->info;
        }
        else
        {
            sum->left = ptr2->info;
            sum->right = ptr1->info;
        }
        //remove one of them, the other will return to the minPriority queue as a sum of both
        free(ptr1);
        ptr2->info = sum;
        //insert the sum in the minPriority queue again(keeping the order intact)
        ptr = minPriority;
        pPtr = NULL;
        while(ptr != NULL)
        {
            if(ptr->info->count > ptr2->info->count)
                break;
            pPtr = ptr;
            ptr = ptr->next;
        }
        if(pPtr == NULL)//place sum as first
        {
            ptr2->next = minPriority;
            minPriority = ptr2;
        }
        else//place sum between pPtr and ptr
        {
            pPtr->next = ptr2;
            ptr2->next = ptr;
        }
    }
    //remove ptr1, now that it has no more use
    free(ptr1);
    return sum;
}

void createCodeNames(Code* huff, unsigned char* codeName, int level)
{//what should happen when we have a text file with only one distinct character? meanwhile it does nothing
    if(huff != NULL)
    {
        if(huff->character != NULL && level != 0)//is not an internal node and is not the case where the file has only one distinct character
        {
            printf("%c\n", *huff->character);
            huff->codeName = codeName;
            huff->sizeCodeName = level;
        }
        else
        {
            codeName = (unsigned char*) realloc(codeName, sizeof(char) * (level + 1));
            codeName[level] = '0';
            for(int i = 0; i < level + 1; i++)
                printf("%c", codeName[i]);
            printf("\n");
            createCodeNames(huff->left, codeName, level + 1);
            unsigned char* codeName2 = (unsigned char*) malloc(sizeof(char) * (level + 1));
            memcpy(codeName2, codeName, sizeof(char) * (level + 1));
            codeName2[level] = '1';
            for(int i = 0; i < level + 1; i++)
                printf("%c", codeName2[i]);
            printf("\n");
            createCodeNames(huff->right, codeName2, level + 1);
        }
    }
    else
        free(codeName);
}


//

int createMap(char* fileName, Node** map)
{
    int count = 0;
    FILE *fptr;
    Node* temp;
    fptr = fopen(fileName, "r");
    unsigned char* aux;
    if(fptr != NULL)
    {
        while(1)
        {
            aux = (unsigned char*) malloc(sizeof(unsigned char));
            if(fscanf(fptr, "%c", aux) == EOF)
            {
                destroyInfo(aux);
                break;
            }
            count++;
            if(insertNodeRBTree(map, *map, aux) == 0)//aux is already in tree
                destroyInfo(aux);
        };
        fclose(fptr);
    }
    return count;
}

void printHuff(Code* huff, int level)
{
    if(huff != NULL)
    {
        if(huff->right != NULL)
            printHuff(huff->right, level+1);
        for(int i = 0; i < level; i++)
            printf("\t");
        if(huff->character == NULL)
            printf("+: %d\n", huff->count);
        else
        {
        if(*huff->character != '\n')
            printf("%c: %d code:", *huff->character, huff->count);
        else
            printf("enter: %d code:", huff->count);
            for(int i = 0; i < huff->sizeCodeName; i++)
                printf("%c", huff->codeName[i]);
            printf("\n");
        }
        if(huff->left != NULL)
            printHuff(huff->left, level+1);
    }
}

int main()
{
    char fileName[50];
    Node* map = NULL;
    Queue* queue = NULL;
    Code* huff = NULL;
    printf("Insira o nome do arquivo a ser comprimido:\n");
    scanf("%s", fileName);
    int count = createMap(fileName, &map);//define all characters in the fileName file and count it's ocurrences
    if(map != NULL)
    {
        printRBTree(map, 0);//print the redBlack Tree that holds the characters
        queue = insertionSort(map);//Get a minimum priority queue from the redBlackTree
        huff = createHuffmanTree(queue);//get the huffman tree of those characters
        createCodeNames(huff, NULL, 0);
        printHuff(huff, 0);
    }
    else
        printf("Arquivo não encontrado\n");
    return 0;
}
