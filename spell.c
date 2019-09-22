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
#include <errno.h>
#include "dictionary.h"

void lower_string(char s[]) {
   int c = 0;
   
   while (s[c] != '\0') {
      if (s[c] >= 'A' && s[c] <= 'Z') {
         s[c] = s[c] + 32;
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

    // Open dictionary file
    FILE* file = fopen(dictionary_file, "r");
    if (file == NULL)
    {
        puts("No dictionary file found.");
        return false;
    }

    // Initiate variable to store current word
    char* dword = calloc(LENGTH+ 1,sizeof(char)); 
    
    // For each line, get the word
    char* line = NULL;  
    size_t len = 45;
    ssize_t read;

    line = (char *)malloc(len * sizeof(char));
    if( line == NULL)
    {
        perror("Not able to allocate buffer for line.");
        exit(1);
    }

    while ((read = getline(&line, &len, file)) != -1) {
        if ((line)[read - 1] == '\n') 
        {
          (line)[read - 1] = '\0';
          --read;
        } 
        dword = line;
        lower_string(dword);
        node* new_node = malloc(sizeof(node));
        strcpy(new_node->word,dword);

        // Add new lowered word to hashtable
        int hashkey = hash_function(dword);
        new_node->next = hashtable[hashkey];
        hashtable[hashkey] = new_node; 
    }

    // Handle any unexpected errors
    if (ferror(file)) {
        puts("Experienced an error with the dictionary file.");
    }

    // Free the line and close the file
    free(line);
    line = NULL;
    fclose(file);
    return true;    
}

bool check_word(const char* word, hashmap_t hashtable[]) {
  char* lower_word = calloc(LENGTH+ 1,sizeof(char)); 
  strcpy(lower_word, word);
  lower_string(lower_word);
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
  size_t len = 1000;
  ssize_t read;

  line = (char *)malloc(len * sizeof(char));
  if( line == NULL)
  {
      perror("Not able to allocate buffer for line.");
      exit(1);
  }

  read = getline(&line, &len, fp);
  if (errno == ENOMEM || errno == EOVERFLOW) {
    num_misspelled++;
  }
  while (read >= 0) {
      if ((line)[read - 1] == '\n') 
      {
        (line)[read - 1] = '\0';
        --read;
      } 

      // Tokenize string on spaces
      char* token;
      token = strtok(line, " ");
      if (token == NULL)
          return -1;
      do
      {
        // Strip start and end punctuation
        int i;
        char* p;
        for(i=strlen(token)-1; ispunct(token[i]);--i)
            token[i]='\0';
  
        for(p=token;ispunct(*p);++p);

        // Validate remaining word/token
        bool result = check_word(p, hashtable); 
        if (!result){
          misspelled[num_misspelled] = p;
          num_misspelled++;
        }
        line = NULL;
      } while ((token = strtok(NULL, " ")) != NULL);

      read = getline(&line, &len, fp);
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
