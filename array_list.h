//----------------------------------------------------------------------
// FILE: array_list.h
// NAME: Matthew Moore
// DATE: September, 2020
// DESC: Implements a resizable array version of the list
//       class. Elements are added by default to the last available
//       index in the array.
//----------------------------------------------------------------------

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "list.h"
#include "linked_list.h"

#include <iostream>

using namespace std;

template<typename T>
class ArrayList : public List<T>
{
public:
  ArrayList();
  ArrayList(const ArrayList<T>& rhs);
  ~ArrayList();
  ArrayList& operator=(const ArrayList<T>& rhs);

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
  
  // helper functions for merge and quick sort
  void merge_sort(size_t start, size_t end);
  void quick_sort(size_t start, size_t end);


private:
  T* items;
  size_t capacity;
  size_t length; // Equivalent to the indexes + 1

  // helper to resize items array
  void resize();
};


template<typename T>
ArrayList<T>::ArrayList()
  : capacity(10), length(0)
{
  items = new T[capacity];
  
}


template<typename T>
ArrayList<T>::ArrayList(const ArrayList<T>& rhs)
  : capacity(10), length(0), items(nullptr)
{
  // defer to assignment operator
  *this = rhs;
  
}


template<typename T> 
ArrayList<T>::~ArrayList()
{
	// Deletes the entire array
	delete [] items;
}


template<typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& rhs)
{
	
	if (this != &rhs) { // protects against self-assignment case
	  length = 0;
	  delete [] items;
	  capacity = rhs.capacity;
	  length = rhs.length;
	  items = new T[capacity];
	  
	  // Fill array with elements from rhs
	  for (size_t i = 0; i < rhs.length; i++) {
			items[i] = rhs.items[i];
	  }
	// Assign the new memory to the object
	}
	
	return *this;
}

template<typename T>
void ArrayList<T>::add(const T& item) { 
	// Adjusts array's occupied spaces
	// Stores the newly added element in next open space
	if (capacity == length) { // Max elements in the array, more room needed
	  resize();
	}
	items[length] = item;

	length = length + 1;
}

template<typename T> 
bool ArrayList<T>::add(size_t index, const T& item) {
  if (capacity == length) { // Max elements in the array, more room needed
	 resize();
  }
  
  if (index > length || index < 0) { // Invalid index
    return false;
  }
  else { // Insertion of element, within the array
    if (length == 0) {
	  // No current elements
	  items[0] = item;
    }
    else if (length == index) {
      // Add to end case
	 items[index] = item;
    }
    else {
      for (size_t i = length; i > index; --i) { 
	    // Leaves a space at the given index value for a new element
	    items[i] = items[i - 1];
	  }
	  items[index] = item;
    }
  }	
  length = length + 1;
  return true;
}

template<typename T>
bool ArrayList<T>:: get(size_t index, T& return_item) const {
	if (index >= length || index < 0) { // Invalid index values
		return false;
	}
	else {
		// Valid index number, so provide value
		return_item = items[index];
		return true;
	}
}

template<typename T>
bool ArrayList<T>:: set(size_t index, const T& new_item) {
	if (index >= length || index < 0) { // Invalid index values
	  return false;
	}
	else {
	  // Valid index number, so set new  value
	  items[index] = new_item;
	  return true;
	}
}

template<typename T>
bool ArrayList<T>:: remove(size_t index) {
	if (index >= length || index < 0) { // Invalid index values
		return false;
	}
	else {
		// Valid index number, so remove value
		for (size_t i = index + 1; i < length; i++) {
			items[i - 1] = items[i];
		}
		// Re-evaluate the size of the array
		length = length - 1;
		return true;
	}
}

template<typename T> 
size_t ArrayList<T>:: size() const {
	
	return length;
}


template<typename T>
void ArrayList<T>:: resize() {
  // Creates new array of double capacity, and storing the items
  T * newArr = new T[capacity * 2];
  for (size_t i = 0; i < length; i++) {
    newArr[i] = items[i];
  }
  // Deleting the old array
  delete [] items;
  // Resetting the capacity
  capacity = capacity * 2;
  // Creating a new array with the new capacity
  items = new T[capacity];
  // Readding the array values
  items = newArr;
}
		
