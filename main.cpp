#include "header.h"
#include "set.cpp"
#include "hashTable.cpp"
#include "Array.cpp"
#include "Stack.cpp"
#include <getopt.h>

Data loadData(const string& filename) {
    Data data;
    ifstream file(filename);
    if (file.is_open()) {
        getline(file, data.name); // Считываем имя структуры
        string line;
        data.elementCount = 0;
        while (getline(file, line) && data.elementCount < MAX_ELEMENTS) {
            data.elements[data.elementCount++] = line; // Добавляем элементы
        }
        file.close();
    } else {
        // Если файл не найден, создаем его
        ofstream newFile(filename);
        if (newFile.is_open()) {
            cout << "Файл " << filename << " создан. Перезапустите программу" << endl;
            newFile.close();
        } else {
            cerr << "Ошибка создания файла: " << filename << endl;
        }
        exit(0);
    }
    return data;
}

void saveData(const string& filename, const Data& data) {
    ofstream file(filename);
    if (file.is_open()) {
        file << data.name << endl; // Записываем имя структуры
        for (int i = 0; i < data.elementCount; ++i) {
            file << data.elements[i] << endl; // Записываем элементы
        }
        file.close();
    } else {
        cerr << "Ошибка открытия файла: " << filename << endl;
    }
}

// Очистка файла
void clearFile(const string& filename) {
    ofstream file(filename, ofstream::trunc); // Перезаписываем файл
    file.close();
}


// Функция для копирования элементов из массива в множество
template<typename T, size_t N>
void copyElementsToSet(Set<T, N>& ourSet, const T elements[], int count) {
    for (int i = 0; i < count; ++i) {
        ourSet.insert(elements[i]);
    }
}

// Функция для копирования элементов из множества в массив
template<typename T, size_t N>
void copyElementsFromSet(Set<T, N>& ourSet, T elements[], int& count) {
    count = 0; // Сброс счетчика перед копированием
    for (size_t i = 0; i < N; ++i) {
        if (ourSet.isFull(i)) { // Проверка на занятость
            elements[count++] = ourSet.getValue(i); // Копирование элемента
        }
    }
}



// Функция для копирования элементов из массива в стек
template<typename T>
void copyElementsToStack(Stack<T>& ourStack, const string elements[], int count) {
    for (int i = count - 1; i >= 0; --i) { // Обратный порядок для стека
        ourStack.push(elements[i]);
    }
}

// Функция для копирования элементов из стека в массив
template<typename T>
void copyElementsFromStack(Stack<T>& ourStack, string elements[], int& count) {
    while (!ourStack.isEmpty()) {
        elements[count++] = ourStack.top();
        ourStack.pop();
    }
}

// Функция для копирования элементов из массива в массив MyArray
template<typename T>
void CopyElementsToArray(MyArray<T>& myArray, const string elements[], int count) {
    for (int i = 0; i < count; ++i) {
        myArray.push_back(elements[i]);
    }
}

// Функция для копирования элементов из массива MyArray в массив
template<typename T>
void CopyElementsFromArray(MyArray<T>& myArray, string elements[], int& count) {
    for (int i = 0; i < myArray.length(); ++i) {
        elements[count++] = myArray[i];
    }
}


// Функция для преобразования массива строк в хеш-таблицу
void convertArrayToHashTable(const string elements[], int elementCount, HashTable<string, string>& ht) {
    for (int i = 0; i < elementCount; ++i) {
        stringstream ss(elements[i]);
        string key;
        string value;
        if (ss >> key >> value) {
            ht.insert(key, value);
        } else {
            cerr << "Ошибка формата данных в массиве." << endl;
        }
    }
}

// Функция для преобразования хеш-таблицы в массив строк
void convertHashTableToArray(const HashTable<string, string>& ht, string elements[], int& elementCount) {
    elementCount = 0;
    for (size_t i = 0; i < ht.getSize(); ++i) {
        auto current = ht.getNodeAtIndex(i);
        while (current != nullptr) {
            stringstream ss;
            ss << current->key << " " << current->value;
            elements[elementCount++] = ss.str();
            current = current->next;
        }
    }
}


