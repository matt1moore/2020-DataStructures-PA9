
#ifndef BIN_SEARCH_COLLECTION_H
#define BIN_SEARCH_COLLECTION_H

#include "array_list.h"
#include "array_list_collection.h"
#include "collection.h"


template<typename K, typename V>
class BinSearchCollection : public Collection<K,V> 
{
public:
  void add(const K& a_key, const V& a_val);
  void remove(const K& a_key);
  bool find(const K& search_key, V& the_val) const;
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  void keys(ArrayList<K>& all_keys) const;
  void sort(ArrayList<K>& all_keys_sorted) const;
  size_t size() const;
private:
  ArrayList<std::pair<K,V>> kv_list;
  // binary search helper function
  bool bin_search(const K& key, size_t& index) const;
};

template<typename K, typename V>
void BinSearchCollection<K,V>::add(const K& a_key, const V& a_val)
{
  pair<K,V> p;
  size_t index;
  bool found;
  
  found = bin_search(a_key,index);
  p.first = a_key;
  p.second = a_val;
  
  if (found == false) {
    kv_list.add(index,p);
  }
  else {
    // Key already there, do nothing
  }
}
template<typename K, typename V>
void BinSearchCollection<K,V>::remove(const K& a_key)
{
  size_t index;
  bool found;
  
  found = bin_search(a_key,index);
  
  if (found == true) {
    // We can remove the element
	kv_list.remove(index);
  }
  else {
    // Do nothing since item not found
  }
}
template<typename K, typename V>
bool BinSearchCollection<K,V>::find(const K& search_key, V& the_val) const 
{
  pair<K,V> p;
  size_t index;
  bool found;
  
  found = bin_search(search_key,index);
  
  if (found == true) {
    // Element is found
	kv_list.get(index,p);
	the_val = p.second;
	return true;
  }
  else {
    return false;
  }
	
}
template<typename K, typename V>
void BinSearchCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const 
{
  pair<K,V> p;
  size_t index;
  
  bin_search(k1,index); 
  // No need to see if k1 was found, index is the only item with meaning
  kv_list.get(index,p);
  
  while (p.first <= k2 && index < kv_list.size()) {
    keys.add(p.first);
    ++index;
    kv_list.get(index,p);
  }
}
template<typename K, typename V>
void BinSearchCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  pair<K,V> p;
  for (int i = 0; i < kv_list.size(); ++i) {
    kv_list.get(i,p);
	all_keys.add(p.first);
  }
}
template<typename K, typename V>
void BinSearchCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const 
{
  keys(all_keys_sorted);
}
template<typename K, typename V>
size_t BinSearchCollection<K,V>::size() const
{
  size_t array_size = 0;
  array_size = kv_list.size();
  return array_size;
}
template<typename K, typename V>
bool BinSearchCollection<K,V>::bin_search(const K& key, size_t& index) const
{
  size_t left = 0, right = kv_list.size() - 1;
  pair<K,V> p;
  index = (right + left) / 2;
  kv_list.get(index,p);
  
  if (right == -1) {
    // No elements in the list 
	index = 0;
	return false;
  }
  while (p.first != key && right != left) {
	// Runs until index is at the value
    if (key < p.first) {
      // Key pair is greater than current position
	  right = index;
    }
    else {
      // Key pair is less than the current position
	  left = index + 1;
	}
	index = (right + left) / 2;
    kv_list.get(index,p);
  }
  
  if (p.first == key) {
    // Found the key
	return true;
  }
  else if (key > p.first) {
    // No key found, so set position to the right
	index = index + 1;
	return false;
  }
  else {
    // No key found, so set posiiton to current index
	return false;
  }
	
}

#endif
