# ---------------------  ALL TESTS  --------------------- # 
test: test_hashtable test_linkedlist test_iterator

compile_all:
	gcc -Wall --debug hash_table_tests.c hash_table.c linked_list.c -lcunit -g -o hash_table_tests
	gcc -Wall --debug linked_list.c linked_list_tests.c -lcunit -g -o linked_list_tests
	gcc -Wall --debug iterator_tests.c iterator.c linked_list.c -lcunit -g -o iterator_tests

memtest:
	valgrind --leak-check=full ./hash_table_tests --error-on-fail
	valgrind --leak-check=full ./linked_list_tests --error-on-fail
	valgrind --leak-check=full ./iterator_tests --error-on-fail


# ---------------------  HASH TABLE   --------------------- # 
test_hashtable: hash_table.c hash_table_tests.c
	gcc -Wall --debug hash_table_tests.c hash_table.c linked_list.c -lcunit -g -o hash_table_tests
	./hash_table_tests

memtest_hashtable: test_hashtable
	valgrind --leak-check=full ./hash_table_tests

gcov_hashtable: hash_table_tests
	gcc -Wall --debug -fprofile-arcs -ftest-coverage hash_table.c linked_list.c hash_table_tests.c -lcunit -g -o hash_table_tests
	./hash_table_tests
	gcov hash_table_tests.c
	gcov hash_table.c


# ---------------------  LINKED LIST  --------------------- # 
test_linkedlist: linked_list.c linked_list_tests.c
	gcc -Wall --debug linked_list.c linked_list_tests.c -lcunit -g -o linked_list_tests
	./linked_list_tests

memtest_linkedlist: test_linkedlist
	valgrind --leak-check=full ./linked_list_tests

gcov_linkedlist: linked_list_tests
	gcc -Wall --debug -fprofile-arcs -ftest-coverage linked_list.c linked_list_tests.c -lcunit -g -o linked_list_tests
	./linked_list_tests
	gcov linked_list_tests.c
	gcov linked_list.c


# ---------------------  ITERATOR  --------------------- # 
test_iterator: iterator.c iterator_tests.c
	gcc -Wall --debug iterator_tests.c iterator.c linked_list.c -lcunit -g -o iterator_tests
	./iterator_tests

memtest_iterator: test_iterator
	valgrind --leak-check=full ./iterator_tests

gcov_iterator: iterator_tests
	gcc -Wall --debug -fprofile-arcs -ftest-coverage iterator.c linked_list.c iterator_tests.c -lcunit -g -o iterator_tests
	./iterator_tests
	gcov iterator_tests.c
	gcov iterator.c


# ---------------------  FREQ-COUNT  --------------------- # 
compile_freqcount: freq_count.c hash_table.c linked_list.c iterator.c
	gcc -Wall --debug freq_count.c hash_table.c linked_list.c iterator.c -g -o freq_count

freqcount_small: compile_freqcount
	./freq_count small.txt

freqcount_1k: compile_freqcount
	./freq_count 1k-long-words.txt

freqcount_10k: compile_freqcount
	./freq_count 10k-words.txt

freqcount_16k: compile_freqcount
	./freq_count 16k-words.txt

memtest_freqcount_1k: compile_freqcount #leak!
	valgrind --leak-check=full ./freq_count 1k-long-words.txt

memtest_freqcount_10k: compile_freqcount
	valgrind --leak-check=full ./freq_count 10k-words.txt

memtest_freqcount_16k: compile_freqcount
	valgrind --leak-check=full ./freq_count 16k-words.txt

memtest_freqcount_small: compile_freqcount #leak!
	valgrind --leak-check=full ./freq_count small.txt
	

# ---------------------  PROFILING  --------------------- # 
gprof_freq_small:
	gcc -Wall -O2 hash_table.c linked_list.c iterator.c freq_count.c -g -o freq_count -pg
	./freq_count "small.txt"
	gprof freq_count > prof_small.txt

gprof_freq_1k:
	gcc -Wall -O2 hash_table.c linked_list.c iterator.c freq_count.c -g -o freq_count -pg
	./freq_count "1k-long-words.txt"
	gprof freq_count > prof_1k-long-words.txt

gprof_freq_10k:
	gcc -Wall -O2 hash_table.c linked_list.c iterator.c freq_count.c -g -o freq_count -pg
	./freq_count "10k-words.txt"
	gprof freq_count > prof_10k-words.txt

gprof_freq_16k:
	gcc -Wall -g -pg hash_table.c linked_list.c iterator.c freq_count.c -o freq_count
	./freq_count "16k-words.txt"
	gprof freq_count > prof_16k-words.txt


# ---------------------  CLEAN  --------------------- # 
clean:
	rm -f *.o *.gcov *.gcda *.gcno hash_table_tests iterator_tests linked_list_tests freq_count
