#include "dictionary.h"
#include "dictionary.c"
#include "spell.c"
#define DICTIONARY "wordlist_files/wordlist.txt"
#define JUSTICE "wordlist_files/justice.txt"

int main()
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(JUSTICE, hashtable);

    // check_word("Justice", hashtable);

    // char *misspelled[MAX_MISSPELLED];
    // FILE *fp = fopen("tests/punct.txt", "r");
    // int num_misspelled = check_words(fp, hashtable, misspelled);

    return 0;
}