#include <cstdint>
#include <array>
#include <cmath>
#include <unordered_map>
#include "data_structures/dynamic_array.hpp"
#include "data_structures/linked_list.hpp"

#define DYNAMIC_HASH_TABLE_RESIZE_FACTOR 2

/*******************************************************************************
Template declarations and definitions must be kept in header only to avoid
linking errors.
*******************************************************************************/

template <typename K, typename V> class Hash_Table { 

    public:

        /***********************************************************************
        CONSTRUCTORS
        ***********************************************************************/
		// Default Constructor
		Hash_Table();

        // Copy Constructor - l-value reference (&)
        // Copies the values from the provided Hash Table into this Hash Table
		Hash_Table(const Hash_Table& src);

        // Move Constructor - r-value reference (&&)
        // Moves the values from the provided Hash Table into this Hash Table
        Hash_Table(Hash_Table&& src);

        /***********************************************************************
        DESTRUCTOR
        ***********************************************************************/
        ~Hash_Table();

        /***********************************************************************
        DATA STRUCTURE OPERATIONS
        ***********************************************************************/
        // Inserts key-value pair into the hash table.
		void insert(const K& key, const V& value);

		// Removes key from the table.
		void remove(const K& key);

        // Replaces value associated with a given key with the given value.
		void replace(const K& key, const V& value);

        /* Search for a key in the hash table and returns a pointer to the value 
        associated with the key. */
        V* search(const K& key);

        /***********************************************************************
        DATA STRUCTURE OPERATOR OVERLOADS
        ***********************************************************************/
        Hash_Table& operator = (const Hash_Table& src);
     
        Hash_Table& operator = (Hash_Table&& src) noexcept;

    private:

        struct cell
        {
            K key;
            V value;
        };

        Dynamic_Array<Linked_List<cell*>*>* ht;

        uint64_t hash_function (const K& key);

        std::size_t original_num_of_buckets;

        std::size_t size;

        const double MAX_LOAD_FACTOR = 3.0;

};

/*******************************************************************************
CONSTRUCTORS
*******************************************************************************/
template <typename K, typename V> Hash_Table<K, V>::Hash_Table() {

    this->ht = new Dynamic_Array<Linked_List<cell*>*>();
    this->ht->set_resize_factor(DYNAMIC_HASH_TABLE_RESIZE_FACTOR);
    this->original_num_of_buckets = this->ht->get_size();
    this->size = 0;

    /* Insert nullptrs until maximum index reaches the size because we can only 
    access elements upto the maximum index. */
    int next_maximum_index = ht->get_maximum_index() + 1;
    for (int i = next_maximum_index; i < this->ht->get_size(); i++) {
        this->ht->insert(nullptr, i);
    }

}

template <typename K, typename V> Hash_Table<K, V>::Hash_Table(const Hash_Table& src) {

    ~Hash_Table();

    this->ht                      = src.ht;
    this->original_num_of_buckets = src.original_num_of_buckets;
    this->size                    = src.size;

}

template <typename K, typename V> Hash_Table<K, V>::Hash_Table(Hash_Table&& src) {

    ~Hash_Table();

    this->ht                      = src.ht;
    this->original_num_of_buckets = src.original_num_of_buckets;
    this->size                    = src.size;

    src.ht   = nullptr;
    src.size = 0;

}

/*******************************************************************************
DESTRUCTOR
*******************************************************************************/
template <typename K, typename V> Hash_Table<K, V>::~Hash_Table() {

    for (int da_i = 0; da_i <= this->ht->get_maximum_index(); da_i++) {

        if ((*(this->ht))[da_i] != nullptr) {
            for (std::size_t ll_i = 0; ll_i < (*(this->ht))[da_i]->get_size(); ll_i++) {

                // Delete cells in linked list.
                if ((*((*(this->ht))[da_i]))[ll_i] != nullptr) {
                    delete (*((*(this->ht))[da_i]))[ll_i];
                } 

            }
        }

        // Delete linked list in the dynamic array.
        if ((*(this->ht))[da_i] != nullptr) {
            delete (*(this->ht))[da_i];
        }
    }

    // Delete the dynamic array.
    if (ht != nullptr) {
        delete ht;
    }

}

/*******************************************************************************
DATA STRUCTURE OPERATIONS
*******************************************************************************/
template <typename K, typename V> void Hash_Table<K, V>::insert(const K& key, const V& value) {

    /* Check the load factor which is the number of entries (size of the hash 
    table) / number of buckets (length of dynamic array). If it is higher than 
    the maximum load factor, increase the length of the dynamic array. */
    double load_factor = ((double)(this->size + 1)) / ((double)ht->get_size());

    if (load_factor > MAX_LOAD_FACTOR) {

        /* Insert nullptrs until maximum index reaches the next size because we 
        can only access elements upto the maximum index and we want to grow the
        array because the load factor is higher than the maximum. */
        int next_maximum_index = this->ht->get_maximum_index() + 1;
        int next_size          = this->ht->get_size() * DYNAMIC_HASH_TABLE_RESIZE_FACTOR;
        for (int i = next_maximum_index; i < next_size; i++) {
            this->ht->insert(nullptr, i);
        }

    }

    /* Hash the key then take the modulo to determine the bucket the key-value
    pair belongs to. */
    uint64_t bucket_index = hash_function(key) % ht->get_size();

    // Place the value in the bucket only if key doesn't exist.
    if (this->search(key) == nullptr) {

        if ((*(this->ht))[bucket_index] == nullptr) {
            (*(this->ht))[bucket_index] = new Linked_List<cell*>;
        }

        cell* cell_to_be_inserted  = new cell;
        cell_to_be_inserted->key   = key;
        cell_to_be_inserted->value = value;
        (*(this->ht))[bucket_index]->insert(cell_to_be_inserted);
    }

    this->size++;

}

