#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "private_resources.h"

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

// Helper function used in hash_table_tests.c to test apply_to_all TODO: Move somewhere else?
bool is_key_divisable(ioopm_eq_function key_eq, ioopm_eq_function ignored_eq, elem_t key, elem_t ignored_value, void *x) {
  int *dividor_pointer = x;
  int dividor = *dividor_pointer;
  if (key.integer % dividor == 0) {
    return true;
  }
  return false;
}

bool compare_strings(elem_t a, elem_t b) {
    char *str1 = a.pointer;
    char *str2 = b.pointer;
    if (!strcmp(str1, str2)) {
        return true;
    } else {
        return false;
    }
}

bool compare_integers(elem_t a, elem_t b) {
    return a.integer == b.integer;
}

void change_char_value(elem_t ignored_value, elem_t *value, void *x) {
    elem_t *pointer_value = x;
    elem_t new_value = *pointer_value;
    char *new_char = new_value.pointer;
    value->pointer = new_char;
}

//new comment to test github integration tests

void test_create_destroy() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    CU_ASSERT_PTR_NOT_NULL(ht);
    ioopm_hash_table_destroy(ht);
}

void test_insert_once() { // Q49
    elem_t key = int_elem(5);
    elem_t value = ptr_elem("Hello there");
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key).success); // Check that key is not in ht yet TODO: Necessary?
    ioopm_hash_table_insert(ht, key, value);
    char *look_up_value = ioopm_hash_table_lookup(ht, key).value.pointer; 
    CU_ASSERT_STRING_EQUAL(value.pointer, look_up_value);
    ioopm_hash_table_destroy(ht);
}

void test_insert_existing_key() {
    elem_t key = int_elem(5);
    elem_t value = ptr_elem("Hello there");
    elem_t other_value = ptr_elem("Goodbye");
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, key, value);
    // Check if value of key is value
    CU_ASSERT_STRING_EQUAL(value.pointer, ioopm_hash_table_lookup(ht, key).value.pointer);
    
    ioopm_hash_table_insert(ht, key, other_value);
    // Check if value of key has changed to oter_value
    CU_ASSERT_STRING_EQUAL(other_value.pointer, ioopm_hash_table_lookup(ht, key).value.pointer);

    ioopm_hash_table_destroy(ht);
}

void test_insert_negative_key() {
    elem_t key = int_elem(-5);
    elem_t value = ptr_elem("Hello there");
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    
    ioopm_hash_table_insert(ht, key, value);
    char *look_up_value = ioopm_hash_table_lookup(ht, key).value.pointer;

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key).success); // Make sure inserted element is in ht
    CU_ASSERT_STRING_EQUAL(look_up_value, value.pointer); // Make sure the corect value is in ht

    ioopm_hash_table_destroy(ht);
}

void test_insert_empty_value() {
    elem_t key = int_elem(2);
    elem_t value = ptr_elem("");
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, key, value);

    char *look_up_value = ioopm_hash_table_lookup(ht, key).value.pointer;

    CU_ASSERT_STRING_EQUAL(look_up_value, value.pointer);
    ioopm_hash_table_destroy(ht);
}

void test_insert_null_value() {
    elem_t key = int_elem(2);
    elem_t value = ptr_elem(NULL);
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, key, value);

    char *look_up_value = ioopm_hash_table_lookup(ht, key).value.pointer;

    CU_ASSERT_PTR_NULL(look_up_value);

    ioopm_hash_table_destroy(ht);
}

void test_insert_to_same_bucket() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    char *values[] = {"first", "second", "third"};
    for (int i = 0; i < 3; i++) {
        ioopm_hash_table_insert(ht, int_elem(i*17), ptr_elem(values[i]));
    }
    for (int i = 0; i < 3; i++) {
        char *look_up_value = ioopm_hash_table_lookup(ht, int_elem(i*17)).value.pointer;
        CU_ASSERT_STRING_EQUAL(look_up_value, values[i]);
    }
    ioopm_hash_table_destroy(ht);
}

void test_lookup_empty() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);

    // Iterate over each bucket and make sure it is empty
    /* for (int i = 0; i < Primes[0]+1; ++i) { // TODO: 1 more than No_Buckets, why?
        CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(i)).success);
    } */
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(-1)).success);
    ioopm_hash_table_destroy(ht);
}

