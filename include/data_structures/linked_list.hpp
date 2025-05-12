#include <stddef.h>

#include <stdexcept>

/*******************************************************************************
Template declarations and definitions must be kept in header only to avoid
linking errors.
*******************************************************************************/

template <typename T>
class Linked_List {
 public:
  /***********************************************************************
  CONSTRUCTORS
  ***********************************************************************/
  // Default Constructor
  Linked_List();

  // Copy Constructor - l-value reference (&)
  // Copies the values from the provided Linked List into this Linked List
  Linked_List(const Linked_List& src);

  // Move Constructor - r-value reference (&&)
  // Moves the values from the provided Linked List into this Linked List
  Linked_List(Linked_List&& src);

  /***********************************************************************
  DESTRUCTOR
  ***********************************************************************/
  ~Linked_List();

  /***********************************************************************
   GET FUNCTIONS FOR DATA STRUCTURE PROPERTIES
  ***********************************************************************/
  // Returns the current size of the linked list.
  std::size_t get_size();

  /***********************************************************************
  DATA STRUCTURE OPERATIONS
  ***********************************************************************/
  // Inserts data of type T onto the end of the linked list.
  void insert(const T& data);

  // Inserts data of type T into the linked list (at the specified index).
  void insert(const T& data, std::size_t index);

  // Remove node at the end of the linked list.
  void remove();

  // Removes data at the specified index
  void remove(std::size_t index);

  // Replaces data of type T in the linked list at given index.
  void replace(T& data, std::size_t index);

  /***********************************************************************
  DATA STRUCTURE OPERATOR OVERLOADS
  ***********************************************************************/
  T& operator[](std::size_t index) const;

  Linked_List& operator=(const Linked_List& src);

  Linked_List& operator=(Linked_List&& src) noexcept;

 private:
  struct cell {
    T data;
    cell* left;
    cell* right;
  };

  std::size_t size;

  cell* polar_left;

  cell* polar_right;
};

/*******************************************************************************
CONSTRUCTORS
*******************************************************************************/
template <typename T>
Linked_List<T>::Linked_List() {
  this->size = 0;
  this->polar_left = nullptr;
  this->polar_right = nullptr;
}

template <typename T>
Linked_List<T>::Linked_List(const Linked_List& src) {
  ~Linked_List();

  // Copy src's pointers.
  this->size = src.size;
  this->polar_left = src.polar_left;
  this->polar_right = src.polar_right;
}

template <typename T>
Linked_List<T>::Linked_List(Linked_List&& src) {
  ~Linked_List();

  // Copy src's pointers.
  this->size = src.size;
  this->polar_left = src.polar_left;
  this->polar_right = src.polar_right;

  // Return the src Linked List into a newly created state.
  src.size = 0;
  src.polar_left = nullptr;
  src.polar_right = nullptr;
}

/*******************************************************************************
DESTRUCTOR
*******************************************************************************/
template <typename T>
Linked_List<T>::~Linked_List() {
  // Keep removing the last element until the linked list is empty.
  while (this->size != 0) {
    remove();
  }
}

/*******************************************************************************
 GET FUNCTIONS FOR DATA STRUCTURE PROPERTIES
*******************************************************************************/
template <typename T>
std::size_t Linked_List<T>::get_size() {
  return this->size;
}

/*******************************************************************************
DATA STRUCTURE OPERATIONS
*******************************************************************************/
template <typename T>
void Linked_List<T>::insert(const T& data) {
  cell* to_be_inserted = new cell;
  to_be_inserted->data = data;
  to_be_inserted->left = nullptr;
  to_be_inserted->right = nullptr;

  /* If polar left and polar right is null then this is the first element
  being inserted and is now both polar left and polar right. */
  if ((this->polar_left == nullptr) && (this->polar_right == nullptr)) {
    polar_left = to_be_inserted;
    polar_right = to_be_inserted;

  } else {
    // Just append to the end of the linked list (polar right).
    cell* prev_polar_right = polar_right;
    to_be_inserted->left = prev_polar_right;
    prev_polar_right->right = to_be_inserted;
    polar_right = to_be_inserted;
  }

  this->size++;
}

