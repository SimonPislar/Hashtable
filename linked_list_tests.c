#include <CUnit/Basic.h>
#include <stdbool.h>
#include "linked_list.h"
#include "private_resources.h"

bool is_even(elem_t a, void *ignored_extra) {
    int key = a.integer;
    return key % 2 == 0;
}

void add_num(elem_t *value, void *num) {
    int *number = num;
    int the_number = *number;
    *value = int_elem(value->integer + the_number);
}

bool compare_int_elements(elem_t a, elem_t b) {
  return b.integer == a.integer;
}

bool compare_string_elements(elem_t a, elem_t b) {
    if (!strcmp(a.pointer, b.pointer)) { //strcmp returns 0 if strings are equal
        return true;
    }
    return false;
}

bool compare_element_to_string(elem_t a, void *b) {
    char *casted_b = b;
    if (!strcmp(a.pointer, casted_b)) { //strcmp returns 0 if strings are equal
        return true;
    }
    return false;
}

int init_suite(void) {
    // Change this function if you want to do something *before* you
    // run a test suite
    return 0;
}

int clean_suite(void) {
    // Change this function if you want to do something *after* you
    // run a test suite
    return 0;
}

// These are example test functions. You should replace them with
// functions of your own.

void test_ioopm_linked_list_create_destroy() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    CU_ASSERT_PTR_NOT_NULL(list);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_append_int_empty() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(7));
    CU_ASSERT_PTR_NOT_NULL(list->first->next);
    CU_ASSERT_PTR_NOT_NULL(list->last);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_append_string_empty() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_string_elements);
    ioopm_linked_list_append(list, ptr_elem("Hej"));
    CU_ASSERT_PTR_NOT_NULL(list->first->next);
    CU_ASSERT_PTR_NOT_NULL(list->last);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_append_to_non_empty() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(7));
    ioopm_linked_list_append(list, int_elem(4));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) 7));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) 4));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_append_empty_string_to_non_empty() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_string_elements);
    ioopm_linked_list_append(list, ptr_elem("hypernevrokustiskakontradiafragmavibrationer"));
    ioopm_linked_list_append(list, ptr_elem(""));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, ptr_elem("hypernevrokustiskakontradiafragmavibrationer")));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, ptr_elem("")));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_append_negative() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(-7));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) -7));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) -7));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_prepend_int_empty() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_prepend(list, int_elem(7));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) 7));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) 7));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_prepend_string_empty() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_string_elements);
    ioopm_linked_list_prepend(list, ptr_elem("hello"));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, ptr_elem("hello")));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, ptr_elem("hello")));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_prepend_to_non_empty() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_prepend(list, int_elem(7));
    ioopm_linked_list_prepend(list, int_elem(4));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) 4));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) 7));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_prepend_negative() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_prepend(list, int_elem(-7));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) -7));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) -7));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_insert_int_empty() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_insert(list, 0, int_elem(7));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) 7));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) 7));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_insert_string_empty() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_string_elements);
    ioopm_linked_list_insert(list, 0, ptr_elem("tja"));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, ptr_elem("tja")));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, ptr_elem("tja")));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_insert_to_non_empty_in_first_spot() { // TODO: Aren't we supposed to change index 0 from 7 to 5?
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_insert(list, 0, int_elem(7));
    ioopm_linked_list_insert(list, 0, int_elem(5));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) 5));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) 7));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_insert_to_non_empty_in_last_spot() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_insert(list, 0, int_elem(7));
    ioopm_linked_list_insert(list, 1, int_elem(5));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) 7));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) 5));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_insert_int_to_non_empty_in_middle_spot() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_insert(list, 0, int_elem(7));
    ioopm_linked_list_insert(list, 1, int_elem(5));
    ioopm_linked_list_insert(list, 1, int_elem(3));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) 7));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) 5));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_insert_string_to_non_empty_in_middle_spot() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_string_elements);
    ioopm_linked_list_insert(list, 0, ptr_elem("hejsan"));
    ioopm_linked_list_insert(list, 1, ptr_elem("Simon"));
    ioopm_linked_list_insert(list, 1, ptr_elem("Ida"));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, ptr_elem("hejsan")));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, ptr_elem("Simon")));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_insert_long_string_to_non_empty_in_middle_spot() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_string_elements);
    ioopm_linked_list_insert(list, 0, ptr_elem("hejsan"));
    ioopm_linked_list_insert(list, 1, ptr_elem("Bilplåtsmålafärgspenselsputsmedelsförpackningstryckpressputspoleringsmedelsförvaringsställesluftsrengöringsmedelsförpackningskorksetikettspåklistrarmedelspaketsinnehållsförteckningsbläckfärgstillverkarsfabrikspostadress"));
    ioopm_linked_list_insert(list, 1, ptr_elem("penna"));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, ptr_elem("hejsan")));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, ptr_elem("Bilplåtsmålafärgspenselsputsmedelsförpackningstryckpressputspoleringsmedelsförvaringsställesluftsrengöringsmedelsförpackningskorksetikettspåklistrarmedelspaketsinnehållsförteckningsbläckfärgstillverkarsfabrikspostadress")));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_insert_negative() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_insert(list, 0, int_elem(-7));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) -7));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) -7));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_remove_int_from_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(5));
    ioopm_linked_list_append(list, int_elem(3));
    ioopm_linked_list_append(list, int_elem(2));
    elem_t removed_value = ioopm_linked_list_remove(list, 1);
    CU_ASSERT_TRUE(list->eq_function(removed_value, (elem_t) 3));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_remove_string_from_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_string_elements);
    ioopm_linked_list_append(list, ptr_elem("Hej"));
    ioopm_linked_list_append(list, ptr_elem("hejsan"));
    ioopm_linked_list_append(list, ptr_elem("tja"));
    elem_t removed_value = ioopm_linked_list_remove(list, 1);
    CU_ASSERT_TRUE(list->eq_function(removed_value, ptr_elem("hejsan")));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_remove_first() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(5));
    ioopm_linked_list_append(list, int_elem(3));
    elem_t removed_value = ioopm_linked_list_remove(list, 0);
    CU_ASSERT_TRUE(list->eq_function(removed_value, (elem_t) 5));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) 3));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) 3));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_remove_last() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(5));
    ioopm_linked_list_append(list, int_elem(3));
    ioopm_linked_list_append(list, int_elem(6));
    elem_t removed_value = ioopm_linked_list_remove(list, 2);
    CU_ASSERT_TRUE(list->eq_function(removed_value, (elem_t) 6));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) 5));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) 3));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_get_first() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(5));
    ioopm_linked_list_append(list, int_elem(16));
    ioopm_linked_list_append(list, int_elem(13));
    elem_t item = ioopm_linked_list_get(list, 0);
    CU_ASSERT_TRUE(list->eq_function(item, (elem_t) 5));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_get_last() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(5));
    ioopm_linked_list_append(list, int_elem(16));
    ioopm_linked_list_append(list, int_elem(13));
    elem_t item = ioopm_linked_list_get(list, 2);
    CU_ASSERT_TRUE(list->eq_function(item, (elem_t) 13));
    ioopm_linked_list_destroy(list);
}


