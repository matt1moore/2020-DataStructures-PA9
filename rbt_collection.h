//----------------------------------------------------------------------
// Name: 
// File: rbt_collection.h
// Date: Fall, 2020
// Desc: A Key-Value Collection implementation using a Red-Black tree.
//----------------------------------------------------------------------


#ifndef RBT_COLLECTION_H
#define RBT_COLLECTION_H


#include "string.h"
#include "collection.h"
#include "array_list.h"


template<typename K, typename V>
class RBTCollection : public Collection<K,V>
{
public:

  // create an empty collection
  RBTCollection();
  
  // copy constructor
  RBTCollection(const RBTCollection<K,V>& rhs);

  // assignment operator
  RBTCollection<K,V>& operator=(const RBTCollection<K,V>& rhs);

  // delete collection
  ~RBTCollection();
  
  // add a new key-value pair into the collection 
  void add(const K& a_key, const V& a_val);

  // remove a key-value pair from the collectiona
  void remove(const K& a_key);

  // find and return the value associated with the key
  bool find(const K& search_key, V& the_val) const;

  // find and return each key >= k1 and <= k2 
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  
  // return all of the keys in the collection 
  void keys(ArrayList<K>& all_keys) const;

  // return all of the keys in ascending (sorted) order
  void sort(ArrayList<K>& all_keys_sorted) const;

  // return the number of key-value pairs in the collection
  size_t size() const;

  // return the height of the tree
  size_t height() const;

  // for testing:

  // check if tree satisfies the red-black tree constraints
  bool valid_rbt() const;
  
  // pretty-print the red-black tree (with heights)
  void print() const;

  
private:
  
  // RBT node structure
  enum color_t {RED, BLACK};
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
    Node* parent;
    color_t color;
  };

  // root node
  Node* root;

  // number of k-v pairs stored in the collection
  size_t node_count;

  // helper to empty entire hash table
  void make_empty(Node* subtree_root);

  // copy helper
  void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root); 
    
  // helper to recursively find range of keys
  void find(const Node* subtree_root, const K& k1, const K& k2,
            ArrayList<K>& keys) const;

  // helper to build sorted list of keys (used by keys and sort)
  void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;

  // rotate right helper
  void rotate_right(Node* k2);

  // rotate left helper
  void rotate_left(Node* k2);

  // restore red-black constraints in add
  void add_rebalance(Node* x);

  // restore red-black constraints in remove
  void remove_rebalance(Node* x, bool going_right);
  
  // height helper
  size_t height(Node* subtree_root) const;
  
  // ------------
  // for testing:
  // ------------
  
  // validate helper
  bool valid_rbt(Node* subtree_root) const;

  // validate helper
  size_t black_node_height(Node* subtree_root) const;
  
  // recursive pretty-print helper
  void print_tree(std::string indent, Node* subtree_root) const;
};


// TODO: Finish the above functions below
template<typename K, typename V> 
RBTCollection<K,V>::RBTCollection()
  : root(nullptr), node_count(0)
{

}

template<typename K, typename V> 
RBTCollection<K,V>::RBTCollection(const RBTCollection<K,V>& rhs)
  : root(nullptr), node_count(0)
{
  *this = rhs;
}

template<typename K, typename V>
RBTCollection<K,V>& RBTCollection<K,V>::operator=(const RBTCollection<K,V>& rhs)
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
	  root->color = rhs.root->color;
	  root->parent = rhs.root->parent;
	  root->left = nullptr;
	  root->right = nullptr; 
	  copy(root,rhs.root); 
	}
  }
  return *this; 
}


template<typename K, typename V> 
RBTCollection<K,V>::~RBTCollection()
{
  make_empty(root);
  root = nullptr; 
}

