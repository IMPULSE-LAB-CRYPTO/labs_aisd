#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <cstdlib> // Для rand() и srand()
#include <ctime>   // Для time()

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
        static const int MAX_LOAD_FACTOR = 0.7;
        
        TableEntry* entries;
        size_t capacity;
        size_t element_count;


        // Рехеширование таблицы
        void rehash_table() {
            size_t old_capacity = capacity;
            TableEntry* old_entries = entries;
            
            capacity *= 2;
            entries = new TableEntry[capacity];
            element_count = 0;

            for (size_t i = 0; i < old_capacity; ++i) {
                if (old_entries[i].is_occupied && !old_entries[i].is_deleted) {
                    insert_element(old_entries[i].key, old_entries[i].value);
                }
            }
            
            delete[] old_entries;
        }   

        // Линейное пробирование
        size_t linear_probing(size_t hash, int attempt) const {
            return (hash + attempt) % capacity;
        }

        // Метод умножения и сдвига для хеширования строк
        size_t multiply_shift_hash(const std::string& key) const {
            const int word_size = 32; // Размер машинного слова
            const int shift_bits = static_cast<int>(log2(capacity));
            const uint64_t multiplier = 2654435761; // Константа из golden ratio
            
            uint64_t hash_value = 0;
            
            // Хеширование строки посимвольно
            for (char ch : key) {
                hash_value = (hash_value * multiplier) + static_cast<uint64_t>(ch);
            }
            
            uint32_t final_hash = static_cast<uint32_t>(hash_value >> (word_size - shift_bits));
            return final_hash % capacity;
        }

        

    public:
        // 1. Конструктор пустой таблицы заданного размера
        RomanNumeralHashTable(size_t initial_size) : capacity(initial_size), element_count(0) {
            if (initial_size == 0) throw std::invalid_argument("Size cannot be zero");
            entries = new TableEntry[capacity];
        }

        // Конструктор с базовыми числами
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

        // 2. Конструктор со случайными значениями
        RomanNumeralHashTable(size_t initial_size, bool random_fill) : capacity(initial_size), element_count(0) {
            if (initial_size == 0) throw std::invalid_argument("Size cannot be zero");
            entries = new TableEntry[capacity];
            
            if (random_fill) {
                const std::string romans[] = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
                const int values[] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
                const size_t count = sizeof(values) / sizeof(values[0]);
                
                size_t elements_to_add = static_cast<size_t>(capacity * MAX_LOAD_FACTOR);
                for (size_t i = 0; i < elements_to_add; ++i) {
                    size_t index = i % count;
                    insert_element(romans[index], values[index]);
                }
            }
        }
    
        // 3. Конструтор копирования
        RomanNumeralHashTable(const RomanNumeralHashTable& other) : capacity(other.capacity), element_count(other.element_count) {
            entries = new TableEntry[capacity];
            for (size_t i = 0; i < capacity; ++i) {
                entries[i] = other.entries[i];
            }
        }   

        // 4. Деструктор
        ~RomanNumeralHashTable() {
            delete[] entries;
        }

        // 5. Оператор присваивания
        RomanNumeralHashTable& operator=(const RomanNumeralHashTable& other) {
            if (this != &other) {
                delete[] entries;
                
                capacity = other.capacity;
                element_count = other.element_count;
                entries = new TableEntry[capacity];
                
                for (size_t i = 0; i < capacity; ++i) {
                    entries[i] = other.entries[i];
                }
            }
            return *this;
        }

        // 6. Вывод содержимого таблицы
        void print() const {
            std::cout << "Hash Table (capacity: " << capacity 
                      << ", size: " << element_count << ")\n";
            for (size_t i = 0; i < capacity; ++i) {
                if (entries[i].is_occupied && !entries[i].is_deleted) {
                    std::cout << "[" << i << "]: " << entries[i].key 
                              << " => " << entries[i].value << "\n";
                }
            }
        }

        // 7. Вставка значения по ключу
        bool insert_element(const std::string& key, int value) {
            if (element_count >= capacity * MAX_LOAD_FACTOR) {
                rehash_table();
            }

            size_t hash = multiply_shift_hash(key);
            size_t attempt = 0;

            while (attempt < capacity) {
                size_t index = linear_probing(hash, attempt);
                
                if (!entries[index].is_occupied || entries[index].is_deleted) {
                    entries[index].key = key;
                    entries[index].value = value;
                    entries[index].is_occupied = true;
                    entries[index].is_deleted = false;
                    element_count++;
                    return true;
                }
                
                if (entries[index].key == key && !entries[index].is_deleted) {
                    return false; // Ключ уже существует
                }
                
                attempt++;
            }
            
            return false;
        }


        // 8. Присвоение
        void assign(const std::string& key, int value) {
            size_t hash = multiply_shift_hash(key);
            size_t attempt = 0;

            while (attempt < capacity) {
                size_t index = linear_probing(hash, attempt);
                
                if (!entries[index].is_occupied || entries[index].is_deleted) {
                    entries[index].key = key;
                    entries[index].value = value;
                    entries[index].is_occupied = true;
                    entries[index].is_deleted = false;
                    element_count++;
                    return;
                }
                
                if (entries[index].key == key) {
                    entries[index].value = value;
                    return;
                }
                
                attempt++;
            }
            
            rehash_table();
            assign(key, value);
        }

        // 9. Проверка наличия элемента
        bool contains(int value) const {
            for (size_t i = 0; i < capacity; ++i) {
                if (entries[i].is_occupied && !entries[i].is_deleted && 
                    entries[i].value == value) {
                    return true;
                }
            }
            return false;
        }

        // 10. Поиск элемента
        int* search(const std::string& key) {
            size_t hash = multiply_shift_hash(key);
            int attempt = 0;

            while (attempt < static_cast<int>(capacity)) {
                size_t current = linear_probing(hash, attempt);
                
                if (!entries[current].is_occupied && !entries[current].is_deleted) {
                    return nullptr;
                }
                
                if (entries[current].is_occupied && !entries[current].is_deleted && 
                    entries[current].key == key) {
                    return &entries[current].value;
                }
                
                attempt++;
            }

            return nullptr;
        }
    
        // 11. Удаление элемента
        bool remove_element(const std::string& key) {
            size_t hash = multiply_shift_hash(key);
            int attempt = 0;

            while (attempt < static_cast<int>(capacity)) {
                size_t current = linear_probing(hash, attempt);
                
                if (!entries[current].is_occupied && !entries[current].is_deleted) {
                    return false;
                }
                
                if (entries[current].is_occupied && !entries[current].is_deleted && 
                    entries[current].key == key) {
                    entries[current].is_deleted = true;
                    element_count--;
                    return true;
                }
                
                attempt++;
            }

            return false;
        }
        

        // 12. Количество элементов с совпадающим хешем
        int count(const std::string& key) const {
            size_t hash = multiply_shift_hash(key);
            int result = 0;

            for (size_t i = 0; i < capacity; ++i) {
                if (entries[i].is_occupied && !entries[i].is_deleted) {
                    size_t current_hash = multiply_shift_hash(entries[i].key);
                    if (current_hash == hash) {
                        result++;
                    }
                }
            }
            
            return result;
        }


};


