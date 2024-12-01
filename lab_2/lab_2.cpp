#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <complex>
#include <random>
#include <cstdlib>
#include <ctime>
#include <locale>
#include <windows.h>

using namespace std;


// Узел списка
template<typename T>
struct Node {
    T data;
    Node* next;
    
    Node(T value) : data(value), next(nullptr) {}
};


// Класс односвязного циклического связного списка
template<typename T>
class LinkedList {
private:
    Node<T>* head;
    
public:
    // Конструктор по умолчанию
    LinkedList() : head(nullptr) {}

    // Конструктор копирования
    LinkedList(const LinkedList& other) {
        if (other.head == nullptr) {
            head = nullptr;
            return;
        }
        head = new Node<T>(other.head->data);
        Node<T>* current = head;
        Node<T>* otherCurrent = other.head->next;
        while (otherCurrent != nullptr && otherCurrent != other.head) {
            current->next = new Node<T>(otherCurrent->data);
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
        current->next = head; // Замыкаем список
    }

    // Конструктор с заполнением случайными значениями
    LinkedList(int size) : head(nullptr) {
        std::srand(static_cast<unsigned int>(std::time(0)));
        for (int i = 0; i < size; ++i) {
            push_tail(std::rand() % 100); // Заполняем случайными значениями от 0 до 99
        }
    }

    // Деструктор
    ~LinkedList() {
        if (head == nullptr) return;
        Node<T>* current = head;
        Node<T>* nextNode;
        do {
            nextNode = current->next;
            delete current;
            current = nextNode;
        } while (current != head);
    }

    // Оператор присваивания
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            this->~LinkedList(); // уничтожаем текущий список
            new (this) LinkedList(other); // инициализируем новым копированием
        }
        return *this;
    }

    // Добавление элемента в конец списка
    void push_tail(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (head == nullptr) {
            head = newNode;
            newNode->next = head;
        } else {
            Node<T>* current = head;
            while (current->next != head) {
                current = current->next;
            }
            current->next = newNode;
            newNode->next = head;
        }
    }

    // Добавление другого списка (перегрузка)
    void push_tail(const LinkedList& other) {
        if (other.head == nullptr) return;
        Node<T>* otherCurrent = other.head;
        do {
            push_tail(otherCurrent->data);
            otherCurrent = otherCurrent->next;
        } while (otherCurrent != other.head);
    }

    // Добавление элемента в начало списка
    void push_head(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (head == nullptr) {
            head = newNode;
            newNode->next = head;
        } else {
            Node<T>* current = head;
            while (current->next != head) {
                current = current->next;
            }
            newNode->next = head;
            head = newNode;
            current->next = head;
        }
    }

    // Удаление элемента из начала списка
    void pop_head() {
        if (head == nullptr) throw std::runtime_error("List is empty.");
        Node<T>* current = head;
        if (current->next == head) { // Если один элемент
            delete current;
            head = nullptr;
        } else {
            Node<T>* tail = head;
            while (tail->next != head) {
                tail = tail->next;
            }
            head = head->next;
            tail->next = head;
            delete current;
        }
    }

    // Удаление элемента из конца списка
    void pop_tail() {
        if (head == nullptr) throw std::runtime_error("List is empty.");
        Node<T>* current = head;
        if (current->next == head) { // Если один элемент
            delete current;
            head = nullptr;
        } else {
            Node<T>* prev = nullptr;
            while (current->next != head) {
                prev = current;
                current = current->next;
            }
            prev->next = head;
            delete current;
        }
    }

    // Удаление всех элементов с определённым значением
    void delete_node(const T& value) {
        while (head != nullptr && head->data == value) {
            pop_head();
        }
        if (head == nullptr) return;
        Node<T>* current = head;
        Node<T>* prev = nullptr;
        do {
            if (current->data == value) {
                if (prev) {
                    prev->next = current->next;
                    delete current;
                    current = prev->next;
                } else {
                    pop_head();
                    current = head; // Если голова была удалена, возвращаемся к ней
                }
            } else {
                prev = current;
                current = current->next;
            }
        } while (current != head);
    }
    
    // Доступ по индексу
    T& operator[](int index) {
        if (index < 0) throw std::out_of_range("Index out of range.");
        Node<T>* current = head;
        for (int i = 0; i < index && current != nullptr; ++i) {
            current = current->next;
        }
        if (current == nullptr) throw std::out_of_range("Index out of range.");
        return current->data;
    }

    const T& operator[](int index) const {
        if (index < 0) throw std::out_of_range("Index out of range.");
        Node<T>* current = head;
        for (int i = 0; i < index && current != nullptr; ++i) {
            current = current->next;
        }
        if (current == nullptr) throw std::out_of_range("Index out of range.");
        return current->data;
    }

    // Вывод списка
    void print() const {
        if (head == nullptr) {
            std::cout << "List is empty." << std::endl;
            return;
        }
        Node<T>* current = head;
        do {
            std::cout << current->data << " ";
            current = current->next;
        } while (current != head);
        std::cout << std::endl;
    }
};

    // Задача 1: Найти простые числа в диапазоне [1; N]
void findPrimes(int N) {
    LinkedList<int> primesList;

    for (int num = 2; num <= N; ++num) {
        bool isPrime = true;
        for (int i = 2; i * i <= num; ++i) {
            if (num % i == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            primesList.push_tail(num);
        }
    }

    std::cout << "Simple numbers from 1 to " << N << ": ";
    primesList.print();
}

// Задача 2: Представление многочлена в виде списка
/*
void evaluatePolynomial(const LinkedList<std::pair<int, int>>& polynomial, int x) {
    int result = 0;
    Node<std::pair<int, int>>* current = polynomial.head;

    if (current != nullptr) {
        do {
            result += current->data.first * std::pow(x, current->data.second);
            current = current->next;
        } while (current != polynomial.head);
    }

    std::cout << "Значение многочлена при x = " << x << ": " << result << std::endl;
}*/

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //setlocale(LC_ALL, "ru_RU");
    try {
        // Задача 1
        int N;
        std::cout << "Put N to find simple numbers: ";
        std::cin >> N;
        findPrimes(N);


         // Задача 2
         /*
        LinkedList<std::pair<int, int>> polynomial; // Хранит пары: {коэффициент, степень}
        polynomial.push_tail({3, 2}); // 3x^2
        polynomial.push_tail({2, 1}); // 2x^1
        polynomial.push_tail({1, 0}); // 1x^0
         int x;
        std::cout << "Введите значение x для оценки многочлена: ";
        std::cin >> x;
        evaluatePolynomial(polynomial, x)
        */
    }

    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
