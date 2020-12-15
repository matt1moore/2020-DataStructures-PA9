//----------------------------------------------------------------------
// Name: Matthew Moore
// File: bst_collection.h
// Date: Fall 2020
// Desc: Binary Search Tree Implementation 
//---

#ifndef BST_COLLECTION_H
#define BST_COLLECTION_H

#include "array_list.h"
#include "collection.h"


template<typename K, typename V>
class BSTCollection : public Collection<K,V> 
{
public:
  BSTCollection();
  BSTCollection(const BSTCollection<K,V>& rhs);
  ~BSTCollection();
  BSTCollection& operator=(const BSTCollection<K,V>& rhs);

  void add(const K& a_key, const V& a_val);
  void remove(const K& a_key);
  bool find(const K& search_key, V& the_val) const;
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  void keys(ArrayList<K>& all_keys) const;
  void sort(ArrayList<K>& all_keys_sorted) const;
  size_t size() const;  
  size_t height() const;
  
private:
  // tree node
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
  };
  // root node of the tree
  Node* root;
  // number of k-v pairs stored in the collection
  size_t node_count;
  // remove all elements in the bst
  void make_empty(Node* subtree_root);
  // copy helper
  void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root);
  // remove helper
  Node* remove(Node* subtree_root, const K& a_key);
  // helper to recursively build up key list
  void find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
  // helper to recursively build sorted list of keys
  void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;
  // helper to recursively find height of the tree
  size_t height(const Node* subtree_root) const;
  
};

// Function Definitions
template<typename K, typename V>
BSTCollection<K,V>::BSTCollection()
  : root(nullptr), node_count(0)
{

}
template<typename K, typename V>
BSTCollection<K,V>::BSTCollection(const BSTCollection<K,V>& rhs)
  : root(nullptr), node_count(0)
{
  // Defer to the assignment operator
  *this = rhs;
}
template<typename K, typename V>
BSTCollection<K,V>::~BSTCollection()
{
  make_empty(root); 
  root = nullptr;
}
template<typename K, typename V>
BSTCollection<K,V>& BSTCollection<K,V>::operator=(const BSTCollection<K,V>& rhs)
{
  if (this != &rhs) { // protects against the self assignment case 
    node_count = 0;
	if (root != nullptr) {
	  make_empty(root);
	  root = nullptr;
	}
	// Assignment made to make the binary trees identical, with unique memory addresses
	node_count = rhs.node_count;
	
	if (rhs.root != nullptr) {
	  // Create the root node for the copy assignment
	  root = new Node;
	  root->key = rhs.root->key;
	  root->value = rhs.root->value;
	  root->left = nullptr;
	  root->right = nullptr; 
	  copy(root,rhs.root); 
	}
  }
  return *this; 
    
}
template<typename K, typename V>
void BSTCollection<K,V>::add(const K& a_key, const V& a_val)
{
  // Creating the new node
  Node * curr_ptr = root;
  Node * newNode = new Node;
  newNode->key = a_key;
  newNode->value = a_val;
  newNode->left = nullptr;
  newNode->right = nullptr;
		
  // Case of first element being added
  if (curr_ptr == nullptr) {
    // Root points to NULL so add initial node as the root
	root = newNode;
	root->left = nullptr;
	root->right = nullptr;
	++node_count;
	return;
  }
  
  // Traversing down a path to the end 
  while (curr_ptr != nullptr) {
    if (a_key > curr_ptr->key) {
	  if (curr_ptr->right == nullptr) {
	    // leaf node has been reached
		curr_ptr->right = newNode;
		++node_count;
		return;
	  }
	  // The key value being added is larger than the current node in the list, so go right
	  curr_ptr = curr_ptr->right;
	}
    else if (a_key < curr_ptr->key) {
	  if (curr_ptr->left == nullptr) {
	    // leaf node has been reached
		curr_ptr->left = newNode;
		++node_count;
		return;
	  }
	  // The key value added is smaller than the current node, so go left
	  curr_ptr = curr_ptr->left;
	}
	else {
	  // Value already exists in the list.. so do nothing
	  return;
	}
  }
}
template<typename K, typename V>
void BSTCollection<K,V>::remove(const K& a_key)
{
  remove(root,a_key); 
}
template<typename K, typename V>
bool BSTCollection<K,V>::find(const K& search_key, V& the_val) const
{
  Node * curr_ptr = root;
  
  while (curr_ptr != NULL) {
    // Search down a path in the binary tree to find the node
	if (curr_ptr->key == search_key) {
      // Key has been located
	  the_val = curr_ptr->value;
	  return true;
	}
	else if (search_key < curr_ptr->key) {
	  // key being removed is greater than the current key
	  curr_ptr = curr_ptr->left;
	}
	else if (search_key > curr_ptr->key) {
	  // key being removed is less than the current key
	  curr_ptr = curr_ptr->right;
	}
  }
  // This point is only reached if the node searched for is not found
  return false;
}
template<typename K, typename V>
void BSTCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  find(root,k1,k2,keys);
}
template<typename K, typename V>
void BSTCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  keys(root,all_keys); 
}
template<typename K, typename V>
void BSTCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
}
template<typename K, typename V>
size_t BSTCollection<K,V>::size() const
{
  return node_count;
}	