void test_lookup_non_existing_key() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(8)).success);
    ioopm_hash_table_destroy(ht);
}

void test_lookup_negative_key() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(-2), ptr_elem("Hello"));
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(-2)).success);
    ioopm_hash_table_destroy(ht);
}

void test_removing_key_from_single_input_ht() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("hej"));
    // Remove entry with key 5, and assign it's value to str
    char *str = ioopm_hash_table_remove(ht, int_elem(5)).value.pointer;
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(5)).success);
    // Check that removed value is equal to inserted string
    CU_ASSERT_STRING_EQUAL("hej", str);

    ioopm_hash_table_destroy(ht);
}
void test_removing_key_from_multiple_input_ht() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    char *values[] = {"first", "second", "third"};
    for (int i = 0; i < 3; i++) {
        ioopm_hash_table_insert(ht, int_elem(i*17), ptr_elem(values[i]));
    }
    
    // Remove entry with key 17, and assign it's value to str
    char *str = ioopm_hash_table_remove(ht, int_elem(17)).value.pointer;
    // Check that removed value is equal to inserted string
    CU_ASSERT_STRING_EQUAL("second", str);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2);

    ioopm_hash_table_destroy(ht);
}

void test_removing_non_existing_key() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);

    // Remove function should return an option_t, who's value will be
    // initialized to NULL 
    char *str = ioopm_hash_table_remove(ht, int_elem(5)).value.pointer;
    CU_ASSERT_PTR_NULL(str);

    ioopm_hash_table_destroy(ht);
}

void test_size_empty_hash_table() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
    ioopm_hash_table_destroy(ht);
}

void test_size_single_entry_hash_table() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("hej"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
    ioopm_hash_table_destroy(ht);
}

void test_size_multiple_entry_hash_table() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    for (int i = 0; i < 34; i++) {
        ioopm_hash_table_insert(ht, int_elem(i), ptr_elem("hej"));
    }
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 34);
    ioopm_hash_table_destroy(ht);
}

void test_size_increase() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("hej"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
    ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("tjena"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2);
    ioopm_hash_table_destroy(ht);
}

void test_size_decrease() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("hej"));
    ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("tjena"));
    elem_t removed_value = ioopm_hash_table_remove(ht, int_elem(4)).value; // Returned value ignored, just intrerested in side effect
    removed_value = removed_value; //Cheat to get rid of warning saying "unused variable"
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
    ioopm_hash_table_destroy(ht);
}

void test_if_non_empty_hash_table_is_empty() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(7), ptr_elem("tjabba"));
    CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
    ioopm_hash_table_destroy(ht);
}

void test_if_empty_hash_table_is_empty() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
    ioopm_hash_table_destroy(ht);
}

void test_clear_hash_table_single_entry() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(10), ptr_elem("hej"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(10)).success);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(2)).success);

    ioopm_hash_table_clear(ht);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(10)).success);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(2)).success);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("hejsan")); // Make sure we can use ht again
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(10)).success); //Looks at bucket 10 after clear, corrupt? FIXED
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(2)).success);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
    ioopm_list_t *keys = ioopm_hash_table_keys(ht);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(keys), 1); //Looks at bucket 10 after clear, corrupt? FIXED

    ioopm_hash_table_clear(ht); //Looks at bucket 10, corrupt? FIXED
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(2)).success); //Looks at bucket 2 after clear, corrupt? FIXED

    ioopm_linked_list_destroy(keys);
    ioopm_hash_table_destroy(ht);

}

void test_clear_hash_table_multiple_entry() { // Q49
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    for (int i = 0; i < 34; i++) {
        ioopm_hash_table_insert(ht, int_elem(i), ptr_elem("hej"));
    }

    ioopm_hash_table_clear(ht);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(9)).success); //Looks at bucket 10 after clear, corrupt? FIXED
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("hej")); // Make sure we can use ht again
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(2)).success); //Looks at bucket 10 after clear, corrupt? FIXED

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_keys() {
    int keys[5] = {3, 10, 20, 0, 99}; // 3 and 20 will map på same bucket when there are 17 buckets
    bool found_record[5] = {false}; // Keeps track of which keys we find
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);

    // Iterate over keys and insert to hash table
    for (int i = 0; i < 5; i++) {
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem("hej"));
    }

    ioopm_list_t *keys_found = ioopm_hash_table_keys(ht); // Get keys from ht
    int length_of_new_keys = ioopm_linked_list_size(keys_found);

    // Iterate over keys you inserted to ht
    for (int i = 0; i < length_of_new_keys; i++) {
        if (ioopm_linked_list_contains(keys_found, int_elem(keys[i]))) {
            found_record[i] = true;
        } else {
            CU_FAIL("Found a key that was never inserted!");
        }
    }

    // Iterate over found-array to make sure we found all keys
    for (int i = 0; i < 5; i++) {
        if (!found_record[i]) {
            CU_FAIL("One key wasn't found!");
        }
    }

    ioopm_linked_list_destroy(keys_found);
    ioopm_hash_table_destroy(ht);
}

//Q50: new test added after gcov
void test_hash_table_values_empty_hash_table() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_list_t *found_values = ioopm_hash_table_values(ht); // Get values from ht
    CU_ASSERT_EQUAL(ioopm_linked_list_size(found_values), 0);
    CU_ASSERT_PTR_NULL(found_values->first->next);
    ioopm_linked_list_destroy(found_values);
    ioopm_hash_table_destroy(ht);
}

void test_hash_table_values() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    char *values[5] = {"hej", "hejsan", "tja", "tjenixen", "tjena"};
    bool found[5] = {false}; // Keeps track of which keys we find

    // Fill ht
    for (int i = 0; i < 5; i++) {
        ioopm_hash_table_insert(ht, int_elem(i), ptr_elem(values[i]));
    }

    ioopm_list_t *found_values = ioopm_hash_table_values(ht); // Get values from ht
    int length_of_values = ioopm_linked_list_size(found_values);

    // Iterate over values found in ht
    for (int i = 0; i < length_of_values; i++) {
        if (ioopm_linked_list_contains(found_values, ptr_elem(values[i]))) {
            found[i] = true;
        } else {
            CU_FAIL("Found a value that was never inserted!");
        }
    }

    // Iterate over found-array to make sure we found all values
    for (int i = 0; i < 5; i++) {
        if (!found[i]) {
            CU_FAIL("One value wasn't found!");
        }
    }

    ioopm_linked_list_destroy(found_values);
    ioopm_hash_table_destroy(ht);
}

void test_hash_table_keys_empty() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_list_t *new_keys = ioopm_hash_table_keys(ht);
    CU_ASSERT_PTR_NULL(new_keys->first->next); // Check if linked list of found keys is empty

    ioopm_linked_list_destroy(new_keys);
    ioopm_hash_table_destroy(ht);
}

void test_hash_tables_keys_and_values() {
    int keys_to_insert[5] = {3, 10, 20, 0, 99}; // 3 and 20 will map to same bucket if there are 17 buckets
    char *values_to_insert[5] = {"three", "ten", "twenty", "zero", "ninetynine"};
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);

    // Fill ht
    for (int i = 0; i <5; i++){
        ioopm_hash_table_insert(ht, int_elem(keys_to_insert[i]), ptr_elem(values_to_insert[i]));
    }

    ioopm_list_t *found_keys = ioopm_hash_table_keys(ht); // Get keys from ht
    ioopm_list_t *found_values = ioopm_hash_table_values(ht); // Get values from ht

    // Iterate over found keys and found values
    for (int i = 0; i < 5; i++) {
        int found_key = ioopm_linked_list_get(found_keys, i).integer;
        char *found_value = ioopm_linked_list_get(found_values, i).pointer;

        // Iterate over inserted keys_to_insert and values_to_insert
        // and check that keys and values at current index are equivalent
        for (int j = 0; j < 5; j++) {
            if (found_key == keys_to_insert[j]) {
                CU_ASSERT_STRING_EQUAL(found_value, values_to_insert[j]);
            }
        }
    }

    ioopm_linked_list_destroy(found_keys);
    ioopm_linked_list_destroy(found_values);
    ioopm_hash_table_destroy(ht);
}

void test_has_key_single_entry() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("hej"));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(4)));

    ioopm_hash_table_destroy(ht);
}

void test_has_key_multiple_entries() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    int keys_to_insert[5] = {3, 10, 20, 0, 99}; // 3 and 20 will map to same bucket if there are 17 buckets
    char *values_to_insert[5] = {"three", "ten", "twenty", "zero", "ninetynine"};
    for (int i = 0; i <5; i++){
        ioopm_hash_table_insert(ht, int_elem(keys_to_insert[i]), ptr_elem(values_to_insert[i]));
    }
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(20)));
    ioopm_hash_table_destroy(ht);
}

void test_has_key_invalid_key() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("hej"));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(7)));

    ioopm_hash_table_destroy(ht);
}

void test_has_key_empty_hash_table() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(7)));

    ioopm_hash_table_destroy(ht);
}

void test_has_value_single_entry() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    int keys_to_insert[5] = {3, 10, 20, 0, 99}; // 3 and 20 will map to same bucket if there are 17 buckets
    char *values_to_insert[5] = {"three", "ten", "twenty", "zero", "ninetynine"};
    for (int i = 0; i <5; i++){
        ioopm_hash_table_insert(ht, int_elem(keys_to_insert[i]), ptr_elem(values_to_insert[i]));
    }
    CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem(values_to_insert[0]))); 
    ioopm_hash_table_destroy(ht);
}

void test_has_value_multiple_entries() {
    char *value = "hej";
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem(value));
    CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem(value))); 
    ioopm_hash_table_destroy(ht);
}

void test_has_value_invalid_value() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("hej"));
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("hallå")));
    ioopm_hash_table_destroy(ht);
}

void test_has_value_empty_hash_table() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("hallå")));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_all_all_holds() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(8), ptr_elem("hej"));
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("hejsan"));
    int divisor = 2;
    CU_ASSERT_TRUE(ioopm_hash_table_all(ht, is_key_divisable, &divisor));
    ioopm_hash_table_destroy(ht);
}

void test_hash_table_all_one_holds() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(8), ptr_elem("hej"));
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("hejsan"));
    int divisor = 8;
    CU_ASSERT_FALSE(ioopm_hash_table_all(ht, is_key_divisable, &divisor));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_all_none_holds() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    ioopm_hash_table_insert(ht, int_elem(8), ptr_elem("hej"));
    ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("hejsan"));
    int divisor = 3;
    CU_ASSERT_FALSE(ioopm_hash_table_all(ht, is_key_divisable, &divisor));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_all_empty_hash_table() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    int divisor = 2;
    CU_ASSERT_FALSE(ioopm_hash_table_all(ht, is_key_divisable, &divisor));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_apply_to_all() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    for (int i = 0; i < 4; i++) {
        ioopm_hash_table_insert(ht, int_elem(i), ptr_elem("Hello"));
    }

    elem_t new_value = ptr_elem("hej");
    ioopm_hash_table_apply_to_all(ht, change_char_value, &new_value);
    CU_ASSERT_TRUE(ioopm_hash_table_all(ht, compare_values, &new_value));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_apply_to_all_empty_ht() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(compare_strings, compare_integers, NULL);
    
    elem_t new_value = ptr_elem("hej");
    ioopm_hash_table_apply_to_all(ht, change_char_value, &new_value);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

    ioopm_hash_table_destroy(ht);
}

int main(int argc, char *argv[]) {
    // Try to set up CUnit, exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // Create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite my_test_suite = CU_add_suite("Test suite for hash tables", init_suite, clean_suite);
    if (my_test_suite == NULL) {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
            (CU_add_test(my_test_suite, "Create/destroy ht                                  ", test_create_destroy) == NULL) ||
            (CU_add_test(my_test_suite, "Insert once in ht                                  ", test_insert_once) == NULL) ||
            (CU_add_test(my_test_suite, "Insert already existing key in ht                  ", test_insert_existing_key) == NULL) ||
            (CU_add_test(my_test_suite, "Insert negative key in ht                          ", test_insert_negative_key) == NULL) ||
            (CU_add_test(my_test_suite, "Insert empty value in ht                           ", test_insert_empty_value) == NULL) ||
            (CU_add_test(my_test_suite, "Insert null value in hash table ht                 ", test_insert_null_value) == NULL) ||
            (CU_add_test(my_test_suite, "Insert multiple elements to same bucket            ", test_insert_to_same_bucket) == NULL) ||
            (CU_add_test(my_test_suite, "Look up an empty ht                                ", test_lookup_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Look up negative key                               ", test_lookup_negative_key) == NULL) ||
            (CU_add_test(my_test_suite, "Look up a non-existing key                         ", test_lookup_non_existing_key) == NULL) ||
            (CU_add_test(my_test_suite, "Remove existing key from ht with one entry         ", test_removing_key_from_single_input_ht) == NULL) ||
            (CU_add_test(my_test_suite, "Remove existing key from ht with multiple entries  ", test_removing_key_from_multiple_input_ht) == NULL) ||
            (CU_add_test(my_test_suite, "Remove non-existing key                            ", test_removing_non_existing_key) == NULL) ||
            (CU_add_test(my_test_suite, "Size of empty ht                                   ", test_size_empty_hash_table) == NULL) ||
            (CU_add_test(my_test_suite, "Size of single entry ht                            ", test_size_single_entry_hash_table) == NULL) ||
            (CU_add_test(my_test_suite, "Size of multiple entry ht                          ", test_size_multiple_entry_hash_table) == NULL) ||
            (CU_add_test(my_test_suite, "If size changes when new entry is added            ", test_size_increase) == NULL) ||
            (CU_add_test(my_test_suite, "If size changes when an entry is removed           ", test_size_decrease) == NULL) ||
            (CU_add_test(my_test_suite, "If non-empty ht is empty                           ", test_if_non_empty_hash_table_is_empty) == NULL) ||
            (CU_add_test(my_test_suite, "If empty ht is empty                               ", test_if_empty_hash_table_is_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Clear single entry ht                              ", test_clear_hash_table_single_entry) == NULL) ||
            (CU_add_test(my_test_suite, "Clear multiple entry ht                            ", test_clear_hash_table_multiple_entry) == NULL) ||
            (CU_add_test(my_test_suite, "Getting keys                                       ", test_hash_table_keys) == NULL) ||
            (CU_add_test(my_test_suite, "Getting keys from empty ht                         ", test_hash_table_keys_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Getting values from empty ht                       ", test_hash_table_values_empty_hash_table) == NULL) ||
            (CU_add_test(my_test_suite, "Getting values from ht                             ", test_hash_table_values) == NULL) ||
            (CU_add_test(my_test_suite, "Match values and keys                              ", test_hash_tables_keys_and_values) == NULL) ||
            (CU_add_test(my_test_suite, "Has key in single entry ht                         ", test_has_key_single_entry) == NULL) ||
            (CU_add_test(my_test_suite, "Has key in multiple entries ht                     ", test_has_key_multiple_entries) == NULL) ||
            (CU_add_test(my_test_suite, "Has non-existing key                               ", test_has_key_invalid_key) == NULL) ||
            (CU_add_test(my_test_suite, "Has key with and empty ht                          ", test_has_key_empty_hash_table) == NULL) ||
            (CU_add_test(my_test_suite, "Has value single entry                             ", test_has_value_single_entry) == NULL) ||
            (CU_add_test(my_test_suite, "Has value multiple entries                         ", test_has_value_multiple_entries) == NULL) ||
            (CU_add_test(my_test_suite, "Has non-existing value                             ", test_has_value_invalid_value) == NULL) ||
            (CU_add_test(my_test_suite, "Has value with an empty ht                         ", test_has_value_empty_hash_table) == NULL) ||
            (CU_add_test(my_test_suite, "Predicate holds for all                            ", test_hash_table_all_all_holds) == NULL) ||
            (CU_add_test(my_test_suite, "Predicate holds for one                            ", test_hash_table_all_one_holds) == NULL) ||
            (CU_add_test(my_test_suite, "Predicate holds for none                           ", test_hash_table_all_none_holds) == NULL) ||
            (CU_add_test(my_test_suite, "Predicate on empty ht                              ", test_hash_table_all_empty_hash_table) == NULL) ||
            (CU_add_test(my_test_suite, "Apply some function to all entries                 ", test_hash_table_apply_to_all) == NULL) ||
            (CU_add_test(my_test_suite, "Apply some function to empty ht                    ", test_hash_table_apply_to_all_empty_ht) == NULL) ||
            0)
    {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }
    // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
    // Use CU_BRM_NORMAL to only print errors and a summary
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // This is where the tests are actually run!
    CU_basic_run_tests();

    if (CU_get_number_of_failures() > 0 && argc > 1 && !(strcmp(argv[1], "--error-on-fail"))) {
        CU_cleanup_registry();
        return 1;
    }

    // Tear down CUnit before exiting
    CU_cleanup_registry();
    return CU_get_error();
}