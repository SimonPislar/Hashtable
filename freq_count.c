#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hash_table.h"
#include "common.h"
#include "private_resources.h"
#include "iterator.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

static int cmpstringp(const void *p1, const void *p2) {
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys) {
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

// Checks if word is in ht and either adds it or update the frequency
void process_word(char *word, ioopm_hash_table_t *ht) {
  int freq =
    ioopm_hash_table_has_key(ht, ptr_elem(word))?
    (ioopm_hash_table_lookup(ht, ptr_elem(word))).value.integer:
    0;
  char *dup_word = strdup(word); // Bad for performance!
  ioopm_hash_table_insert(ht, ptr_elem(dup_word), int_elem(freq + 1));
  if (freq != 0) { // If the word already occurs in ht we need to free dup_word in order to not cause a leak
      free(dup_word);
  }
}

// Get number of unique words from a file and add them and their frequency to ht
void process_file(char *filename, ioopm_hash_table_t *ht) {
  FILE *f = fopen(filename, "r");
  while (true)
  {
    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, f);
    if (feof(f)) {
      free(buf);
      break;
    }
    for (char *word = strtok(buf, Delimiters); word && *word; word = strtok(NULL, Delimiters)) {
      process_word(word, ht);
    }
    free(buf);
  }
  fclose(f);
}

// Hash function used for calculating bucket when key is string
int string_sum_hash(elem_t e) {
    char *str = e.pointer;
    int result = 0;
    do {
        result += *str;
    }
    while (*++str != '\0');
    return result;
}

bool string_eq(elem_t e1, elem_t e2) {
    return (strcmp(e1.pointer, e2.pointer) == 0);
}

void free_elem_t_pointer(elem_t key, elem_t *ignored_value, void *ignored_extra) {
    char *str = key.pointer;
    free(str);
}

int main(int argc, char *argv[]) {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(string_eq , string_eq, string_sum_hash);
    ioopm_list_t *list_of_keys = NULL;
    ioopm_iter_t *iterator = NULL;

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) { // Fill ht with words from input file
            printf("\nargc: %d\n", argc);
            process_file(argv[i], ht);
        }

        size_t size = ioopm_hash_table_size(ht);
        int counter = 0;
        char *keys[size];

        list_of_keys = ioopm_hash_table_keys(ht); 
        iterator = ioopm_list_iterator(list_of_keys);

        while (ioopm_iterator_has_next(iterator)) { // Put keys (words from file) in an array
            char *current_key = ioopm_iterator_current(iterator).pointer;
            keys[counter] = current_key;

            counter++;
            ioopm_iterator_next(iterator);
        }

        sort_keys(keys, size);

        for (int i = 0; i < size; ++i) { // Count unique words
            int freq = (ioopm_hash_table_lookup(ht, ptr_elem(keys[i]))).value.integer;
            printf("%s: %d\n", keys[i], freq);
        }
        } else {
            puts("Usage: freq-count file1 ... filen");
    }

    ioopm_linked_list_destroy(list_of_keys);
    ioopm_iterator_destroy(iterator);
    ioopm_hash_table_apply_to_all(ht, free_elem_t_pointer, NULL); //Free every word that our keys points to
    ioopm_hash_table_destroy(ht);
}
