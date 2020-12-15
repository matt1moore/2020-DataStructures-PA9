//----------------------------------------------------------------------
// FILE: linked_list.h
// NAME: Matthew Moore
// DATE: September, 2020
// DESC: Implements a linked list version of the list class. Elements
//       are added by default to the end of the list via a tail pointer.
//       Implemented sorting algorithms that sort lists in ascending order.
//----------------------------------------------------------------------


#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "list.h"

#include <iostream>
 
 
using namespace std;

template<typename T>
class LinkedList : public List<T>
{
public:
  LinkedList();
  LinkedList(const List<T>& rhs);
  ~LinkedList();
  LinkedList& operator=(const List<T>& rhs);

  void add(const T& item);
  bool add(size_t index, const T& item);
  bool get(size_t index, T& return_item) const;
  bool set(size_t index, const T& new_item);
  bool remove(size_t index);
  size_t size() const;
  void selection_sort();
  void insertion_sort();
  void merge_sort();
  void quick_sort();
  void sort();

private:
  struct Node {
    T value;
    Node* next;
  };
  Node* head;
  Node* tail;
  size_t length;

  // helper to delete linked list
  void make_empty();
  
  // helper functions for merge and quick sort
  Node * merge_sort(Node* left, int len);
  Node * quick_sort(Node* start, int len);
};


template<typename T>
LinkedList<T>::LinkedList()
  : head(nullptr), tail(nullptr), length(0)
{
	
}


template<typename T>
LinkedList<T>::LinkedList(const List<T>& rhs)
  : head(nullptr), tail(nullptr), length(0)
{
  // defer to assignment operator
  *this = rhs;
}


// TODO: Finish the remaining functions below

template<typename T> 
LinkedList<T>::~LinkedList() {
	Node * currNode = NULL;
	Node * nextNode = head;
	
	while (currNode != NULL) {
		// Traversal of Nodes in the list
		nextNode = currNode->next;
		// Deletes the current Node
		delete currNode;
		currNode = nextNode;
	}
	// Good practice to set head to NULL when list is empty
	head = NULL; 
}

template<typename T> 
LinkedList<T>& LinkedList<T>::operator=(const List<T>& rhs)
{
	int i = 1;
	if (this != &rhs) { // protects against self-assignment chance
		if (head != nullptr) { // Clears the lhs if not already clear
			~LinkedList();
		}
		Node * copyPtr = nullptr;
		Node * currPtr = rhs.head;
		while (currPtr != nullptr) {
			if (head == NULL) {
				// Sets the head to point to initial element
				copyPtr = new Node(currPtr->get(length,0));
				head = copyPtr;
			}
			else {
				// Sets all next pointers to point new elements
				copyPtr->add(new Node(currPtr->get(length,i)));
				copyPtr = copyPtr->next;
				++i;
			}
			currPtr = currPtr->next;;
		}
	}
	return *this;
}

template<typename T>
void LinkedList<T>::add(const T& item) {
	// Assigns value to new node
  Node * newNode = new Node;
  newNode->value = item;
  
  if (head == NULL) { // List is empty if true
    head = newNode;
	tail = newNode;
  }
  else { // List contains elements
    tail->next = newNode;
	tail = newNode;
  }
  newNode->next = NULL;
  length = length + 1;
}

template<typename T>
bool LinkedList<T>:: add(size_t index, const T& item) {
	if (index > length || index < 0) { // Invalid index
		return false;
	}
	Node * newNode = new Node;
	newNode->value = item;
	
		if (head == NULL) { // For an empty list
			head = newNode;
			tail = newNode;
			newNode->next = NULL;
		}
		else { // New node will be inserted at the index
			Node * currPtr = head;
			Node * prevPtr = NULL;
			size_t i = 0;
			while (i < index) {
				// Traverse list to the correct index
				prevPtr = currPtr;
				currPtr = currPtr->next;
				++i;
			}
			if (i == 0) { // The element will be at head of the list
			  head = newNode;
			  newNode->next = currPtr;
			}
			else if (index == length) {
			  // Case of adding element to the end
			  newNode->next = NULL;
			  prevPtr->next = newNode;
			  tail = newNode;
			}
			else {
			  // Regular case
			  prevPtr->next = newNode;
			  newNode->next = currPtr;
			}
		}
	length = length + 1;
	return true;
}

template<typename T> 
bool LinkedList<T>::get(size_t index, T& return_item) const {
	if (index >= length || index < 0) { // Invalid index
		return false;
	}
	else if (head == tail) { // One element in the list
		Node * currPtr = head;
		return_item = currPtr->value;
		return true;
	}
	else {
		Node * currPtr = head;
		for (size_t i = 0; i < index; ++i) { 
			// Traverses until index number is hit
			currPtr = currPtr->next;
		}
		return_item = currPtr->value;
				
		return true;
	}
}

