
#include <stdbool.h>
#include <stdlib.h>
#include <printf.h>
#include <stdio.h>

#include "private_resources.h"
#include "hash_table.h"
#include "linked_list.h"

#define Success(v) (ioopm_option_t) {.success = true, .value = v}; //Returns an ioopm_option_t with values {success = true, value = v}
#define Failure() (ioopm_option_t) {.success = false};             //Returns an ioopm_option_t with values {success = false, value = NULL}

typedef struct entry entry_t;
struct entry {
    elem_t key;
    elem_t value;
    entry_t *next;
};

typedef void (apply_entry_function)(ioopm_hash_table_t *ht, entry_t *entry, void *extra);

struct hash_table {
    entry_t **buckets; 
    float load_factor;
    size_t no_buckets;
    size_t size; // Number of elements in hash_table
    ioopm_eq_function value_eq;
    ioopm_eq_function key_eq;
    ioopm_hash_function key_convert;
};

static int extract_int_hash_key(elem_t key) { 
    return key.integer;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_eq_function value_eq, ioopm_eq_function key_eq, ioopm_hash_function key_convert) {
    ioopm_hash_table_t *hash_table = calloc(1, sizeof(ioopm_hash_table_t));
    hash_table->load_factor = Load_Factor;
    hash_table->no_buckets = Primes[0];
    hash_table->buckets = calloc(hash_table->no_buckets, sizeof(entry_t));
    hash_table->size = 0;
    hash_table->key_eq = key_eq;
    hash_table->value_eq = value_eq;

    hash_table->key_convert = key_convert;
    if (hash_table->key_convert == NULL) {  // Set default hash-function to handle integers as keys
        hash_table->key_convert = extract_int_hash_key;
    }

    return hash_table;
}

static void hash_table_resize(ioopm_hash_table_t *ht) {
    int size_options = sizeof(Primes) / sizeof(Primes[0]); // Get number of size options
    if (ht->no_buckets == Primes[size_options]) { // If ht is already at max-size, dont do anything 
        return;
    }

    // Cache keys and values from ht
    ioopm_list_t *keys = ioopm_hash_table_keys(ht);
    ioopm_list_t *values = ioopm_hash_table_values(ht);

    ioopm_hash_table_clear(ht); // Get rid of old buckets array
    free(ht->buckets);

    for (int i = 0; i < size_options; i++) {
        if (ht->no_buckets == Primes[i]) {
            ht->no_buckets = Primes[i+1];
            break;
        }
    }

    ht->buckets = calloc(ht->no_buckets, sizeof(entry_t)); // Create new buckets, one size larger than before
    size_t size_of_keys = ioopm_linked_list_size(keys);
    for (int i = 0; i < size_of_keys; i++) {
        ioopm_hash_table_insert(ht, ioopm_linked_list_get(keys, i), ioopm_linked_list_get(values, i));
    }

    ioopm_linked_list_destroy(keys);
    ioopm_linked_list_destroy(values);
}

static void entry_destroy(entry_t *entry) {
    free(entry);
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) { 
    ioopm_hash_table_clear(ht); //Destroy every entry in each bucket and the buckets array
    free(ht->buckets); //Deallocate hash table data structure
    free(ht);
}

/* static entry_t *auxillary_find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *dummy, elem_t key) {
    entry_t *previous = dummy;
    entry_t *cursor = previous;
    do { // Update cursor until we find matching key, or hit end of bucket
            cursor = cursor->next;
            if (ht->key_eq(cursor->key, key)) {
                return previous;
            }
            previous = cursor;
        } while (cursor->next != NULL);
    return previous;
}

static entry_t *find_previous_entry_for_key (ioopm_hash_table_t *ht, entry_t *dummy, elem_t key) {
    entry_t *previous = dummy;
    if (dummy->next == NULL) { // If list is empty, previous entry is the dummy
        return previous;
    } 
    return auxillary_find_previous_entry_for_key(ht, dummy, key); 
} */

/*static entry_t **find_entry_for_key (ioopm_hash_table_t *ht, entry_t **current_entry, elem_t key) {
    while (*current_entry && !(ht->key_eq((*current_entry)->key, key))) { // check if we're at the end of the list, or if we've the key
        current_entry = &(*current_entry)->next;
    }
    return current_entry; // return entry with correct key, or first entry if no key was found
}*/

