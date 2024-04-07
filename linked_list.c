#include <stdlib.h>
#include "private_resources.h"
#include "linked_list.h"

typedef void (apply_elem_function)(ioopm_list_t *list, item_t *value, void *extra);

item_t *item_create(elem_t value, item_t *next) {
    item_t *new_item = calloc(1, sizeof(item_t));
    new_item->value = value;
    new_item->next = next;
    return new_item;
}

item_t *first_link(ioopm_list_t *list) {
    item_t *dummy = list->first;
    return dummy->next;
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_function) {
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
    list->eq_function = eq_function;
    list->first = item_create(int_elem(0), NULL);
    list->last = list->first;
    return list;
}

static void item_destroy(item_t *item){
    free(item);
    item = NULL;
}

void ioopm_linked_list_destroy(ioopm_list_t *list) {
    ioopm_linked_list_clear(list);
    item_destroy(list->first);
    free(list);
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value) {
    item_t *new_item = item_create(value, NULL);
    if (first_link(list) == NULL) { // If the list is empty, set first and last to be new_item
        list->first->next = new_item;
        list->last = new_item;
        return;
    }
    list->last->next = new_item; // Else, update "old" last item to point to new_item
    list->last = new_item;      // and set new_item to be the new last item
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value) {
    item_t *new_item = item_create(value, list->first->next);
    if(list->first->next == NULL) { // If list is empty, set new_item to be last item
        list->last = new_item;
    }
    list->first->next = new_item; // Update first to point to new_item
}

static item_t *find_previous_link(ioopm_list_t *list, int index) {
    item_t *previous_item = list->first;
    item_t *cursor = list->first->next;
    for (int i = 0; i < index; i++) { // Iterate over list to find item prior to item at index
        previous_item = cursor;
        cursor = cursor->next;
    }
    return previous_item;
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value) {
    if (index == 0) { // Prepend if you want to insert at index 0
        ioopm_linked_list_prepend(list, value);
        return;
    }
    item_t *previous_item = find_previous_link(list, index);
    if (previous_item == list->last) { // Append if you want to insert at last index
        ioopm_linked_list_append(list, value);
        return;
    }
    item_t *new_item = item_create(value, previous_item->next);
    previous_item->next = new_item;
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index) {
    item_t *previous_item = find_previous_link(list, index);
    item_t *cursor = previous_item->next;

    previous_item->next = cursor->next; // Update previous item to point to its new next item
    elem_t destroyed_value = cursor->value;

    if (list->last == cursor) { // Check if we want to destroy last item
        list->last = previous_item;
    }
    item_destroy(cursor);
    return destroyed_value;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, int index) {
    return find_previous_link(list, index)->next->value;
}

static void iterate_over_list(ioopm_list_t *list, apply_elem_function function, void *extra) {
    item_t *cursor = list->first->next;
    while (cursor != NULL) {
        function(list, cursor, extra);
        cursor = cursor->next;
    }
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element) {
    item_t *cursor = list->first->next;
    while (cursor !=  NULL) {
        if (list->eq_function(cursor->value, element)) { // Check if cursor is the element we're looking for
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

static void count_item(ioopm_list_t *ignored_list, item_t *ignored_value, void *counter) {
    size_t *ptr_size_counter = counter;
    *ptr_size_counter += 1; 
}

size_t ioopm_linked_list_size(ioopm_list_t *list) {
    size_t counter = 0;
    iterate_over_list(list, count_item, &counter);
    return counter;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list) {
    return ioopm_linked_list_size(list) == 0;
}

void ioopm_linked_list_clear(ioopm_list_t *list) {
    item_t *cursor = list->first->next;
    item_t *cache = cursor;
    while (cursor != NULL) { 
        cache = cursor->next; // Cache pointer to item after cursor
        item_destroy(cursor); // Destroy cursor
        cursor = cache; // New cursor is item after destroyed item
    }
    list->first->next = NULL;
    list->last = list->first;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_list_predicate prop, void *extra) {
    item_t *cursor = list->first->next;
    if (ioopm_linked_list_size(list) == 0) { // Predicate doesn't hold if list is empty
        return false;
    }
    while (cursor != NULL) { 
        if (!prop(cursor->value, extra)) { // Return false if predicate doesn't hold for cursor
            return false;
        }
        cursor = cursor->next;
    }
    return true; // We only come here if predicate holds for all items
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_list_predicate prop, void *extra) {
    item_t *cursor = list->first->next;
    if (ioopm_linked_list_size(list) == 0) { // Predicate doesn't hold if list is empty
        return false;
    }
    while (cursor != NULL) {
        if (prop(cursor->value, extra)) { // Return true if predicate holds for cursor
            return true;
        }
        cursor = cursor->next;
    }
    return false; // We only come here if predicate doesn't hold for any item
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_list_function fun, void *extra) {
    item_t *cursor = list->first->next;
    while (cursor != NULL) {
        fun(&cursor->value, extra); 
        cursor = cursor->next;
    }
}