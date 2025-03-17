#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>


using namespace std;

//генератор ПСЧ
size_t lcg(){
    static size_t x = 0;
    x = (1021*x+24631) % 116640;
    return x;
}


struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};


class BinarySearchTree {
    private:
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
        return false; // Элемент уже существует
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
}


// Функция для измерения производительности
void measurePerformance() {
    std::vector<int> sizes = {1000, 10000, 100000};
    int attempts = 100; // Количество попыток для усреднения

    for (int size : sizes) {
        std::vector<double> insertTimes;
        std::vector<double> searchTimes;
        std::vector<double> eraseTimes;

        for (int i = 0; i < attempts; ++i) {
            BinarySearchTree bst;

            // Замер времени вставки
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < size; ++j) {
                bst.insert(lcg());
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            insertTimes.push_back(elapsed.count());

            // Замер времени поиска (выполняем поиск 1000 раз для точности)
            int searchKey = lcg();
            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 1000; ++j) {
                bst.contains(searchKey);
            }
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            searchTimes.push_back(elapsed.count() / 1000); // Усредняем время

            // Замер времени удаления (выполняем удаление 1000 раз для точности)
            int eraseKey = lcg();
            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 1000; ++j) {
                bst.erase(eraseKey);
            }
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            eraseTimes.push_back(elapsed.count() / 1000); // Усредняем время
        }

        // Вычисление среднего времени
        double avgInsertTime = std::accumulate(insertTimes.begin(), insertTimes.end(), 0.0) / attempts;
        double avgSearchTime = std::accumulate(searchTimes.begin(), searchTimes.end(), 0.0) / attempts;
        double avgEraseTime = std::accumulate(eraseTimes.begin(), eraseTimes.end(), 0.0) / attempts;

        std::cout << "Size: " << size << std::endl;
        std::cout << "Average insert time: " << avgInsertTime << " seconds" << std::endl;
        std::cout << "Average search time: " << avgSearchTime << " seconds" << std::endl;
        std::cout << "Average erase time: " << avgEraseTime << " seconds" << std::endl;
        std::cout << std::endl;
    }
}


int main() {
    setlocale(LC_ALL, "ru_RU");
    try {
        //Инициализируем дерво
        BinarySearchTree bst;

        //Заполняем древо элементами
        std::cout << "Inserting elements" << std::endl;
        bst.insert(5);
        bst.insert(3);
        bst.insert(7);
        bst.insert(2);
        bst.insert(4);
        bst.insert(6);
        bst.insert(8);

        //Выводим древо
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

        auto start = std::chrono::high_resolution_clock::now();
        std::vector<int> uniqueElements = getUniqueElements(input);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start; //Замерка времени выполнения

        std::cout << "Unique elements: ";
        for (int x : uniqueElements) std::cout << x << " ";
        std::cout << std::endl;
        std::cout << "Time taken to get unique elements: " << elapsed.count() << " seconds" << std::endl;

        // Измерение производительности
        std::cout << "\nMeasuring performance..." << std::endl;
        measurePerformance();
    }

    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}