template<typename T> 
bool LinkedList<T>::set(size_t index, const T& new_item) {
	if (index >= length || index < 0) {
		return false;
	}
	else if (head == tail) { // One element in the list
		head->value = new_item;
		return true;
	}
	else {
		Node * currPtr = head;
		for (size_t i = 0; i < index; i++) {
			//Traversal of list until index is reached
			currPtr = currPtr->next;
		}
		// Set new value to the index value
		currPtr->value = new_item;
		return true;
	}
}
	
template<typename T>
bool LinkedList<T>::remove(size_t index) {
	size_t i = 0;
	if (index >= length || index < 0) {
		return false;
	}
	else if (head == tail) {
		// Singular element in the list
		Node * currPtr = head;
		delete currPtr;
		head = nullptr;
		tail = nullptr;
		length = length - 1;
		return true;
	}
	else {
		Node * currPtr = head;
		Node * prevPtr = nullptr;
		while (i < index) {
			// Traversal of list until index is reached
			prevPtr = currPtr;
			currPtr = currPtr->next;
			++i;
		}
		// Remove the value at this index and fix the pointers
		if (index == 0) { // Front of the list
			head = currPtr->next;
		}
		else if (index == length - 1) { // Element is at the end of the list
			tail = prevPtr;
			prevPtr->next = nullptr;
		}
		else {
			prevPtr->next = currPtr->next;
		}
		delete currPtr;
		length = length - 1;
		return true;
	}
	
}

template<typename T>
size_t LinkedList<T>::size() const {
	return length;
}

template<typename T>
void LinkedList<T>::selection_sort() {
	Node * outerNode = head;
	Node * innerNode = nullptr;
	Node * minNode = nullptr;
	Node * tmpNode = nullptr;
	Node * prevNode = nullptr;
	Node * prevMin = nullptr;
	Node * outerPrev = nullptr;
	
	
	
	if (head == NULL) {
		// No elements to sort !!!
	}
	else if (head == tail) {
		// There is only one element, so already sorted !!!
	}
	else {
		
		while (outerNode->next != NULL) {
			// Runs through list, finding the minimum in the unsorted section
			minNode = outerNode;
			innerNode = outerNode;
			prevMin = outerPrev;
			while (innerNode != NULL) {
				// Searches through unsorted region for smallest element
				if (innerNode->value < minNode->value) {
					// New minimum has been found
					minNode = innerNode;
					prevMin = prevNode;
					
				}
				prevNode = innerNode;
				innerNode = innerNode->next;
			}
			// Now swap the new min value into the first element of the unsorted portion
			if (outerNode == minNode) { // Case of equivalence,
				// Thus nothing happens
			}
			else if (outerNode == head) { // Case of outer node equalling head, swap occurs
				head = minNode;
				tmpNode = minNode->next; 
				if (outerNode->next == minNode) { // Case of first and second element swapping
					minNode->next = outerNode;
					outerNode->next = tmpNode->next;
				}
				else { // Normal case
					minNode->next = outerNode->next;
					prevMin->next = outerNode;
					outerNode->next = tmpNode;
				}
			}
			else if (outerNode->next == minNode) { // Case of the outerNode and minimum Node being next to each other
				tmpNode = minNode->next;
				minNode->next = outerNode;
				outerPrev->next = minNode;
				outerNode->next = tmpNode;
			}
			else { // Regular swapping of nodes
				tmpNode = minNode->next; 
				minNode->next = outerNode->next;
				outerPrev->next = minNode; 
				prevMin->next = outerNode;
				outerNode->next = tmpNode;
			}
			
			if (minNode == tail) { // Used only in the first swap of a list of numbers
				tail = outerNode;
				outerNode->next = nullptr;
			}
			
			
			// Resets the outer node to the first element in the unsorted array
			outerNode = minNode->next;
			// A prev node must be set to the last element of the sorted region, for swapping
			outerPrev = minNode;
			
		}
	}
	length = size();
	
}

template<typename T>
void LinkedList<T>::insertion_sort() {
	Node * outerNode = head;
	Node * innerNode = nullptr;
	Node * innerPrev = head;
	Node * finalSorted = nullptr; // Generally the first sorted element
	Node * tmpNode = nullptr;
	Node * outerPrev = nullptr;
	
	if (head == NULL) {
		// No elements in the list
	}
	else if (head == tail) { 
		// List is one element long and already sorted
	}
	else {
		// List has at least two elements
		while (outerNode->next != NULL) {
			finalSorted = outerNode;
			outerNode = outerNode->next;
			innerNode = head;
			
			if (outerNode == tail) { // Final element is being sorted
				tail = finalSorted;
			}
			// First element of unsorted portion initially set to head
			head = outerNode;
			if (innerNode->next == outerNode) { // Case 1: Inner node and outer node are adjacent
				tmpNode = outerNode->next;
				outerNode->next = innerNode;
				innerNode->next = tmpNode;
			}	
			else { // Case 2: Normal case
				finalSorted->next = outerNode->next;
				outerNode->next = innerNode;
				if (finalSorted == tail) { // Last element to sort
					finalSorted->next = nullptr;
				}
			}
				
			while (outerNode != finalSorted && outerNode->value > innerNode->value) { // Outer prev is the final element in the sorted region here
				// Swapping of items next to each other
				if (outerNode == head) {
					head = innerNode;
					tmpNode = innerNode;
					innerNode->next = outerNode;
					outerNode->next = tmpNode->next;
				}
				else {
					// Normal swapping case
					tmpNode = innerNode;
					outerPrev->next = innerNode;
					innerNode->next = outerNode;
					outerNode->next = tmpNode->next;
				}
				
				
				// Shifting inner node to be the element to the right of the outer pointer
				outerPrev = innerNode;
				innerNode = outerNode->next;
			}
	
			
			// Reset the Nodes to their correct possitions
			outerNode = finalSorted;
		}
	}	
			
}

