/****************************************************************************
 * spell.c
 *
 * Application Security, Assignment 1
 *
 * Adapted from code written by Ben Halperin.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

void lower_string(char word_with_punct[]) {
   int c = 0;
   
   // Convert each letter to lowercase via ascii code
   while (word_with_punct[c] != '\0') {
      if (word_with_punct[c] >= 'A' && word_with_punct[c] <= 'Z') {
         word_with_punct[c] = word_with_punct[c] + 32;
      }
      c++;
   }
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){  
    // Initialize all values in hash table to NULL.
    for(int i=0; i<HASH_SIZE; i++)
    {
      hashtable[i] = NULL;
    }

    // Open dictionary file, returning false if not found
    FILE* file = fopen(dictionary_file, "r");
    if (file == NULL)
    {
        puts("No dictionary file found.");
        return false;
    }

    // Initiate variable to store current word
    char* lowered_word = calloc(LENGTH+ 1,sizeof(char)); 
    
    // For each line, get the word
    char* line = NULL;  
    size_t len = 45;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        // Remove newline captured by getline function
        if ((line)[read - 1] == '\n') 
        {
          (line)[read - 1] = '\0';
          --read;
        } 

        // Lowercase the word
        lowered_word = line;
        lower_string(lowered_word);

        // Add lowered word to hashtable
        node* new_node = malloc(sizeof(node));
        strcpy(new_node->word,lowered_word);
        int hashkey = hash_function(lowered_word);
        new_node->next = hashtable[hashkey];
        hashtable[hashkey] = new_node; 
    }

    // Handle any unexpected errors with the file
    if (ferror(file)) {
        puts("Experienced an error with the dictionary file.");
    }

    // Free the lowered_word, reset line and close the file
    free(lowered_word);
    line = NULL;
    fclose(file);
    return true;    
}

bool check_word(const char* word, hashmap_t hashtable[]) {
  // Lowercase the word
  char* lower_word = calloc(LENGTH+ 1,sizeof(char)); 
  strcpy(lower_word, word);
  lower_string(lower_word);

  // Iterate through the hash to look for the related bucket
  int bucket = hash_function(lower_word);
  hashmap_t cursor = hashtable[bucket];
  while (cursor != NULL) {
    if(strcmp(lower_word,cursor->word) == 0){
      return true;
    }
    cursor = cursor->next;
  }
  return false;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
  int num_misspelled = 0;

  // For each line, get the word
  char* line = NULL;  
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, fp)) != -1) {
      // Remove newline captured by getline function
      if ((line)[read - 1] == '\n') 
      {
        (line)[read - 1] = '\0';
        --read;
      } 

      // Tokenize string on spaces
      char* token;
      char* delimiter = " ";
      token = strtok(line, delimiter);
      if (token == NULL){
        return -1;
      }

      // While there are tokenized words    
      do
      {
        // Strip start and end punctuation
        int i;
        char* stripped_word;
        for(i=strlen(token)-1; ispunct(token[i]);--i) {
          token[i]='\0';
        }
        for(stripped_word=token;ispunct(*stripped_word);++stripped_word);

        // Validate stripped word
        bool word_found = check_word(stripped_word, hashtable); 
        if (!word_found){
          misspelled[num_misspelled] = stripped_word;
          num_misspelled++;
        }
        line = NULL;
      } while ((token = strtok(NULL, delimiter)) != NULL);
  }

  // Handle any unexpected errors
  if (ferror(fp)) {
      puts("Experienced an error with the dictionary file.");
  }

  // Free the line and close the file
  free(line);
  line = NULL;
  fclose(fp);

  return num_misspelled;
}
