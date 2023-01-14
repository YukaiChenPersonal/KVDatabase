# KV Database
## Project discription
This project is a lightweight key-value storage engine based on skip list, implemented in C++, supports remote operations.
## Technical details
### Project functions and supported operations:
Store by key, supports insertion, deletion, modification and query of data, display the stored data, write to disk, load from disk and database display show database size. Allow a client to connect with a server using TCP/IP protocol while the server receives commands from the client and operates on the database.
### Project entry:
The relevant function functions have been declared and defined in the .h file, main.o is used as a server, client.o used as a client. If only database itself is to be used, enter with skipListTest.o.
### Project modules:
class Node and class SkipList in skiplist.h, realization of the database itself.   
client.cpp implements a client that can operate the database from distance.   
main.cpp implements the function of a server, deals with commands coming from the client, do corresponding operations to the database and send back results to the client.     
skipListTest.cpp lists all types database operations. It's for demonstration and testing.
### Difficulties:
This project is completely developed around the data structure of the skip list, with the focus on the mastery of the skip list.
Need to master: the structure of the skip list, the space occupation, the algorithm and time complexity of inserting, deleting, modifying and quarying, and the advantages of the skip list compared with the general linked list.
How do nodes in the skip list point to subsequent nodes?
The node class maintains an array storing pointers to the skip list nodes, and stores the next node of the current node in each layer of the skip list.
## Future improvments:
1. Different types of key.
2. Concurrent operations.
3. Not all database operations are supported remotely.
4. Currently the program doesn't support input of spaces when using remote control.
## My experience
Get fimiliar with skip lists, have better understanding of data structures, get practiced with object-oriented programming.
