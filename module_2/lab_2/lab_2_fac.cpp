#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>

class RomanNumeralHashTable {
private:
    struct TableEntry {
        std::string key;
        int value;
        bool is_occupied;
        bool is_deleted;

        TableEntry() : key(""), value(0), is_occupied(false), is_deleted(false) {}
    };

    static const size_t INITIAL_CAPACITY = 16;
    static const double MAX_LOAD_FACTOR;
    
    TableEntry* entries;
    size_t capacity;
    size_t element_count;
    
    // Метод многократного сдвига
    size_t multiply_shift_hash(const std::string& key) const {
        const int w = 32; // Размер машинного слова
        const int l = static_cast<int>(log2(capacity));
        uint64_t a = 2654435761; // Константа из golden ratio
        
        uint64_t hash = 0;
        for (char ch : key) {
            hash = (hash * a + static_cast<uint64_t>(ch)) % (1ULL << w);
        }
        
        return (hash >> (w - l)) % capacity;
    }

    // Хеш-функция для римских чисел
    size_t roman_numeral_hash(const std::string& roman) const {
        int value = 0;
        for (size_t i = 0; i < roman.size(); ++i) {
            int current = roman_char_to_value(roman[i]);
            if (i + 1 < roman.size()) {
                int next = roman_char_to_value(roman[i+1]);
                if (current < next) {
                    value -= current;
                } else {
                    value += current;
                }
            } else {
                value += current;
            }
        }
        return static_cast<size_t>(value);
    }

    int roman_char_to_value(char c) const {
        switch (c) {
            case 'I': return 1;
            case 'V': return 5;
            case 'X': return 10;
            case 'L': return 50;
            case 'C': return 100;
            case 'D': return 500;
            case 'M': return 1000;
            default: return 0;
        }
    }

    // Линейное пробирование
    size_t linear_probing(size_t hash, size_t attempt) const {
        return (hash + attempt) % capacity;
    }

    void rehash() {
        size_t old_capacity = capacity;
        TableEntry* old_entries = entries;
        
        capacity *= 2;
        entries = new TableEntry[capacity];
        element_count = 0;

        for (size_t i = 0; i < old_capacity; ++i) {
            if (old_entries[i].is_occupied && !old_entries[i].is_deleted) {
                insert(old_entries[i].key, old_entries[i].value);
            }
        }
        
        delete[] old_entries;
    }

    
public:
    // 1. Конструктор пустой хэш таблицы
    RomanNumeralHashTable(size_t initial_size = INITIAL_CAPACITY) 
        : capacity(initial_size), element_count(0) {
        if (initial_size == 0) throw std::invalid_argument("Size cannot be zero");
        entries = new TableEntry[capacity];
    }

    // 2. Конструктор со случайными значениями
    RomanNumeralHashTable(size_t initial_size, bool random_fill) 
        : capacity(initial_size), element_count(0) {
        if (initial_size == 0) throw std::invalid_argument("Size cannot be zero");
        entries = new TableEntry[capacity];
        
        if (random_fill) {
            const std::string romans[] = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
            const int values[] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
            const size_t count = sizeof(values) / sizeof(values[0]);
            
            size_t elements_to_add = static_cast<size_t>(capacity * MAX_LOAD_FACTOR);
            for (size_t i = 0; i < elements_to_add; ++i) {
                size_t index = i % count;
                insert(romans[index], values[index]);
            }
        }
    }

    // 3. Конструктор копирования
    RomanNumeralHashTable(const RomanNumeralHashTable& other) 
        : capacity(other.capacity), element_count(other.element_count) {
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

    // 6. Печать содержимого
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
    bool insert(const std::string& key, int value) {
        if (element_count >= capacity * MAX_LOAD_FACTOR) {
            rehash();
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
        
        //rehash();
        //assign(key, value);
    }

    // 9. Проверка наличия элемента
    bool contains(int value) const {
        for (size_t i = 0; i < capacity; ++i) {
            if (entries[i].is_occupied && !entries[i].is_deleted && 
                entries[i].value == value) {
                return true;
            }
            //else if (entries[i].)
        }
        return false;
    }

    //Вариант поиска
    int* search(const std::string& key) {
        size_t hash = multiply_shift_hash(key);
        
        // Ищем "вверх" от исходного хэша
        for(size_t i = hash; i < capacity; ++i) {
            if(!entries[i].is_occupied && !entries[i].is_deleted) break;
            if(entries[i].is_occupied && !entries[i].is_deleted && 
               entries[i].key == key) {
                return &entries[i].value;
            }
        }
        
        // Ищем "вниз" от исходного хэша
        for(size_t i = hash; i-- > 0; ) {
            if(!entries[i].is_occupied && !entries[i].is_deleted) break;
            if(entries[i].is_occupied && !entries[i].is_deleted && 
               entries[i].key == key) {
                return &entries[i].value;
            }
        }
        
        return nullptr;
    }

    // 11. Удаление элемента
    bool erase(const std::string& key) {
        size_t hash = multiply_shift_hash(key);
        size_t attempt = 0;

        while (attempt < capacity) {
            size_t index = linear_probing(hash, attempt);
            
            if (!entries[index].is_occupied && !entries[index].is_deleted) {
                return false;
            }
            
            if (entries[index].is_occupied && !entries[index].is_deleted && 
                entries[index].key == key) {
                entries[index].value = 0;
                entries[index].is_deleted = true;
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

        // Специальный метод для перевода римских чисел
        int roman_to_decimal(const std::string& roman) const {
            size_t hash = roman_numeral_hash(roman);
            size_t attempt = 0;

            while (attempt < capacity) {
                size_t index = linear_probing(hash, attempt);
                
                if (!entries[index].is_occupied && !entries[index].is_deleted) {
                    break;
                }
                
                if (entries[index].is_occupied && !entries[index].is_deleted && 
                    entries[index].key == roman) {
                    return entries[index].value;
                }
                
                attempt++;
            }

            // Если не нашли в таблице, вычисляем напрямую
            int result = 0;
            for (size_t i = 0; i < roman.size(); ++i) {
                int current = roman_char_to_value(roman[i]);
                if (i + 1 < roman.size()) {
                    int next = roman_char_to_value(roman[i+1]);
                    if (current < next) {
                        result -= current;
                    } else {
                        result += current;
                    }
                } else {
                    result += current;
                }
            }
            
            return result;
        }
};

const double RomanNumeralHashTable::MAX_LOAD_FACTOR = 0.7;

int main() {
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
        table.insert("I", 1);
        table.insert("V", 5);
        table.insert("X", 10);
        table.insert("L", 50);
        table.insert("C", 100);
        table.insert("D", 500);
        table.insert("M", 1000);
        
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
        std::cout << "Erase 'L': " << (table.erase("L") ? "Success" : "Failed") << std::endl;
        table.print();
        
        // Вставка или присвоение
        std::cout << "\nInsert or assign 'V' to 6:" << std::endl;
        table.assign("V", 6);
        table.print();
        
        // Проверка наличия
        std::cout << "\nContains value 100: " << (table.contains(100) ? "Yes" : "No") << std::endl;
        
        // Подсчет коллизий
        std::cout << "Count elements with same hash as 'I': " << table.count("I") << std::endl;
        
        // 3. Решение задачи - перевод римских чисел
        std::cout << "\n=== Roman to Decimal Conversion ===" << std::endl;
        std::string roman;
        std::cout << "Enter a Roman numeral: ";
        std::cin >> roman;
        
        int decimal = table.roman_to_decimal(roman);
        std::cout << roman << " = " << decimal << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}