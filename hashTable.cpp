#include "header.h"

template <typename KeyType, typename ValueType>
HashNode<KeyType, ValueType>::HashNode(const KeyType& key, const ValueType& value)
    : key(key), value(value), next(nullptr) {}


template <typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::HashTable(size_t initialSize, size_t bloomFilterSize, size_t numHashFunctions)
    : size(initialSize),
      table(new HashNode<KeyType, ValueType>*[size]),
      bloomFilter(bloomFilterSize, numHashFunctions)
{
    for (size_t i = 0; i < size; ++i) {
        table[i] = nullptr;
    }
}

template <typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::~HashTable() {
    for (size_t i = 0; i < size; ++i) {
        HashNode<KeyType, ValueType>* current = table[i];
        while (current != nullptr) {
            HashNode<KeyType, ValueType>* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] table;
}

template <typename KeyType, typename ValueType>
size_t HashTable<KeyType, ValueType>::hash(const KeyType& key) const {
    return std::hash<KeyType>()(key) % size;
}

template <typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value) {
    size_t index = hash(key);
    HashNode<KeyType, ValueType>* newNode = new HashNode<KeyType, ValueType>(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    bloomFilter.insert(key);
}

template <typename KeyType, typename ValueType>
ValueType HashTable<KeyType, ValueType>::get(const KeyType& key) const {
    if (!bloomFilter.mayContain(key)) {
        cout << "Blocked by bloom filter" << endl;
        throw std::out_of_range("Ключ не найден в хеш-таблице");
    }
    size_t index = hash(key);
    HashNode<KeyType, ValueType>* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    throw std::out_of_range("Ключ не найден в хеш-таблице");
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::remove(const KeyType& key) {
    if (!bloomFilter.mayContain(key)) {
        cout << "Blocked by bloom filter" << endl;
        return false;
    }
    size_t index = hash(key);
    HashNode<KeyType, ValueType>* current = table[index];
    HashNode<KeyType, ValueType>* previous = nullptr;
    while (current != nullptr) {
        if (current->key == key) {
            if (previous == nullptr) {
                table[index] = current->next;
            } else {
                previous->next = current->next;
            }
            delete current;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::contains(const KeyType& key) const {
    if (!bloomFilter.mayContain(key)) {
        cout << "Blocked by bloom filter" << endl;
        return false;
    }

    size_t index = hash(key);
    HashNode<KeyType, ValueType>* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename KeyType, typename ValueType>
size_t HashTable<KeyType, ValueType>::getSize() const {
    return size;
}

template <typename KeyType, typename ValueType>
HashNode<KeyType, ValueType>* HashTable<KeyType, ValueType>::getNodeAtIndex(size_t index) const {
    if (index >= size) {
        return nullptr;
    }
    return table[index];
}

template <typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::print() const {
    for (size_t i = 0; i < size; ++i) {
        cout << "Индекс " << i << ": ";
        HashNode<KeyType, ValueType>* current = table[i];
        while (current != nullptr) {
            cout << "(" << current->key << ", " << current->value << ") ";
            current = current->next;
        }
        cout << endl;
    }
}

template <typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::BloomFilter::BloomFilter(size_t bloomFilterSize, size_t numHashFunctions)
    : size(bloomFilterSize), numHashes(numHashFunctions) {
    bits = new bool[size];
    for (size_t i = 0; i < size; ++i) {
        bits[i] = false;
    }
}

template <typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::BloomFilter::~BloomFilter() {
    delete[] bits;
}

template <typename KeyType, typename ValueType>
size_t HashTable<KeyType, ValueType>::BloomFilter::hash1(const KeyType& key) const {
    return std::hash<KeyType>()(key) % size;
}

template <typename KeyType, typename ValueType>
size_t HashTable<KeyType, ValueType>::BloomFilter::hash2(const KeyType& key) const {
    return (std::hash<KeyType>()(key) * 31) % size;
}

template <typename KeyType, typename ValueType>
size_t HashTable<KeyType, ValueType>::BloomFilter::hash3(const KeyType& key) const {
    return (std::hash<KeyType>()(key) * 53) % size;
}

template <typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::BloomFilter::insert(const KeyType& key) {
    size_t hashIndex1 = hash1(key);
    size_t hashIndex2 = hash2(key);
    size_t hashIndex3 = hash3(key);
    bits[hashIndex1] = true;
    bits[hashIndex2] = true;
    bits[hashIndex3] = true;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::BloomFilter::mayContain(const KeyType& key) const {
    size_t hashIndex1 = hash1(key);
    size_t hashIndex2 = hash2(key);
    size_t hashIndex3 = hash3(key);
    return bits[hashIndex1] && bits[hashIndex2] && bits[hashIndex3];
}
