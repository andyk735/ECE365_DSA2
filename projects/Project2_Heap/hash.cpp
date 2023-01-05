#include "hash.h"
#include <iostream>
using namespace std;

// initialize hashTable constructor (calculate capacity, resize data to capacity, set filled = 0)
hashTable::hashTable(int size) {
  capacity = getPrime(size);
  data.resize(capacity);
  filled = 0;
}

// insert valid dictionary word into hashTable
int hashTable::insert(const std::string &key, void *pv) {
  int hashVal = hash(key);              // get hash value of key by using hash function

  // check if (1) the key already has an value (2) if the value is the same as key
  while(data[hashVal].isOccupied) {
    if(data[hashVal].key == key) {
        if(data[hashVal].isDeleted){ // deleted key exists
        data[hashVal].isDeleted = false;
        data[hashVal].pv = pv;
        return 0;}
      return 1;
      }    // return 1 if key already exists in hash table
    hashVal++;
    if(hashVal == capacity) {hashVal = 0;}  // Linear Probing: if hashVal reaches capacity, set hashVal to zero
  }

  // set hashItem members into data[hashVal]
  data[hashVal].key = key;
  data[hashVal].isOccupied = true;
  data[hashVal].isDeleted = false;
  data[hashVal].pv = pv;

  // if number of filled items reaches half the capacity, rehash
  if(filled++ > (capacity / 2)) {
    if(rehash() == false) {return 2;}   // if rehash fails, return 2
  }
  return 0;
}

// check if hashTable contains the key already. true if it does, false if not.
bool hashTable::contains(const std::string &key) {
  if(findPos(key) != -1) {return true;}
  return false;
}

// Get the pointer associated with the specified key.
// If the key does not exist in the hash table, return nullptr.
// If an optional pointer to a bool is provided,
// set the bool to true if the key is in the hash table,
// and set the bool to false otherwise.
void *hashTable::getPointer(const std::string &key, bool *b) {
  int pos = findPos(key);
  if(pos >= 0){
    if(b != NULL) {*b = true;}        // checks if the item is there and if it is associated with NULL
    return data[pos].pv;              // return pointer to specified key
  }
  if(b != NULL){*b = false;}
  return NULL;
}

// Set the pointer associated with the specified key.
// Returns 0 on success,
// 1 if the key does not exist in the hash table.
int hashTable::setPointer(const std::string &key, void *pv) {
  int pos = findPos(key);
  if(pos >= 0){
    data[pos].pv = pv;                // set pointer of specified key to the input pointer
    return 0;
  }
  return 1;
}

// Delete the item with the specified key.
// Returns true on success,
// false if the specified key is not in the hash table.
bool hashTable::remove(const std::string &key) {
  int pos = findPos(key);
  if(pos >= 0){
    data[pos].isDeleted = true;       // lazily delete the item
    return true;
  }
  return false;
  }

// hash function desgined by (http://www.cse.yorku.ca/~oz/hash.html)
int hashTable::hash(const std::string &key) {
  unsigned long hash = 5381;
  int c;

  for(int i = 0; i < key.size(); i++) {
    hash = ((hash << 5) + hash) + key[i];
  }

  return hash % (capacity - 1);
}

// find hash value position of a specified key
int hashTable::findPos(const std::string &key) {
  int pos = hash(key);
  while(data[pos].isOccupied) {
    if(data[pos].key == key) {
      if(data[pos].isDeleted) {return -1;}
      return pos;
    }
    pos++;              // linear probe until you find the key
    if(pos == capacity) {pos = 0;}
  }
  return -1;
}

// rehash if half the capacity of the hash table is met
bool hashTable::rehash() {
  vector<hashItem> temp = data;       // set temporary hashItem vector equal to data
  data.clear();
  capacity = getPrime(capacity);
  try {data.resize(capacity);}        // resize data to the new, larger capacity
  catch(bad_alloc) {
    cout << "Could not rehash: memory allocation failed." << endl;
    return false;
  }

  filled = 0;

  // go through the temporary list and reinsert the keys into the rehashed data list with larger capacity
  for(int i = 0; i < temp.size(); i++) {
    if((temp[i].isOccupied == true) && (temp[i].isDeleted == false)) {
      insert(temp[i].key, temp[i].pv);
    }
  }
  return true;
}

// returns a prime integer at least as large as the capacity
unsigned int hashTable::getPrime(int size) {
  // precomputed list of prime numbers
  static int primeNums[] = {6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739};
  unsigned int i = 0;
  for(int i : primeNums) {
    if(i > 2*size){return i;}
  }
  return primeNums[(sizeof(primeNums) / sizeof(primeNums[0])) - 1];
}
