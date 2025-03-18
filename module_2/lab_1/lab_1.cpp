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


// // Функция для измерения производительности
// void measurePerformance() {
//     std::vector<int> sizes = {1000, 10000, 100000};
//     int attempts = 100; // Количество попыток для усреднения

//     for (int size : sizes) {
//         std::vector<double> insertTimes;
//         std::vector<double> searchTimes;
//         std::vector<double> eraseTimes;

//         for (int i = 0; i < attempts; ++i) {
//             BinarySearchTree bst;

//             // Замер времени вставки
//             auto start = std::chrono::high_resolution_clock::now();
//             for (int j = 0; j < size; ++j) {
//                 bst.insert(lcg());
//             }
//             auto end = std::chrono::high_resolution_clock::now();
//             std::chrono::duration<double> elapsed = end - start;
//             insertTimes.push_back(elapsed.count());

//             // Замер времени поиска (выполняем поиск 1000 раз для точности)
//             int searchKey = lcg();
//             start = std::chrono::high_resolution_clock::now();
//             for (int j = 0; j < 1000; ++j) {
//                 bst.contains(searchKey);
//             }
//             end = std::chrono::high_resolution_clock::now();
//             elapsed = end - start;
//             searchTimes.push_back(elapsed.count() / 1000); // Усредняем время

//             // Замер времени удаления (выполняем удаление 1000 раз для точности)
//             int eraseKey = lcg();
//             start = std::chrono::high_resolution_clock::now();
//             for (int j = 0; j < 1000; ++j) {
//                 bst.erase(eraseKey);
//             }
//             end = std::chrono::high_resolution_clock::now();
//             elapsed = end - start;
//             eraseTimes.push_back(elapsed.count() / 1000); // Усредняем время
//         }

//         // Вычисление среднего времени
//         double avgInsertTime = std::accumulate(insertTimes.begin(), insertTimes.end(), 0.0) / attempts;
//         double avgSearchTime = std::accumulate(searchTimes.begin(), searchTimes.end(), 0.0) / attempts;
//         double avgEraseTime = std::accumulate(eraseTimes.begin(), eraseTimes.end(), 0.0) / attempts;

//         std::cout << "Size: " << size << std::endl;
//         std::cout << "Average insert time: " << avgInsertTime << " seconds" << std::endl;
//         std::cout << "Average search time: " << avgSearchTime << " seconds" << std::endl;
//         std::cout << "Average erase time: " << avgEraseTime << " seconds" << std::endl;
//         std::cout << std::endl;
//     }
// }

void runTests() {
    const int sizes[] = {1000, 10000};
    const int fillAttempts = 100;
    const int operationAttempts = 1000;

    for (int size : sizes) {
        std::cout << "\nSize: " << size << std::endl;

        double bstFillTime = 0;
        double vectorFillTime = 0;

        for (int i = 0; i < fillAttempts; i++) {
            BinarySearchTree bst;
            std::vector<int> vec;

            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < size; j++)
                bst.insert(lcg() % (size * 10));
            auto end = std::chrono::high_resolution_clock::now();
            bstFillTime += std::chrono::duration<double, std::micro>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < size; j++)
                vec.push_back(lcg() % (size * 10));
            end = std::chrono::high_resolution_clock::now();
            vectorFillTime += std::chrono::duration<double, std::micro>(end - start).count();
        }

        BinarySearchTree bst;
        std::vector<int> vec;

        for (int i = 0; i < size; i++) {
            int val = lcg() % (size * 10);
            bst.insert(val);
            vec.push_back(val);
        }

        double bstSearchTime = 0, vectorSearchTime = 0;
        double bstInsertTime = 0, vectorInsertTime = 0;
        double bstEraseTime = 0, vecEraseTime = 0;

        for (int i = 0; i < operationAttempts; i++) {
            int val = lcg() % (size * 10);

            // Измеряем время для группы операций поиска
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 1000; j++) {
                bst.contains(val);
            }
            auto end = std::chrono::high_resolution_clock::now();
            bstSearchTime += std::chrono::duration<double, std::micro>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 1000; j++) {
                std::find(vec.begin(), vec.end(), val);
            }
            end = std::chrono::high_resolution_clock::now();
            vectorSearchTime += std::chrono::duration<double, std::micro>(end - start).count();

            // Измеряем время для группы операций вставки
            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 1000; j++) {
                bst.insert(val);
            }
            end = std::chrono::high_resolution_clock::now();
            bstInsertTime += std::chrono::duration<double, std::micro>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 1000; j++) {
                vec.push_back(val);
            }
            end = std::chrono::high_resolution_clock::now();
            vectorInsertTime += std::chrono::duration<double, std::micro>(end - start).count();

            // Измеряем время для группы операций удаления
            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 1000; j++) {
                bst.erase(val);
            }
            end = std::chrono::high_resolution_clock::now();
            bstEraseTime += std::chrono::duration<double, std::micro>(end - start).count();

            // Удаляем элемент из середины вектора
            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 1000; j++) {
                auto it = std::find(vec.begin(), vec.end(), val);
                if (it != vec.end()) {
                    vec.erase(it);
                }
            }
            end = std::chrono::high_resolution_clock::now();
            vecEraseTime += std::chrono::duration<double, std::micro>(end - start).count();
        }

        // Усредняем время для группы операций
        bstSearchTime /= (operationAttempts * 1000);
        vectorSearchTime /= (operationAttempts * 1000);
        bstInsertTime /= (operationAttempts * 1000);
        vectorInsertTime /= (operationAttempts * 1000);
        bstEraseTime /= (operationAttempts * 1000);
        vecEraseTime /= (operationAttempts * 1000);

        std::cout << "BST Fill Time: " << bstFillTime / fillAttempts << " micro s\n";
        std::cout << "Vector Fill Time: " << vectorFillTime / fillAttempts << " micro s\n";
        std::cout << "BST Search Time: " << bstSearchTime << " micro s\n";
        std::cout << "Vector Search Time: " << vectorSearchTime << " micro s\n";
        std::cout << "BST Insert Time: " << bstInsertTime << " micro s\n";
        std::cout << "Vector Insert Time: " << vectorInsertTime << " micro s\n";
        std::cout << "BST Erase Time: " << bstEraseTime << " micro s\n";
        std::cout << "Vector Erase Time: " << vecEraseTime << " micro s\n";
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
        runTests();
    }

    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}