template<typename T> 
void LinkedList<T>::merge_sort() {
  Node * i = head;
  int k = length - 1; // Value based upon index
	
  if (head == NULL) {
    // Case already sorted, 1 element
  }
  else if (head->next == NULL) {
    // Case already sorted, 1 element
  }
  else {
	// Normal case
   //  head = merge_sort(i, k);
  }
  
}

template<typename T>
void LinkedList<T>::quick_sort() {
  if (length <= 1) {
    return;
  }
  
  head = quick_sort(head, length);
  
  // Traverse list to set tail
  tail = head;
  while (tail->next != NULL) {
    tail = tail->next;
  }
  
}


/* 

  CODE ERROR : SEG fault occurs upon the iteration where the list merges to reach a length of 4
  Time Spent: Over 6 hours, unable to decipher how to initialize mergeHead pointer
  */
template<typename T>
typename LinkedList<T>::Node *
LinkedList<T>::merge_sort(Node * left, int len) {
  Node * right = left;
  size_t mid = 0;
  Node * mergeHead = left;
  Node * mergeTail = left;
  
  if (len > 1) { // As long as there are two or more elements
    while ((mid + 1) < (len / 2)) { // Traverses list to set the right Node to the start of right partition
      right = right->next;
  	  ++mid;
    }
		
    Node * leftTail = right;
	
	right = right->next;

	leftTail->next = NULL;
	
	++mid;
	
    Node * leftPos = merge_sort(left, mid);
    Node * rightPos = merge_sort(right, len - mid);
	
	// Add smallest element first to the tmp linked list
	while (leftPos != NULL && rightPos != NULL) {
		if (leftPos->value <= rightPos->value) {
		  // The value from the left partition is greater, so transfer the right value
		  mergeTail->next = leftPos;
		  mergeTail = leftPos;
		  leftPos = leftPos->next;
		}
		else {
		  // The value from the left partition is greater, so transfer the right value
		  mergeTail->next = rightPos;
		  mergeTail = rightPos;
		  rightPos = rightPos->next;
		}
	}
	// Attatch the remaining elements in the partition if the end is not reached
	if (left != NULL) {
	  mergeTail->next = left;
	}
	
	if (right != NULL) { 
	  mergeTail->next = right;
    }
	
  }

  return mergeHead;
}


template<typename T> 
typename LinkedList<T>::Node *
LinkedList<T>::quick_sort(Node * left, int len) {
  Node * pivot = left;
  Node * smaller = NULL;
  Node * larger = NULL;
  Node * tmp = NULL;
  Node * tmpPivot = NULL;
  size_t smaller_len = 0;
  size_t larger_len = 0;
  
  // BEST CASE : List is either 1 or no elements
  if (len <= 1) {
    return left;
  }
  
  while (pivot->next != NULL) {	
    // Partition elements until pivot is only element
	tmpPivot = pivot->next->next;
	if (pivot->next->value > pivot->value) {
	  // Partition to front of larger list if true
      pivot->next->next = larger;
	  larger = pivot->next;
	  pivot->next = tmpPivot;
	  ++larger_len;
	}
	else {
	  // Partition to front of smaller list
	  pivot->next->next = smaller;
	  smaller = pivot->next;
	  pivot->next = tmpPivot;
	  ++smaller_len;
	}
	// Adjust remaining element in the pivot list
	pivot->next = tmpPivot;
  }
  
  smaller = quick_sort(smaller,smaller_len);
  larger = quick_sort(larger,larger_len);
  
  if (smaller == NULL) {
    // Pivot is the first element
	left = pivot;
  }
  else {
    // Reattatch all elements to the original list
    left = smaller;
    Node * theGluer = smaller;
    for (int i = 1; i < smaller_len; ++i) {
      // Traverse smaller list to final element
      theGluer = theGluer->next;
    }
    theGluer->next = pivot;
  }
   pivot->next = larger;
   
   return left;
}


template <typename T>
void LinkedList<T>::sort() {
  quick_sort();
}
		
#endif