//----------------------------------------------------------------------
// FILE: hash_table_collection.h
// NAME: Matthew Moore
// DATE: October, 2020
// DESC: Implements a hash table as a linear data structure which effectively allows 
//  for the adding, removing, and finding of elements through the use of a hash function
//  that established a hash code for every key and its corresponding value pair.
//----------------------------------------------------------------------

#ifndef HASH_TABLE_COLLECTION_H
#define HASH_TABLE_COLLECTION_H

#include "array_list.h"
#include "array_list_collection.h"
#include "bin_search_collection.h"
#include "collection.h"


template<typename K,typename V>
class HashTableCollection : public Collection<K,V>
{ 
public:
  HashTableCollection();
  HashTableCollection(const HashTableCollection<K,V>& rhs);
  ~HashTableCollection();
  HashTableCollection& operator=(const HashTableCollection<K,V>& rhs);
  
  
  void add(const K& a_key, const V& a_val);
  void remove(const K& a_key);
  bool find(const K& search_key, V& the_val) const;
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  void keys(ArrayList<K>& all_keys) const;
  void sort(ArrayList<K>& all_keys_sorted) const;
  size_t size() const;
  
  // 3 public "statistics" functions
  
  size_t min_chain_length();
  size_t max_chain_length();
  double avg_chain_length();
  
  
private:
  // the chain (linked list) nodes
  struct Node {
    K key;
	V value;
	Node * next;
  };
  
  // The (resizeable) hash table
  Node* * hash_table;
  // Table length
  size_t length; // useful for load factor threshold
  // Table capacity
  size_t table_capacity;
  // Load factor on hash table
  double load_factor_threshold = 0.75;
  // Function for resizing and rehashing
  void resize_and_rehash();
  
  std::hash<K>hash_fun; // K- based hash function object
};

template<typename K,typename V>
HashTableCollection<K,V>::HashTableCollection()
 : table_capacity(16), length(0)
{
  hash_table = new Node *[table_capacity];
  
  for (size_t i = 0; i < table_capacity; ++i) {
    hash_table[i] = nullptr;
  }
}

template<typename K,typename V>
HashTableCollection<K,V>::HashTableCollection(const HashTableCollection<K,V>& rhs)
  : table_capacity(0), length(0), hash_table(nullptr)
{
  // Defer to the assignment operator
  *this = rhs;
}

template<typename K,typename V>
HashTableCollection<K,V>::~HashTableCollection()
{
  for (size_t i = 0; i < table_capacity; ++i) {
    Node * ptr = hash_table[i];
	Node * next_ptr = NULL;
    while (ptr != NULL) {
	  next_ptr = ptr->next;
	  delete ptr;
	  ptr = next_ptr;
	}
  }
}
template<typename K,typename V>
HashTableCollection<K,V>& HashTableCollection<K,V>::operator=(const HashTableCollection<K,V>& rhs)
{
  if (this != &rhs) { // protects against self-assignment case
    length = 0;
	if (hash_table != NULL) {
	  // If the hash table is not already empty, then make it empty
	  for (size_t i = 0; i < table_capacity; ++i) {
        Node * ptr = hash_table[i];
	    Node * next_ptr = NULL;
        while (ptr != NULL) {
	      next_ptr = ptr->next;
	      delete ptr;
	      ptr = next_ptr;
	    }
      }
	  delete [] hash_table;
	  hash_table = NULL; 
	}
    table_capacity = rhs.table_capacity;
    hash_table = new Node *[table_capacity];
	
	// Set all the hash_table index pointers to NULL initially
    for (size_t i = 0; i < table_capacity; ++i) {
      hash_table[i] = nullptr;
    }
	
	// Fill array with elements from rhs
    for (size_t i = 0; i < rhs.table_capacity; i++) {
	  Node * ptr = rhs.hash_table[i];
	  while (ptr != NULL) {
		add(ptr->key,ptr->value);
	    ptr = ptr->next;
	  }
    }
  }
  return *this;
}
  
  
template<typename K,typename V>
void HashTableCollection<K,V>::add(const K& a_key, const V& a_val)
{
  if (avg_chain_length() >= load_factor_threshold) {
    // The average chain length is growing too high, so rehash
	resize_and_rehash();
  }
  // Assigns value to new node
  Node * newNode = new Node;
  newNode->key = a_key;
  newNode->value = a_val;
  
  // Find the location where to has the node to
  size_t code = hash_fun(a_key); // get int - based value for key
  size_t index = code % table_capacity; // calculate the index
  
  // Now insert at the front of the linked list, adjusting all other potential elements
  if (hash_table[index] == NULL) {
    hash_table[index] = newNode;
	newNode->next = NULL;
  }
  else {
    newNode->next = hash_table[index];
    hash_table[index] = newNode;
  }
  length = length + 1;
}

template<typename K,typename V>
void HashTableCollection<K,V>::remove(const K& a_key)
{
  
  // Find the location where to has the node to
  size_t code = hash_fun(a_key); // get int - based value for key
  size_t index = code % table_capacity; // calculate the index
  
  if (!hash_table[index]) {
    return;
  }
  // Traverse the specific chain to the correct key value pair within the bucket
  Node * ptr = hash_table[index];
  Node * prev_ptr = NULL;
  
  while (ptr != NULL) {
    if (ptr->key == a_key) {
      // Key value pair has been found
	  if (ptr == hash_table[index]) {
	    // CASE 1: Key at front of the list
		hash_table[index] = ptr->next;
	  }
	  else {
	    // CASE 2: Key is anywhere else in the chain
		prev_ptr->next = ptr->next;
	  }
	  delete ptr;
	  length = length - 1;
	  return;
	}
	prev_ptr = ptr;
    ptr = ptr->next;
  }
  
  // If program reaches this point then there is nothing in the bucket
  // Thus, do nothing
}

