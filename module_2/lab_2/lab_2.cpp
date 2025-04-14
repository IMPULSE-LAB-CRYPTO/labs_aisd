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
    
    public:
        // Конструкторы
        RomanNumeralHashTable() : capacity(INITIAL_CAPACITY), element_count(0) {
            entries = new TableEntry[capacity];
            initialize_roman_numerals();
        }
        
        // Деструктор
        ~RomanNumeralHashTable() {
            delete[] entries;
        }
    
        // Копирующий конструктор
        RomanNumeralHashTable(const RomanNumeralHashTable& other) 
            : capacity(other.capacity), element_count(other.element_count) {
            entries = new TableEntry[capacity];
            for (size_t i = 0; i < capacity; ++i) {
                entries[i] = other.entries[i];
            }
        }

        
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