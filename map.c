#include"RB.h"
#include<string.h>
#include<limits.h>
#include<math.h>
#define BITSINBYTE 8

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

void destroyCode(Code* ptr)
{
    if(ptr != NULL)
    {
        free(ptr->character);
        free(ptr->codeName);
        destroyCode(ptr->left);
        destroyCode(ptr->right);
        free(ptr);
    }
}

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
        ptr->left = NULL;
        ptr->right = NULL;
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
    return NULL;
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

void destroyQueue(Queue* ptr)
{
    if(ptr != NULL)
    {
        destroyCode(ptr->info);
        destroyQueue(ptr->next);
        free(ptr);
    }
}

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

int crescent(int value1, int value2)
{
    if(value1 > value2)
        return 1;
    return 0;
}

int decrescent(int value1, int value2)
{
    if(value1 < value2)
        return 1;
    return 0;
}

Queue* insertionSort(Queue* queue, int count, int (*op) (int, int))
{
    Queue* output = NULL;
    Queue* keyPtr = NULL;
    Queue* pPtr = NULL;
    Queue* ptr = NULL;
    if(queue == NULL)
        return NULL;
    //counting how many distint characters there are
    ptr = queue;
    //popping elements from queue into output until they are in crescent order
    for(int i = 0; i < count; i++)
    {
        keyPtr = ptr = queue;
        pPtr = NULL;
        while(ptr != NULL && ptr->next != NULL)
        {
            if((*op) (ptr->next->info->count, keyPtr->info->count))//comparation 
            {
                keyPtr = ptr->next;
                pPtr = ptr;//keep track of previous ptr so we can remove keyPtr from queue
            }
            ptr = ptr->next;
        }
        if(pPtr != NULL)//smallest isnt the first
            pPtr->next = keyPtr->next;//isolate keyPtr
        else//smallest is the first
            queue = queue->next;
        keyPtr->next = output;//insert keyPtr as first element in output
        output = keyPtr;//update output's head
    }
    /*ptr = output;
    while(ptr != NULL)
    {
        printf("%c = %d\n", *ptr->info->character, ptr->info->count);
        ptr = ptr->next;
    }*/
    return output;
}

Queue* copyQueue(Queue* original)
{
    Queue* copy = NULL;
    Queue* cPtr = NULL;
    Queue* ptr = original;
    while(ptr != NULL)
    {
        if(copy == NULL)
        {
            copy = createQueue(ptr->info);
            cPtr = copy;
        }
        else
        {
            cPtr->next = createQueue(ptr->info);
            cPtr = cPtr->next;
        }
        ptr = ptr->next;
    }
    return copy;
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
            codeName[level] = 0;
            /*for(int i = 0; i < level + 1; i++)
                printf("%c", codeName[i]);
            printf("\n");*/
            createCodeNames(huff->left, codeName, level + 1);
            unsigned char* codeName2 = (unsigned char*) malloc(sizeof(char) * (level + 1));
            memcpy(codeName2, codeName, sizeof(char) * (level + 1));
            codeName2[level] = 1;
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
            printf("%d", huff->codeName[i]);
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
    return count;
}

void compress(const char* input, const char* output, Queue* table, int length)
{
    Code* element = NULL;
    Code* holdInfo = NULL;
    Queue* holdNext = NULL;
    Queue* holdCurrent = NULL;
    unsigned char* holdCharacter = NULL;
    unsigned char* holdCodeName = NULL;
    unsigned char* compressed = NULL;
    unsigned char aux = ' ';
    long long int sizeBits = 0;
    long long int size = 0;
    long long int byte = 0;
    int pos = 0;
    FILE* fPtr = fopen(input, "r");
    if(fPtr != NULL)
    {
        sizeBits = getCompressedSize(table);
        size = ceil((float) sizeBits / BITSINBYTE);
        compressed = (unsigned char*) calloc(size, sizeof(char));//alocate memory to store the compressed string
        while(fscanf(fPtr, "%c", &aux) != EOF)//take a character from input
        {
            element = getElement(aux, table);// take the codeName of the current character
            for(int i = 0; i < element->sizeCodeName; i++)//write codeName into compressed
            {
                aux = element->codeName[i];//get a byte
                aux = aux << (7-pos);//move it's significant bit to position
                compressed[byte] = compressed[byte] | aux;//add to compress
                if(++pos == BITSINBYTE)
                {
                    pos = 0;
                    byte++;
                }
            }
        }
        fclose(fPtr);
        fPtr = fopen(output, "wb");
        fwrite(&sizeBits, sizeof(long long int), 1, fPtr);//write sizeBits
        fwrite(&length, sizeof(int), 1, fPtr);
        table = insertionSort(table, length, crescent);
        holdCurrent = table;
        while(holdCurrent != NULL)
        {
            holdNext = holdCurrent->next;
            holdInfo = holdCurrent->info;
            holdCharacter = holdInfo->character;
            holdCodeName = holdInfo->codeName;
            holdCurrent->next = NULL;
            holdCurrent->info->codeName = NULL;
            holdCurrent->info->character = NULL;
            holdCurrent->info = NULL;
            fwrite(holdCurrent, sizeof(Queue), 1, fPtr);
            fwrite(holdInfo, sizeof(Code), 1, fPtr);
            fwrite(holdCharacter, sizeof(char), holdInfo->count, fPtr);
            fwrite(holdCodeName, sizeof(char), holdInfo->sizeCodeName, fPtr);
            destroyQueue(holdCurrent);
            holdCurrent = holdNext;
        }
        fwrite(compressed, sizeof(char), size, fPtr);//write compress into output file
        fclose(fPtr);
    }
}