template <typename T>
void Linked_List<T>::insert(const T& data, std::size_t index) {
  cell* to_be_inserted = new cell;
  to_be_inserted->data = data;
  to_be_inserted->left = nullptr;
  to_be_inserted->right = nullptr;

  // Insert the first node.
  if (((this->polar_left == nullptr) && (this->polar_right == nullptr)) &&
      (index == 0)) {
    polar_left = to_be_inserted;
    polar_right = to_be_inserted;

    // Insert a new polar left.
  } else if (index == 0) {
    cell* prev_polar_left = polar_left;
    to_be_inserted->right = prev_polar_left;
    prev_polar_left->left = to_be_inserted;
    polar_left = to_be_inserted;

    // Insert a new polar right.
  } else if (index == this->size) {
    this->insert(data);
    return;  // Return to not increase size twice.

    // Inserting at some arbritary index.
  } else if (index < this->size) {
    std::size_t current_index = 0;
    cell* current_cell = polar_left;

    // Keep going to the right until we reach the index.
    while (current_index != index) {
      current_cell = current_cell->right;
      current_index++;
    }

    /* Insert the to_be_inserted cell at the index, current cell is
    currently at that index.

        current_cell_left <---> current_cell <---> current_cell_right
                            ^ insert to_be_inserted

    */
    to_be_inserted->left = current_cell->left;
    to_be_inserted->right = current_cell;
    current_cell->left->right = to_be_inserted;
    current_cell->left = to_be_inserted;

  } else {
    return;  // Should never get here.
  }

  this->size++;
}

template <typename T>
void Linked_List<T>::remove() {
  if (polar_right->left != nullptr) {
    /* Null the right pointer of the node immediate to the left of polar
    right (this node is the new polar right). Delete polar right. */
    cell* old_polar_right = polar_right;
    polar_right->left->right = nullptr;
    polar_right = polar_right->left;
    delete old_polar_right;

    /* There is no node to the left of polar right. Polar right is the only
    node. */
  } else {
    delete polar_right;
    polar_right = nullptr;
    polar_left = nullptr;
  }

  this->size--;
}

template <typename T>
void Linked_List<T>::remove(std::size_t index) {
  if (index == 0) {
    if (polar_left->right != nullptr) {
      /* Null the left pointer of the node immediate to the right of polar
      left (this node is the new polar left). Delete polar left. */
      cell* old_polar_left = polar_left;
      polar_left->right->left = nullptr;
      polar_left = polar_left->right;
      delete old_polar_left;

      /* There is no node to the right of polar left. Polar left is the only
      node. */
    } else {
      delete polar_left;
      polar_left = nullptr;
      polar_right = nullptr;
    }

  } else if (index == (this->size - 1)) {
    this->remove();
    return;  // Return to not decrease size twice.

  } else if (index < (this->size - 1)) {
    std::size_t current_index = 0;
    cell* current_cell = polar_left;

    // Keep going to the right until we reach the index.
    while (current_index != index) {
      current_cell = current_cell->right;
      current_index++;
    }

    /* We are guaranteed to be between two nodes just do pointer
    manipulation. */
    current_cell->left->right = current_cell->right;
    current_cell->right->left = current_cell->left;
    delete current_cell;

  } else {
    return;  // Should never get here.
  }

  this->size--;
}

template <typename T>
void Linked_List<T>::replace(T& data, std::size_t index) {
  if (index == 0) {
    polar_left->data = data;
  } else if (index == (this->size - 1)) {
    polar_right->data = data;
  } else if (index < (this->size - 1)) {
    std::size_t current_index = 0;
    cell* current_cell = polar_left;

    // Keep going to the right until we reach the index.
    while (current_index != index) {
      current_cell = current_cell->right;
      current_index++;
    }

    current_cell->data = data;

  } else {
    return;  // Should never get here.
  }
}

/*******************************************************************************
DATA STRUCTURE OPERATOR OVERLOADS
*******************************************************************************/
template <typename T>
T& Linked_List<T>::operator[](std::size_t index) const {
  if (index == 0) {
    return polar_left->data;
  } else if (index == (this->size - 1)) {
    return polar_right->data;
  } else if (index < (this->size - 1)) {
    std::size_t current_index = 0;
    cell* current_cell = polar_left;

    // Keep going to the right until we reach the index.
    while (current_index != index) {
      current_cell = current_cell->right;
      current_index++;
    }

    return current_cell->data;

  } else {
    throw std::out_of_range(
        "Tried to access data from an index higher than data was stored.");
  }
}

template <typename T>
Linked_List<T>& Linked_List<T>::operator=(const Linked_List<T>& src) {
  ~Linked_List();

  // Copy src's pointers.
  this->size = src.size;
  this->polar_left = src.polar_left;
  this->polar_right = src.polar_right;

  return *this;
}

template <typename T>
Linked_List<T>& Linked_List<T>::operator=(Linked_List<T>&& src) noexcept {
  ~Linked_List();

  // Copy src's pointers.
  this->size = src.size;
  this->polar_left = src.polar_left;
  this->polar_right = src.polar_right;

  // Return the src Linked List into a newly created state.
  src.size = 0;
  src.polar_left = nullptr;
  src.polar_right = nullptr;

  return *this;
}
