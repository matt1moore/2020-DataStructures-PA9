//----------------------------------------------------------------------
// Name: Matthew Moore
// File: avl_collection.h
// Date: Fall 2020
// Desc: AVL Tree Implementation which maintains a Binary Search tree
// which always remains balanced, so the height of the left and right subtree
// of any given node is no greater than 1 and no less than negative one
//---

#ifndef AVL_COLLECTION_H
#define AVL_COLLECTION_H

#include "array_list.h"
#include "collection.h"

template<typename K, typename V>
class AVLCollection : public Collection<K,V> 
{
public:
  AVLCollection();
  AVLCollection(const AVLCollection<K,V>& rhs);
  ~AVLCollection();
  AVLCollection& operator=(const AVLCollection<K,V>& rhs);
  
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
	int height; 
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
  // add helper
  Node* add(Node* subtree_root, const K& a_key, const V& a_val);
  // remove helper
  Node* remove(Node* subtree_root, const K& a_key);
  // helper to recursively build up key list
  void find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
  // helper to recursively build sorted list of keys
  void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;
  // helper to recursively find height of the tree
  size_t height(const Node* subtree_root) const;
  // rotate right helper
  Node* rotate_right(Node* k2);
  // rotate left helper
  Node* rotate_left(Node* k2);
  // rebalance the subtree rooted at subtree_root
  Node* rebalance(Node* subtree_root);
  // print out the tree in ascending order
  void print_tree(std::string indent, Node* subtree_root);

};



// Function Definitions
template<typename K, typename V>
AVLCollection<K,V>::AVLCollection()
  : root(nullptr), node_count(0)
{

}
template<typename K, typename V>
AVLCollection<K,V>::AVLCollection(const AVLCollection<K,V>& rhs)
  : root(nullptr), node_count(0)
{
  // Defer to the assignment operator
  *this = rhs;
}
template<typename K, typename V>
AVLCollection<K,V>::~AVLCollection()
{
  make_empty(root); 
  root = nullptr;
}
template<typename K, typename V>
AVLCollection<K,V>& AVLCollection<K,V>::operator=(const AVLCollection<K,V>& rhs)
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
	  root->height = rhs.root->height;
	  root->left = nullptr;
	  root->right = nullptr; 
	  copy(root,rhs.root); 
	}
  }
  return *this; 
    
}
template<typename K, typename V>
void AVLCollection<K,V>::add(const K& a_key, const V& a_val)
{
  if (!root) {
	// SPECIAL CASE: First node being added
	Node * newNode = new Node;
	newNode->key = a_key;
	newNode->value = a_val;
	newNode->right = nullptr;
	newNode->left = nullptr;
	newNode->height = 1;
	++node_count;
	root = newNode;
  }
  else {
	// REGULAR CASE: Nodes being added to a tree with a root
    add(root,a_key,a_val);
  }
}
template<typename K, typename V>
void AVLCollection<K,V>::remove(const K& a_key)
{
  remove(root,a_key); 
}
template<typename K, typename V>
bool AVLCollection<K,V>::find(const K& search_key, V& the_val) const
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
void AVLCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  find(root,k1,k2,keys);
}
template<typename K, typename V>
void AVLCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  keys(root,all_keys); 
}
template<typename K, typename V>
void AVLCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
}
template<typename K, typename V>
size_t AVLCollection<K,V>::size() const
{
  return node_count;
}	

template<typename K, typename V>
size_t AVLCollection<K,V>::height() const
{
  return height(root);
}

// HELPER FUNCTIONS

template<typename K, typename V>
void AVLCollection<K,V>::make_empty(Node* subtree_root)
{
  if (!subtree_root) {
    // BASE CASE: leaf nodes hit
	return;
  }
  make_empty(subtree_root->left);
  make_empty(subtree_root->right); 
  subtree_root->right = nullptr;
  subtree_root->left = nullptr;
  delete subtree_root;
  
}

