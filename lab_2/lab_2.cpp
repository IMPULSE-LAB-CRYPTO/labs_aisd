#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <complex>
#include <random>


using namespace std;


int main() {
    setlocale(LC_ALL, "ru_RU");
    try {
        std::cout << "Hellddddddddddddddddddddddddddddddddddsecond";
    }

    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
