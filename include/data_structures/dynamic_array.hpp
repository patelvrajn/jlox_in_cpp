#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

/*******************************************************************************
Template declarations and definitions must be kept in header only to avoid
linking errors.
*******************************************************************************/

template <typename T>
class Dynamic_Array {
 public:
  /*************************************************************************
  CONSTRUCTORS
  *************************************************************************/
  // Default Constructor
  Dynamic_Array();

  // Overloaded default constructor to manually set the initial size of the
  // dynamic array
  Dynamic_Array(std::size_t initial_size);

  // Copy Constructor = l-value reference argument (&)
  // Copies the values from the provided Dynamic Array into this Dynamic
  // Array
  Dynamic_Array(const Dynamic_Array& src);

  // Move Constructor = r-value reference argument (&&)
  // Moves the values from the provided Dynamic Array into this Dynamic
  // Array
  Dynamic_Array(Dynamic_Array&&) noexcept;

  /*************************************************************************
  DESTRUCTOR
  *************************************************************************/
  ~Dynamic_Array();

  /*************************************************************************
  SET FUNCTIONS FOR DATA STRUCTURE PROPERTIES
  *************************************************************************/
  // Sets the resize factor of the dynamic array to the specified value.
  void set_resize_factor(std::size_t resize_factor);

  /*************************************************************************
  GET FUNCTIONS FOR DATA STRUCTURE PROPERTIES
  *************************************************************************/
  // Returns the resize factor of the dynamic array.
  int get_resize_factor();

  // Returns the current size of the dynamic array.
  int get_size();

  // Returns the maximum index of the dynamic array.
  int get_maximum_index();

  /*****************************************************************************
  BEGIN AND END ITERATORS
  *****************************************************************************/
  T* begin();

  T* end();

  /*************************************************************************
  DATA STRUCTURE OPERATIONS
  *************************************************************************/
  // Inserts data of type T into the dynamic array (at the specified index).
  void insert(const T& data, int index);

  // Replaces data of type T in the dynamic array at given index.
  void replace(const T& data, int index);

  // Removes data at the specified index
  void remove(int index);

  // Merges the given Dynamic Array with this Dynamic Array
  void merge(Dynamic_Array& src);

  /*************************************************************************
  DATA STRUCTURE OPERATOR OVERLOADS
  *************************************************************************/
  // Overloads the square bracket operator [] to index the dynamic array.
  T& operator[](int index) const;

  // Prints the current state of the dynamic array to standard output.
  // Overload of operator << must be a friend.
  template <typename U>
  friend std::ostream& operator<<(std::ostream& os,
                                  const Dynamic_Array<U>& src);

  // Overloads the = operator to provide a method of copying.
  Dynamic_Array& operator=(const Dynamic_Array& src);

  // Overloads the = operator to provide a method of moving.
  Dynamic_Array& operator=(Dynamic_Array&& src) noexcept;

  /*************************************************************************
  DATA STRUCTURE SEARCH ALGORITHMS
  *************************************************************************/
  // Performs a linear search for data in the dynamic array starting at the
  // specified index.
  int linear_search(T& data, int offset = 0);

  /*************************************************************************
  DATA STRUCTURE SERIALIZATION AND DESERIALIZATION
  *************************************************************************/
  // Serializes this data structure into a string.
  std::string serialize(std::string (*TtoString)(T)) const;

  // Deserializes this data structure using a string of a given format.
  Dynamic_Array<T>* deserialize(std::string serialized_da,
                                T (*StringToT)(std::string)) const;

 private:
  // A pointer to memory to be initialized to an array of T-typed data.
  T* array;

  // Stores the size of the dynamic array.
  std::size_t size;

  // Stores the largest index of the dynamic array that contains data.
  int maximum_index;

  // Stores the factor in which to multiply the size of the array when
  // resized.
  std::size_t resize_factor;

  // Resizes the array dynamically.
  void resize();
};

/*******************************************************************************
CONSTRUCTORS
*******************************************************************************/
template <typename T>
Dynamic_Array<T>::Dynamic_Array() {
  this->size = 4;
  this->resize_factor = 2;
  this->maximum_index = -1;

  array = new T[this->size];
}

template <typename T>
Dynamic_Array<T>::Dynamic_Array(std::size_t initial_size) {
  this->size = initial_size;
  this->resize_factor = 2;
  this->maximum_index = -1;

  array = new T[this->size];
}

template <typename T>
Dynamic_Array<T>::Dynamic_Array(const Dynamic_Array<T>& src) {
  // Obtain all member variables from the src Dynamic Array
  this->size = src.size;
  this->maximum_index = src.maximum_index;
  this->resize_factor = src.resize_factor;

  // Delete this' array and point to src's array.
  delete[] this->array;
  this->array = src.array;
}

