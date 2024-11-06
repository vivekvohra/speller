// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 17576;

// Hash table array of linked lists
node *table[N];

// Number of words in the dictionary
int count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
    node *cursor = table[index];

    // Traverse linked list at the hash table index
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0) // Case-insensitive comparison
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hashvalue = 0;
    for (int i = 0; i < 3 && word[i] != '\0'; i++)
    {
        hashvalue = hashvalue * 26 + (tolower(word[i]) - 'a');
    }
    return hashvalue % N; // Use N as the size of the hash table
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open %s.\n", dictionary);
        return false;
    }

    char wordlist[LENGTH + 1];

    // Read words from file and insert into hash table
    while (fscanf(file, "%s", wordlist) != EOF)
    {
        count++;

        // Allocate memory for a new node
        node *newnode = malloc(sizeof(node));
        if (newnode == NULL)
        {
            fclose(file);
            return false;
        }

        // Copy the word into the node and initialize next pointer
        strcpy(newnode->word, wordlist);
        newnode->next = NULL;

        // Hash the word to obtain an index
        int index = hash(wordlist);

        // Insert node into hash table
        if (table[index] == NULL)
        {
            table[index] = newnode;
        }
        else
        {
            // Collision: Insert at beginning of linked list
            newnode->next = table[index];
            table[index] = newnode;
        }
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *tmp = NULL;
    node *cursor = NULL;

    // Loop through all buckets in the hash table
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];

        // Free each linked list in the bucket
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
