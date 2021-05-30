#include"RB.h"
#include<string.h>
#include<limits.h>
#include<math.h>
#define BITSINBYTE 8

/*
This algorithm offers two functionalities:
to compress a file
to decompress a file
*/

typedef struct code
{
    long unsigned int count;
    unsigned int sizeCodeName;
    unsigned char* character;
    unsigned char* codeName;
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
    else
        printf("Error while incrementing: received a NULL pointer\n");
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

//-----------------------------------Functions to integrate with RB library

void printKey(Node* node)
{
    if(node != NULL)
    {
        if(*getCode(node)->character != '\n')
            printf("%c\t%ld", *getCode(node)->character, getCode(node)->count);
        else
            printf("enter\t%ld", getCode(node)->count);
    }
}

int compareInfo(void* info1, void* info2)
{
    if (*(unsigned char*)info1 == *(unsigned char*)info2)
        return 0; 
    else if (*(unsigned char*)info1 > *(unsigned char*)info2)
        return 1;
    return -1;
}

void* getKey(Node* node)
{
    if(node != NULL)
        return getCode(node)->character;
    return NULL;
}

void destroyInfo(void* info)
{
    free(((Code*)info)->character);
    free(((Code*)info)->codeName);
    free(info);
}

//-----------------------------------End

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

int crescent(long unsigned int value1, long unsigned int value2)
{
    if(value1 > value2)
        return 1;
    return 0;
}

int decrescent(long unsigned int value1, long unsigned int value2)
{
    if(value1 < value2)
        return 1;
    return 0;
}

void insertionSort(Queue** input, int (*op) (long unsigned int, long unsigned int))
{
    Queue* output = NULL;
    Queue* keyPtr = NULL;
    Queue* pPtr = NULL;
    Queue* ptr = NULL;
    /*
    step 1: search input queue for me biggest/smallest
    step 2: pop it from input
    step 3: insert in first position of output
    step 4: repeat until input is empty
    step 5: point pointer input towards output
    */
    while(*input != NULL)
    {
        pPtr = NULL;
        keyPtr = ptr = *input;
        /*
        pPtr stores the element who's next is keyPtr(if NULL, indicates keyPtr is first element)
        keyPtr stores the biggest/smallest element in queue
        ptr is the iteration control
        */
        while(ptr->next != NULL)
        {
            if((*op) (ptr->next->info->count, keyPtr->info->count))
            {
               pPtr = ptr;
               keyPtr = ptr->next;
            }
            ptr = ptr->next;
        }
        if(pPtr != NULL)
            pPtr->next = keyPtr->next;
        else
            *input = keyPtr->next;
        keyPtr->next = output;
        output = keyPtr;
    }
    *input = output;
}

void printCode(Code* code)
{
    if(code != NULL)
    {
        if(*code->character != '\n')
            printf("Caractere: %c\n", *code->character);
        else
            printf("Caractere: enter\n");
        printf("Contagem de repetições: %lu\n", code->count);
        printf("Elementos no código : %d\n", code->sizeCodeName);
        printf("Código: ");
        for(int i = 0; i < code->sizeCodeName; i++)
            printf("%d", code->codeName[i]);
        printf("\n");
        if(code->left != NULL)
            printf("Caractere à esquerda: %c\n", *code->left->character);
        if(code->right!= NULL)
            printf("Caractere à direita: %c\n", *code->right->character);
    }
    else
        printf("Code NULL\n");
}

void printQueue(Queue* queue)
{
    if(queue != NULL)
    {
        printCode(queue->info);
        printQueue(queue->next);
    }
}

Queue* copyQueue(Queue* original)
{
/*
return a new queue whose elements are the same as original's, but it's info has the same adress.
*/
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

void createCodeNames(Code* huff, unsigned char* codeName, long unsigned int level)
{
    if(huff != NULL)
    {
        if(huff->character != NULL && level != 0)//is not an internal node and is not the case where the file has only one distinct character
        {
            //printf("%c\n", *huff->character);
            huff->codeName = codeName;
            huff->sizeCodeName = level;
        }
        else if(huff->character != NULL)
        {
            //the file has no disting character
            huff->codeName = 0;
            huff->sizeCodeName = 1;
        }
        else
        {
            codeName = (unsigned char*) realloc(codeName, sizeof(char) * (level + 1));
            codeName[level] = 0;
            createCodeNames(huff->left, codeName, level + 1);
            unsigned char* codeName2 = (unsigned char*) malloc(sizeof(char) * (level + 1));
            memcpy(codeName2, codeName, sizeof(char) * (level + 1));
            codeName2[level] = 1;
            createCodeNames(huff->right, codeName2, level + 1);
         }
    }
    else
        free(codeName);
}

//

long unsigned int createMap(const char* fileName, Node** map)
/*
this algorithm fills a redblacktree with characters taken from input file
*/
{
    long unsigned int count = 0;
    FILE *fptr;
    fptr = fopen(fileName, "r");
    unsigned char* aux;
    Node* ptr = NULL;
    Node* ptr2 = NULL;
    int answer = 0;
    if(fptr != NULL)
    {
        while(1)
        {
            aux = (unsigned char*) malloc(sizeof(char));
            //take input from file 
            if(fscanf(fptr, "%c", aux) == EOF)
            {
                free(aux);
                break;
            }
            //insert in tree
            ptr = createNodeRBTree(createCode(aux));
            answer = insertNodeRBTree(map, *map, ptr);
            if(answer == 0)//some error ocurred 
            {
                printf("Unknow error during insertion in redBlackTree\n");
                destroyNodeRBTree(ptr);
            }
            else if (answer == 2)//aux weight is already present in tree
            {
                ptr2 = searchInfoRBTree(*map, aux);//search not working
                incrementCode(ptr2);
                destroyNodeRBTree(ptr);
            }
            count++;
        }
        fclose(fptr);
    }
    return count;//total amount of characters in file
}

void printHuff(Code* huff, long unsigned int level)
{
    if(huff != NULL)
    {
        if(huff->right != NULL)
            printHuff(huff->right, level+1);
        //print identation
        for(long unsigned int i = 0; i < level; i++)
            printf("\t");
        //print internal node
        if(huff->character == NULL)
            printf("+: %ld ocorrencias\n", huff->count);
        //print leafe
        else
        {
            if(*huff->character != '\n')
                printf("caracter: %c ocorre %ld vezes. codenome:", *huff->character, huff->count);
            else
                printf("caracter enter ocorre %ld vezes. codenome:", huff->count);
            //print code
            for(unsigned int i = 0; i < huff->sizeCodeName; i++)
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
            //search foo in queue 
            if(*ptr->info->character == foo)
                return ptr->info;
            ptr = ptr->next;
        }
    }
    return NULL;
}

void compress(const char* input, char* output1, Queue* table, long unsigned int amount)
{
    Code* element = NULL;
    Queue* holdCurrent = NULL;
    Queue* holdNext= NULL;
    Code* holdInfo = NULL;
    unsigned char* holdChar = NULL;
    unsigned char* holdCode = NULL;
    const char* output = strcat(output1, ".art");//name of format to be saved
    unsigned char compress = 0;
    unsigned char aux = ' ';
    long unsigned int pos = 0;
    FILE* fPtr = fopen(input, "r");
    //calculate the num of elements in table
    holdCurrent = table;
    pos = 0;
    while(holdCurrent != NULL)
    {
        pos++;
        holdCurrent = holdCurrent->next;
    }
    if(fPtr != NULL)
    {
        FILE* fOPtr = fopen(output, "wb");
        if(fOPtr != NULL)
        {
            //store table into file
            fwrite(&pos, sizeof(long unsigned int), 1, fOPtr);//store num of elements in table
            holdCurrent = table;
            while(holdCurrent != NULL)
            {
                holdNext = holdCurrent->next;
                holdCurrent->next = NULL;
                holdInfo = holdCurrent->info;
                holdCurrent->info= NULL;
                fwrite(holdCurrent, sizeof(Queue), 1, fOPtr);
                holdCurrent->next = holdNext;
                holdCurrent->info = holdInfo;
                holdChar = holdCurrent->info->character;
                holdCurrent->info->character = NULL;
                holdCode = holdCurrent->info->codeName;
                holdCurrent->info->codeName = NULL;
                holdCurrent->info->left = holdCurrent->info->right = NULL;
                fwrite(holdCurrent->info, sizeof(Code), 1, fOPtr);
                holdCurrent->info->character = holdChar;
                holdCurrent->info->codeName = holdCode;
                fwrite(holdCurrent->info->character, sizeof(unsigned char), 1, fOPtr);
                /*
                Intents to store the codenames in compress form, storing any codeName with less than 9 characters into one byte, any codeName with more than 8 and less than 17 characters into two bytes and so on...
                */
                pos = 0;
                compress = 0;
                for(unsigned int i = 0; i < holdCurrent->info->sizeCodeName; i++)
                {
                    aux = holdCurrent->info->codeName[i];
                    aux = aux << (7 - pos);
                    compress = compress | aux;
                    if(++pos == BITSINBYTE)
                    {
                        pos = 0;
                        fwrite(&compress, sizeof(unsigned char), 1, fOPtr);
                        compress = 0;
                    }
                }
                if(pos != 0)//if pos is not zero, that means thats compress has information that has not been inserted into file yet.
                    fwrite(&compress, sizeof(unsigned char), 1, fOPtr);
                holdCurrent = holdCurrent->next;
            }
            pos = 0;
            compress = 0;
            //read input from file
            while(fscanf(fPtr, "%c", &aux) != EOF)//take a character from input
            {
                element = getElement(aux, table);// take the codeName of the current character
                for(unsigned int i = 0; i < element->sizeCodeName; i++)//write codeName into compress
                {
                    aux = element->codeName[i];//get a byte
                    /*data = fopen("compress", "a");
                    fprintf(data, "%d", aux);
                    fclose(data);*/
                    aux = aux << (7-pos);//move it's significant bit to position
                    compress = compress | aux;//add to compress
                    if(++pos == BITSINBYTE)
                    {
                        pos = 0;
                        fwrite(&compress, sizeof(char), 1, fOPtr);
                        compress = 0;
                    }
                }
            }
            if(pos != 0)
                fwrite(&compress, sizeof(char), 1, fOPtr);
            fclose(fOPtr);
        }
        else
            printf("Falha ao salvar as alterações\n");
        fclose(fPtr);
    }
    else
        printf("Erro ao gravar alterações\n");
}

void decompress(const char* input, const char* output)
{
    FILE* fPtr = fopen(input, "rb");
    Code* element = NULL;
    Code* huff = NULL;
    Queue* holdCurrent = NULL;
    Queue* table = NULL;
    unsigned char compressed = 0;
    unsigned char aux = ' ';
    long unsigned int pos = 0;
    if(strstr(input, ".art") == NULL)
    {
        printf("Formato não reconhecido\n");
        return;
    }
    //retrieve compressed file into a compressed string
    if(fPtr != NULL)
    {
        fread(&pos, sizeof(long unsigned int), 1, fPtr);
        for(long unsigned int i = 0; i < pos; i++)
        {
            holdCurrent = (Queue*) calloc(sizeof(Queue), 1);
            fread(holdCurrent, sizeof(Queue), 1, fPtr);
            holdCurrent->info = (Code*) calloc(sizeof(Code), 1);
            fread(holdCurrent->info, sizeof(Code), 1, fPtr);
            holdCurrent->info->character = (unsigned char*) calloc(sizeof(char), 1);
            fread(holdCurrent->info->character, sizeof(char), 1, fPtr);
            holdCurrent->info->codeName = (unsigned char*) calloc(sizeof(char), holdCurrent->info->sizeCodeName);
            for(unsigned int j = 0; j < holdCurrent->info->sizeCodeName; j++)
            {
                if(j % BITSINBYTE == 0)
                    fread(&compressed, sizeof(char), 1, fPtr);
                aux = compressed;
                compressed = compressed << 1;//update compressed
                holdCurrent->info->codeName[j] = aux >> 7;//writting it to codeName
            }
            printCode(holdCurrent->info);
            holdCurrent->next = table;//table is written in decrescent order, so we insert each one on the beginning of queue and we'll have a crescent ordered queue
            table = holdCurrent;
        }
        if(table == NULL)
            return;
        //create a huffman tree from table 
        printQueue(table);
        huff = createHuffmanTree(copyQueue(table));
        printf("Árvore de Huffman do arquivo fonte: \n");
        printHuff(huff, 0);
        if(huff == NULL)
            return;
        FILE* fOPtr = fopen(output, "w");
        //transform compressed string of codenames into decompressed string, so we can acess it's individual bits
        if(fOPtr != NULL)
        {
            element = huff;//point element to root of huffman tree
            while(fread(&compressed, sizeof(char), 1, fPtr) == 1)
            {
                pos = 0;//we just read a new character from input, so we reset position counter
                while(pos++ < BITSINBYTE)//ensures position is between 0 and 7
                {
                    aux = compressed;
                    compressed = compressed << 1;//update input character
                    aux = aux >> 7;//move current bit to the last position
                    if(aux == 0)//not a valid character
                        element = element->left;
                    else
                        element = element->right;
                    if(element->character != NULL)
                    {//found a valid character
                        fprintf(fOPtr, "%c", *element->character);
                        element = huff;//point element to root of huffman tree
                    }
                }

            }
            fclose(fPtr);
        }
        else
            printf("Falha ao salvar as alterações\n");
    }
    else
    {
        printf("Arquivo não encontrado\n");
        return;
    }
}

void Compress(const char* fileInputName, char* fileOutputName)
{
    Node* map = NULL;
    Queue* queue = NULL;
    Code* huff = NULL;
    long unsigned int amount = createMap(fileInputName, &map);//define all characters in the fileName file and count it's ocurrences
    if(map != NULL)
    {
        printRBTree(map,printKey, 0);//print the redBlack Tree that holds the characters
        getQueue(map, &queue);
        insertionSort(&queue, crescent);//Get a minimum priority queue from the redBlackTree elements
        huff = createHuffmanTree(copyQueue(queue));//get the huffman tree of those characters
        createCodeNames(huff, NULL, 0);//calculate the codeName of the characters
        //printHuff(huff, 0);
        insertionSort(&queue, decrescent);//creating table of contents in decrescent order
        //create the compact file 
        compress(fileInputName, fileOutputName, queue, amount);
        destroyRBTree(map);
    }
    else
        printf("Arquivo não encontrado\n");
}

void menu()
{
    char fileInputName[50];
    char fileOutputName[50];
    char aux;
    printf("Escolha uma funcionalidade\n[C] Comprimir um arquivo de texto\n[D] Descomprimir um arquivo\n>");
    scanf(" %c", &aux);
    switch(aux)
    {
        case'C':case'c':
            printf("Insira o nome do arquivo de entrada\n>");
            scanf("%s", fileInputName);
            printf("Insira o nome do arquivo de saida\n>");
            scanf("%s", fileOutputName);
            Compress(fileInputName, fileOutputName);
            break;
        case'D':case'd':
            printf("Insira o nome do arquivo de entrada\n>");
            scanf("%s", fileInputName);
            printf("Insira o nome do arquivo de saida\n>");
            scanf("%s", fileOutputName);
            decompress(fileInputName, fileOutputName);
            break;
        default:
            printf("Opção não reconhecida\n");
            break;
    }
}

int main()
{
    menu();
    return 0;
}
