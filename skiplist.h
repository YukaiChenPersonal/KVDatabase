/*
Author:     Yukai Chen
Description:    This file contains two key objects: Node and Skiplist.
                These two objects are the core of this program.
                This file declares and defines these objects as well as their members
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cmath>
#include <mutex>

#define OUTPUT_FLIE "./store/DumpFile"

std::mutex mtx;
std::string delimiter = ":";

/*
A node is the storage unit in a skip list.
It stores values(In this context K and V), and points to the next node on different levels.
Use template to implement node, allowing key and value to support various data types.
*/
template<typename K, typename V>
class Node{
    private:
        K key;
        V value;
    public:
        Node(){}
        Node(K, V, int);
        ~Node();
        K get_key() const;
        V get_value() const;
        void set_value(V);
        /*
        nexts is an array of pointers.
        In a linked list, we use a pointer to point to the next node of current node.
        However in a skip list, we need to point to the next nodes of DIFFERENT LEVELS.
        So here an array of pointers is used.
        */
        Node<K, V> ** nexts;
        /*
        nodeLevel determines how many levels a node has.
        This value should not exceed the max level of the skip list.
        */
        int nodeLevel;
    };

//Constructor.
template<typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level){
    this->key = k;
    this->value = v;
    this->nodeLevel = level;
    //Based on the height of this node, need to set up index for all levels.
    this->nexts = new Node<K,V>*[level+1];
    //Fill nexts with 0s.
    memset(this->nexts, 0, sizeof(Node<K,V>*)*(level+1));
};

//Destructor.
template<typename K, typename V> 
Node<K, V>::~Node() {
    delete []nexts;
};

template<typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
};

template<typename K, typename V>
V Node<K, V>::get_value() const{
    return value;
};

template<typename K, typename V>
void Node<K,V>::set_value(V value){
    this->value = value;
};

/*
The SkipList class is the fundamental data structure of this program.
It supports various operations, like insert, delete, quary, write, read...
*/
template<typename K, typename V>
class SkipList{
    private:
        int _maxLevel;           //Max level of the skip list.
        int _currentLevel;       //Current level of the skip list.
        int _currentCount;       //Current number of nodes.
        Node<K, V>* _header;     //Pointer to header node.

        //File operator objects
        std::ofstream _writer;
        std::ifstream _reader;

    private:
        void getKeyValueFromString(const std::string& str, std::string* key, std::string* value);
        bool stringValid(const std::string& str);
    
    public:
        SkipList(int);
        ~SkipList();
        Node<K, V>* create_node(K, V, int);
        int generateNodeLevel();

        int insertByKey(K, V);
        int searchByKey(K);
        int deleteByKey(K);
        int getSize();

        void writeToDisk();
        void readFromDisk();

        void display();
};

//Constructor
template<typename K, typename V>
SkipList<K, V>::SkipList(int maxLevel){
    this->_maxLevel = maxLevel;
    this->_currentLevel = 0;
    this->_currentCount = 0;

    K k;
    V v;
    this->_header = new Node<K, V>(k, v, this->_maxLevel);
};

//Destructor
template<typename K, typename V>
SkipList<K, V>::~SkipList(){
    if(_writer.is_open()){
        _writer.close();
    }
    if(_reader.is_open()){
        _reader.close();
    }
    delete _header;
};

template<typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(const K k, const V v, int level) {
    Node<K, V> *n = new Node<K, V>(k, v, level);
    return n;
}

/*
Insert function:
Insert K-V pair into skip list based on the value of key
return values: 1 for key already exists, 0 for successful insertion.
*/

template<typename K, typename V>
int SkipList<K, V>::insertByKey(const K k, const V v){
    std::cout<< "Operation: insert by key "<< k << "........." << std::endl; 
    mtx.lock();
    Node<K, V>* cur = this->_header;
    /*
    update is an array of pointers.
    each element of update is the previous node of the insert position ON A CERTAIN LEVEL
    */
    Node<K, V>* update[_maxLevel+1];
    memset(update, 0, sizeof(Node<K,V>*)*(_maxLevel+1));
    /*
    Start from the highest level of skip list.
    Get insert position on each level and store these positions in update.
    */
    for(int i=this->_currentLevel; i>=0; i--){
        while(cur->nexts[i] != NULL && cur->nexts[i]->get_key()<k){
            cur = cur->nexts[i];
        }
        update[i] = cur;
    }

    /*
    move cur to the next node of insert position on Level 0.
            target: 15
                    |
    ....13....insert position....17....
        |                         |
     update[0]                   cur
    */
    cur = cur->nexts[0];

    //If key of next node equals the key of the new node, return 1.
    if(cur!=NULL && cur->get_key() == k){
        std::cout<<"Key "<<k<<" already exists, insertion failed..."<<std::endl;
        mtx.unlock();
        return 1;
    }

    /*
    If cur is NULL, then the new node will be inserted at the end of the skip list.
    If cur's key doesn't match the new key, the new node will be inserted between update[0] and cur. 
    */

   if(cur==NULL || cur->get_key()!=k){
        int nodeLevel = generateNodeLevel();

        //if nodeLevel is bigger than the current level of skip list, update skip list.
        if(nodeLevel > _currentLevel){
            for(int i = _currentLevel+1; i<nodeLevel+1; i++){
                update[i] = _header;
            }
            _currentLevel = nodeLevel;
        }

        Node<K,V>* newNode = create_node(k, v, nodeLevel);
        for(int i=0; i<nodeLevel; i++){
            newNode->nexts[i] = update[i]->nexts[i];
            update[i]->nexts[i] = newNode; 
        }

        std::cout << "Key "<< k <<", value "<< v <<" successfully inserted."<< std::endl;
        _currentCount++;
   }
    mtx.unlock();
    return 0;
}

