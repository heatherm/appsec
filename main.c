#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
//#include "dictionary.c"
//#include "spell.c"

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Usage: ./main file_to_check.txt wordlist.txt");
    }
    FILE * text_file = fopen(argv[1], "r");
    if (text_file == NULL){
        printf("Unable to find file to spell check.");
        exit(1);
    }

    hashmap_t hashtable[HASH_SIZE];
    char * misspelled[MAX_MISSPELLED];
    int num_misspelled = 0;
    char * dictionary_file = argv[2];
    if (load_dictionary(dictionary_file, hashtable)){
        num_misspelled = check_words(text_file, hashtable, misspelled);
    }
    else{
        printf("Unable to load dictionary file\n");
        exit(1);
    }

    printf("There were %d misspelled words\n", num_misspelled);

    exit(0);
}