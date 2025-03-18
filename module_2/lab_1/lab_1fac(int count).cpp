#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>


using namespace std;


class BinarySearchTree {
private:
    struct Node {
        int key;
        int count;
        Node* left;
        Node* right;

        Node(int k) : key(k), count(1), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* copyTree(Node* node) {
        if (node == nullptr) return nullptr;
        Node* newNode = new Node(node->key);
        newNode->count = node->count;
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

    Node* insertNode(Node* node, int key) {
        if (node == nullptr) {
            return new Node(key);
        }
        if (key < node->key) {
            node->left = insertNode(node->left, key);
        } else if (key > node->key) {
            node->right = insertNode(node->right, key);
        } else {
            node->count++; 
        }
        return node;
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

    int getCount(Node* node, int key) const {
        if (node == nullptr) return 0;
        if (key < node->key) {
            return getCount(node->left, key);
        } else if (key > node->key) {
            return getCount(node->right, key);
        }
        return node->count;
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

    void insert(int key) {
        root = insertNode(root, key);
    }

    bool contains(int key) const {
        return containsNode(root, key);
    }

    bool erase(int key) {
        return eraseNode(root, key);
    }

    int getCount(int key) const {
        return getCount(root, key);
    }


};


// Функция для получения уникальных элементов
std::vector<int> getUniqueElements(const std::vector<int>& input) {
    BinarySearchTree bst;
    std::vector<int> uniqueElements;

    for (int key : input) {
        bst.insert(key);
    }

    for (int key : input) {
        if (bst.getCount(key) == 1) {
            uniqueElements.push_back(key);
        }
    }

    return uniqueElements;
}


int main() {
    setlocale(LC_ALL, "ru_RU");
    try {
        //Инициализируем дерво
        BinarySearchTree bst;

        //Заполняем древо элементами
        std::cout << "Inserting elements" << std::endl;
        bst.insert(3);
        bst.insert(2);
        bst.insert(2);
        bst.insert(4);
        bst.insert(2);


        std::vector<int> input = {3, 2, 2, 4, 2};
        std::cout << "Input vector: ";
        for (int x : input) std::cout << x << " ";
        std::cout << std::endl;

        int c = bst.getCount(2);
        std::cout << "\nCount of 2: " << c << " ";
    
        std::vector<int> uniqueElements = getUniqueElements(input);
    
        std::cout << "\nUnique elements: ";
        for (int x : uniqueElements) std::cout << x << " ";
        std::cout << std::endl;
    }

    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}