int main() {
    setlocale(LC_ALL, "ru_RU");
    
    try {
        // 1. Демонстрация конструкторов
        std::cout << "=== Empty table constructor ===" << std::endl;
        RomanNumeralHashTable emptyTable(8);
        emptyTable.print();
        
        std::cout << "\n=== Random fill constructor ===" << std::endl;
        RomanNumeralHashTable randomTable(10, true);
        randomTable.print();
        
        // 2. Демонстрация операций
        std::cout << "\n=== Operations demonstration ===" << std::endl;
        RomanNumeralHashTable table;
        
        // Вставка элементов
        table.insert_element("I", 1);
        table.insert_element("V", 5);
        table.insert_element("X", 10);
        table.insert_element("L", 50);
        table.insert_element("C", 100);
        table.insert_element("D", 500);
        table.insert_element("M", 1000);
        
        // Печать
        table.print();
        
        // Поиск
        std::cout << "\nSearch 'X': ";
        int* value = table.search("X");
        if (value) {
            std::cout << *value << std::endl;
        } else {
            std::cout << "Not found" << std::endl;
        }
        
        // Удаление
        std::cout << "Erase 'L': " << (table.remove_element("L") ? "Success" : "Failed") << std::endl;
        table.print();
        
        // Присвоение
        std::cout << "\nInsert or assign 'V' to 6:" << std::endl;
        table.assign("V", 6);
        table.print();
        
        // Проверка наличия
        std::cout << "\nContains value 100: " << (table.contains(100) ? "Yes" : "No") << std::endl;
        
        // Подсчет коллизий
        std::cout << "Count elements with same hash as 'I': " << table.count("I") << std::endl;

    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}