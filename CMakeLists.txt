cmake_minimum_required(VERSION 3.23)
project(inlupp1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(inlupp1
        hash_table.c
        hash_table.h
        hash_table_tests.c
        linked_list.c
        linked_list.h
        linked_list_tests.c
        iterator.c
        iterator.h
        iterator_tests.c
        freq_count.c)