template<typename K, typename V> 
void RBTCollection<K,V>::add(const K& a_key, const V& a_val)
{
  
  // SPECIAL CASE: First node being added
  Node * newNode = new Node;
  newNode->key = a_key;
  newNode->value = a_val;
  newNode->color = RED;
  newNode->right = nullptr;
  newNode->left = nullptr;
  newNode->parent = nullptr;
  ++node_count;

  Node * x = root;
  Node * p = nullptr;
  while (x != nullptr) {
    //  Traverse down a path to get to add node to a leaf nodes child
	add_rebalance(x);
	p = x;
	if (a_key < x->key) {
	  // The added key is smaller than the current key so traverse to the left
      x = x->left;
	}
	else {
	  // The added key is larger than the current key so traverse to the right
      x = x->right;
	}
  }
  if (p == nullptr) {
    // Root addition case
	root = newNode;
  }
  else if (a_key < p->key) {
    // Key value is less than the parent so add to left subtree
	p->left = newNode;
	newNode->parent = p; 
  }
  else {
    // Key value is greater than the parent so add to right subtree
	p->right = newNode;
	newNode->parent = p; 
  }
  add_rebalance(newNode);
  root->color = BLACK;
  
}
template<typename K, typename V> 
void RBTCollection<K,V>::remove(const K& a_key)
{
  // Book varible to check if the key is found
  bool found = false;
  // Create the sentinel as the fake root
  Node * sentinel = new Node;
  sentinel->right = root;
  sentinel->left = nullptr;
  sentinel->color = RED;
  sentinel->parent = nullptr;
  // Set a curr pointer
  Node * x = root;
  // Set a parent pointer
  Node * p = sentinel;
  // Iteravely find node to delete
  while (x != nullptr && found != true) {
    if (a_key < x->key) {
	  // The key to be found is less than the current key
	  remove_rebalance(x,false);
	  x = x->left;
	}
	else if (a_key > x->key) {
	  // The key to be found is greater than the current key
	  remove_rebalance(x,true);
	  x = x->right;
	}
	else {
	  // The key has been found to remove
	  // Rebalance called with true if x has a left and false if x left is nullptr
	  remove_rebalance(x,x->left);
	  found = true;
	  p = x->parent;
	}
  }
  // Either not found or sitting at the node to delete
  if (found == false) {
    // The item was not found so return
	return;
  }
  if (x->left == nullptr || x->right == nullptr) {
    // Current element has either 1 or no children
	if (x == root) {
	  // Special case of adjusting a new root
	  if (x->left) {
	    // If the left exists, it becomes the new root
		sentinel->right = x->left; 
	  }
	  else if (x->right) {
	    // If the right exists then it will be the new root
		sentinel->right = x->right;
	  }
	  else {
	    // Removing final element in list
		sentinel->right = nullptr;
	  }
	}
	else if (x->key < p->key) {
      // The current key is less than its parent so it must be on the left
	  sentinel->right = root; 
	  p->left = nullptr;
	}
	else {
	  // The current key is greater than its parent so on the right
	  sentinel->right = root;
	  p->right = nullptr;
	}
	delete x;
	x = nullptr;
  }
  else {
    // X has 2 children 
	// Locate the successor
	Node * s = x->right;
	remove_rebalance(s,false); 
	while (s->left != nullptr) {
      s = s->left;
	  remove_rebalance(s,false);
	}
	
	// Make sure sentinel is working right 
	sentinel->right = root;
	// Copy s key_value into x
	x->key = s->key;
	x->value = x->value; 
	// Remove s cases
	if (s->key < s->parent->key) {
	  // The key being removed is to the left
	  s->parent->left = nullptr;
	}
	else {
	  // The key being removed is on the right
	  s->parent->right = nullptr;
	}
	delete s;
	s = nullptr;
  }
  // Now we do the dirty clean up work
  root = sentinel->right;
  if (root != nullptr) {
	// Set root color whenever it exists
    root->color = BLACK;
  }
  delete sentinel;
  // Decrementing node count for the removed node ?
  --node_count;
}

template<typename K, typename V> 
bool RBTCollection<K,V>::find(const K& search_key, V& the_val) const
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
void RBTCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  find(root,k1,k2,keys);
}

