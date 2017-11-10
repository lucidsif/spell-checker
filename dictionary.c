/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "dictionary.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 1076200
#define PRIME 1076143

typedef struct node 
{
    char data[LENGTH + 1];
    struct node *next;
} node;

// create hashTable
node *hashTable[SIZE];
int collisions = 0;
int dictionarySize = 0;

// based on djb2 hash algorithm
int hash (const char* word)
{
    int hash = 5381;
    int n;
    for (int i = 0; word[i] != '\0'; i++) {
        if(isalpha(word[i])) {
            n = word [i] - 'a' + 1;
        } else {
            n = 25;
        }
        hash = ((hash << 5) + n) % SIZE;
    }
    return hash;    
}

// create head node function
node *createHeadNode(char *word) 
{
    node *head = malloc(sizeof(node));
    if (head == NULL) {
        printf("no memory for head node\n");
        return head;
    }
    strcpy(head->data, word);
    head->next = NULL;
    return head;
}

//create prepend function
void prependList(node **head, char *word) 
{
    node *newNode = malloc(sizeof(node));
    strcpy(newNode->data, word);
    newNode->next = *head;
    *head = newNode;
}

bool traverseList(node *head, const char *word) 
{
    node *current = head;
    while (current != NULL) {
        if (current->data == word) {
            return true;
        }
        current = current->next;
    }
    return false;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // TODO
    char lower[LENGTH + 1];
    int l = strlen(word);
    for(int i = 0; i < l; i++)
        lower[i] = tolower(word[i]);
    lower[l] = '\0';
    
    int index = hash(lower);
    
    if (hashTable[index] == NULL)
    {
        return false;
    }
    
    node* current = hashTable[index];
    
    while (current != NULL) {
        if (strcmp(lower, current->data) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL) {
        printf("could not open %s\n", dictionary);
        return 2;
    }
    char word[LENGTH + 1] = {0};
    
    while (fscanf(fp, "%s\n", word)!= EOF) {
        dictionarySize++;
        long index = hash(word);
           if (hashTable[index] == NULL) {
            hashTable[index] = createHeadNode(word);
            hashTable[index]->next = NULL;
        } else {
            collisions++;
            prependList(&hashTable[index], word);
        }
    }
    fclose(fp);
    return true;
    // loop through each word
        // if word is a proper string
            // hash word and return index
            // if there is a linked list at the index, prepend
            // else create linked list at the index
    //return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
        // TODO
    // if dictionary is loaded, return number of words
    if (dictionarySize > 0)
    {
        return dictionarySize;
    }
     
    // if dictionary hasn't been loaded, return 0
    else
        return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    for (int i = 0; i < SIZE; i++) {
        if (hashTable[i] != NULL) {
            // free linked lists
            node *current = hashTable[i];
            while (current != NULL) {
                node *temp = current;
                current = current->next;
                free(temp);
            }
        }
    }
    return true;
}

