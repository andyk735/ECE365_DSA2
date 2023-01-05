#include "heap.h"
#include "hash.h"
#include <iostream>
using namespace std;

// initialize heap constructor (resize data vector to capacity+1, set filled = 0, create new hashTable called mapping)
heap::heap(int capacity)
{
  // Allocate space for the nodes (0 slot is not used)
  data.resize(capacity+1);
  filled = 0;
  mapping = new hashTable(capacity*2);
  this->capacity = capacity;
}

// insert node with given string and key into heap
int heap::insert(const std::string &id, int key, void *pv) {
  if(filled >= capacity) {return 1;}              // return 1 if heap is already filled to capacity
  else if(mapping->contains(id)) {return 2;}      // return 2 if node already exists in heap(mapping hashTable)

  // assign node id, key, and pointer
  filled++;
  data[filled].id = id;
  data[filled].key = key;
  data[filled].pData = pv;

  mapping->insert(id, &data[filled]);             // actually insert the node into the mapping hashTable
  percolateUp(filled);                            // percolate up for insertion
  return 0;
}

// change the key value for the specified string id
int heap::setKey(const std::string &id, int key) {
  bool b;
  node *pnode =  static_cast<node *> (mapping->getPointer(id, &b));  // cast as pointer to node of the id
  if(!b) {return 1;}                    // check if id is associated w/ NULL or not

  int oldkey = pnode->key;              // store old key
  int pos = getPos(pnode);
  data[pos].key = key;
  if(filled == 1) {return 0;}           // if only one node in heap, setKey is done
  if(oldkey < key) {percolateDown(pos); return 0;}    // percolate down if new key is greater than old key
  else {percolateUp(pos); return 0;}                  // percolate up if new key is less than old key
}

// delete root from heap
int heap::deleteMin(string *pId, int *pKey, void *ppData) {
  if (filled != 0) {
    // update values
    if (pId != nullptr) {*pId = data[1].id;}
    if (pKey != nullptr) {*pKey = data[1].key;}
    if (ppData != nullptr) {*(static_cast<void**> (ppData)) = data[1].pData;}

    // remove the root from the hashTable and replace heap root with last item
    mapping->remove(data[1].id);
    data[1] = data[filled];
    filled--;
    percolateDown(1);         // percolate down for deletion

    return 0;
  }
  else {return 1;}            // heap is empty
}

// remove item from heap
int heap::remove(const std::string &id, int *pKey, void *ppData) {
  if (mapping->contains(id)) {
    node *pnode = static_cast<node*> (mapping->getPointer(id));       // get the pointer to the specified node id
    if (pKey != nullptr) {
      *pKey = pnode->key;     // write to pKey the key of the node that is being deleted
    }
    if (ppData != nullptr) {
      *(static_cast<void**> (ppData)) = pnode->pData;     // write the associated node pointer
    }

    // remove the node from the hashtable
    mapping->remove(id);
    int oldkey = pnode->key;
    *pnode = data[filled];
    int newkey = pnode->key;
    filled--;

    // get position of pnode and sort through heap to resort heap
    int pos = getPos(pnode);
    if (newkey < oldkey) {percolateUp(pos);}
    else if (oldkey < newkey){percolateDown(pos);}

    return 0;
  }
  else {return 1;}            // heap does not contain the id
}

// move a node up the tree to its correct position, used with insertion
void heap::percolateUp(int posCur) {
  node temp = data[posCur];     // store node into temp node

  while (posCur > 1 && temp.key < data[posCur/2].key) {     // while the current key is less than the parent key, until posCur is in the correct position
    data[posCur] = data[posCur/2];
    mapping->setPointer(data[posCur].id, &data[posCur]);
    posCur /= 2;
  }

  data[posCur] = temp;          // change the nodes position
  mapping->setPointer(data[posCur].id, &data[posCur]);
}

// move a node down the tree to its correct position, used with deletion
void heap::percolateDown(int posCur) {
  int correctPos;
  data[0] = data[posCur];     // store node into data[0], could have used temp node again, but tried it this way

  while (2*posCur <= filled) {                              // while left child of current position is less than or equal to filled
      int left = 2 * posCur;
      int right = left + 1;

      if (left < filled && data[right].key < data[left].key) {correctPos = right;}    // set correctPos to right child if right key is less than left key
      else {correctPos = left;}

      if (data[correctPos].key < data[0].key) {             // check if correctPos key is less than starting node key
          data[posCur] = data[correctPos];
          mapping->setPointer(data[posCur].id, &data[posCur]);}

      else {break;}
      posCur = correctPos;    // set posCur to correctPos
  }
  // change the nodes position
  data[posCur] = data[0];
  mapping->setPointer(data[posCur].id, &data[posCur]);
}

// get position of the node
int heap::getPos(node *pn) {
  int pos = pn - &data[0];
  return pos;
}