template<typename K, typename V> 
void RBTCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  keys(root,all_keys);
}

template<typename K, typename V> 
void RBTCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
}

template<typename K, typename V> 
size_t RBTCollection<K,V>::size() const
{
  return node_count;
}

template<typename K, typename V> 
size_t RBTCollection<K,V>::height() const
{
  return height(root); 
}

//------------------------------------
// Recursive Functions:
//------------------------------------

template<typename K, typename V> 
void RBTCollection<K,V>::make_empty(Node* subtree_root)
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
void RBTCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root)
{
 if(!rhs_subtree_root) { 
    // BASE CASE: all leaf nodes hit
	return;
  }
  
  if (rhs_subtree_root->left != nullptr) {
	Node * newNode = new Node;
    newNode->key = rhs_subtree_root->left->key;
    newNode->value = rhs_subtree_root->left->value;
	newNode->parent = rhs_subtree_root->left->parent;
	newNode->color = rhs_subtree_root->left->color;
    newNode->left = nullptr;
    newNode->right = nullptr; 
    lhs_subtree_root->left = newNode;
	copy(lhs_subtree_root->left,rhs_subtree_root->left);
  }
  if (rhs_subtree_root->right!= nullptr) {
	Node * newNode = new Node;
    newNode->key = rhs_subtree_root->right->key;
    newNode->value = rhs_subtree_root->right->value;
	newNode->parent = rhs_subtree_root->right->parent;
	newNode->color = rhs_subtree_root->right->color;
    newNode->left = nullptr;
    newNode->right = nullptr; 
    lhs_subtree_root->right = newNode;
	copy(lhs_subtree_root->right,rhs_subtree_root->right);
  }
}

template<typename K, typename V> 
void RBTCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const
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
void RBTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const
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
void RBTCollection<K,V>::rotate_right(Node* k2)
{
  // Placing k1 into the k2 position
  Node * k1 = k2->left;
  // Making the k2 left subtree assigned with the k1 right subtree
  k2->left = k1->right;
  // 1. update k2's new left node's parent, which could be a null node
  if (k2->left != nullptr) {
    k2->left->parent = k2;
  }
  // 2. Update k1's parent, with k2's parent
  k1->parent = k2->parent;
  //3. Set k1's new parent to point to k1
  if (k1 == nullptr) {
    // We do not worry about this for now
  }
  else if (k1->parent == nullptr) {
    // k1 is becoming the root, so no need to set the parent
  }
  else if (k1->parent->key > k1->key) {
	// If k1's parent is greater then k1, then it will be on the left
    k1->parent->left = k1;
  }
  else {
	// Otherwise k1's parent's right will be k1
	k1->parent->right = k1;
  }
  // Making the k1 right subtree now everything from k2
  k1->right = k2;
  k2->parent = k1;
  
  if (k2 == root) {
    // If k2 is the root then we must adjust the root to k1
	root = k1;
  }
}

template<typename K, typename V> 
void RBTCollection<K,V>::rotate_left(Node* k2)
{
  // Same exact process as the right rotation exxept on the opposite side
  Node * k1 = k2->right;
  k2->right = k1->left;
  // 1. update k2's new right node's parent, which could be a null node
  if (k2->right != nullptr) {
    k2->right->parent = k2;
  }
  // 2. Update k1's parent, with k2's parent
  k1->parent = k2->parent;
  //3. Set k1's new parent to point to k1
  if (k1 == nullptr) {
    // We do not worry about this for now
  }
  else if (k1->parent == nullptr) {
    // Case of k1 becoming the root
  }
  else if (k1->parent->key > k1->key) {
	// If k1's parent is greater then k1, then it will be on the left
    k1->parent->left = k1;
  }
  else {
	// Otherwise k1's parent's right will be k1
	k1->parent->right = k1;
  }
  k1->left = k2;
  k2->parent = k1;
  
  if (k2 == root) {
    // Must switch root since k1 is now on a higher level than k2
	root = k1;
  }
}

