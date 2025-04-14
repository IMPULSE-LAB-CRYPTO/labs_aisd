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
        
        // Рехеширование таблицы
        void rehash_table() {
            size_t old_capacity = capacity;
            TableEntry* old_entries = entries;
            
            capacity *= 2;
            entries = new TableEntry[capacity];
            size_t old_element_count = element_count;
            element_count = 0;

            for (size_t i = 0; i < old_capacity; ++i) {
                if (old_entries[i].is_occupied && !old_entries[i].is_deleted) {
                    insert_element(old_entries[i].key, old_entries[i].value);
                }
            }
            
            delete[] old_entries;
            
            // Восстанавливаем точное количество элементов
            element_count = old_element_count;
        }

    public:
        // Конструкторы
        RomanNumeralHashTable() : capacity(INITIAL_CAPACITY), element_count(0) {
            entries = new TableEntry[capacity];
            initialize_roman_numerals();
        }
        

        void initialize_roman_numerals() {
            insert_element("I", 1);
            insert_element("IV", 4);
            insert_element("V", 5);
            insert_element("IX", 9);
            insert_element("X", 10);
            insert_element("XL", 40);
            insert_element("L", 50);
            insert_element("XC", 90);
            insert_element("C", 100);
            insert_element("CD", 400);
            insert_element("D", 500);
            insert_element("CM", 900);
            insert_element("M", 1000);
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


        // Вставка элемента
        bool insert_element(const std::string& key, int value) {
            if (element_count >= capacity * MAX_LOAD_FACTOR) {
                rehash_table();
            }

            size_t index = compute_hash(key);
            int attempt = 0;

            while (attempt < static_cast<int>(capacity)) {
                size_t current = get_probed_index(index, attempt);
                
                if (!entries[current].is_occupied || entries[current].is_deleted) {
                    entries[current].key = key;
                    entries[current].value = value;
                    entries[current].is_occupied = true;
                    entries[current].is_deleted = false;
                    element_count++;
                    return true;
                }
                if (entries[current].key == key && !entries[current].is_deleted) {
                    return false;
                }
                attempt++;
            }
            return false;
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