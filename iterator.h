#pragma once

#include <stdbool.h>
#include "common.h"

/**
 * @file iterator.h
 * @author Simon Pislar and Ida Hellqvist
 * @date 09 Oct 2022
 * @brief Iterator thats iterates over a linked list.
 *
 * @see https://uppsala.instructure.com/courses/68435/assignments/130155
 */

typedef struct iter ioopm_iter_t;

/// @brief creates an iterator for a given list
/// @param list the list to be iterated over
/// @return an iteration positioned at the start of list
ioopm_iter_t *ioopm_list_iterator(ioopm_list_t *list);

/// @brief checks if there are more elements to iterate over
/// @param iter the iterator
/// @return true if there is at least one more element 
bool ioopm_iterator_has_next(ioopm_iter_t *iter);

/// @brief steps the iterator forward one step
/// @param iter the iterator
/// @pre iter is not at the end of the list. Recommended use
/// is to call ioopm_iterator_has_next before calling
/// ioopm_iterator_next
void ioopm_iterator_next(ioopm_iter_t *iter);

/// @brief Remove the current element from the underlying list
/// @param iter the iterator
/// @return the removed element
elem_t ioopm_iterator_remove(ioopm_iter_t *iter);

/// @brief repositions the iterator at the start of the underlying list
/// @param iter the iterator
void ioopm_iterator_reset(ioopm_iter_t *iter);

/// @brief returns the current element from the underlying list
/// @param iter the iterator
/// @return the current element
elem_t ioopm_iterator_current(ioopm_iter_t *iter);

/// @brief destroys the iterator and returns its resources
/// @param iter the iterator
void ioopm_iterator_destroy(ioopm_iter_t *iter);