template<typename K,typename V>
bool HashTableCollection<K,V>::find(const K& search_key, V& the_val) const
{
  // Find the location where to has the node to
  size_t code = hash_fun(search_key); // get int - based value for key
  size_t index = code % table_capacity ; // calculate the index
  
  Node * ptr = hash_table[index];
  while (ptr != NULL) {
    // Traverse chain within bucket until the value is found
	if (ptr->key == search_key) {
      // Found the search key!
	  the_val = ptr->value;
	  return true;
	}
	ptr = ptr->next;
  }
  
  // Program only reaches this point if no key is found in the chain
  // Thus, return false
  return false;
}

template<typename K,typename V>
void HashTableCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  // First key value pair has been located
  for (size_t i = 0; i < table_capacity; ++i) {
    Node * ptr = hash_table[i];
    while (ptr != NULL) {
	  // Traverse the list until the second key value pair is found
	  if (ptr->key >= k1 && ptr->key <= k2) {
	    // Add all elements which fall in the range
		keys.add(ptr->key);
	  }
	  ptr = ptr->next;
	}
  }

}

template<typename K,typename V>
void HashTableCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  // First key value pair has been located
  for (size_t i = 0; i < table_capacity; ++i) {
    Node * ptr = hash_table[i];
    while (ptr != NULL) {
	  // Traverse the list until all keys are added
	  all_keys.add(ptr->key);
	  ptr = ptr->next;
	}
  }
}

template<typename K,typename V>
void HashTableCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
  all_keys_sorted.sort();
}

template<typename K,typename V>
size_t HashTableCollection<K,V>::size() const
{
  return length;
}

template<typename K,typename V>
size_t HashTableCollection<K,V>::min_chain_length()
{
  // The maximum possible length any one chain could be is the value of length
  size_t min_chain = length;
  size_t curr_chain = 0;
  
  for (size_t i = 0; i < table_capacity; ++i) {
    Node * ptr = hash_table[i];
	curr_chain = 0; 
    while (ptr != NULL) {
	  ++curr_chain;
	  ptr = ptr->next;
	}
	if (curr_chain < min_chain && curr_chain != 0) {
	  // New minimum chain length has been found
	  min_chain = curr_chain;
	}
  }
  return min_chain;
}

template<typename K,typename V>
size_t HashTableCollection<K,V>::max_chain_length()
{
  // The minimum possible length any one chain could be is the value of 0
  size_t max_chain = 0;
  size_t curr_chain = 0;
  
  for (size_t i = 0; i < table_capacity; ++i) {
    Node * ptr = hash_table[i];
	curr_chain = 0;
    while (ptr != NULL) {
	  ++curr_chain;
	  ptr = ptr->next;
	}
	if (curr_chain > max_chain) {
	  // New minimum chain length has been found
	  max_chain = curr_chain;
	}
  }
  return max_chain;
}

template<typename K,typename V>
double HashTableCollection<K,V>::avg_chain_length()
{
  double avg_length;
  avg_length = static_cast<double>(length) / table_capacity;
  
  return avg_length;
}

template<typename K,typename V>
void HashTableCollection<K,V>::resize_and_rehash()
{
  int new_length = 0;
  // Creates new array of double capacity, and storing the items
  Node* * new_hash_table = new Node*[table_capacity * 2];
  // Confirm that all the head nodes in the table are set to NULL
  for (size_t i = 0; i < table_capacity * 2; ++i) {
    new_hash_table[i] = NULL;
  }
  
  for (size_t i = 0; i < table_capacity; ++i) {
    Node * ptr = hash_table[i];
    while (ptr != NULL) {
	  Node * new_node = new Node;
      new_node->key = ptr->key;	  
	  new_node->value = ptr->value;
	  // Adding each key and value pair from the old hash table to the new one
	  // Find the location where to has the node to
      size_t code = hash_fun(new_node->key); // get int - based value for key
      size_t index = code % (table_capacity * 2); // calculate the index
  
      // Now insert at the front of the linked list, adjusting all other potential elements
      if (new_hash_table[index] == NULL) {
        new_hash_table[index] = new_node;
	    new_node->next = NULL;
      }
      else {
        new_node->next = new_hash_table[index];
        new_hash_table[index] = new_node;
      }
	  ++new_length;
	  ptr = ptr->next;
	}
  }
  
  // Deleting the old array
  if (hash_table != NULL) {
    for (size_t i = 0; i < table_capacity; ++i) {
	  Node * ptr = hash_table[i];
	  Node * next_ptr = NULL;
	  while (ptr != NULL) {
	    next_ptr = ptr->next;
	    delete ptr;
	    ptr = next_ptr;
	  }
    }
  }
  for (size_t i = 0; i < table_capacity; ++i) {
    hash_table[i] = NULL;
  }
  // Adjust the size of the table capacity to the correct size
  table_capacity = table_capacity * 2;
  // Creating a new array with the new capacity
  hash_table = new Node *[table_capacity];
  // Readding the array values
  hash_table = new_hash_table;
  // Confirm that the length is accurate
  length = new_length;
}




#endif