void generateSubarrays(MyArray<string> Array, int n, int index, MyArray<string>& current) {
    // Печатаем текущий подмассив
    cout << "{";
    for (int i = 0; i < current.length(); ++i) {
        cout << current[i];
        if (i < current.length() - 1) {
            cout << ", ";
        }
    }
    cout << "}" << endl;

    // Рекурсивный случай
    for (int i = index; i < n; ++i) {
        // Добавляем элемент в текущий подмассив
        current.push_back(Array[i]);
        // Рекурсивно вызываем функцию для следующего элемента
        generateSubarrays(Array, n, i + 1, current);
        // Удаляем последний элемент для возврата к предыдущему состоянию
        int last = current.length()-1;
        current.erase(last);
    }
}


int main(int argc, char** argv) {
    int option;
    string filename;
    string query;

    while ((option = getopt(argc, argv, "f:q:")) != -1) {
        switch (option) {
            case 'f':
                filename = optarg;
                break;
            case 'q':
                query = optarg;
                break;
            default:
                cerr << "Некорректные аргументы" << endl;
                return 1;
        }
    }

    // Загрузка данных из файла
    Data data = loadData(filename);

    // Парсинг запроса
    istringstream iss(query);
    string command;
    string structName;
    string item;
    int index;
    string key;
    string value;

    iss >> command >> structName;

    data.name = structName;

    if (command == "SETADD"){
        iss >> item;
        Set<string, 10> mySet;
        copyElementsToSet(mySet, data.elements, data.elementCount);
        mySet.insert(item);
        data.elementCount = 0;
        copyElementsFromSet(mySet, data.elements, data.elementCount);
        saveData(filename, data); // Сохраняем обновленные данные
        cout << item << endl;
    } else if(command == "SETDEL"){
        iss >> item;
        Set<string, 10> mySet;
        copyElementsToSet(mySet, data.elements, data.elementCount);
        if (mySet.erase(item)){
            data.elementCount = 0;
            copyElementsFromSet(mySet, data.elements, data.elementCount);
            saveData(filename, data); // Сохраняем обновленные данные
            cout << "Элемент удалён" << endl;
        }
        else {
            cerr << "Ошибка. Элемент не найден в множестве" << endl;
        }
    } else if(command == "SET_AT"){
        iss >> item;
        Set<string, 10> mySet;
        copyElementsToSet(mySet, data.elements, data.elementCount);
        if (mySet.contains(item)) {
            cout << "Элемент присутствует в множестве" << endl;
        }
        else {
            cout << "Элемент отсутствует в множестве" << endl;
        }
    } else if (command == "SETPRINT"){
        Set<string, 10> mySet;
        copyElementsToSet(mySet, data.elements, data.elementCount);
        cout << "Множество:" << endl;
        mySet.print();
    } else if (command == "SPUSH") {
        iss >> item;
        Stack<string> myStack;
        copyElementsToStack(myStack, data.elements, data.elementCount);
        myStack.push(item); // Добавляем элемент
        data.elementCount = 0;
        copyElementsFromStack(myStack, data.elements, data.elementCount);
        saveData(filename, data); // Сохраняем обновленные данные
        cout << item << endl;
    } else if (command == "SPOP") {
        Stack<string> myStack;
        copyElementsToStack(myStack, data.elements, data.elementCount);
        if (!myStack.isEmpty()) {
            myStack.pop(); // Удаляем элемент
            data.elementCount = 0;
            copyElementsFromStack(myStack, data.elements, data.elementCount);
            saveData(filename, data);
            cout << "Элемент удален." << endl;
        } else {
            cerr << "Стек пуст. Невозможно удалить элемент." << endl;
        }
    } else if (command == "STOP") {
        Stack<string> myStack;
        copyElementsToStack(myStack, data.elements, data.elementCount);
        if (!myStack.isEmpty()) {
            cout << "Верхний элемент: " << myStack.top() << endl;
        } else {
            cerr << "Стек пуст. Нет верхнего элемента." << endl;
        }
    } else if (command == "SPRINT") {
        Stack<string> myStack;
        copyElementsToStack(myStack, data.elements, data.elementCount);
        myStack.print(); // Вывод элементов стека
    } else if (command == "MPUSH") {
        iss >> item;
        MyArray<string> myArray;
        CopyElementsToArray(myArray, data.elements, data.elementCount);
        myArray.push_back(item);
        data.elementCount = 0;
        CopyElementsFromArray(myArray, data.elements, data.elementCount);
        saveData(filename, data);
        cout << item << endl;
    } else if (command == "MINS") {
        iss >> item >> index;
        MyArray<string> myArray;
        CopyElementsToArray(myArray, data.elements, data.elementCount);
        if (index >= 0 && index <= myArray.length()) {
            myArray.insert(index, item); // Вставляем элемент
            data.elementCount = 0;
            CopyElementsFromArray(myArray, data.elements, data.elementCount);
            saveData(filename, data);
            cout << item << endl;
        } else {
            cerr << "Некорректный индекс" << endl;
        }
    } else if (command == "MGET") {
        iss >> index;
        MyArray<string> myArray;
        CopyElementsToArray(myArray, data.elements, data.elementCount);
        if (index >= 0 && index < myArray.length()) {
            cout << myArray[index] << endl; // Вывод элемента
        } else {
            cerr << "Некорректный индекс" << endl;
        }
    } else if (command == "MDEL") {
        iss >> index;
        MyArray<string> myArray;
        CopyElementsToArray(myArray, data.elements, data.elementCount);
        if (index >= 0 && index < myArray.length()) {
            myArray.erase(index);
            data.elementCount = 0;
            CopyElementsFromArray(myArray, data.elements, data.elementCount);
            saveData(filename, data);
            cout << "Элемент удален." << endl;
        } else {
            cerr << "Некорректный индекс" << endl;
        }
    } else if (command == "MREP") {
        iss >> item >> index;
        MyArray<string> myArray;
        CopyElementsToArray(myArray, data.elements, data.elementCount);
        if (index >= 0 && index < myArray.length()) {
            myArray.replace(index, item);
            data.elementCount = 0;
            CopyElementsFromArray(myArray, data.elements, data.elementCount);
            saveData(filename, data);
            cout << item << endl;
        } else {
            cerr << "Некорректный индекс" << endl;
        }
    } else if (command == "MLEN") {
        MyArray<string> myArray;
        CopyElementsToArray(myArray, data.elements, data.elementCount);
        cout << myArray.length() << endl;
    } else if (command == "MARR"){
        MyArray<string> myArray;
        MyArray<string> curArray;
        CopyElementsToArray(myArray, data.elements, data.elementCount);
        int n = myArray.length();
        generateSubarrays(myArray, n, 0, curArray);
    }

    else if (command == "MPRINT") {
        MyArray<string> myArray;
        CopyElementsToArray(myArray, data.elements, data.elementCount);
        cout << myArray << endl;
    } else if (command == "HSET") {
        iss >> key >> value;
        HashTable<string, string> ht(10); // Создаем хеш-таблицу
        convertArrayToHashTable(data.elements, data.elementCount, ht);
        if (ht.contains(key)){
            ht.remove(key);
        }
        ht.insert(key, value);
        data.elementCount = 0;
        convertHashTableToArray(ht, data.elements, data.elementCount);
        saveData(filename, data);
        cout << "Элемент добавлен." << endl;
    } else if (command == "HGET") {
        iss >> key;
        HashTable<string, string> ht(10);
        convertArrayToHashTable(data.elements, data.elementCount, ht);
        try {
            string getValue = ht.get(key);
            cout << "Значение: " << getValue << endl;
        } catch (out_of_range& e) {
            cerr << "Ошибка: Ключ не найден" << endl;
        }
    } else if (command == "HDEL") {
        iss >> key;
        HashTable<string, string> ht(10);
        convertArrayToHashTable(data.elements, data.elementCount, ht);
        if (ht.remove(key)){
            data.elementCount = 0;
            convertHashTableToArray(ht, data.elements, data.elementCount);
            saveData(filename, data);
            cout << "Элемент удален." << endl;
        }
        else{
            cerr << "Ошибка: Ключ не найден" << endl;
        }

    } else if (command == "HPRINT") {
        HashTable<string, string> ht(10);
        convertArrayToHashTable(data.elements, data.elementCount, ht);
        ht.print();
    }
    else if (command == "CLEAR") {
        clearFile(filename);
    } else {
        cerr << "Неизвестная команда" << endl;
    }
    return 0;
}

