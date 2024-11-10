#include "header.h"

// Конструктор по умолчанию
template <typename T>
MyArray<T>::MyArray() : data(nullptr), size(0), capacity(0) {}

// Конструктор с указанием начальной емкости
template <typename T>
MyArray<T>::MyArray(int capacity) : size(0), capacity(capacity) {
    if (capacity > 0) {
        data = new T[capacity];
    } else {
        data = nullptr;
    }
}

// Копирующий конструктор
template <typename T>
MyArray<T>::MyArray(const MyArray& other) : size(other.size), capacity(other.capacity) {
    data = new T[capacity];
    for (int i = 0; i < size; i++) {
        data[i] = other.data[i];
    }
}

// Оператор присваивания
template <typename T>
MyArray<T>& MyArray<T>::operator=(const MyArray& other) {
    if (this != &other) {
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

// Деструктор
template <typename T>
MyArray<T>::~MyArray() {
    delete[] data;
}

// Добавление элемента в конец массива
template <typename T>
void MyArray<T>::push_back(const T& value) {
    if (size == capacity) {
        resize(capacity + 1);
    }
    data[size++] = value;
}

// Добавление элемента по индексу
template <typename T>
void MyArray<T>::insert(int index, const T& value) {
    if (index < 0 || index > size) {
        throw out_of_range("Индекс за пределами массива");
    }
    if (size == capacity) {
        resize(capacity + 1);
    }
    for (int i = size; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    size++;
}

// Получение элемента по индексу
template <typename T>
T& MyArray<T>::operator[](int index) {
    if (index < 0 || index >= size) {
        throw out_of_range("Индекс за пределами массива");
    }
    return data[index];
}

// Получение элемента по индексу (константный)
template <typename T>
const T& MyArray<T>::operator[](int index) const {
    if (index < 0 || index >= size) {
        throw out_of_range("Индекс за пределами массива");
    }
        return data[index];
}

// Удаление элемента по индексу
template <typename T>
void MyArray<T>::erase(int index) {
    if (index < 0 || index >= size) {
        throw out_of_range("Индекс за пределами массива");
    }
    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    size--;
}

// Замена элемента по индексу
template <typename T>
void MyArray<T>::replace(int index, const T& value) {
    if (index < 0 || index >= size) {
        throw out_of_range("Индекс за пределами массива");
    }
    data[index] = value;
}

// Длина массива
template <typename T>
int MyArray<T>::length() const {
    return size;
}

// Изменение размера массива
template <typename T>
void MyArray<T>::resize(int newCapacity) {
    T* newData = new T[newCapacity];
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}
