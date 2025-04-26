#include "gtest/gtest.h"
#include "data_structures/hash_table.hpp"
#include <cstdlib>

TEST (HashTableSuite, InsertData) {

    const std::size_t NUM_OF_ELEMENTS_TO_INSERT = 11;

    Hash_Table<int, int>* ht = new Hash_Table<int, int>;

    int inserted_keys[NUM_OF_ELEMENTS_TO_INSERT];
    int inserted_values[NUM_OF_ELEMENTS_TO_INSERT];

    srand(time(0));
    for (std::size_t i = 0; i < NUM_OF_ELEMENTS_TO_INSERT; i++) {
        int random_key   = rand();
        int random_value = rand();
        ht->insert (random_key, random_value);
        inserted_keys[i]   = random_key;
        inserted_values[i] = random_value;
    }

    for (std::size_t i = 0; i < NUM_OF_ELEMENTS_TO_INSERT; i++) {
        ASSERT_EQ(ht->search(inserted_keys[i]), inserted_values[i]);
    }

}

