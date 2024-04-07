#pragma once

#include <stdbool.h>
#include "linked_list.h"

/**
 * @file hash_table.h
 * @author Simon Pislar and Ida Hellqvist
 * @date 9 Oct 2022
 * @brief Simple hash table that maps keys elements to string string elements.
 *
 * @see https://uppsala.instructure.com/courses/68435/assignments/130155
 */

typedef struct hash_table ioopm_hash_table_t;
typedef struct option ioopm_option_t;

typedef bool(*ioopm_predicate)(ioopm_eq_function key_eq, ioopm_eq_function value_eq, elem_t key, elem_t value, void *extra);
typedef void(*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);
typedef int(*ioopm_hash_function)(elem_t key);

/// @brief create a new hash table
/// @param value_eq a function for comparing values
/// @param key_eq a function for comparing values
/// @param key_convert a function that converts a key to an integer value
/// If key_convert is NULL, default for keys are integers
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_eq_function value_eq, ioopm_eq_function key_eq, ioopm_hash_function key_convert);

/// @brief delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// keys of negative numerical value will be converted to absolute value
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht // TODO: explain using option 1 from ticket 1
/// @param ht hash table operated upon
/// @param key key to lookup 
/// keys of negative numerical value will be converted to absolute value
/// @return a struct containing value mapped to key and a bool showing if value is valid or not
ioopm_option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// keys of negative numerical value will be converted to absolute value
/// @return the value mapped to by key
ioopm_option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

/// @brief returns the number of key => value entries in the hash table
/// @param ht hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param ht hash table operated upon
/// @return true if size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param ht hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param ht hash table operated upon
/// @return a linked list of keys for hash table ht 
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param ht hash table operated upon
/// @return a linked list of values for hash table ht 
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param ht hash table operated upon
/// @param key the key sought
/// keys of negative numerical value will be converted to absolute value
/// @return true if key exists in ht, else false
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param ht hash table operated upon
/// @param value the value sought
/// @return true if value exists in ht, else false
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return true if pred holds for all entries in ht, else false
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return true if pred holds for at least one entry in ht, else false
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg);