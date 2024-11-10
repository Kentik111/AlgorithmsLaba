#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
#ifndef HEADER_H
#define HEADER_H

// Максимальное количество элементов
const int MAX_ELEMENTS = 100;

// Структура для хранения данных о структуре
struct Data {
    string name;
    string elements[MAX_ELEMENTS]; // Массив для хранения элементов
    int elementCount; // Количество элементов в массиве
};

template <typename T>
class DynamicArray {
private:
    T* data; // Указатель на данные
    size_t capacity; // Вместимость массива
    size_t size; // Текущий размер массива

public:
    DynamicArray(size_t cap);
    ~DynamicArray();

    void push_back(const T& value);
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    size_t getSize() const;
    bool isOccupied(size_t index) const;
};

template <typename T, size_t N>
class Set {
private:
    struct Node {
        T value;
        bool isOccupied; // Флаг занятости
    };

    DynamicArray<Node> table; // Динамический массив для хранения элементов
    size_t size; // Количество элементов в множестве

    size_t hash(const T& value) const;

public:
    Set();

    void insert(const T& value);
    bool erase(const T& value);
    bool contains(const T& value) const;

    size_t getSize() const;
    const T& getValue(size_t index) const;
    bool isFull(size_t index) const;
    void print() const;
};

//Хеш-таблица


template <typename KeyType, typename ValueType>
struct HashNode {
    KeyType key;
    ValueType value;
    HashNode* next;

    HashNode(const KeyType& key, const ValueType& value);
};

template <typename KeyType, typename ValueType>
class HashTable {
private:
    size_t size;
    HashNode<KeyType, ValueType>** table;

    size_t hash(const KeyType& key) const;

    struct BloomFilter {
    private:
        bool* bits;
        size_t size;
        size_t numHashes;

        size_t hash1(const KeyType& key) const;
        size_t hash2(const KeyType& key) const;
        size_t hash3(const KeyType& key) const;

    public:
        BloomFilter(size_t bloomFilterSize, size_t numHashFunctions);
        ~BloomFilter();
        void insert(const KeyType& key);
        bool mayContain(const KeyType& key) const;
    };

    BloomFilter bloomFilter;

public:
    HashTable(size_t initialSize = 10, size_t bloomFilterSize = 20, size_t numHashFunctions = 3);
    ~HashTable();

    void insert(const KeyType& key, const ValueType& value);
    ValueType get(const KeyType& key) const;
    void remove(const KeyType& key);
    bool contains(const KeyType& key) const;
    size_t getSize() const;
    HashNode<KeyType, ValueType>* getNodeAtIndex(size_t index) const;
    void print() const;
};

//Массив

template <typename T>
class MyArray {
private:
    T* data;
    int size;
    int capacity;

public:
    // Конструктор по умолчанию
    MyArray();

    // Конструктор с указанием начальной емкости
    MyArray(int capacity);

    // Копирующий конструктор
    MyArray(const MyArray& other);

    // Оператор присваивания
    MyArray& operator=(const MyArray& other);

    // Деструктор
    ~MyArray();

    // Добавление элемента в конец массива
    void push_back(const T& value);

    // Добавление элемента по индексу
    void insert(int index, const T& value);

    // Получение элемента по индексу
    T& operator[](int index);

    // Получение элемента по индексу (константный)
    const T& operator[](int index) const;

    // Удаление элемента по индексу
    void erase(int index);

    // Замена элемента по индексу
    void replace(int index, const T& value);

    // Длина массива
    int length() const;

    // Чтение массива (для вывода на экран)
    friend ostream& operator<<(ostream& out, const MyArray& array) {
        out << "[";
        for (int i = 0; i < array.size; i++) {
            out << array.data[i];
            if (i < array.size - 1) {
                out << ", ";
            }
        }
        out << "]";
        return out;
    }

private:
    // Изменение размера массива
    void resize(int newCapacity);
};

//Стек
template<typename T>
class Stack {
public:
    Stack();
    ~Stack();

    bool isEmpty() const;
    void push(T value);
    void pop();
    T top() const;
    void print() const;

private:
    struct Node;
    Node* topNode;
};

#endif
