#include "header.h"

template <typename T>
DynamicArray<T>::DynamicArray(size_t cap) : capacity(cap), size(0) {
    data = new T[capacity]; // Выделение памяти
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data; // Освобождение памяти
}

template <typename T>
void DynamicArray<T>::push_back(const T& value) {
    if (size < capacity) {
        data[size] = value;
        ++size;
    } else {
        cout << "Dynamic array is full!" << endl;
    }
}

template <typename T>
T& DynamicArray<T>::operator[](size_t index) {
    return data[index]; // Доступ к элементам массива
}

template <typename T>
const T& DynamicArray<T>::operator[](size_t index) const {
    return data[index];
}

template <typename T>
size_t DynamicArray<T>::getSize() const {
    return size;
}

template <typename T>
bool DynamicArray<T>::isOccupied(size_t index) const {
    return index < size; // Проверка, занят ли элемент
}

template <typename T, size_t N>
Set<T, N>::Set() : table(N), size(0) {
    for (size_t i = 0; i < N; ++i) {
        table[i].isOccupied = false; // Инициализация флага занятости
    }
}

template <typename T, size_t N>
size_t Set<T, N>::hash(const T& value) const {
    return std::hash<T>()(value) % N; // Хеш-функция
}

template <typename T, size_t N>
void Set<T, N>::insert(const T& value) {
    if (contains(value)) {
        return; // Проверка на дубликат
    }

    if (size >= N) {
        std::cout << "Set is full!" << std::endl;
        return; // Множество переполнено
    }

    size_t index = hash(value);
    while (table[index].isOccupied) {
        index = (index + 1) % N;
    }

    table[index].value = value;
    table[index].isOccupied = true;
    ++size;
}

template <typename T, size_t N>
bool Set<T, N>::erase(const T& value) {
    size_t index = hash(value);
    size_t startIndex = index;

    do {
        if (table[index].isOccupied && table[index].value == value) {
            table[index].isOccupied = false;
            --size;
            return true;
        }
        index = (index + 1) % N;
    } while (index != startIndex && table[index].isOccupied);

    return false; // Элемент не найден
}

template <typename T, size_t N>
bool Set<T, N>::contains(const T& value) const {
    size_t index = hash(value);
    size_t startIndex = index;

    do {
        if (table[index].isOccupied && table[index].value == value) {
            return true;
        }
        index = (index + 1) % N;
    } while (index != startIndex && table[index].isOccupied);

    return false; // Элемент не найден
}

template <typename T, size_t N>
size_t Set<T, N>::getSize() const {
    return size;
}

template <typename T, size_t N>
const T& Set<T,N>::getValue(size_t index) const {
    return table[index].value;
}

template <typename T, size_t N>
bool Set<T,N>::isFull(size_t index) const {
    return table[index].isOccupied;
}

template <typename T, size_t N>
void Set<T, N>::print() const {
    cout << "{";
    bool first = true;
    for (size_t i = 0; i < N; ++i) {
        if (table[i].isOccupied) {
            if (!first) {
                cout << ", ";
            }
            cout << table[i].value;
            first = false;
        }
    }
    cout << "}" << endl;
}




