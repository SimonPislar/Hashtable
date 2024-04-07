#pragma once

/**
 * @file common.h
 * @author Simon Pislar and Ida Hellqvist
 * @date 09 Oct 2022 
 * @brief Resources used by several files
 *
 * @see https://uppsala.instructure.com/courses/68435/assignments/130155
 */

typedef struct list ioopm_list_t; 
typedef struct option ioopm_option_t;

typedef union elem elem_t;

/// Compares two elements and returns true if they are equal
typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

union elem 
{
  int integer;
  unsigned int u_integer;
  bool boolean;
  float decimal;
  void *pointer;
};