# KV Database
## Project discription
This project is a lightweight key-value storage engine based on skip list, implemented in C++.
## Technical details
### Project functions and supported operations:
Store by key, supports insertion, deletion, modification and query of data, display the stored data, write to disk, load from disk and database display show database size.
### Project entry:
The relevant function functions have been declared and defined in the .h file, and the main function is used to call the relevant functions to perform the above operations.
### Project modules:
class Node,
class SkipList.
### Difficulties:
This project is completely developed around the data structure of the skip list, with the focus on the mastery of the skip list.
Need to master: the structure of the skip list, the space occupation, the algorithm and time complexity of inserting, deleting, modifying and quarying, and the advantages of the skip list compared with the general linked list.
How do nodes in the skip list point to subsequent nodes?
The node class maintains an array storing pointers to the skip list nodes, and stores the next node of the current node in each layer of the skip list.
## Future improvments:
1. Different types of key.
2. Automatic stress test.
3. Support remote operation.
## My experience
Get fimiliar with skip lists, have better understanding of data structures, get practiced with object-oriented programming.