/*
static entry_t **find_entry_for_key (ioopm_hash_table_t *ht, entry_t **current_entry_in_bucket, elem_t key) {
    while ((*current_entry_in_bucket) != NULL) {
        if (ht->key_eq((*current_entry_in_bucket)->key, key)) {
            return current_entry_in_bucket;
        } else {
            current_entry_in_bucket = &(*current_entry_in_bucket)->next;
        }
    }
    return current_entry_in_bucket;
}

*/
//Svansrekursiv funktion. Value semantics reference semantics
static entry_t **find_entry_for_key(ioopm_hash_table_t *ht, entry_t **current_entry_in_bucket, elem_t key) {
    if ((*current_entry_in_bucket) != NULL) {
        if (ht->key_eq((*current_entry_in_bucket)->key, key)) {
            return current_entry_in_bucket;
        } else {
            return find_entry_for_key(ht, &(*current_entry_in_bucket)->next, key);
        }
    } else {
        return current_entry_in_bucket;
    }
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next) {
    entry_t *new_entry = calloc(1, sizeof(entry_t));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = next;
    return new_entry;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) {
    // Check if hash table needs resizing
    if (ht->size > ht->load_factor * ht->no_buckets) {
        hash_table_resize(ht);
    }

    int valid_key = abs(ht->key_convert(key));
    int bucket = valid_key % ht->no_buckets; // Calculate bucket entry to insert
    entry_t **entry_position = find_entry_for_key(ht, &ht->buckets[bucket], key); // Get position for to new entry

    if (*entry_position && ht->key_eq((*entry_position)->key, key)) { // If entry at entry_position has the key we want to insert
        (*entry_position)->value = value; //update value
    } else {
        //Insert new entry at first place
        entry_t *new_entry = entry_create(key, value, *entry_position); // Create new entry and point to previous first entry
        *entry_position = new_entry; // Make new entry the first element in buckets
        ht->size += 1;
    }
}

ioopm_option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key) {
    int valid_key = abs(ht->key_convert(key));
    int bucket = valid_key % ht->no_buckets; //Calculate bucket entry to lookup
    entry_t **entry_position = find_entry_for_key(ht, &ht->buckets[bucket], key); //Get entry we're looking for

    if (*entry_position && ht->key_eq((*entry_position)->key, key)) { //Check if entry_position exists and if it has the key we're looking for
        return Success((*entry_position)->value); //Return successfull find (an ioopm_option_t containing the value of key we're looking for and bool success = true)
    } else {//Key was not found
        return Failure(); //Return failing find (an ioopm_option_t containing the value NULL and bool success = false)
    }
}

ioopm_option_t remove_entry (ioopm_hash_table_t *ht, int bucket_index, elem_t key) {
    entry_t **entry_position = find_entry_for_key(ht, &ht->buckets[bucket_index], key); //Get position of entry we want to remove
    elem_t removed_value = (*entry_position)->value; //Get value to remove
    entry_t *entry_to_remove = *entry_position; // pointer to value we want to remove
    *entry_position = (*entry_position)->next; // links the list correctly

    entry_destroy(entry_to_remove); //Destroy entry to remove
    ht->size -= 1;
    return Success(removed_value); //Return removed value
}

ioopm_option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key) {
    if (ioopm_hash_table_lookup(ht, key).success) { //Check if entry with key exists in ht
        int valid_key = abs(ht->key_convert(key));
        int bucket_index = valid_key % ht->no_buckets; //Calculate which bucket entry key is in

        return remove_entry(ht, bucket_index, key);
    }
    return Failure(); //Return failure if key doesn't exist in ht
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht) {
    return ht->size; //Return number of entries in ht
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht) {
    return (ioopm_hash_table_size(ht) == 0);
}

static void get_keys(ioopm_hash_table_t *ignored, entry_t *entry, void *linked_list) {
    ioopm_list_t *list = linked_list;
    elem_t key = entry->key;
    ioopm_linked_list_append(list, key);
}

static void iterate_over_hash_table(ioopm_hash_table_t *ht, apply_entry_function function, void *extra) {
    //Iterate over buckets in the buckets array
    for (int i = 0; i < ht->no_buckets; i++) {
        entry_t **cursor = &ht->buckets[i];
        while (*cursor) { // For each bucket, iterate over its entries and call function
            entry_t **next = &(*cursor)->next;
            function(ht, *cursor, extra); //apply function to current entry
            cursor = next; //Step forward to next entry, and repeat loop
        }
    }
}

static void clear_entry(ioopm_hash_table_t *ht, entry_t *entry, void* extra) {
    entry_destroy(entry);
    ht->size -= 1;
}

/*void ioopm_hash_table_clear(ioopm_hash_table_t *ht){
    if (ioopm_hash_table_is_empty(ht)) {
        printf("\nHT empty\n");
        return;
    } else {
        iterate_over_hash_table(ht, clear_entry, NULL);
    }
}*/

