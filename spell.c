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
#include "spell.h"

// int main(int argc, char * argv[])
// {
//   if(argc != 2){
//     printf("Usage: $ ./spell_check a_tale_of_two_cities.txt wordlist.txt\n");
//     return 0;
//   }

// 	FILE *file = fopen(argv[1], "r");
  
//   if(file == NULL){
//     printf("File to spell check does not exist\n");
//     return 0;
//   }

//   char *line = NULL;
//   size_t len = 0;

//   while(getline(&line, &len, file) != -1) {
//       hashmap_t
//       printf("line length: %zd\n", strlen(line));
//   }

//   printf("\n\nMax line size: %zd\n", len);

//   fclose(file);
//   free(line); 
// }

