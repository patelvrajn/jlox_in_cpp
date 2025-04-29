#include "gtest/gtest.h"
#include "data_structures/dynamic_array.hpp"
#include <cstdlib>

// Tests the initial size of the Dynamic Array.
TEST (DynamicArraySuite, InitialSize) {

    const std::size_t INITIAL_SIZE = 13;

    Dynamic_Array<int>* da = new Dynamic_Array<int>(INITIAL_SIZE);
    
    // Insert elements upto the initial size.
    srand(time(0));
    for (std::size_t i = 0; i < INITIAL_SIZE; i++) {
        da->insert (rand(), i);
    }

    // Check if size and maximum index are at their expected values.
    ASSERT_EQ (da->get_size(), INITIAL_SIZE);
    ASSERT_EQ (da->get_maximum_index(), (INITIAL_SIZE - 1));

    delete da;

}

// Tests the dynamic growth of the dynamic array.
TEST (DynamicArraySuite, Upsizing) {

    const std::size_t INITIAL_SIZE    = 11;
    const std::size_t RESIZE_FACTOR   = 3;
    const std::size_t EXPECTED_UPSIZE = (INITIAL_SIZE * RESIZE_FACTOR);

    Dynamic_Array<int>* da = new Dynamic_Array<int>(INITIAL_SIZE);
    
    // Set the factor to which to grow the array by.
    da->set_resize_factor(RESIZE_FACTOR);

    // Insert elements upto to expected upsized length.
    srand(time(0));
    for (std::size_t i = 0; i < EXPECTED_UPSIZE; i++) {
        da->insert (rand(), i);
    }

    // Check if size and maximum index are at their expected values.
    ASSERT_EQ (da->get_size(), EXPECTED_UPSIZE);
    ASSERT_EQ (da->get_maximum_index(), (EXPECTED_UPSIZE - 1));

    delete da;

}

// Tests the insert functionality of the dynamic array.
TEST (DynamicArraySuite, InsertData) {

    const std::size_t INITIAL_SIZE = 11;

    Dynamic_Array<int>* da = new Dynamic_Array<int>(INITIAL_SIZE);

    int inserted_elements[INITIAL_SIZE + 1];

    srand(time(0));
    for (std::size_t i = 0; i <= INITIAL_SIZE; i++) {
        int element = rand();
        da->insert (element, i); // Insert at the end of the dynamic array.
        inserted_elements[i] = element;
    }

    for (std::size_t i = 0; i <= INITIAL_SIZE; i++) {
        ASSERT_EQ((*da)[i], inserted_elements[i]);
    }

    // Insert at the beginning of the dynamic array.
    int element_to_insert = rand();
    da->insert(element_to_insert, 0); 

    ASSERT_EQ((*da)[0], element_to_insert);
    ASSERT_EQ((*da)[1], inserted_elements[0]);

    // Insert at an arbitrary position in the dynamic array.
    element_to_insert = rand();
    da->insert(element_to_insert, 4); 

    ASSERT_EQ((*da)[3], inserted_elements[2]);
    ASSERT_EQ((*da)[4], element_to_insert);
    ASSERT_EQ((*da)[5], inserted_elements[3]);

    delete da;

}

TEST (DynamicArraySuite, RemoveData) {

    const std::size_t INITIAL_SIZE = 5;

    Dynamic_Array<int>* da = new Dynamic_Array<int>(INITIAL_SIZE);

    da->insert(11, 0); 
    da->insert(15, 1);
    da->insert(1,  2); 
    da->insert(7,  3);
    da->insert(52, 4);

    da->remove(4); // Remove last element.
    da->remove(0); // Remove first element.
    da->remove(1); // Remove an arbitrary element.

    ASSERT_EQ((*da)[0], 15);
    ASSERT_EQ((*da)[1], 7);

    delete da;

}
