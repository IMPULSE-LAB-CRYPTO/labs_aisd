#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <complex>
#include <string>


using namespace std;


//генератор ПСЧ
size_t lcg(){
    static size_t x = 0;
    x = (1021*x+24631) % 116640;
    return x;
}

template <typename T>
class BinarySearchTree {
    private:
    struct Node {
        T key;
        Node* left;
        Node* right;
    
        Node(const T& k) : key(k), left(nullptr), right(nullptr) {}
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

    bool insertNode(Node*& node, const T& key) {
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

    bool containsNode(Node* node, const T& key) const {
        if (node == nullptr) return false;
        if (key < node->key) {
            return containsNode(node->left, key);
        } else if (key > node->key) {
            return containsNode(node->right, key);
        }
        return true;
    }

    bool eraseNode(Node*& node, const T& key) {
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

    bool insert(const T& key) {
        return insertNode(root, key);
    }

    bool contains(const T& key) const {
        return containsNode(root, key);
    }

    bool erase(const T& key) {
        return eraseNode(root, key);
    }

};


// Функция для получения уникальных элементов
template <typename T>
std::vector<T> getUniqueElements(const std::vector<T>& container) {
    BinarySearchTree<T> bst;
    std::vector<T> uniqueElements;
    
    for (const T& key : container) {
        bst.insert(key);
    }

    for (const T& key : container) {
        if (bst.contains(key)) {
            uniqueElements.push_back(key);
            bst.erase(key);
        }
    }

    return uniqueElements;

}

//для std::complex
template <typename T>
bool operator<(const std::complex<T>& lhs, const std::complex<T>& rhs) {
    return std::norm(lhs) < std::norm(rhs); // Сравнение по модулю
}

template <typename T>
bool operator>(const std::complex<T>& lhs, const std::complex<T>& rhs) {
    return std::norm(lhs) > std::norm(rhs); // Сравнение по модулю
}


int main() {
    setlocale(LC_ALL, "ru_RU");
    try {
        // Демонстрация работы с int
        {
            BinarySearchTree<int> bst;

            std::cout << "Inserting elements (int):" << std::endl;
            bst.insert(5);
            bst.insert(3);
            bst.insert(7);
            bst.insert(2);
            bst.insert(4);
            bst.insert(6);
            bst.insert(8);

            std::cout << "Our tree (int): ";
            bst.print();

            std::cout << "Contains 4: " << (bst.contains(4) ? "true" : "false") << std::endl;
            std::cout << "Contains 10: " << (bst.contains(10) ? "true" : "false") << std::endl;

            std::vector<int> input = {3, 2, 2, 4, 2};
            std::cout << "Input vector (int): ";
            for (int x : input) std::cout << x << " ";
            std::cout << std::endl;

            auto uniqueElements = getUniqueElements(input);
            std::cout << "Unique elements (int): ";
            for (int x : uniqueElements) std::cout << x << " ";
            std::cout << std::endl;
        }

        // Демонстрация работы с std::complex<float>
        {
            BinarySearchTree<std::complex<float>> bst;

            std::cout << "\nInserting elements (std::complex<float>):" << std::endl;
            bst.insert({1.0f, 2.0f});
            bst.insert({3.0f, 4.0f});
            bst.insert({-1.0f, 0.0f});
            bst.insert({2.0f, 3.0f});

            std::cout << "Our tree (std::complex<float>): ";
            bst.print();

            std::cout << "Contains {1.0, 2.0}: " << (bst.contains({1.0f, 2.0f}) ? "true" : "false") << std::endl;
            std::cout << "Contains {5.0, 6.0}: " << (bst.contains({5.0f, 6.0f}) ? "true" : "false") << std::endl;

            std::vector<std::complex<float>> input = {{1.0f, 2.0f}, {3.0f, 4.0f}, {1.0f, 2.0f}};
            std::cout << "Input vector (std::complex<float>): ";
            for (const auto& x : input) std::cout << "(" << x.real() << ", " << x.imag() << ") ";
            std::cout << std::endl;

            auto uniqueElements = getUniqueElements(input);
            std::cout << "Unique elements (std::complex<float>): ";
            for (const auto& x : uniqueElements) std::cout << "(" << x.real() << ", " << x.imag() << ") ";
            std::cout << std::endl;
        }

        // Демонстрация работы с std::string
        {
            BinarySearchTree<std::string> bst;

            std::cout << "\nInserting elements (std::string):" << std::endl;
            bst.insert("apple");
            bst.insert("banana");
            bst.insert("cherry");
            bst.insert("date");

            std::cout << "Our tree (std::string): ";
            bst.print();

            std::cout << "Contains 'banana': " << (bst.contains("banana") ? "true" : "false") << std::endl;
            std::cout << "Contains 'grape': " << (bst.contains("grape") ? "true" : "false") << std::endl;

            std::vector<std::string> input = {"apple", "banana", "apple", "cherry"};
            std::cout << "Input vector (std::string): ";
            for (const auto& x : input) std::cout << x << " ";
            std::cout << std::endl;

            auto uniqueElements = getUniqueElements(input);
            std::cout << "Unique elements (std::string): ";
            for (const auto& x : uniqueElements) std::cout << x << " ";
            std::cout << std::endl;
        }

    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}