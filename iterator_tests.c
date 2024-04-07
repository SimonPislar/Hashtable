#include <CUnit/Basic.h>
#include <stdbool.h>
#include "private_resources.h"
#include "common.h"
#include <stdio.h>
#include "linked_list.h"
#include "iterator.h"

// Helper function to be able to test elem_t's of type int
bool compare_int_elements(elem_t a, elem_t b) {
    return b.integer == a.integer;
}

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

void test_ioopm_list_create_iterator_empty_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_iter_t *iter= ioopm_list_iterator(list);
    CU_ASSERT_PTR_NULL(iter->current->next); // Check that iterator points to nothing, since there is no first element in list
    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(iter);
}

void test_ioopm_list_create_iterator_non_empty_list() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(-1));
    ioopm_iter_t *iterator = ioopm_list_iterator(list);
    CU_ASSERT_EQUAL(iterator->current->next->value.integer, 1); // Check that iterator points to first element in list
    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(iterator);
}

void test_ioopm_iterator_has_next_true() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(10));
    ioopm_iter_t *iterator = ioopm_list_iterator(list);
    CU_ASSERT_TRUE(ioopm_iterator_has_next(iterator)); // Check that there is an element after the first element
    CU_ASSERT_EQUAL(iterator->current->next->next->value.integer, 10); // Check that this "next element" has correct value
    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(iterator);
}

void test_ioopm_iterator_has_next_false() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(10));
    ioopm_iter_t *iterator = ioopm_list_iterator(list);
    iterator->current = iterator->current->next; // Move iterator one step forward
    iterator->current = iterator->current->next; // Move iterator one step forward
    CU_ASSERT_FALSE(ioopm_iterator_has_next(iterator)); // Check that there are no more elements
    CU_ASSERT_EQUAL(iterator->current->value.integer, 10); // Check that last element has correct value
    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(iterator);
}


void test_ioopm_iterator_current() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(10));
    ioopm_iter_t *iterator = ioopm_list_iterator(list);
    elem_t first_value = ioopm_iterator_current(iterator);
    CU_ASSERT_EQUAL(first_value.integer, 1); // Check that value of current iterator is the same as value of first list element
    iterator->current = iterator->current->next; // Move iterator one step forward
    elem_t second_value = ioopm_iterator_current(iterator);
    CU_ASSERT_EQUAL(second_value.integer, 10); // Check that value of current iterator is the same as value of second list element
    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(iterator);
}

void test_ioopm_iterator_next() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(10));
    ioopm_linked_list_append(list, int_elem(13));
    ioopm_linked_list_append(list, int_elem(9));
    ioopm_iter_t *iterator = ioopm_list_iterator(list);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iterator).integer, 1); // Check that value of current iterator is the same as value of first list element
    ioopm_iterator_next(iterator); // Get next value
    CU_ASSERT_EQUAL(ioopm_iterator_current(iterator).integer, 10); // Check that value of updated iterator is same as what old iterator said next value would be
    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_iterator_remove_first_elem() { //TODO: Check if links after remove is correct
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(10));
    ioopm_linked_list_append(list, int_elem(13));
    ioopm_linked_list_append(list, int_elem(9));
    ioopm_iter_t *iterator = ioopm_list_iterator(list);
    elem_t removed_item = ioopm_iterator_remove(iterator);
    CU_ASSERT_EQUAL(removed_item.integer, 1);
    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_iterator_remove_last_elem() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(10));
    ioopm_linked_list_append(list, int_elem(13));
    ioopm_linked_list_append(list, int_elem(9));
    ioopm_iter_t *iterator = ioopm_list_iterator(list);
    iterator->current = iterator->current->next->next->next;
    elem_t removed_item = ioopm_iterator_remove(iterator);
    CU_ASSERT_EQUAL(removed_item.integer, 9);
    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_iterator_remove_middle_elem() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(10));
    ioopm_linked_list_append(list, int_elem(13));
    ioopm_linked_list_append(list, int_elem(9));
    ioopm_iter_t *iterator = ioopm_list_iterator(list);
    iterator->current = iterator->current->next->next;
    elem_t removed_item = ioopm_iterator_remove(iterator);
    CU_ASSERT_EQUAL(removed_item.integer, 13);
    CU_ASSERT_EQUAL(list->first->next->next->next->value.integer, 9);
    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_iterator_remove_list_with_one_elem() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_iter_t *iterator = ioopm_list_iterator(list);
    elem_t removed_item = ioopm_iterator_remove(iterator);
    CU_ASSERT_EQUAL(removed_item.integer, 1);
    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

void test_ioopm_iterator_reset() {
    ioopm_list_t *list = ioopm_linked_list_create(compare_int_elements);
    ioopm_linked_list_append(list, int_elem(1));
    ioopm_linked_list_append(list, int_elem(10));
    ioopm_linked_list_append(list, int_elem(13));
    ioopm_linked_list_append(list, int_elem(9));
    ioopm_iter_t *iterator = ioopm_list_iterator(list);
    for ( int i = 0; i < 3; i++) {
        ioopm_iterator_next(iterator);
    }
    ioopm_iterator_reset(iterator);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iterator).integer, 1);
    ioopm_iterator_destroy(iterator);
    ioopm_linked_list_destroy(list);
}

int main(int argc, char *argv[]) {
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite my_test_suite = CU_add_suite("Test suite for iterators", init_suite, clean_suite);
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
            (
            (CU_add_test(my_test_suite, "Create iterator on empty list                          ", test_ioopm_list_create_iterator_empty_list) == NULL) ||
            (CU_add_test(my_test_suite, "Create iterator on non empty list                      ", test_ioopm_list_create_iterator_non_empty_list) == NULL) ||
            (CU_add_test(my_test_suite, "If iterator has next on list that has next             ", test_ioopm_iterator_has_next_true) == NULL) ||
            (CU_add_test(my_test_suite, "If iterator has next on list that doesn't have next    ", test_ioopm_iterator_has_next_false) == NULL) ||
            (CU_add_test(my_test_suite, "If current updates after changes after an iteration    ", test_ioopm_iterator_current) == NULL) ||
            (CU_add_test(my_test_suite, "If iterator can check next value                       ", test_ioopm_iterator_next) == NULL) ||
            (CU_add_test(my_test_suite, "Removing first value                                   ", test_ioopm_iterator_remove_first_elem) == NULL) ||
            (CU_add_test(my_test_suite, "Removing last value                                    ", test_ioopm_iterator_remove_last_elem) == NULL) ||
            (CU_add_test(my_test_suite, "Removing middle value                                  ", test_ioopm_iterator_remove_middle_elem) == NULL) ||
            (CU_add_test(my_test_suite, "Removing in list with one value                        ", test_ioopm_iterator_remove_list_with_one_elem) == NULL) ||
            (CU_add_test(my_test_suite, "Resetting iterator puts cursor first list item         ", test_ioopm_iterator_reset) == NULL) ||
            0
            )
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