void test_ioopm_linked_list_get_int_non_edge() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(5));
    ioopm_linked_list_append(list, int_elem(16));
    ioopm_linked_list_append(list, int_elem(13));
    elem_t item = ioopm_linked_list_get(list, 1);
    CU_ASSERT_TRUE(list->eq_function(item, (elem_t) 16));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_get_empty_string_non_edge() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_string_elements);
    ioopm_linked_list_append(list, ptr_elem("hejsan svejsan"));
    ioopm_linked_list_append(list, ptr_elem(""));
    ioopm_linked_list_append(list, ptr_elem("hej"));
    elem_t item = ioopm_linked_list_get(list, 1);
    CU_ASSERT_TRUE(list->eq_function(item, ptr_elem("")));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_get_string_non_edge() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_string_elements);
    ioopm_linked_list_append(list, ptr_elem("flagstangsknoppsputspoleringsmedel"));
    ioopm_linked_list_append(list, ptr_elem("hypernevrokustiskakontradiafragmavibrationer"));
    ioopm_linked_list_append(list, ptr_elem("hej"));
    elem_t item = ioopm_linked_list_get(list, 1);
    CU_ASSERT_TRUE(list->eq_function(item, ptr_elem("hypernevrokustiskakontradiafragmavibrationer")));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_contains_valid_item() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(5));
    ioopm_linked_list_append(list, int_elem(16));
    ioopm_linked_list_append(list, int_elem(13));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(list, (elem_t) 16));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(list, (elem_t) 13));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(list, (elem_t) 5));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_contains_wierd_string_item() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, ptr_elem("hej"));
    ioopm_linked_list_append(list, ptr_elem(""));
    ioopm_linked_list_append(list, ptr_elem("Bilplåtsmålafärgspenselsputsmedelsförpackningstryckpressputspoleringsmedelsförvaringsställesluftsrengöringsmedelsförpackningskorksetikettspåklistrarmedelspaketsinnehållsförteckningsbläckfärgstillverkarsfabrikspostadress"));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(list, ptr_elem("hej")));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(list, ptr_elem("")));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(list, ptr_elem("Bilplåtsmålafärgspenselsputsmedelsförpackningstryckpressputspoleringsmedelsförvaringsställesluftsrengöringsmedelsförpackningskorksetikettspåklistrarmedelspaketsinnehållsförteckningsbläckfärgstillverkarsfabrikspostadress")));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_contains_invalid_item() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(5));
    ioopm_linked_list_append(list, int_elem(16));
    ioopm_linked_list_append(list, int_elem(13));
    CU_ASSERT_FALSE(ioopm_linked_list_contains(list, (elem_t) 20));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_contains_empty_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    CU_ASSERT_FALSE(ioopm_linked_list_contains(list, (elem_t)20));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_size_non_empty_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(4));
    ioopm_linked_list_append(list, int_elem(5));
    ioopm_linked_list_append(list, int_elem(6));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 3);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_size_empty_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_is_empty_empty_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_is_empty_non_empty_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(5));
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_clear_empty_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_clear(list);
    CU_ASSERT_PTR_NULL(list->first->next);
    CU_ASSERT_PTR_NOT_NULL(list->last);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_clear_non_empty_int_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(5));
    ioopm_linked_list_append(list, int_elem(7));
    ioopm_linked_list_append(list, int_elem(895));
    ioopm_linked_list_clear(list);
    CU_ASSERT_PTR_NULL(list->first->next);
    CU_ASSERT_PTR_NOT_NULL(list->last);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_clear_non_empty_string_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_string_elements);
    ioopm_linked_list_append(list, ptr_elem("laptop"));
    ioopm_linked_list_append(list, ptr_elem("flagstångsknoppsputspoleringsmedel"));
    ioopm_linked_list_append(list, ptr_elem("telefon"));
    ioopm_linked_list_clear(list);
    CU_ASSERT_PTR_NULL(list->first->next);
    CU_ASSERT_PTR_NOT_NULL(list->last);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_all_empty_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    CU_ASSERT_FALSE(ioopm_linked_list_all(list, is_even, NULL));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_all_predicate_holds() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(896));
    ioopm_linked_list_append(list, int_elem(10));
    ioopm_linked_list_append(list, int_elem(24));
    ioopm_linked_list_append(list, int_elem(-2));
    CU_ASSERT_TRUE(ioopm_linked_list_all(list, is_even, NULL));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_all_predicate_does_not_hold() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(896));
    ioopm_linked_list_append(list, int_elem(11));
    ioopm_linked_list_append(list, int_elem(24));
    ioopm_linked_list_append(list, int_elem(-2));
    CU_ASSERT_FALSE(ioopm_linked_list_all(list, is_even, NULL));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_any_empty_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    CU_ASSERT_FALSE(ioopm_linked_list_any(list, is_even, NULL));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_any_predicate_holds() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(896));
    ioopm_linked_list_append(list, int_elem(10));
    ioopm_linked_list_append(list, int_elem(24));
    ioopm_linked_list_append(list, int_elem(-2));
    CU_ASSERT_TRUE(ioopm_linked_list_any(list, is_even, NULL));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_any_predicate_holds_strings() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_string_elements);
    ioopm_linked_list_append(list, ptr_elem("Heeej"));
    ioopm_linked_list_append(list, ptr_elem("Hej"));
    ioopm_linked_list_append(list, ptr_elem("Hj"));
    ioopm_linked_list_append(list, ptr_elem(""));
    CU_ASSERT_TRUE(ioopm_linked_list_any(list, compare_element_to_string, "Hej"));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_any_predicate_does_not_hold() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(897));
    ioopm_linked_list_append(list, int_elem(11));
    ioopm_linked_list_append(list, int_elem(23));
    ioopm_linked_list_append(list, int_elem(-5));
    CU_ASSERT_FALSE(ioopm_linked_list_any(list, is_even, NULL));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_apply_to_all() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(897));
    ioopm_linked_list_append(list, int_elem(11));
    ioopm_linked_list_append(list, int_elem(23));
    ioopm_linked_list_append(list, int_elem(-5));
    int number_to_add = 3;
    ioopm_linked_list_apply_to_all(list, add_num, &number_to_add);
    CU_ASSERT_TRUE(list->eq_function(list->first->next->value, (elem_t) 900));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->next->value, (elem_t) 14));
    CU_ASSERT_TRUE(list->eq_function(list->first->next->next->next->value, (elem_t) 26));
    CU_ASSERT_TRUE(list->eq_function(list->last->value, (elem_t) -2));
    ioopm_linked_list_destroy(list);
}

