#include <iostream>
using namespace std;

template <typename T>
class DynamicArray {
private:
    T* data;
    size_t capacity;
    size_t size;

public:
    DynamicArray(size_t cap) : capacity(cap), size(0) {
        data = new T[capacity];
    }

    ~DynamicArray() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (size < capacity) {
            data[size] = value;
            ++size;
        } else {
            cout << "Dynamic array is full!" << endl;
        }
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    size_t getSize() const {
        return size;
    }

    bool isOccupied(size_t index) const {
        return index < size;
    }


};

template <typename T, size_t N>
struct Node {
    T value;
    bool isOccupied;
};

template <typename T, size_t N>
class Set {
private:
    Node<T, N> table[N];
    size_t size;

public:
    Set() : size(0) {
        for (size_t i = 0; i < N; ++i) {
            table[i].isOccupied = false;
        }
    }

    size_t hash(const T& value) const {
        return std::hash<T>()(value) % N;
    }

    void insert(const T& value) {
        if (contains(value)) {
            return;
        }

        if (size >= N) {
            cout << "Set is full!" << endl;
            return;
        }

        size_t index = hash(value);
        while (table[index].isOccupied) {
            index = (index + 1) % N;
        }

        table[index].value = value;
        table[index].isOccupied = true;
        ++size;
    }

    bool erase(const T& value) {
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

        return false;
    }

    bool contains(const T& value) const {
        size_t index = hash(value);
        size_t startIndex = index;

        do {
            if (table[index].isOccupied && table[index].value == value) {
                return true;
            }
            index = (index + 1) % N;
        } while (index != startIndex && table[index].isOccupied);

        return false;
    }

    size_t getSize() const {
        return size;
    }

    const T& getValue(size_t index) const {
        return table[index].value;
    }

    bool isFull(size_t index) const {
        return table[index].isOccupied;
    }

    bool isEmpty() const {
        return size == 0;
    }

    void print() const {
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
    // Функция для обнуления множества
    void clear() {
        for (size_t i = 0; i < N; ++i) {
            table[i].isOccupied = false;
        }
        size = 0;
    }
};


// Функция для подсчета суммы элементов множества
int sumSet(const Set<int, 10>& set) {
    int sum = 0;
    for (size_t i = 0; i < 10; ++i) {
        if (set.isFull(i)) {
            sum += set.getValue(i);
        }
    }
    return sum;
}

bool findSubsetWithSum(Set<int, 10>& set, int targetSum, int currentSum, int startIndex, Set<int, 10>& currentSubset) {
    if (currentSum == targetSum) {
        currentSubset.print(); // Выводим подмножество, когда достигаем целевой суммы
        return true; // Возвращаем true, чтобы отметить, что нашли подмножество
    }

    for (int i = startIndex; i < 10; ++i) {
        if (set.isFull(i)) {
            int value = set.getValue(i);
            if (currentSum + value <= targetSum) {
                currentSubset.insert(value);
                set.erase(value);
                if (findSubsetWithSum(set, targetSum, currentSum + value, i + 1, currentSubset)) {
                    return true;
                } else {
                set.insert(value);
                currentSubset.erase(value);
                }
            }
        }
    }

    return false; // Подмножество не найдено
}

bool canDivideIntoSubsets(Set<int, 10>& set, int targetSum) {
    int totalSum = sumSet(set);

    if (totalSum % targetSum != 0) {
        cout << "Нельзя поделить на подмножества" << endl;
        return false;
    }

    Set<int, 10> currentSubset;
    while (!set.isEmpty()) { // Пока множество не пустое
        if (!findSubsetWithSum(set, targetSum, 0, 0, currentSubset)) {
            // Если не найдено подмножество, выводим сообщение об ошибке
            cout << "Нельзя поделить на подмножества" << endl;
            return false;
        }
        currentSubset.clear(); // Очищаем подмножество перед поиском следующего
    }

    return true;
}



int main() {
    Set<int, 10> numbers;
    cout << "Введите размер множества:" << endl;
    int setSize;
    cin >> setSize;
    if (cin.fail()){
        cout << "Некорректный ввод" << endl;
        exit(0);
    }
    if (setSize < 0 || setSize > 10){
        cout << "Некорректный размер" << endl;
        exit(0);
    }
    for (int i = 0; i < setSize; i++){
        int element;
        cout << "Введите элемент множества:" << endl;
        cin >> element;
        if (cin.fail()){
            cout << "Некорректный ввод" << endl;
            exit(0);
        }
        numbers.insert(element);

    }
    cout << "Множество: ";
    numbers.print();
    cout << "Введите сумму:" << endl;
    int summa;
    cin >> summa;
    if (cin.fail()){
        cout << "Некорректный ввод" << endl;
        exit(0);
    }
    cout << "Разбиение на подмножества с суммой " << summa << endl;
    if (canDivideIntoSubsets(numbers, summa)) {
        cout << "Разбиение успешно выполнено" << endl;
    } else {
        cout << "Невозможно разбить множество на подмножества с равной суммой" << endl;
    }

    return 0;
}