template<typename K, typename V>
size_t BSTCollection<K,V>::height() const
{
  return height(root);
}

// HELPER FUNCTIONS

template<typename K, typename V>
void BSTCollection<K,V>::make_empty(Node* subtree_root)
{
  if (!subtree_root) {
    // BASE CASE
	return;
  }
  make_empty(subtree_root->left);
  make_empty(subtree_root->right); 
  subtree_root->right = nullptr;
  subtree_root->left = nullptr;
  delete subtree_root;
  
}

template<typename K, typename V>
void BSTCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root)
{
  if(!rhs_subtree_root) { 
    // BASE CASE
	return;
  }
  
  if (rhs_subtree_root->left != nullptr) {
	Node * newNode = new Node;
    newNode->key = rhs_subtree_root->left->key;
    newNode->value = rhs_subtree_root->left->value;
    newNode->left = nullptr;
    newNode->right = nullptr; 
    lhs_subtree_root->left = newNode;
	copy(lhs_subtree_root->left,rhs_subtree_root->left);
  }
  if (rhs_subtree_root->right!= nullptr) {
	Node * newNode = new Node;
    newNode->key = rhs_subtree_root->right->key;
    newNode->value = rhs_subtree_root->right->value;
    newNode->left = nullptr;
    newNode->right = nullptr; 
    lhs_subtree_root->right = newNode;
	copy(lhs_subtree_root->right,rhs_subtree_root->right);
  }
  
  
  
}

template<typename K, typename V>
typename BSTCollection<K,V>::Node *
BSTCollection<K,V>::remove(Node* subtree_root, const K& a_key)
{
  Node * tmp = nullptr;
  if (subtree_root == nullptr) {
    // BASE CASE
	return NULL;
  }
  
  // Locator of the node which needs to be removed
  if (subtree_root != nullptr && (a_key < subtree_root->key)) {
	// Removing element is to the left
    subtree_root->left = remove(subtree_root->left,a_key);
  }
  else if (subtree_root != nullptr && (a_key > subtree_root->key)) {
	// Removing element is to the right
    subtree_root->right = remove(subtree_root->right,a_key);
  }
  else if (subtree_root != nullptr && (a_key == subtree_root->key)) {
    // The element to remove has been reached
	if (height(subtree_root) == 1) {
	  // CASE 1: Leaf Node
	  if (subtree_root == root) {
	    // Special Case of root removed
		root = tmp;
	  }
	  delete subtree_root;
	  --node_count;
      // Assigning value to subtree_root to get reconnected to end of tree
	  subtree_root = tmp;
	}
	else if (height(subtree_root) == 2 && (subtree_root->right == nullptr || subtree_root->left == nullptr)) {
	  // CASE 2: Single Child Case
	  if (subtree_root->right == nullptr) {
	    // Tells us that only the left subtree exists
	    tmp = subtree_root->left;
	    }
	  else {
	    // Tells us that only the right subtree exists
		tmp = subtree_root->right;
	  }
	  if (subtree_root == root) { 
	    root = tmp; 
	  }
	  delete subtree_root;
	  --node_count; 
	  // Assigning value to subtree_root to get reconnected to end of tree
	  subtree_root = tmp;
	}
	else if (subtree_root->right->left == nullptr) {
	  // CASE 3: Two Children and no need for in order successor
	  tmp = subtree_root->right;
	  subtree_root->right->left = subtree_root->left;
	  if (subtree_root == root) {
	    // Case of the root being removed
		root = subtree_root->right;
	  }
	  delete subtree_root;
	  --node_count;
	  subtree_root = tmp;
	}
	else {
	  // CASE 4: Two Children and inorder successor needed
	  // Initially go to the right
	  tmp = subtree_root->right; 
	  // Traverse left until the left most node is reached
	  while (tmp->left != nullptr) {
	    tmp = tmp->left;
	  }
	  // Copy over the values of this node
	  subtree_root->key = tmp->key;
	  subtree_root->value = tmp->value;
	  // Finally remove the inorder successor
	  subtree_root->right = remove(subtree_root->right,tmp->key); 
	}
  }
  return subtree_root;
}
template<typename K, typename V>
void BSTCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const
{
  if (!subtree_root) {
    // Empty tree base case
	return;
  }

  if (k1 > subtree_root->key) {
    find(subtree_root->right,k1,k2,keys);
  }
  else if (k1 <= subtree_root->key && subtree_root->key <= k2) {
	keys.add(subtree_root->key);
    find(subtree_root->left,k1,k2,keys);
	find(subtree_root->right,k1,k2,keys);
  }
  else if (k2 < subtree_root->key) {
    find(subtree_root->left,k1,k2,keys); 
  }
}
template<typename K, typename V>
void BSTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const
{
  if (!subtree_root) {
    // Base case
    return;
  }
  keys(subtree_root->left,all_keys);
  all_keys.add(subtree_root->key);
  keys(subtree_root->right,all_keys);
}
template<typename K, typename V>
size_t BSTCollection<K,V>::height(const Node* subtree_root) const
{
  size_t left, right;
  
  if (subtree_root == NULL) {
    return 0;
  }
  else {
    left = height(subtree_root->left);
    right = height(subtree_root->right);
    return max(left,right) + 1;
  }
}

#endif