void test_ioopm_linked_list_apply_to_all_empty_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    int number_to_add = 3;
    ioopm_linked_list_apply_to_all(list, add_num, &number_to_add);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
    CU_ASSERT_PTR_NULL(list->first->next);
    CU_ASSERT_PTR_NOT_NULL(list->last);
    ioopm_linked_list_destroy(list);
}


int main(int argc, char *argv[]) {
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite my_test_suite = CU_add_suite("Test suite for linked lists", init_suite, clean_suite);
    if (my_test_suite == NULL) {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // This is where we add the test functions to our test suite.
    // For each call to CU_add_test we specify the test suite, the
    // name or description of the test, and the function that runs
    // the test in question. If you want to add another test, just
    // copy a line below and change the information
    if (
            (CU_add_test(my_test_suite, "Create/destroy linked list               ", test_ioopm_linked_list_create_destroy) == NULL) ||
            (CU_add_test(my_test_suite, "Append to empty list                     ", test_ioopm_linked_list_append_int_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Append to string to empty list           ", test_ioopm_linked_list_append_string_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Append to non-empty list                 ", test_ioopm_linked_list_append_to_non_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Append to empty string to non-empty list ", test_ioopm_linked_list_append_empty_string_to_non_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Append negative value                    ", test_ioopm_linked_list_append_negative) == NULL) ||
            (CU_add_test(my_test_suite, "Prepend to empty list                    ", test_ioopm_linked_list_prepend_int_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Prepend string to empty list             ", test_ioopm_linked_list_prepend_string_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Prepend to non-empty list                ", test_ioopm_linked_list_prepend_to_non_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Prepend negative value                   ", test_ioopm_linked_list_prepend_negative) == NULL) ||
            (CU_add_test(my_test_suite, "Insert to empty list                     ", test_ioopm_linked_list_insert_int_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Insert string to non empty list          ", test_ioopm_linked_list_insert_string_empty) == NULL) ||
            (CU_add_test(my_test_suite, "Insert to non-empty first spot           ", test_ioopm_linked_list_insert_to_non_empty_in_first_spot) == NULL) ||
            (CU_add_test(my_test_suite, "Insert to non-empty last spot            ", test_ioopm_linked_list_insert_to_non_empty_in_last_spot) == NULL) ||
            (CU_add_test(my_test_suite, "Insert to non-empty middle spot          ", test_ioopm_linked_list_insert_int_to_non_empty_in_middle_spot) == NULL) ||
            (CU_add_test(my_test_suite, "Insert string to non-empty middle spot   ", test_ioopm_linked_list_insert_string_to_non_empty_in_middle_spot) == NULL) ||
            (CU_add_test(my_test_suite, "Insert long string to non-empty list     ", test_ioopm_linked_list_insert_long_string_to_non_empty_in_middle_spot) == NULL) ||
            (CU_add_test(my_test_suite, "Insert negative to empty list            ", test_ioopm_linked_list_insert_negative) == NULL) ||
            (CU_add_test(my_test_suite, "Remove from non-empty list               ", test_ioopm_linked_list_remove_int_from_list) == NULL) ||
            (CU_add_test(my_test_suite, "Remove first                             ", test_ioopm_linked_list_remove_first) == NULL) ||
            (CU_add_test(my_test_suite, "Remove last                              ", test_ioopm_linked_list_remove_last) == NULL) ||
            (CU_add_test(my_test_suite, "Remove string from list                  ", test_ioopm_linked_list_remove_string_from_list) == NULL) ||
            (CU_add_test(my_test_suite, "Get first                                ", test_ioopm_linked_list_get_first) == NULL) ||
            (CU_add_test(my_test_suite, "Get last                                 ", test_ioopm_linked_list_get_last) == NULL) ||
            (CU_add_test(my_test_suite, "Get non edge                             ", test_ioopm_linked_list_get_int_non_edge) == NULL) ||
            (CU_add_test(my_test_suite, "Get empty string from non edge           ", test_ioopm_linked_list_get_empty_string_non_edge) == NULL) ||
            (CU_add_test(my_test_suite, "Get string from non edge                 ", test_ioopm_linked_list_get_string_non_edge) == NULL) ||
            (CU_add_test(my_test_suite, "If list cointains valid item             ", test_ioopm_linked_list_contains_valid_item) == NULL) ||
            (CU_add_test(my_test_suite, "If list cointains invalid item           ", test_ioopm_linked_list_contains_invalid_item) == NULL) ||
            (CU_add_test(my_test_suite, "If empty list cointains item             ", test_ioopm_linked_list_contains_empty_list) == NULL) ||
            (CU_add_test(my_test_suite, "If list cointains wierd string           ", test_ioopm_linked_list_contains_wierd_string_item) == NULL) ||
            (CU_add_test(my_test_suite, "size of non-empty list                   ", test_ioopm_linked_list_size_non_empty_list) == NULL) ||
            (CU_add_test(my_test_suite, "size of empty list                       ", test_ioopm_linked_list_size_empty_list) == NULL) ||
            (CU_add_test(my_test_suite, "is empty on empty list                   ", test_ioopm_linked_list_is_empty_empty_list) == NULL) ||
            (CU_add_test(my_test_suite, "is empty on non-empty list               ", test_ioopm_linked_list_is_empty_non_empty_list) == NULL) ||
            (CU_add_test(my_test_suite, "clear empty list                         ", test_ioopm_linked_list_clear_empty_list) == NULL) ||
            (CU_add_test(my_test_suite, "clear non-empty list                     ", test_ioopm_linked_list_clear_non_empty_int_list) == NULL) ||
            (CU_add_test(my_test_suite, "clear non-empty string list              ", test_ioopm_linked_list_clear_non_empty_string_list) == NULL) ||
            (CU_add_test(my_test_suite, "Predicate all empty list                 ", test_ioopm_linked_list_all_empty_list) == NULL) ||
            (CU_add_test(my_test_suite, "Predicate holds for all                  ", test_ioopm_linked_list_all_predicate_holds) == NULL) ||
            (CU_add_test(my_test_suite, "Predicate does not hold for all          ", test_ioopm_linked_list_all_predicate_does_not_hold) == NULL) ||
            (CU_add_test(my_test_suite, "Predicate any empty list                 ", test_ioopm_linked_list_any_empty_list) == NULL) ||
            (CU_add_test(my_test_suite, "Predicate holds for any integer          ", test_ioopm_linked_list_any_predicate_holds) == NULL) ||
            (CU_add_test(my_test_suite, "Predicate holds for any string           ", test_ioopm_linked_list_any_predicate_holds_strings) == NULL) ||
            (CU_add_test(my_test_suite, "Predicate does not hold for any          ", test_ioopm_linked_list_any_predicate_does_not_hold) == NULL) ||
            (CU_add_test(my_test_suite, "Apply function to all items              ", test_ioopm_linked_list_apply_to_all) == NULL) ||
            (CU_add_test(my_test_suite, "Apply function to all items empty list   ", test_ioopm_linked_list_apply_to_all_empty_list) == NULL) ||
            0
            )
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