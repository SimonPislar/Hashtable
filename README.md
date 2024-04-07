# ASSIGNMENT 1
By Ida Hellqvist and Simon Pislar, september-october 2022. 

## RUNNING TESTS AND FILES
### All tests
Run tests for all files (excluding freq_count):
`make test`

Run test with Valgrind:
`make memtest`

### Testing individual files
You can run tests for individual files with the commands below. Replace # in the commands with one of following options depending on which file to inspect:
- `hashtable`
- `linkedlist`
- `iterator`

**Run test:** `make test_#`

**Run test with Valgrind:** `make memtest_#`

**Produce code coverage report using gcov:** `make gcov_#`

### Running `freq_count.c`
You can also run `freq_count.c` with provided test input:
- **Run `freq_count` with specified input:** `make freqcount_#`
- **Run `freq_count` with Valgrind and specified input:** `make memtest_freqcount_#`

Where you replace # with one of the following options depending on which input you want to use:
- `small` for `small.txt`
- `1k`for `1k-words.txt`
- `10k`for `10k-words.txt`
- `16k` for `16k-words.txt`

To produce a profiling report on `freq_count.c` using gprof, run following command replacing # with one of the options above depending on which input you want to use.
- `make gprof_freq_#`

## Clean
To clean up output files and files produced by gcov and gprof, run `make clean`

## LINE COVERAGE
- `hash_table.c` 99.48%
- `linked_list.c` 100%
- `iterator.c` 100%

## DESIGN DECISIONS
- Error handled with a data type `option_t`,  a struct containing the value of called function and a bool that states whether the returned value is valid or not.
- Keys of negative numerical value are not allowed. They will be converted to their absolute value.
- Values in the hash table are not removed or freed when the hash table is destroyed.

## INITIAL PROFILING RESULTS
Profiling done by using gprof. Analyzing done by looking at number of calls (procentage of time was only applicable for `16k-words.txt`).

### Top 3 functions overall VS top 3 functions in our code for each input
#### 1k-long-words.txt:
1. Top 3 functions: 
    1. `string_eq` Calls: 17 065. Time: -
    2. `compare_keys` Calls: 12 063. Time: -
    3. `string_sum_hash` Calls: 2 000. Time: -
2. Top 3 functions (our code): same as above, no library functions in top 3

#### 10k-words.txt:
1. Top 3 functions: 
    1. `string_eq` Calls: 1 159 890. Time: -
    2. `compare_keys` Calls: 1 019 530. Time: -
    3. `string_sum_hash` Calls: 20 000. Time: -
2. Top 3 functions (our code): same as above, no library functions in top 3

#### 16k-words.txt:
1. Top 3 functions: 
    1. `compare_keys` Calls: 26 738 062. Time: 75.12%.
    2. `string_eq` Calls: 29 267 600. Time: 16.69%.
    3. `ioopm_hash_table_any` Calls: 16 992. Time: 8.35%.
2. Top 3 functions (our code): same as above, no library functions in top 3

#### small.txt:
1. Top 3 functions: 
    1. `string_eq` Calls: 455. Time: -
    2. `compare_keys` Calls: 351. Time: -
    3. `string_sum_hash` Calls: 54. Time: -
2. Top 3 functions (our code): same as above, no library functions in top 3

### 3. Top functions across all inputs 
1. `string_eq` - appears as top function in 3 of 4 inputs
2. `compare_keys` - appears as top function i 1 input, and first runner up in the 3 other inputs 
3. `string_sum_hash` - appears as second runner up in 3 of 4 inputs

The top functions are almost completly consistent across all inputs. Which makes sense because we use strings as keys, and everytime we want to, for example, insert a new element we have to compare its key to all the keys already in the hash table. `string_eq`and `compare_keys`are used for this. So they are not just being called once every insert, but several times. Namely once for every element already in the hash table (or until the key is found), explaining these functions place as top functions. 

`string_sum_hash` is also called everytime we insert an element, or look for an element in the hash table. Though, it is only called once per insert or lookup (for finding which bucket to use) explaining why the number of calls to it is considerably lower than the top two functions. 

### 4. Is there some kind of trend? 
We guess the trend is that the functions called when inserting or looking for an element are called most frequent no matter the size of the input. It is also where the program spend most of its time (over 90% of its time when looking at 16-k words as input). In other words, inserting or looking for an element generates alot of work for the program.

### 5. Do the result correspond to our expectations? 
The result pretty much corresponds with our expectations. We hadn't thought of exactly which functions were going to be called most frequently or take up the most time. But we expected that the program was going to spend alot of time comparing things and looping through the hash table, since it has to do that for every input. And for every input word that gets processed, it is likely that it has to be compared with more elements than the previous word.

### 6. Based on the result, is there a way to make our program go faster?
The obvious choice when trying to make our program go faster is to look at `string_eq` and `compare_keys` and see if they ow the way they are being called can be improved. Improving the other functions will most likely not make that much difference to the overall performance since they are called so much less frequent than the top three functions. 