template<typename T> 
void ArrayList<T>::selection_sort() {
	
  T temp;
  int index = 0;
  for (int j = 0; j < length - 1; j++) {
	// Outerloop which places the minimum at the front of the sorted portion
	index = j; 
	if ((j + 1) < length && index < length) {
	  // Confirms that the index and inner loop is located within the array
	  for (int i = j + 1; i < length; i++) {
		  // Inner loop which searches for the current min in unsorted array
		  if (items[i] < items[index]) { 
			  // New min has been found
			  index = i;
		  }
	  }
	  // Swap the min element and first element in unsorted portion
      temp = items[j];
      items[j] = items[index];
      items[index] = temp;
    }
	else {
	  // Case of getting out of range in the performance tests
	  return;
	}
  }
}

template<typename T> 
void ArrayList<T>::insertion_sort() {
	T tmpT;
	size_t i;
	for (size_t j = 1; j < length; ++j) {
		// Outerloop which always is the first element of the unsorted region
		i = j; // resets inner loop to the beginning of unsorted region
		while (items[i - 1] > items[i] && i > 0) {
			// Inner loop swaps with elements in sorted region until no the element is sorted
			tmpT = items[i];
			items[i] = items[i - 1];
			items[i - 1] = tmpT;
			--i;
		}
	}
}	

template<typename T>
void ArrayList<T>::merge_sort() {
	size_t i = 0, k = length - 1;
	merge_sort(i, k);
	
}

template<typename T>
void ArrayList<T>::quick_sort() {
	size_t i = 0, k = length - 1;
	if (length <= 1) {
	  return;
	}
	quick_sort(i, k);
}

template<typename T>
void ArrayList<T>::merge_sort(size_t start, size_t end) {
	size_t mid = 0;

	if (start < end) {
		mid = (start + end) / 2; // Finds the midpoint in the partition
	
		// Recursively sort left and right partitions
		merge_sort(start, mid);
		merge_sort(mid + 1, end);
		
		// Merge left and right partition in sorted order
		size_t mergedSize = end - start + 1; // Size of merged partition
		size_t mergePos = 0; // Position to insert merged items
		size_t leftPos = 0; // Position of elements in left partition
		size_t rightPos = 0; // Position of elements in right partition
		
		T * mergedItems = new T[mergedSize]; // Dynamic temp array 
		
		leftPos = start; // Initialize left partition to beginning of list
		rightPos = mid + 1; // Initialize right partition position to beginning
		
		// Add smallest element from left or right partition to merged numebrs
		while (leftPos <= mid && rightPos <= end) { 
			if (items[leftPos] <= items[rightPos]) {
				mergedItems[mergePos] = items[leftPos]; // Adding to left
				++leftPos;
			}
			else {
				mergedItems[mergePos] = items[rightPos]; // Adding to right
				++rightPos;
			}
			++mergePos;
		}
		
		// If the left partition is not empty, add remaining elements to merged items
		while (leftPos <= mid) {
			mergedItems[mergePos] = items[leftPos];
			++leftPos;
			++mergePos;
		}
		
		// If right partition is not empty, add remaining elements to merged numbers
		while (rightPos <= end) {
			mergedItems[mergePos] = items[rightPos];
			++rightPos;
			++mergePos;
		}
		// Copy merge items back into the regular array
		for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
			items[start + mergePos] = mergedItems[mergePos];
		}
	}
}

template <typename T>
void ArrayList<T>::quick_sort(size_t start, size_t end) {
	size_t mid = 0; // Last value in lower partition

	// BEST CASE: 1 or zero elements in list, already sorted
	if (start >= end) {
		return;
	}
	
	// Partition data within array , value mid is returned as last element in lower partition
	size_t pStart = 0, pEnd = 0, pMid = 0;
	T pivot, tmp;
	bool done = false;
	
	// Pick middle element as pivot
	pMid = start + (end - start) / 2;
	pivot = items[pMid];
	
	// Set the partition starting value and ending value
	pStart = start;
	pEnd = end;
	
	while (!done) {
		// Increment the start while items[start] < pivot
		while (items[pStart] < pivot) {
			++pStart;
		}
		// Decrement the end while pivot < items[end]
		while (pivot < items[pEnd]) {
			--pEnd;
		}
		
		// If there are zero or one elements left, partitioning compelte
		if (pStart >= pEnd) {
			done = true;
		}
		else {
			// Swap the values of items[start] and numbers[end], update start and end
			tmp = items[pStart];
			items[pStart] = items[pEnd];
			items[pEnd] = tmp;
			
			++pStart;
			--pEnd;
		}
	}
	// Set new final low partition element to correct index
	mid = pEnd;
	// Recursively sorting low partition
	quick_sort(start, mid);
	// Recursively sorting high partition
	quick_sort(mid + 1, end);
		
}

template <typename T>
void ArrayList<T>::sort() {
  quick_sort();
}
#endif