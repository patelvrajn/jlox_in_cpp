#include "gtest/gtest.h"
#include "data_structures/linked_list.hpp"
#include <cstdlib>

TEST (LinkedListSuite, InsertData) {

    const std::size_t NUM_OF_ELEMENTS_TO_INSERT = 11;

    Linked_List<int>* ll = new Linked_List<int>();

    int inserted_elements[NUM_OF_ELEMENTS_TO_INSERT];

    srand(time(0));
    for (std::size_t i = 0; i < NUM_OF_ELEMENTS_TO_INSERT; i++) {
        int element = rand();
        ll->insert (element); // Insert at the end of the linked list.
        inserted_elements[i] = element;
    }

    for (std::size_t i = 0; i < NUM_OF_ELEMENTS_TO_INSERT; i++) {
        ASSERT_EQ((*ll)[i], inserted_elements[i]);
    }

    // Insert at the beginning of the linked list.
    int element_to_insert = rand();
    ll->insert(element_to_insert, 0); 

    ASSERT_EQ((*ll)[0], element_to_insert);
    ASSERT_EQ((*ll)[1], inserted_elements[0]);

    // Insert at an arbitrary position in the linked list.
    element_to_insert = rand();
    ll->insert(element_to_insert, 6);

    ASSERT_EQ((*ll)[5], inserted_elements[4]);
    ASSERT_EQ((*ll)[6], element_to_insert);
    ASSERT_EQ((*ll)[7], inserted_elements[5]);

}

TEST (LinkedListSuite, RemoveData) {

    Linked_List<int>* ll = new Linked_List<int>();

    ll->insert(15);
    ll->insert(34);
    ll->insert(72);
    ll->insert(55);
    ll->insert(67);

    ll->remove(4); // Remove last element.
    ll->remove(0); // Remove first element.
    ll->remove(1); // Remove an arbitrary element.

    ASSERT_EQ((*ll)[0], 34);
    ASSERT_EQ((*ll)[1], 55);

}
