#include <iostream>
#include <stdexcept>

using namespace std;

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