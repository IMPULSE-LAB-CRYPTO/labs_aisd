#include <iostream>


class RomanNumeralHashTable {
    private:
        struct TableEntry {
            std::string key;
            int value;
            bool is_occupied;
            bool is_deleted;
    
            TableEntry() : key(""), value(0), is_occupied(false), is_deleted(false) {}
        };
    
        static const size_t INITIAL_CAPACITY = 16; //принадлежит классу в целом
        static const double MAX_LOAD_FACTOR;
        
        TableEntry* entries;
        size_t capacity;
        size_t element_count;
};




int main() {
    setlocale(LC_ALL, "ru_RU");
    try {

    }

    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}