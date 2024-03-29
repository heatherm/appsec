#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist_files/wordlist.txt"
#define DICTIONARY_LONG_WORD "wordlist_files/wordlist_long_word.txt"
#define TESTDICT "wordlist_files/test_wordlist.txt"
#define JUSTICE "wordlist_files/justice.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(true);
    ck_assert(load_dictionary(TESTDICT, hashtable));
    //const char* first = "first";
    // const char* found = lookup(hashtable, first);
    // ck_assert(strcmp(found, first));
}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(true);
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* punctuation_word_2 = "pl.ace";
    const char* word_with_question_marks = "?pl.ace?";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));
    ck_assert(!check_word(word_with_question_marks, hashtable));
}
END_TEST

START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(true);
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("tests/test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

START_TEST(test_check_words_punct)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(true);
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "j.ust";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("tests/punct.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 1);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
}
END_TEST

START_TEST(test_check_words_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(true);
    load_dictionary(DICTIONARY, hashtable);
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("tests/overflow.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 1);
}
END_TEST

START_TEST(test_check_words_overflow_dictionary)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(true);
    int i;
    for(i = 0; i < 100; i++){
        load_dictionary(DICTIONARY, hashtable);
    }
}
END_TEST

START_TEST(test_check_words_overflow_dictionary_long_word)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY_LONG_WORD, hashtable);
    ck_assert(true);
}
END_TEST

START_TEST(test_check_word_buffer_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char incorrect_word[500000];
    for(int i = 0; i < 499999; i++)
        incorrect_word[i] = 'A';
    incorrect_word[499999] = 0;
    ck_assert(!check_word(incorrect_word, hashtable));
}
END_TEST


Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_dictionary_normal);
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);
    tcase_add_test(check_word_case, test_check_words_punct);
    tcase_add_test(check_word_case, test_check_words_overflow);
    tcase_add_test(check_word_case, test_check_words_overflow_dictionary);
    tcase_add_test(check_word_case, test_check_words_overflow_dictionary_long_word);
    tcase_add_test(check_word_case, test_check_word_buffer_overflow);
    
    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