template <typename K, typename V> void Hash_Table<K, V>::remove(const K& key) {

    uint64_t size_divisor_power = 0;

    while (true) {

        /* Hash the key then take the modulo to determine the bucket the key-
        value pair belongs to. */
        uint64_t divided_size = (ht->get_size() / pow (DYNAMIC_HASH_TABLE_RESIZE_FACTOR, size_divisor_power));
        uint64_t bucket_index = hash_function(key) % divided_size;

        // Look thru linked list at index for the key.
        if (((*(this->ht))[bucket_index]) != nullptr) {
            for (int i = 0; i < (*((*(this->ht))[bucket_index])).get_size(); i++) {
                if ((*((*(this->ht))[bucket_index]))[i]->key == key) {

                    delete (*((*(this->ht))[bucket_index]))[i];
                    (*((*(this->ht))[bucket_index])).remove(i);
                    this->size--;
                    return;

                }
            }
        }

        // Stop if the size we reached was the original size of the hash table.
        if (divided_size == this->original_num_of_buckets) {
            break;
        }

        // We must cycle thru different sizes because we are doing lazy hashing.
        size_divisor_power++;

    }

    return;
}

template <typename K, typename V> void Hash_Table<K, V>::replace(const K& key, const V& value) {
    remove(key);
    insert(key, value);
}

template <typename K, typename V> V* Hash_Table<K, V>::search(const K& key) {

    uint64_t size_divisor_power = 0;

    while (true) {

        /* Hash the key then take the modulo to determine the bucket the key-
        value pair belongs to. */
        uint64_t divided_size = (ht->get_size() / pow (DYNAMIC_HASH_TABLE_RESIZE_FACTOR, size_divisor_power));
        uint64_t bucket_index = hash_function(key) % divided_size;

        /* The expected bucket index is the index we expect the key-value pair
        to be in based on the current number of buckets. */
        uint64_t expected_bucket_index = (hash_function(key) % ht->get_size());

        // Look thru linked list at index for the key.
        if (((*(this->ht))[bucket_index]) != nullptr) {
            for (int i = 0; i < (*((*(this->ht))[bucket_index])).get_size(); i++) {

                if ((*((*(this->ht))[bucket_index]))[i]->key == key) {

                    V value = (*((*(this->ht))[bucket_index]))[i]->value;

                    // Get a heap-allocated pointer to the value to return. 
                    V* return_val = new V;
                    *return_val = value;

                    /* Lazy rehashing - move the key-value pair to the expected
                    bucket index. We only move the key-value pairs being searched 
                    for instead of having to do a huge copy on growth of the hash 
                    table. */
                    if (expected_bucket_index != bucket_index) {

                        if ((*(this->ht))[expected_bucket_index] == nullptr) {
                            (*(this->ht))[expected_bucket_index] = new Linked_List<cell*>;
                        }
                        
                        (*(this->ht))[expected_bucket_index]->insert((*((*(this->ht))[bucket_index]))[i]);
                        (*((*(this->ht))[bucket_index])).remove(i);

                    }

                    return return_val;
                }
            }
        }

        // Stop if the size we reached was the original size of the hash table.
        if (divided_size == this->original_num_of_buckets) {
            break;
        }

        // We must cycle thru different sizes because we are doing lazy hashing.
        size_divisor_power++;

    }

    return nullptr;

}

/*******************************************************************************
DATA STRUCTURE OPERATOR OVERLOADS
*******************************************************************************/
template <typename K, typename V> Hash_Table<K, V>& Hash_Table<K, V>::operator = (const Hash_Table& src) {

    ~Hash_Table();

    this->ht                      = src.ht;
    this->original_num_of_buckets = src.original_num_of_buckets;
    this->size                    = src.size;

}

template <typename K, typename V> Hash_Table<K, V>& Hash_Table<K, V>::operator = (Hash_Table&& src) noexcept {

    ~Hash_Table();

    this->ht                      = src.ht;
    this->original_num_of_buckets = src.original_num_of_buckets;
    this->size                    = src.size;

    src.ht   = nullptr;
    src.size = 0;

}

/*******************************************************************************
HASH FUNCTION
*******************************************************************************/
template <typename K, typename V> std::size_t Hash_Table<K, V>::hash_function(const K& key) {

    return std::hash<K>{}(key);

}
