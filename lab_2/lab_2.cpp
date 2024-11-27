#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <complex>
#include <random>
#include <cstdlib>
#include <ctime>

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
    
    ///...


};


int main() {
    setlocale(LC_ALL, "ru_RU");
    try {
        std::cout << "Hellosecond";
        LinkedList list_1 = LinkedList<int>();
        LinkedList list_2 = LinkedList<int>(10);

        
    }

    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