/*
Delete function:
Delete K-V pair from the skip list.
Return values: 0 for successful delete, 1 for key not found.
*/

template<typename K, typename V> 
int SkipList<K, V>::deleteByKey(const K k) {
    std::cout<< "Operation: delete by key "<< k << "........." << std::endl; 
    mtx.lock();
    Node<K, V> *cur = this->_header; 
    Node<K, V> *update[_maxLevel+1];
    memset(update, 0, sizeof(Node<K, V>*)*(_maxLevel+1));

    //Start searching for delete positions from the highest level.
    for(int i = _currentLevel; i>=0; i--){
        while(cur->nexts[i]!=NULL && cur->nexts[i]->get_key()<k){
            cur = cur->nexts[i];
        }
        update[i] = cur;
    }

    cur = cur->nexts[0];
    //If target key is not found, return 1.
    if(cur == NULL || cur->get_key() != k){
        std::cout << "Key " << k << " not found!" << std::endl;
        mtx.unlock();
        return 1;
    }

    if(cur!=NULL && cur->get_key() == k){
        //Start from the lowest level and delete target node of each level
        for(int i=0; i<=_currentLevel; i++){
            //If at a certain level, the next node is not target node, break the loop.
            if(update[i]->nexts[i] != cur){break;}

            update[i]->nexts[i] = cur->nexts[i];
        }

        //update skip list and delete levels with no nodes
        while(_currentLevel > 0 && _header->nexts[_currentLevel] == 0){
            _currentLevel --;
        }

        std::cout << "Key "<< k << " successfully deleted"<< std::endl;
        _currentCount--;
    }
    mtx.unlock();
    return 0;
}

/*
Search function:
Search node by key.
Return values: 1 for successful search, 0 for not found.
*/

template<typename K, typename V> 
int SkipList<K, V>::searchByKey(K k) {
    std::cout<< "Operation: search by key "<< k << "........." << std::endl;
    Node<K, V>* cur = _header;

    for(int i = _currentLevel; i>=0; i--){
        while(cur->nexts[i]!=NULL && cur->nexts[i]->get_key()<k){
            cur = cur->nexts[i];
        }
    }

    cur = cur->nexts[0];

    if (cur && cur->get_key() == k) {
        std::cout << "Key " << k << " found, value: " << cur->get_value() << std::endl;
        return 1;
    }
    std::cout << "Key " << k << " not found."<<std::endl;
    return 0;
}

/*
Write function:
Write data in memeroy into file.
*/

template<typename K, typename V> 
void SkipList<K, V>::writeToDisk() {
    std::cout<< "Operation: write to disk."<< std::endl;
    _writer.open(OUTPUT_FLIE);
    Node<K, V>* node = _header->nexts[0];

    while (node != NULL) {
        _writer << node->get_key() << ":" << node->get_value() << "\n";
        std::cout << node->get_key() << ":" << node->get_value() << ";\n";
        node = node->nexts[0];
    }

    _writer.flush();
    _writer.close();
    return ;
}

/*
Read function:
Read data from file.
*/

template<typename K, typename V> 
void SkipList<K, V>::readFromDisk() {
    std::cout<< "Operation: read form disk."<< std::endl;
    _reader.open(OUTPUT_FLIE);
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while (getline(_reader, line)) {
        getKeyValueFromString(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
        insertByKey(*key, *value);
        std::cout << "key:" << *key << "value:" << *value << std::endl;
    }
    _reader.close();
}

/*
Display funtion:
Display current skip list layout.
*/

template<typename K, typename V> 
void SkipList<K, V>::display() {
    std::cout<< "Operation: display skip list."<< std::endl;
    for (int i = _currentLevel; i >= 0; i--) {
        Node<K, V> *node = this->_header->nexts[i]; 
        std::cout << "Level " << i << ": ";
        while (node != NULL) {
            std::cout << node->get_key() << ":" << node->get_value() << ";";
            node = node->nexts[i];
        }
        std::cout << std::endl;
    }
}

/*
Count funtion:
Return current number of nodes in the skip list.
*/
template<typename K, typename V> 
int SkipList<K, V>::getSize() { 
    return _currentCount;
}

/*
Related functions
*/
template<typename K, typename V>
void SkipList<K, V>::getKeyValueFromString(const std::string& str, std::string* key, std::string* value) {

    if(!stringValid(str)) {
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter)+1, str.length());
}

template<typename K, typename V>
bool SkipList<K, V>::stringValid(const std::string& str) {

    if (str.empty()) {
        return false;
    }
    if (str.find(delimiter) == std::string::npos) {
        return false;
    }
    return true;
}

template<typename K, typename V>
int SkipList<K, V>::generateNodeLevel(){
    int k = 1;
    while (rand() % 2) {
        k++;
    }
    k = (k < _maxLevel) ? k : _maxLevel;
    return k;
};