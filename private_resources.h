#pragma once

#include <stdbool.h>

#include "common.h"
#include "hash_table.h" //correct way to do this?

/**
 * @file common.h
 * @author Simon Pislar and Ida Hellqvist
 * @date 09 Oct 2022
 * @brief Private resources used by several files.
 *
 * @see https://uppsala.instructure.com/courses/68435/assignments/130155
*/

#define Load_Factor 0.75
#define Primes (size_t[]){17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381}

#define int_elem(x) (elem_t) { .integer = (x) }
#define ptr_elem(x) (elem_t) { .pointer = (x) }

typedef struct list ioopm_list_t;
typedef struct link item_t;

// An element of a linked list
struct link {
    elem_t value;
    struct link *next;
};

// The root node" of a linked list
struct list {
    item_t *first;
    item_t *last;
    ioopm_eq_function eq_function;
};

// Used for error handling
struct option {
    bool success;
    elem_t value;
};

// Struct for an iterator
struct iter {
    item_t *current;
    ioopm_list_t *list;
};

bool compare_values(ioopm_eq_function key_eq, ioopm_eq_function value_eq, elem_t ignored_value, elem_t value, void *x); // Declared and used in hash_table.c but test file also needs access to it
void change_char_value(elem_t ignored_value, elem_t *value, void *x); // Declared and used in hash_table.c but test file also needs access to it
