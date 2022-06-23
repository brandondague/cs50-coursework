// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

const unsigned int N = 100000;

// Hash table
node *table[N];
int wordcount = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Hash the input word
    int hash_index = hash(word);
    // Create variable to hold the first node in the bucket
    node *n = table[hash_index];
    // While there is still another node in the linked list
    while (n != NULL)
    {
        // Compare the words
        char *lowercase = n->word;
        for (int i = 0; i < strlen(lowercase); i++)
        {
            lowercase[i] = tolower(lowercase[i]);
        }
        if (strcmp(lowercase, n->word) == 0)
        {
            return true;
        }
        // Set n to the next node in the linked list
        n = n->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int i = 0, sum = 0;
    // Iterate over the length of the word
    while (i < strlen(word))
    {
        // Add the value of the char at each index to the sum
        sum += tolower(word[i]);
        i++;
    }
    // Divide by N and take the remainder as the index for the hash table
    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open and read the dictionary file
    FILE *file = fopen(dictionary, "r");
    // If the file is NULL
    if (file != NULL)
    {
        // Create a buffer of max word length plus the nul char
        char buffer[LENGTH + 1];
        // While the file still has contents
        while (fscanf(file, "%s", buffer) != EOF)
        {
            // Allocate memory for a new node
            node *n = malloc(sizeof(node));
            // Check if memory has been allocated
            if (n != NULL)
            {
                // Copy the word into the word field of the new node
                strcpy(n->word, buffer);
                // Hash the word to find the appropriate bucket
                int hash_index = hash(buffer);
                // Set the next field of the new node to hashed index of the table
                n->next = table[hash_index];
                // Insert into the appropriate bucket
                table[hash_index] = n;
                wordcount++;
            }
            else
            {
                fclose(file);
                return false;
            }
        }
    }
    else
    {
        fclose(file);
        return false;
    }
    // Close file and return true
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return wordcount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // Iterate over the hash table
    for (int i = 0; i < N; i++)
    {
        // Create variable to hold the first node in the bucket
        node *n = table[i];
        // Iterate through the linked list
        while (n != NULL)
        {
            // Create a temp value to keep track of the next pointer in each node
            node *temp = n;
            // Update n to the next node
            n = n->next;
            // Free the original node
            free(temp);
        }
    }
    return true;
}