/*
void ioopm_hash_table_clear(ioopm_hash_table_t *ht) {
    for (int i = 0; i < ht->no_buckets; i++) {
        entry_t *first = (ht->buckets[i]);
        while (first != NULL) {
            entry_t *to_destroy = first;
            first = first->next;
            entry_destroy(to_destroy);
            ht->size -= 1;
        }
    }
}
*/

void ioopm_hash_table_clear(ioopm_hash_table_t *ht) {
    for (int i = 0; i < ht->no_buckets; i++) {
        entry_t **cursor = &ht->buckets[i];
        while (*cursor) {
            entry_t *to_destroy = *cursor;
            (*cursor) = (*cursor)->next;
            entry_destroy(to_destroy);
            ht->size--;
        }
    }
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht) {
    ioopm_list_t *keys = ioopm_linked_list_create(ht->key_eq);
    if (ioopm_hash_table_is_empty(ht)) {
        return keys;
    }
    iterate_over_hash_table(ht, get_keys, keys);
    return keys;
}

static void get_values(ioopm_hash_table_t *ignored, entry_t *entry, void *linked_list) {
    ioopm_list_t *list = linked_list;
    elem_t value = entry->value;
    ioopm_linked_list_append(list, value);
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht) {
    ioopm_list_t *values = ioopm_linked_list_create(ht->key_eq);
    if (ioopm_hash_table_is_empty(ht)) {
        return values;
    }
    iterate_over_hash_table(ht, get_values, values);
    return values;
}

static bool compare_keys(ioopm_eq_function key_eq, ioopm_eq_function ignored_eq, elem_t key, elem_t ignore_value, void *x) {
    elem_t *key_pointer = x; // State that x is a pointer to an int
    elem_t key_to_find = *key_pointer;
    return key_eq(key, key_to_find);
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key) {
    return ioopm_hash_table_any(ht, compare_keys, &key);
}

bool compare_values(ioopm_eq_function ignored_eq, ioopm_eq_function value_eq, elem_t ignored_key, elem_t value, void *x) {
    elem_t *value_to_find = x;
    elem_t compared_value = *value_to_find;
    return value_eq(value, compared_value);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value) {
    return ioopm_hash_table_any(ht, compare_values, &value);
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) {
    if (ioopm_hash_table_size(ht) == 0) {
        return false; // The predicate cannot hold for all if ht is empty
    }
    for (int i = 0; i < ht->no_buckets; i++) {
        entry_t *cursor = &(*ht->buckets[i]); //Pointer to the first entry in the bucket
        entry_t *next_entry = cursor; //initiate cache

        while (cursor != NULL) { // For each bucket, iterate over its entries and call function
            next_entry = cursor->next; //Update next_entry to be entry after cursor
            if (!pred(ht->key_eq, ht->value_eq, cursor->key, cursor->value, arg)) { // Call predicate function on current entry
                return false; // Exit and return false if the predicate doesn't hold
            }
            cursor = next_entry; //Step forward to next entry, and repeat loop
        }
    }
    return true; // We only come here if the predicate holds for all entries
}



bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) {
    if (ht->size == 0) {
        return false; // The predicate cannot hold for any if ht is empty
    }
    for (int i = 0; i < ht->no_buckets; i++) {
        entry_t *cursor = &(*ht->buckets[i]); //Pointer to the first entry in the bucket
        entry_t *next_entry = cursor; //initiate cache


        while (cursor != NULL) { // For each bucket, iterate over its entries and call function
            next_entry = cursor->next; //Update next_entry to be entry after cursor
            if (pred(ht->key_eq, ht->value_eq, cursor->key, cursor->value, arg)) { // Call predicate function on current entry
                return true; // Exit and return true if the predicate holds
            }
            cursor = next_entry; //Step forward to next entry, and repeat loop
        }
    }
    return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg) {
    if (ioopm_hash_table_is_empty(ht)) {
        return; // Cannot do anything if ht is empty
    }
    for (int i = 0; i < ht->no_buckets; i++) {
        entry_t *cursor = &(*ht->buckets[i]); //Pointer to the first entry in the bucket
        entry_t *next_entry = cursor; //initiate cache

        while (cursor != NULL) { // For each bucket, iterate over its entries and call function
            next_entry = cursor->next; //Update next_entry to be entry after cursor
            apply_fun(cursor->key, &cursor->value, arg); // Call function on current entry
            cursor = next_entry; //Step forward to next entry, and repeat loop
        }
    }
}