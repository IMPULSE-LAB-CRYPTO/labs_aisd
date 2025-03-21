#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <stack>

using namespace std;


//генератор ПСЧ
size_t lcg(){
    static size_t x = 0;
    x = (1021*x+24631) % 116640;
    return x;
}


class BinarySearchTree {
    private:
    struct Node {
        int key;
        Node* left;
        Node* right;
    
        Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };
    
    Node* root;

    //Вспомогательные функции
    Node* copyTree(Node* node) {
        if (node == nullptr) return nullptr;
        Node* newNode = new Node(node->key);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }

    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    void printTree(Node* node) const {
        if (node) {
            printTree(node->left);
            std::cout << node->key << " ";
            printTree(node->right);
        }
    }

    bool insertNode(Node*& node, int key) {
        if (node == nullptr) {
            node = new Node(key);
            return true;
        }
        if (key < node->key) {
            return insertNode(node->left, key);
        } else if (key > node->key) {
            return insertNode(node->right, key);
        }
        return false;
    }

    bool containsNode(Node* node, int key) const {
        if (node == nullptr) return false;
        if (key < node->key) {
            return containsNode(node->left, key);
        } else if (key > node->key) {
            return containsNode(node->right, key);
        }
        return true;
    }

    bool eraseNode(Node*& node, int key) {
        if (node == nullptr) return false;
        if (key < node->key) {
            return eraseNode(node->left, key);
        } else if (key > node->key) {
            return eraseNode(node->right, key);
        } else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            } else if (node->left == nullptr) {
                Node* temp = node;
                node = node->right;
                delete temp;
            } else if (node->right == nullptr) {
                Node* temp = node;
                node = node->left;
                delete temp;
            } else {
                Node* minNode = findMin(node->right);
                node->key = minNode->key;
                eraseNode(node->right, minNode->key);
            }
            return true;
        }
    }
    Node* findMin(Node* node) {
        while (node->left != nullptr) node = node->left;
        return node;
    }


    public:
    class Iterator {
        private:
            std::stack<Node*> stack;
            Node* current;

            //помещаем все левые узлы
            void pushLeft(Node* node) {
                while (node != nullptr) {
                    stack.push(node);
                    node = node->left;
                }
            }
    
        public:
            Iterator(Node* root) {
                current = root;
                pushLeft(current);
            }
    
            int operator*() {
                return stack.top()->key;
            }
    
            Iterator& operator++() {
                Node* node = stack.top();
                stack.pop();
                pushLeft(node->right);
                return *this;
            }
    
            bool operator!=(const Iterator& other) {
                return !stack.empty() || other.current != nullptr;
            }
        };

    BinarySearchTree() : root(nullptr) {}

    BinarySearchTree(const BinarySearchTree& other) {
        root = copyTree(other.root);
    }

    ~BinarySearchTree() {
        destroyTree(root);
    }

    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            destroyTree(root);
            root = copyTree(other.root);
        }
        return *this;
    }

    void print() const {
        printTree(root);
        std::cout << std::endl;
    }

    bool insert(int key) {
        return insertNode(root, key);
    }

    bool contains(int key) const {
        return containsNode(root, key);
    }

    bool erase(int key) {
        return eraseNode(root, key);
    }

    // Методы для итераторов
    Iterator begin() {
        return Iterator(root);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

};


// Функция для получения уникальных элементов
std::vector<int> getUniqueElements(const std::vector<int>& container) {
    BinarySearchTree bst;
    std::vector<int> uniqueElements;
    
    for (int key : container) {
        bst.insert(key);
    }

    for (int key : container) {
        if (bst.contains(key)) {
            uniqueElements.push_back(key);
            bst.erase(key);
        }
    }

    return uniqueElements;

};


int main() {
    setlocale(LC_ALL, "ru_RU");
    try {
        // Инициализируем дерево
        BinarySearchTree bst;

        // Заполняем дерево элементами
        std::cout << "Inserting elements" << std::endl;
        bst.insert(5);
        bst.insert(3);
        bst.insert(7);
        bst.insert(2);
        bst.insert(4);
        bst.insert(6);
        bst.insert(8);

        // Выводим дерево
        std::cout << "Our tree: ";
        bst.print();

        // Проверяем наличие элементов
        std::cout << "\nChecking if elements exists" << std::endl;
        if (bst.contains(4)) {
            std::cout << "Contains 4: true" << std::endl;
        } else {
            std::cout << "Contains 4: false" << std::endl;
        }
        if (bst.contains(10)) {
            std::cout << "Contains 10: true" << std::endl;
        } else {
            std::cout << "Contains 10: false" << std::endl;
        }

        // Удаляем элементы
        std::cout << "\nErasing elements" << std::endl;
        if (bst.erase(3)) {
            std::cout << "Erase 3: true" << std::endl;
        } else {
            std::cout << "Erase 3: false" << std::endl;
        }
        if (bst.erase(10)) {
            std::cout << "Erase 10: true" << std::endl;
        } else {
            std::cout << "Erase 10: false" << std::endl;
        }

        // Выводим содержимое дерева после удаления
        std::cout << "Our tree after erasing: ";
        bst.print();

        // Проверяем наличие элементов после удаления
        std::cout << "\nChecking if elements exist after erasing" << std::endl;
        if (bst.contains(3)) {
            std::cout << "Contains 3: true" << std::endl;
        } else {
            std::cout << "Contains 3: false" << std::endl;
        }
        if (bst.contains(4)) {
            std::cout << "Contains 4: true" << std::endl;
        } else {
            std::cout << "Contains 4: false" << std::endl;
        }

        // Тестируем конструктор копирования
        std::cout << "\nTesting copy constructor" << std::endl;
        BinarySearchTree bstCopy(bst);
        std::cout << "Copied tree: ";
        bstCopy.print();

        // Тестируем оператор присваивания
        std::cout << "\nTesting assignment operator" << std::endl;
        BinarySearchTree bstAssigned;
        bstAssigned = bst;
        std::cout << "Contents of assigned tree: ";
        bstAssigned.print();

        // Проверка задачи с уникальными элементами
        std::vector<int> input = {3, 2, 2, 4, 2};
        std::cout << "\nInput vector: ";
        for (int x : input) std::cout << x << " ";
        std::cout << std::endl;

        std::vector<int> uniqueElements = getUniqueElements(input);

        std::cout << "Unique elements: ";
        for (int x : uniqueElements) std::cout << x << " ";
        std::cout << std::endl;

        // Демонстрация работы итераторов
        std::cout << "\nIterating through the tree:" << std::endl;
        for (auto it = bst.begin(); it != bst.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;

    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}