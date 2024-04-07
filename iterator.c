#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "private_resources.h"
#include "iterator.h"
#include "common.h"

ioopm_iter_t *ioopm_list_iterator(ioopm_list_t *list) {
    ioopm_iter_t *result = malloc(sizeof (struct iter));
    result->list = list;
    result->current = list->first;
    return result;
}

bool ioopm_iterator_has_next(ioopm_iter_t *iter) {
    if (iter->current->next != NULL) {
        return true;
    }
    return false;
}

elem_t ioopm_iterator_current(ioopm_iter_t *iter) {
    return iter->current->next->value; 
}

void ioopm_iterator_destroy(ioopm_iter_t *iter) {
    free(iter);
    iter = NULL;
}

void ioopm_iterator_next(ioopm_iter_t *iter) {
    iter->current = iter->current->next;
}

elem_t ioopm_iterator_remove(ioopm_iter_t *iter) {
    item_t *item_to_remove = iter->current->next;
    elem_t removed_value = iter->current->next->value;
    iter->current->next = iter->current->next->next; // Update linking
    free(item_to_remove);
    return removed_value;
}

void ioopm_iterator_reset(ioopm_iter_t *iter) {
    iter->current = iter->list->first;
}