template <typename T>
Dynamic_Array<T>::Dynamic_Array(Dynamic_Array<T>&& src) noexcept {
  // Obtain all member variables from the src Dynamic Array
  this->size = src.size;
  this->maximum_index = src.maximum_index;
  this->resize_factor = src.resize_factor;

  // Delete this' array and point to the src Dynamic Array.
  delete[] this->array;
  this->array = src.array;

  // Return the src Dynamic Array into a newly created state.
  src.size = 4;
  src.maximum_index = -1;
  src.resize_factor = 2;
  src.array = new T[src.size];
}

/*******************************************************************************
DESTRUCTOR
*******************************************************************************/
template <typename T>
Dynamic_Array<T>::~Dynamic_Array() {
  // Deletes the only segment of heap-allocated memory in this data structure.
  delete[] array;
}

/*******************************************************************************
SET FUNCTIONS FOR DATA STRUCTURE PROPERTIES
*******************************************************************************/
template <typename T>
void Dynamic_Array<T>::set_resize_factor(std::size_t resize_factor) {
  // Sets this dynamic array's resize factor to the parameter's value.
  this->resize_factor = resize_factor;
}

/*******************************************************************************
GET FUNCTIONS FOR DATA STRUCTURE PROPERTIES
*******************************************************************************/
template <typename T>
int Dynamic_Array<T>::get_resize_factor() {
  // Return this dynamic array's resize factor.
  return this->resize_factor;
}

template <typename T>
int Dynamic_Array<T>::get_size() {
  // Returns the size of this dynamic array.
  return this->size;
}

template <typename T>
int Dynamic_Array<T>::get_maximum_index() {
  // Returns the maximum index of this dynamic array.
  return this->maximum_index;
}

/*******************************************************************************
BEGIN AND END ITERATORS
*******************************************************************************/
template <typename T>
T* Dynamic_Array<T>::begin() {
  return &(this->array[0]);
}

template <typename T>
T* Dynamic_Array<T>::end() {
  return &(this->array[this->maximum_index]);
}

/*******************************************************************************
DATA STRUCTURE OPERATIONS
*******************************************************************************/
template <typename T>
void Dynamic_Array<T>::insert(const T& data, int index) {
  // Check if the dynamic array's size needs to be resized for another
  // element to be inserted.
  if (((std::size_t)(this->maximum_index + 1)) >= this->size) {
    resize();
  }

  if (index > (this->maximum_index + 1)) {
    throw std::out_of_range(
        "Tried to insert data from an index higher than one position after "
        "data was stored.");
  }

  // Instantiate a new array to copy the current elements to and insert the
  // new element into.
  T* new_array = new T[this->size];

  int counter = 0;

  // Copy elements from current state upto index that the new element is to
  // be inserted.
  while (counter < index) {
    new_array[counter] = array[counter];
    counter++;
  }

  // Insert new element at that index.
  new_array[index] = data;
  counter = index;

  // Finish copying elements.
  while (counter <= this->maximum_index) {
    new_array[counter + 1] = array[counter];
    counter++;
  }

  // Deallocate the memory associated with the old array.
  delete[] array;

  // Reassign the pointer to the newly created array.
  array = new_array;

  // Increase the maximum index because a new element was inserted.
  maximum_index++;
}

template <typename T>
void Dynamic_Array<T>::replace(const T& data, int index) {
  // Check if the replacement takes place within the existing index range.
  // If not throw an exception.
  if (index > this->maximum_index) {
    throw std::out_of_range(
        "Tried to replace data from an index higher than data was stored.");
  } else {
    // Replace data at the given index with the given data.
    array[index] = data;
  }
}

template <typename T>
void Dynamic_Array<T>::remove(int index) {
  // Check if the removal takes place within the existing index range.
  // If not throw an exception.
  if (index > this->maximum_index) {
    throw std::out_of_range(
        "Tried to remove data from an index higher than data was stored.");
  } else {
    // Instantiate a new array to copy the current elements except the element
    // at the specified index.
    T* new_array = new T[this->size];

    int counter = 0;

    // Copy elements from current state upto index that the new element is to
    // be removed.
    while (counter < index) {
      new_array[counter] = array[counter];
      counter++;
    }

    // Skip copying the element that is being removed.
    counter = index + 1;

    // Finish copying elements.
    while (counter <= this->maximum_index) {
      new_array[counter - 1] = array[counter];
      counter++;
    }

    // Deallocate the memory associated with the old array.
    delete[] array;

    // Reassign the pointer to the newly created array.
    array = new_array;

    // Decrease the maximum index because an element was removed.
    maximum_index--;
  }
}

template <typename T>
void Dynamic_Array<T>::merge(Dynamic_Array& src) {
  // Insert all data from src into this Dynamic Array.
  for (int counter = 0; counter <= src.maximum_index; counter++) {
    insert(src[counter], counter);
  }

  // Return the src Dynamic Array into a newly created state.
  src.size = 4;
  src.maximum_index = -1;
  src.resize_factor = 2;
  src.array = new T[src.size];
}