template<typename K, typename V>
void AVLCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root)
{
  if(!rhs_subtree_root) { 
    // BASE CASE: all leaf nodes hit
	return;
  }
  
  if (rhs_subtree_root->left != nullptr) {
	Node * newNode = new Node;
    newNode->key = rhs_subtree_root->left->key;
    newNode->value = rhs_subtree_root->left->value;
	newNode->height = rhs_subtree_root->left->height;
    newNode->left = nullptr;
    newNode->right = nullptr; 
    lhs_subtree_root->left = newNode;
	copy(lhs_subtree_root->left,rhs_subtree_root->left);
  }
  if (rhs_subtree_root->right!= nullptr) {
	Node * newNode = new Node;
    newNode->key = rhs_subtree_root->right->key;
    newNode->value = rhs_subtree_root->right->value;
	newNode->height = rhs_subtree_root->right->height;
    newNode->left = nullptr;
    newNode->right = nullptr; 
    lhs_subtree_root->right = newNode;
	copy(lhs_subtree_root->right,rhs_subtree_root->right);
  }
}
template<typename K, typename V>
typename AVLCollection<K,V>::Node *
AVLCollection<K,V>::add(Node* subtree_root, const K& a_key, const V& a_val)
{
  if (!subtree_root) {
	// Inserting new node at the lead node down a specific path
    // Create the node with a a_key and an a_val
	Node * newNode = new Node;
	newNode->key = a_key;
	newNode->value = a_val;
	newNode->right = nullptr;
	newNode->left = nullptr;
	newNode->height = 1;
	++node_count;
	return newNode;
  }
  else {
    // Adding some node not to the root
	if (a_key < subtree_root->key) {
	  // If the added key is less than the current node key then go left
	  subtree_root->left = add(subtree_root->left,a_key,a_val);
	}
	else {
	  // Otherwise the key is larger than the current node so add to the right
	  subtree_root->right = add(subtree_root->right,a_key,a_val);
	}
  }
  // Backtracking actions: add one to every node back up the path to the root
  subtree_root->height = height(subtree_root);
  // Backtracking: rebalance at the subtree_root (as needed)
  return rebalance(subtree_root);
}
template<typename K, typename V>
typename AVLCollection<K,V>::Node *
AVLCollection<K,V>::remove(Node* subtree_root, const K& a_key)
{
  Node * tmp = nullptr;
  if (subtree_root == nullptr) {
    // BASE CASE: lead nodes
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
  if (subtree_root) {
    // Confirm that all heights are updated back up the path
    subtree_root->height = height(subtree_root);
  }
  return rebalance(subtree_root);

}
template<typename K, typename V>
void AVLCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const
{
  if (!subtree_root) {
    // Empty tree base case for leaf nodes
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
void AVLCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const
{
  if (!subtree_root) {
    // Base case for the leaf nodes
    return;
  }
  keys(subtree_root->left,all_keys);
  all_keys.add(subtree_root->key);
  keys(subtree_root->right,all_keys);
}
template<typename K, typename V>
size_t AVLCollection<K,V>::height(const Node* subtree_root) const
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


template<typename K, typename V>
typename AVLCollection<K,V>::Node *
AVLCollection<K,V>::rotate_right(Node* k2)
{
  // Placing k1 into the k2 position
  Node * k1 = k2->left;
  // Making the k2 left subtree assigned with the k1 right subtree
  k2->left = k1->right;
  // Making the k1 right subtree now everything from k2
  k1->right = k2;
  
  if (k2 == root) {
    // If k2 is the root then we must adjust the root to k1
	root = k1;
  }
  return k1;
}


template<typename K, typename V>
typename AVLCollection<K,V>::Node *
AVLCollection<K,V>::rotate_left(Node* k2)
{
  // Same exact process as the right rotation exxept on the opposite side
  Node * k1 = k2->right;
  k2->right = k1->left;
  k1->left = k2;
  
  if (k2 == root) {
    // If k2 is the root then we must adjust the root to k1
	root = k1;
  }
  return k1;
}


template<typename K, typename V>
typename AVLCollection<K,V>::Node *
AVLCollection<K,V>::rebalance(Node* subtree_root)
{
  if (!subtree_root) {
    // Base Case: for the leaf nodes
	return subtree_root;
  }
  
  Node * lptr = subtree_root->left;
  Node * rptr = subtree_root->right;
  
  if (lptr && !rptr && lptr->height > 1) {
	// SPECIAL CASE: left but no right pointer
    // Check for need of double rotation
	if (!lptr->right) {
	  // If the right does not exist then we know its left-left case
	}
	else if (!lptr->left) {
	  // If the left pointer does not exist then we know its left-right case
	  // LEFT-RIGHT CASE: here is a need for a double rotation
	  subtree_root->left = rotate_left(lptr);
	}
	else if (lptr->right->height > lptr->left->height) {
	  // LEFT-RIGHT CASE: here is a need for a double rotation
	  subtree_root->left = rotate_left(lptr);
	}
	// Do rotation right no matter what
	subtree_root = rotate_right(subtree_root);
	// Finally adjust the height
	subtree_root->height = height(subtree_root);
	subtree_root->left->height = height(subtree_root->left);
	subtree_root->right->height = height(subtree_root->right); 
	
  }
  else if (!lptr && rptr && rptr->height > 1) {
    // SPECIAL CASE: right but no left pointer
	// Check if need of double rotation
	if (!rptr->left) {
	  // If the left does not exist then we know its a right-right case
	}
	else if (!rptr->right) {
	  // If the right pointer does not exist then we know its left-right case
	  // RIGHT-LEFT CASE: here is a need for a double rotation
	  subtree_root->right = rotate_right(rptr);
	}
	else if (rptr->left->height > rptr->right->height) {
	  // RIGHT-LEFT CASE: here is a need for a double rotation
	  subtree_root->right = rotate_right(rptr);
	}
	// Do rotation left no matter what
	subtree_root = rotate_left(subtree_root);
	// Adjust the height
	subtree_root->height = height(subtree_root);
	subtree_root->left->height = height(subtree_root->left);
	subtree_root->right->height = height(subtree_root->right); 
  }
  else if (lptr && rptr && lptr->height > rptr->height + 1) {
    // REGULAR CASE: left and right pointer exist and subtree is left heavy
	// Check if need of double rotation
	if (!lptr->right) {
	  // If the right does not exist then we know its left-left case
	}
	else if (!lptr->left) {
	  // If the left pointer does not exist then we know its left-right case
	  // LEFT-RIGHT CASE: here is a need for a double rotation
	  subtree_root->left = rotate_left(lptr);
	}
	else if (lptr->right->height > lptr->left->height) {
	  // LEFT-RIGHT CASE: here is a need for a double rotation
	  subtree_root->left = rotate_left(lptr);
	}
	// Do rotation right no matter what
	subtree_root = rotate_right(subtree_root);
	// Adjust the height
	subtree_root->height = height(subtree_root);
	subtree_root->left->height = height(subtree_root->left);
	subtree_root->right->height = height(subtree_root->right); 
  }
  else if (lptr && rptr && rptr->height > lptr->height + 1) {
    // REGULAR CASE: left and right pointer exist and subtree is right heavy
	// Check if need of double rotation
	if (!rptr->left) {
	  // If the left does not exist then we know its a right-right case
	}
	else if (!rptr->right) {
	  // If the right pointer does not exist then we know its left-right case
	  // RIGHT-LEFT CASE: here is a need for a double rotation
	  subtree_root->right = rotate_right(rptr);
	}
	if (rptr->left->height > rptr->right->height) {
	  // RIGHT-LEFT CASE: here is a need for a double rotation
	  subtree_root->right = rotate_right(rptr);
	}
	// Do rotation left no matter what
	subtree_root = rotate_left(subtree_root);
	// Adjust the height
	subtree_root->height = height(subtree_root);
	subtree_root->left->height = height(subtree_root->left);
	subtree_root->right->height = height(subtree_root->right); 
  }
  return subtree_root;
}

template<typename K, typename V>
void AVLCollection<K,V>::print_tree(std::string indent, Node* subtree_root)
{
  if (!subtree_root) {
    return;
  }
  std::cout << indent << subtree_root->key << " (h=" << subtree_root->height << ")" << std::endl;
  print_tree(indent + " ", subtree_root->left);
  print_tree(indent + " ", subtree_root->right);
}

#endif