template<typename K, typename V> 
void RBTCollection<K,V>::add_rebalance(Node* x)
{
  Node * p = x->parent;
  if (x->right != nullptr && x->left != nullptr) {
	// Color flip may only occur if the current node has children
    // Case 1: Color Flip
    if (x->color == BLACK && x->right->color == RED && x->left->color == RED) {
      x->color = RED;
	  x->left->color = BLACK;
	  x->right->color = BLACK;
    }
  }
  // Case 2 and 3: Rotations
  if (p == nullptr) {
	// Special case of the curr pointer being the root
	return;
  }
  if (x->color == RED && p->color == RED) {
    if (p->parent->key < p->key) {
      // P is to the right of its parent
	  // Handle inside (right-left) case
	  if (x->key < p->key) {
	    // x is the left child of p
	    rotate_right(p);
	    x = p;
	  }
	  // Handle outside (right-right) case
	  rotate_left(x->parent->parent);
	  x->parent->color = BLACK;
	  x->parent->left->color = RED;
    }
    else if (p->parent->key > p->key) {
      // P is to the left of its parent
	  // Handle inside (left-right) case
	  if (x->key > p->key) {
	    // x is the right child of p
	    rotate_left(p);
	    x = p;
	  }
	  // Handle outside (left-left) case
	  rotate_right(x->parent->parent);
	  x->parent->color = BLACK;
	  x->parent->right->color = RED;
    }
  }	
}