/*******************************************************************************
DATA STRUCTURE OPERATOR OVERLOADS
*******************************************************************************/
template <typename T>
T& Dynamic_Array<T>::operator[](int index) const {
  if (index > this->maximum_index) {
    throw std::out_of_range(
        "Tried to set data from an index higher than data was stored.");
  } else {
    // Returns the data at the index.
    return array[index];
  }
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const Dynamic_Array<U>& src) {
  // Prints each element on a new line prefixed by its index number
  for (int counter = 0; counter <= src.maximum_index; counter++) {
    os << "Element " << counter << " : " << src[counter] << std::endl;
  }

  return os;
}

template <typename T>
Dynamic_Array<T>& Dynamic_Array<T>::operator=(const Dynamic_Array& src) {
  // Obtain all member variables from the src Dynamic Array
  this->size = src.size;
  this->maximum_index = src.maximum_index;
  this->resize_factor = src.resize_factor;

  // Delete this' array and point to src's array.
  delete[] this->array;
  this->array = src.array;

  return *this;
}

template <typename T>
Dynamic_Array<T>& Dynamic_Array<T>::operator=(Dynamic_Array&& src) noexcept {
  // Obtain all member variables from the src Dynamic Array
  this->size = src.size;
  this->maximum_index = src.maximum_index;
  this->resize_factor = src.resize_factor;

  // Delete this' array and point to the src Dynamic Array.
  delete[] this->array;
  this->array = src.array;

  // Return the src Dynamic Array into a newly created state.
  src.size = 4;
  src.maximum_index = -1;
  src.resize_factor = 2;
  src.array = new T[src.size];

  return *this;
}

/*******************************************************************************
DATA STRUCTURE SEARCH ALGORITHMS
*******************************************************************************/
template <typename T>
int Dynamic_Array<T>::linear_search(T& data, int offset) {
  // Checks if the start of the search is in the valid range of data-filled
  // indices.
  if (offset > maximum_index) {
    throw std::out_of_range(
        "Offset parameter was greater than the maximum_index.");
  }

  // Looks through each element one-by-one for the first element that matches
  // the data after the position given.
  for (int counter = offset; counter <= this->maximum_index; counter++) {
    if (array[counter] == data) {
      return counter;
    }
  }
}

/*******************************************************************************
DATA STRUCTURE SERIALIZATION AND DESERIALIZATION
*******************************************************************************/
template <typename T>
std::string Dynamic_Array<T>::serialize(std::string (*TtoString)(T)) const {
  // Empty string to store the serialized Dynamic Array
  std::string serialized_da = "";

  // Concatenates to the empty string a space-delimited list of the T-typed data
  // in the Dynamic Array
  for (int counter = 0; counter <= this->maximum_index; counter++) {
    // Converts T-typed data to a string via the provided function and
    // concatenates it to the empty string
    serialized_da += TtoString(this->array[counter]);

    // Adds a space after each data member except the last one.
    if (counter != maximum_index) {
      serialized_da += " ";
    }
  }

  // Returns the now filled string that holds the serialized Dynamic Array
  return serialized_da;
}

template <typename T>
Dynamic_Array<T>* Dynamic_Array<T>::deserialize(
    std::string serialized_da, T (*StringToT)(std::string)) const {
  // Instantiates a Dynamic Array to store the deserialized elements of type T.
  Dynamic_Array<T>* deserialized_da = new Dynamic_Array<T>();

  std::string space = " ";
  std::size_t position = 0;

  // Loops through the string for "words" that will be the elements of the new
  // Dynamic Array.
  while (position != std::string::npos) {
    // Finds the first space at and after the position
    std::size_t search = serialized_da.find(space, position);

    if (search != std::string::npos) {
      // If not the last word store the substring from the position after the
      // space to the position before the next space as an element of the
      // Dynamic Array. Then set the position to the end of the word.
      std::size_t substring_length = (search + 1) - position;
      deserialized_da->insert(
          StringToT(serialized_da.substr(position, substring_length)));
      position = search + 1;
    } else {
      // If it is the last word store the substring from the position after
      // the space to the last character in the string. Then set the position
      // to the end of the string which will terminate insertion of elements
      // into the Dynamic Array.
      std::size_t substring_length =
          (sizeof(serialized_da) - 2) - (position + 1);
      deserialized_da->insert(
          StringToT(serialized_da.substr(position, substring_length)));
      position = std::string::npos;
    }
  }

  // Returns the now filled Dynamic Array
  return deserialized_da;
}

/*******************************************************************************
PRIVATE DECLARATIONS
*******************************************************************************/
template <typename T>
void Dynamic_Array<T>::resize() {
  // Update the size of the dynamic array
  this->size *= this->resize_factor;

  // Creates a new array on the heap of the new size - if stack allocated array
  // would no longer exist after function call
  T* newly_sized_array = new T[this->size];

  // Copy the members of the old array into the new array
  for (int counter = 0; counter <= this->maximum_index; counter++)
    newly_sized_array[counter] = array[counter];

  // Deletes the heap memory associated with the pointer to the array.
  delete[] array;

  // Assign the array pointer to the new heap memory allocated with the newly
  // sized array
  array = newly_sized_array;
}
