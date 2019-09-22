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

void lower_string(char s[]) {
   int c = 0;
   
   while (s[c] != '\0') {
      if (s[c] >= 'A' && s[c] <= 'Z') {
         s[c] = s[c] + 32;
      }
      c++;
   }
}

// void reformat_string(char *src, char *dst) {
//     for (; *src; ++src)
//         if (!ispunct((unsigned char) *src))
//             *dst++ = tolower((unsigned char) *src);
//     *dst = 0;
// }

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
        // if(strcmp(dword, "justice") == 0){
        //   printf("adding %s at %d", dword, hashkey);
        // }
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
  // printf("looking for %s at %d", lower_word, bucket);
  hashmap_t cursor = hashtable[bucket];
  // printf("%s : %d\n", lower_word, bucket);
  while (cursor != NULL) {
    if(strcmp(lower_word,cursor->word) == 0){
      // printf("Found : %s - returning true\n", lower_word);
      return true;
    }
    // printf(".");
    cursor = cursor->next;
  }
  // printf("Not Found : %s - returning false\n", lower_word);
  return false;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
  int num_misspelled = 0;
  
  char* line = NULL;  
  size_t len = 0;
  printf("calling check ing %s", "\nehlo\n");
  while(getline(&line, &len, fp) != -1) {
    char str[strlen(line) + 1];
    strcpy(str, line);

    printf("calling check ing %s", "\nehlo\n");
    bool result = check_word(line, hashtable); 
    if (result == false){
      misspelled[num_misspelled] = line;
      num_misspelled++;
    }
    // Returns first token 
    // char* token = strtok(str, " "); 
  
    // Keep printing tokens while one of the 
    // delimiters present in str[]. 
    // while (token != NULL) { 
    //     bool result = check_word(token, hashtable); 
    //     if (result == false){
    //       misspelled[num_misspelled] = token;
    //       num_misspelled++;
    //     }
    //     token = strtok(NULL, " "); 
    // } 
  
  }
  return num_misspelled;
}
