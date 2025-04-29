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
        int* ip = ht->search(inserted_keys[i]);
        EXPECT_EQ(*ip, inserted_values[i]);
        delete ip;
    }

    delete ht;

}

TEST (HashTableSuite, RemoveData) {

    Hash_Table<std::string, int>* ht = new Hash_Table<std::string, int>;

    ht->insert("Alex",    1);
    ht->insert("Bob",     2);
    ht->insert("Charlie", 3);
    ht->insert("David",   4);
    ht->insert("Earl",    5);
    ht->insert("Fred",    6);
    ht->insert("Gregory", 7);
    ht->insert("Henry",   8);
    ht->insert("Iris",    9);

    int* ip = ht->search("Bob");
    EXPECT_EQ(*ip, 2);
    delete ip;

    ip = ht->search("David");
    EXPECT_EQ(*ip, 4);
    delete ip;

    ht->remove("Bob");
    ht->remove("David");

    ip = ht->search("Bob");
    EXPECT_EQ(ip, nullptr);

    ip = ht->search("David");    
    EXPECT_EQ(ip, nullptr);

    delete ht;

}
