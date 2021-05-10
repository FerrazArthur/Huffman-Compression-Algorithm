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
{//insert RB content to queue in order using character as key
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

Queue* insertionSortCrescent(Node* RB)
{
    Queue* queue = NULL;
    Queue* output = NULL;
    Queue* maxPtr = NULL;
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
    //printf("count = %d\n", count);
    //popping elements from queue into output until they are in crescent order
    for(int i = 0; i < count; i++)
    {
        maxPtr = ptr = queue;
        pPtr = NULL;
        while(ptr != NULL && ptr->next != NULL)
        {
            if(ptr->next->info->count > maxPtr->info->count)
            {
                maxPtr = ptr->next;
                pPtr = ptr;//keep track of previous ptr so we can remove maxPtr from queue
            }
            ptr = ptr->next;
        }
        if(pPtr != NULL)//smallest isnt the first
            pPtr->next = maxPtr->next;//isolate maxPtr
        else//smallest is the first
            queue = queue->next;
        maxPtr->next = output;//insert maxPtr as first element in output
        output = maxPtr;//update output's head
    }
    /*ptr = output;
    while(ptr != NULL)
    {
        printf("%c = %d\n", *ptr->info->character, ptr->info->count);
        ptr = ptr->next;
    }*/
    return output;
}

Queue* insertionSortDecrescent(Node* RB)
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
    //printf("count = %d\n", count);
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
        if(pPtr != NULL)//biggest isnt the first
            pPtr->next = minPtr->next;//isolate minPtr
        else//biggest is the first
            queue = queue->next;
        minPtr->next = output;//insert minPtr as first element in output
        output = minPtr;//update output's head
    }
    ptr = output;
    while(ptr != NULL)
    {
        printf("%c = %d\n", *ptr->info->character, ptr->info->count);
        ptr = ptr->next;
    }
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
            //printf("%c\n", *huff->character);
            huff->codeName = codeName;
            huff->sizeCodeName = level;
        }
        else
        {
            codeName = (unsigned char*) realloc(codeName, sizeof(char) * (level + 1));
            codeName[level] = '0';
            /*for(int i = 0; i < level + 1; i++)
                printf("%c", codeName[i]);
            printf("\n");*/
            createCodeNames(huff->left, codeName, level + 1);
            unsigned char* codeName2 = (unsigned char*) malloc(sizeof(char) * (level + 1));
            memcpy(codeName2, codeName, sizeof(char) * (level + 1));
            codeName2[level] = '1';
            /*for(int i = 0; i < level + 1; i++)
                printf("%c", codeName2[i]);
            printf("\n");*/
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
            aux = (unsigned char*) malloc(sizeof(char));
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

Code* getElement(unsigned char foo, Queue* table)
{
    if(table != NULL)
    {
        Queue* ptr = table;
        while(ptr != NULL)
        {
            if(*ptr->info->character == foo)
                return ptr->info;
            ptr = ptr->next;
        }
    }
    return NULL;
}

long long int getCompressedSize(Queue* table)
{
    Queue* ptr = table;
    long long int count = 0;
    while(ptr != NULL)
    {
        count += ptr->info->count * ptr->info->sizeCodeName;//multiply frequency by length of codename
        ptr = ptr->next;//do it to all elements in queue(all elements in the input file)
    }
    return ceil(count/8);//the minimum amount of bytes needed to store the compressed file
}

void compress(const char* input, const char* output, Queue* table)
{
    FILE* fPtr = fopen(input, "r");
    Code* element = NULL;
    unsigned char* compressed = NULL;
    unsigned char aux = ' ';
    long long int size = 0;
    long long int byte = 0;
    int pos = 0;
    if(fPtr != NULL)
    {
        size = getCompressedSize(table);
        printf("size = %lld\n", size);
        compressed = (unsigned char*) calloc(size, sizeof(char));//alocate memory to store the compressed string
        while(fscanf(fPtr, "%c", &aux) != EOF)//take a character from input
        {
            element = getElement(aux, table);// take the codeName of the current character
            for(int i = 0; i < element->sizeCodeName; i++)//write codeName into compressed
            {
                aux = element->codeName[i];//get one bit
                aux << 7-pos++;//move it to position
                compressed[byte] = compressed[byte] | aux;//add to compress
                if(pos == 8)
                {
                    pos = 0;
                    byte++;
                }
            }
        }
        fclose(fPtr);
        fPtr = fopen(output, "wb");
        fwrite(compressed, sizeof(char), size, fPtr);//write compress into output file
        fclose(fPtr);
    }
}

int main()
{
    char fileInputName[50];
    char fileOutputName[50];
    Node* map = NULL;
    Queue* queue = NULL;
    Code* huff = NULL;
    printf("Insira o nome do arquivo a ser comprimido:\n");
    scanf("%s", fileInputName);
    int count = createMap(fileInputName, &map);//define all characters in the fileName file and count it's ocurrences
    if(map != NULL)
    {
        //printRBTree(map, 0);//print the redBlack Tree that holds the characters
        queue = insertionSortCrescent(map);//Get a minimum priority queue from the redBlackTree
        huff = createHuffmanTree(queue);//get the huffman tree of those characters
        createCodeNames(huff, NULL, 0);//calculate the codeName of the characters
        printHuff(huff, 0);
        queue = insertionSortDecrescent(map);//creating table of contents in decrescent order
        //create the compact file 
        printf("Insira o nome do arquivo que deseja criar:\n");
        scanf("%s", fileOutputName);
        compress(fileInputName, fileOutputName, queue);
    }
    else
        printf("Arquivo não encontrado\n");
    return 0;
}