template<typename K, typename V> 
void RBTCollection<K,V>::remove_rebalance(Node* x, bool going_right)
{
  // If x is RED we are done
  if (x->color == RED) {
    return;
  }
  // Initialize some pointers
  Node * p = x->parent;
  Node * t = nullptr;
  if (p == nullptr) {
    // x is the root so no sibiling
    // t remains as nullptr 
  }	
  else if (x->key < p->key) {
    // This means x is on the left, so t is to the right of p
	t = p->right;
  }
  else {
    // This means x is on the right, so t is on the left
	t = p->left;
  }
  
  if (x->right && x->left) {
    // Both of x's children exist
	if ((x->right->color == RED && x->left->color == BLACK)|| (x->left->color == RED && x->right->color == BLACK)) {
	  // Potential case 1 situation
	  // CASE 1: x has non-navigation red child
      if (going_right && x->left->color == RED) {
        // Rotate right and color adjust
	    rotate_right(x);
	    // Left child of x becomes the new subtree root
	    x->parent->color = BLACK;
	    x->color = RED; 
	    x->right->color = BLACK; 
	  }
	  else if (!going_right && x->right->color == RED) {
	    // Rotate left and color adjust
	    rotate_left(x);
		// Clean up potential root problems
		if (x == root) {
		  // parent of x will always be the new root
		  root = x->parent;
		}
	    // Right child of x becomes the subtree root
	    x->parent->color = BLACK;
	    x->color = RED;
	    x->left->color = BLACK;
	  }
	}
	else if (!t) {
	  // The sibiling of x does not exist, so we are done
	  return;
	}
	else if (t->color == RED || (x->right->color == RED || x->left->color == RED)) {
	  // No more cases apply if t is red
	  return;
	}
	else if (t->right && t->left) {
	  // The sibiling of x has both children
	  // Note, black has two black children at this point
	  if (t->right->color == BLACK && t->left->color == BLACK) {
	    // CASE 2: Color Flip, all children are black if true !
	    p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else if (t->right->color == RED) {
	    // CASE 3 OR 4: Outside or inside red child
		if (t->key < x->key) {
		  // Left-Right Case
		  rotate_left(t);
	      rotate_right(p); 
		}
		else {
		  // Right-Right Case
		  rotate_left(p);
	      t->color = RED;
	      t->right->color = BLACK;
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	  else {
		// CASE 3 OR 4: Outside or inside red child
	    // t has a red left child 
		if (t->key < x->key) {
		  // Left-Left Case
		  rotate_right(p);
	      t->color = RED;
	      t->left->color = BLACK;
		}
		else {
		  // Right-Left Case
		  rotate_right(t);
	      rotate_left(p);
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	}
	else if (t->right) {
	  // Only the right child of x's sibiling exists
	  if (t->right->color == BLACK) {
	    // CASE 2: Color Flip, all children are black !
        p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else {
	    // t has only a red right child
		// CASE 3 OR 4: Outside or inside red child
		if (t->key < x->key) {
		  // Left-Right Case
		  rotate_left(t);
	      rotate_right(p); 
		}
		else {
		  // Right-Right Case
		  rotate_left(p);
	      t->color = RED;
	      t->right->color = BLACK;
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	}
	else if (t->left) {
      // Only the left child of x's sibiling exists
	  if (t->left->color == BLACK) {
	    // CASE 2: Color Flip, all children are black !
	    p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else {
	    // t has only a red left child
		// CASE 3 OR 4: Outside or inside red child
		if (t->key < x->key) {
		  // Left-Left Case
		  rotate_right(p);
	      t->color = RED;
	      t->left->color = BLACK;
		}
		else {
		  // Right-Left Case
		  rotate_right(t);
	      rotate_left(p);
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	}
	else {
	  // Neither of t's children exist, so they are both black nodes
	  // CASE 2: Color Flip, all children are black if true !
	  p->color = BLACK;
	  t->color = RED;
	  x->color = RED;
	}
  }
  else if (x->right) {
	// The right of x only exists, so check for case 1
	if (x->right->color == RED) {
	  // We found a potential case 1 situation
	  if (!going_right && x->right->color == RED) {
	    // Rotate left and color adjust
	    rotate_left(x);
	    // Right child of x becomes the subtree root
	    x->parent->color = BLACK;
	    x->color = RED;
		if (x->left) {
	      x->left->color = BLACK;
		}
	  }
	}
	else if (!t) {
	  // The sibiling of x does not exist, so we are done
	  return;
	}
	else if (t->color == RED || x->right->color == BLACK) {
	  // No more cases apply if t is red, or if the children of x are not black
	  return;
	}
	else if (t->right && t->left) {
	  // The sibiling of x has both children
	  // Note, black has two black children
	  if (t->right->color == BLACK && t->left->color == BLACK) {
	    // CASE 2: Color Flip, all children are black !
	    p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else if (t->right->color == RED) {
	    // CASE 3 OR 4: Outside or inside red child
		if (t->key < x->key) {
		  // Left-Right Case
		  rotate_left(t);
	      rotate_right(p); 
		}
		else {
		  // Right-Right Case
		  rotate_left(p);
	      t->color = RED;
	      t->right->color = BLACK;
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	  else {
		// CASE 3 OR 4: Outside or inside red child
	    // t has a red left child 
		if (t->key < x->key) {
		  // Left-Left Case
		  rotate_right(p);
	      t->color = RED;
	      t->left->color = BLACK;
		}
		else {
		  // Right-Left Case
		  rotate_right(t);
	      rotate_left(p);
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	}
	else if (t->right) {
	  // Only the right child of x's sibiling exists
	  if (t->right->color == BLACK) {
	    // CASE 2: Color Flip, all children are black !
	    p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else {
	    // t has only a red right child
		// CASE 3 OR 4: Outside or inside red child
		if (t->key < x->key) {
		  // Left-Right Case
		  rotate_left(t);
	      rotate_right(p); 
		}
		else {
		  // Right-Right Case
		  rotate_left(p);
	      t->color = RED;
	      t->right->color = BLACK;
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	}
	else if (t->left) {
      // Only the left child of x's sibiling exists
	  if (t->left->color == BLACK) {
	    // CASE 2: Color Flip, all children are black !
	    p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else {
	    // t has only a red left child
		// CASE 3 OR 4: Outside or inside red child
		if (t->key < x->key) {
		  // Left-Left Case
		  rotate_right(p);
	      t->color = RED;
	      t->left->color = BLACK;
		}
		else {
		  // Right-Left Case
		  rotate_right(t);
	      rotate_left(p);
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	}
	else {
	  // Neither of t's children exist, so they are both black nodes
	  // CASE 2: Color Flip, all children are black if true !
	  p->color = BLACK;
	  t->color = RED;
	  x->color = RED;
	}
  }
  else if (x->left) {
	// The left of x only exists, so check for case 1
	if (x->left->color == RED) {
	  // We found a case 1 situation
	  if (going_right && x->left->color == RED) {
        // Rotate right and color adjust
	    rotate_right(x);
	    // Left child of x becomes the new subtree root
	    x->parent->color = BLACK;
	    x->color = RED; 
		if (x->right) {
	      x->right->color = BLACK; 
		}
	  }
	}
	else if (!t) {
	  // The sibiling of x does not exist, so we are done
	  return;
	}
	else if (t->color == RED || x->left->color == RED) {
	  // No more cases apply if t is red, or if x's children are not black
	  return;
	}
	else if (t->right && t->left) {
	  // The sibiling of x has both children
	  // Note, black has two black children
	  if (t->right->color == BLACK) {
	    // CASE 2: Color Flip, all children are black !
	    p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else if (t->right->color == RED) {
	    // CASE 3 OR 4: Outside or inside red child
		if (t->key < x->key) {
		  // Left-Right Case
		  rotate_left(t);
	      rotate_right(p); 
		}
		else {
		  // Right-Right Case
		  rotate_left(p);
	      t->color = RED;
	      t->right->color = BLACK;
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	  else {
		// CASE 3 OR 4: Outside or inside red child
	    // t has a red left child 
		if (t->key < x->key) {
		  // Left-Left Case
		  rotate_right(p);
	      t->color = RED;
	      t->left->color = BLACK;
		}
		else {
		  // Right-Left Case
		  rotate_right(t);
	      rotate_left(p);
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	}
	else if (t->right) {
	  // Only the right child of x's sibiling exists
	  if (t->right->color == BLACK) {
	    // CASE 2: Color Flip, all children are black !
	    p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else {
	    // t has only a red right child
		// CASE 3 OR 4: Outside or inside red child
		if (t->key < x->key) {
		  // Left-Right Case
		  rotate_left(t);
	      rotate_right(p); 
		}
		else {
		  // Right-Right Case
		  rotate_left(p);
	      t->color = RED;
	      t->right->color = BLACK;
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	}
	else if (t->left) {
      // Only the left child of x's sibiling exists
	  if (t->left->color == BLACK) {
	    // CASE 2: Color Flip, all children are black !
	    p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else {
	    // t has only a red left child
		// CASE 3 OR 4: Outside or inside red child
		if (t->key < x->key) {
		  // Left-Left Case
		  rotate_right(p);
	      t->color = RED;
	      t->left->color = BLACK;
		}
		else {
		  // Right-Left Case
		  rotate_right(t);
	      rotate_left(p);
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	}
	else {
	  // Neither of t's children exist, so they are both black nodes
	  // CASE 2: Color Flip, all children are black !
	  p->color = BLACK;
	  t->color = RED;
	  x->color = RED;
	}
  }
  else {
    // X does not have either children, so 2 black children
	if (!t) {
	  // The sibiling of x does not exist, so we are done
	  return; 
	}
	else if (t->color == RED) {
	  // Immediately return since no further cases apply
	  return;
	}
	else if (t->right && t->left) {
	  // The sibiling of x has both children
	  // Note, black has two black children
	  if (t->right->color == BLACK && t->left->color == BLACK) {
	    // CASE 2: Color Flip, all children are black !
	    p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else if (t->right->color == RED) {
	    // CASE 3 OR 4: Outside or inside red child
		if (t->key < x->key) {
		  // Left-Right Case
		  rotate_left(t);
	      rotate_right(p); 
		}
		else {
		  // Right-Right Case
		  rotate_left(p);
	      t->color = RED;
	      t->right->color = BLACK;
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	  else {
		// CASE 3 OR 4: Outside or inside red child
	    // t has a red left child 
		if (t->key < x->key) {
		  // Left-Left Case
		  rotate_right(p);
	      t->color = RED;
	      t->left->color = BLACK;
		}
		else {
		  // Right-Left Case
		  rotate_right(t);
	      rotate_left(p);
		}
		p->color = BLACK;
	    x->color = RED;
	  }
	}
	else if (t->right) {
	  // Only the right child of x's sibiling exists
	  if (t->right->color == BLACK) {
	    // CASE 2: Color Flip, all children are black !
	    p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else {
	    // CASE 3 OR 4: Outside or inside red child
		// t has a red right child
		if (t->key < x->key) {
		  // Left-Right Case
		  rotate_left(t);
	      rotate_right(p); 
		}
		else {
		  // Right-Right Case
		  rotate_left(p);
	      t->color = RED;
	      t->right->color = BLACK;
		}
		p->color = BLACK;
	    x->color = RED; 
	  }
	}
	else if (t->left) {
      // Only the left child of x's sibiling exists
	  if (t->left->color == BLACK) {
	    // CASE 2: Color Flip, all children are black !
	    p->color = BLACK;
	    t->color = RED;
	    x->color = RED;
	  }
	  else {
		// CASE 3 OR 4: Outside or inside red child
	    // t has a red left child
		if (t->key < x->key) {
		  // Left-Left Case
		  rotate_right(p);
	      t->color = RED;
	      t->left->color = BLACK;
		}
		else {
		  // Right-Left Case
		  rotate_right(t);
	      rotate_left(p);
		}
		p->color = BLACK;
	    x->color = RED; 
	  }
	}
	else {
	  // Neither of t's children exist, so they are both black nodes
	  // CASE 2: Color Flip, all children are black !
	  p->color = BLACK;
	  t->color = RED;
	  x->color = RED;
	}
  }
  //cout << "Rebalance of " << x->key << endl; 
  //print();
}
 

template<typename K, typename V> 
size_t RBTCollection<K,V>::height(Node* subtree_root) const
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


//----------------------------------------------------------------------
// Provided Helper Functions:
//----------------------------------------------------------------------

template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt() const
{
  return !root or (root->color == BLACK and valid_rbt(root));
}


template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt(Node* subtree_root) const
{
  if (!subtree_root)
    return true;
  color_t rc = subtree_root->color;
  color_t lcc = subtree_root->left ? subtree_root->left->color : BLACK;
  color_t rcc = subtree_root->right ? subtree_root->right->color : BLACK;  
  size_t lbh = black_node_height(subtree_root->left);
  size_t rbh = black_node_height(subtree_root->right);
  bool lv = valid_rbt(subtree_root->left);
  bool rv = valid_rbt(subtree_root->right);
  // check equal black node heights, no two consecutive red nodes, and
  // left and right are valid RBTs
  return (lbh == rbh) and (rc != RED or (lcc != RED and rcc != RED)) and lv and rv;
}


template<typename K, typename V>
size_t RBTCollection<K,V>::black_node_height(Node* subtree_root) const
{
  if (!subtree_root)
    return 1;
  size_t hl = black_node_height(subtree_root->left);
  size_t hr = black_node_height(subtree_root->right);
  size_t h = hl > hr ? hl : hr;
  if (subtree_root->color == BLACK)
    return 1 + h;
  else
    return h;
}


template<typename K, typename V>
void RBTCollection<K,V>::print() const
{
  print_tree("", root);
}


template<typename K, typename V>
void RBTCollection<K,V>::print_tree(std::string indent, Node* subtree_root) const
{
  if (!subtree_root)
    return;
  std::string color = "[BLACK]";
  if (subtree_root->color == RED)
    color = "[RED]";
  std::cout << indent << subtree_root->key << " "
	    << color << " (h="
	    << height(subtree_root) << ")" << std::endl;
  print_tree(indent + "  ", subtree_root->left);
  print_tree(indent + "  ", subtree_root->right);
}


#endif
