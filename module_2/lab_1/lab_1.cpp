#include <iostream>
#include <stdexcept>
#include <vector>


using namespace std;


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

    void print() const {
        printTree(root);
        std::cout << std::endl;
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



    

};

int main() {
    setlocale(LC_ALL, "ru_RU");
    try {
        cout << "Matrix TestRandom:" << endl;
    }

    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}