void decompress(const char* input, const char* output)
{
    FILE* fPtr = fopen(input, "rb");
    Code* element = NULL;
    Code* huff = NULL;
    Queue* holdPrevious = NULL;
    Queue* holdCurrent = NULL;
    Queue* table = NULL;
    unsigned char* compressed = NULL;
    unsigned char aux = ' ';
    long long int count = 0;
    long long int size = 0;
    long long int sizeBits = 0;
    long long int byte = 0;
    int pos = 0;
    //retrieve compressed file into a compressed string
    if(fPtr != NULL)
    {
        fread(&sizeBits, sizeof(long long int), 1, fPtr);
        size = ceil((double)sizeBits/BITSINBYTE);
        fread(&pos, sizeof(int), 1, fPtr);
        /*
        holdNext = table;
        while(holdNext != NULL)
        {
            fwrite(holdCurrent, sizeof(Queue), 1, fPtr);
            fwrite(holdCurrent->info, sizeof(Code), 1, fPtr);
            fWrite(holdCurrent->info->character, sizeof(char), holdCurrent->info->count, fPtr);
            fWrite(holdCurrent->info->codeName, sizeof(char), holdCurrent->info->sizeCodeName, fPtr);
        }
        */
        for(int i = 0; i < pos; i++)
        {
            holdCurrent = (Queue*) malloc(sizeof(Queue));
            fread(holdCurrent, sizeof(Queue), 1, fPtr);
            holdCurrent->info = (Code*) malloc(sizeof(Code));
            fread(holdCurrent->info, sizeof(Code), 1, fPtr);
            holdCurrent->info->character = (unsigned char*) calloc(sizeof(char), holdCurrent->info->count);
            fread(holdCurrent->info->character, sizeof(char), holdCurrent->info->count, fPtr);
            holdCurrent->info->codeName = (unsigned char*) calloc(sizeof(char), holdCurrent->info->sizeCodeName);
            fread(holdCurrent->info->codeName, sizeof(char), holdCurrent->info->sizeCodeName, fPtr);
            if(table == NULL)
                table = holdCurrent;
            if(holdPrevious != NULL)
                holdPrevious->next = holdCurrent;
            holdPrevious = holdCurrent;
        }
        if(table == NULL)
            return;
        compressed = (unsigned char*) calloc(size, sizeof(char));
        fread(compressed, sizeof(char), size, fPtr);
        fclose(fPtr);
    }
    else
    {
        printf("Arquivo não encontrado\n");
        return;
    }
    huff = createHuffmanTree(copyQueue(table));
    if(huff == NULL)
        return;
    byte = 0;
    pos = 0;
    fPtr = fopen(output, "w");
    //transform compressed string of codenames into deCompressed string, so we can acess it's individual bits
    if(fPtr != NULL)
    {
        while(1)//runs throught the compressed string
        {
            element = huff;
            while(element->character == NULL)// while element is a internal node
            {
                //take pos bit in compressed[byte]
                aux = compressed[byte];
                //update compressed
                compressed[byte] = compressed[byte] << 1;
                aux = aux >> 7;
                count++;
                //evaluate
                if (aux == 0)
                    element = element->left;
                else 
                    element = element->right;
                //external loop control
                if(++pos == BITSINBYTE)
                {
                    pos = 0;
                    byte++;
                }
            }
            fprintf(fPtr, "%c", *element->character);
            if(count == sizeBits)
                break;
        }
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
        getQueue(map, &queue);
        queue = insertionSort(queue, count, crescent);//Get a minimum priority queue from the redBlackTree elements
        huff = createHuffmanTree(copyQueue(queue));//get the huffman tree of those characters
        createCodeNames(huff, NULL, 0);//calculate the codeName of the characters
        printHuff(huff, 0);
        queue = insertionSort(queue, count, decrescent);//creating table of contents in decrescent order
        //create the compact file 
        printf("Insira o nome do arquivo que deseja criar:\n");
        scanf("%s", fileOutputName);
        compress(fileInputName, fileOutputName, queue, count);
        printf("Insira o nome do arquivo q deseja criar para verificar se esta comprimido certo:\n");
        scanf("%s", fileInputName);
        decompress(fileOutputName, fileInputName);
    }
    else
        printf("Arquivo não encontrado\n");
    return 0;
}
