#include "header.h"
#include "Stack.cpp"

int calculate(const string& expression) {
    Stack<int> values; // Стек для хранения значений
    Stack<char> operators; // Стек для хранения операторов

    for (size_t i = 0; i < expression.length(); ++i) {
        char ch = expression[i];

        if (isdigit(ch)) { // Обработка чисел
            int number = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                number = number * 10 + (expression[i] - '0');
                ++i;
            }
            values.push(number);
            --i; // Вернуться на предыдущий символ
        } else if (ch == '(') { // Открывающая скобка
            operators.push(ch);
        } else if (ch == ')') { // Закрывающая скобка
            while (!operators.isEmpty() && operators.top() != '(') {
                int operand2 = values.top();
                values.pop();
                int operand1 = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                switch (op) {
                    case '*':
                        values.push(operand1 * operand2);
                        break;
                    case '/':
                        if (operand2 == 0) {
                            throw runtime_error("Деление на ноль");
                        }
                        values.push(operand1 / operand2);
                        break;
                    case '+':
                        values.push(operand1 + operand2);
                        break;
                    case '-':
                        values.push(operand1 - operand2);
                        break;
                }
            }
            operators.pop(); // Удаление открывающей скобки
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') { // Операторы
            while (!operators.isEmpty() && (
                (ch == '+' || ch == '-') &&
                (operators.top() == '*' || operators.top() == '/')
            )) { // Вычисление операций с более высоким приоритетом
                int operand2 = values.top();
                values.pop();
                int operand1 = values.top();
                values.pop();
                char op = operators.top();
                operators.pop();
                switch (op) {
                    case '*':
                        values.push(operand1 * operand2);
                        break;
                    case '/':
                        if (operand2 == 0) {
                            throw runtime_error("Деление на ноль");
                        }
                        values.push(operand1 / operand2);
                        break;
                }
            }
            operators.push(ch); // Добавление оператора в стек
        }
    }

    // Вычисление оставшихся операций
    while (!operators.isEmpty()) {
        int operand2 = values.top();
        values.pop();
        int operand1 = values.top();
        values.pop();
        char op = operators.top();
        operators.pop();
        switch (op) {
            case '*':
                values.push(operand1 * operand2);
                break;
            case '/':
                if (operand2 == 0) {
                    throw runtime_error("Деление на ноль");
                }
                values.push(operand1 / operand2);
                break;
            case '+':
                values.push(operand1 + operand2);
                break;
            case '-':
                values.push(operand1 - operand2);
                break;
        }
    }

    return values.top(); // Возвращение результата
}

int main() {
    try {
        string expression;
        cout << "Введите математическое выражение: ";
        getline(cin, expression); // Считывание выражения с пробелами
        int result = calculate(expression);
        cout << "Результат: " << result << endl;
    } catch (const runtime_error& error) {
        cerr << "Ошибка: " << error.what() << endl;
    }
    return 0;
}

