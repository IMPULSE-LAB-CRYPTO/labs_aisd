#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <functional>

template <typename KeyType, typename ValueType>
class RomanNumeralHashTable {
private:
    struct TableEntry {
        KeyType key;
        ValueType value;
        bool is_occupied;
        bool is_deleted;

        TableEntry() : key(), value(), is_occupied(false), is_deleted(false) {}
    };

    static const size_t INITIAL_CAPACITY = 16;
    static const double MAX_LOAD_FACTOR;
    
    TableEntry* entries;
    size_t capacity;
    size_t element_count;
    std::function<size_t(const KeyType&)> hash_function;
    
    // Метод многократного сдвига для строк
    size_t multiply_shift_hash(const std::string& key) const {
        const int w = 32;
        const int l = static_cast<int>(log2(capacity));
        uint64_t a = 2654435761;
        
        uint64_t hash = 0;
        for (char ch : key) {
            hash = (hash * a + static_cast<uint64_t>(ch)) % (1ULL << w);
        }
        
        return (hash >> (w - l)) % capacity;
    }

    // Хеш-функция по умолчанию
    size_t default_hash(const KeyType& key) const {
        std::hash<KeyType> hasher;
        return hasher(key) % capacity;
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
    RomanNumeralHashTable(size_t initial_size = INITIAL_CAPACITY, std::function<size_t(const KeyType&)> hash_func = nullptr) 
        : capacity(initial_size), element_count(0) {
        if (initial_size == 0) throw std::invalid_argument("Size cannot be zero");
        entries = new TableEntry[capacity];

        if (hash_func) {
        hash_function = hash_func;
        } 
        else {
        hash_function = [this](const KeyType& key) { return this->default_hash(key); };
        }
    }

    // 2. Конструктор со случайными значениями (только для std::string, int)
    template <typename K = KeyType, typename V = ValueType, typename std::enable_if<std::is_same<K, std::string>::value && std::is_same<V, int>::value, int>::type = 0>
    RomanNumeralHashTable(size_t initial_size, bool random_fill): capacity(initial_size), element_count(0) {
        if (initial_size == 0) throw std::invalid_argument("Size cannot be zero");
        entries = new TableEntry[capacity];
        
        // Всегда инициализируем хеш-функцию
        hash_function = [this](const std::string& key) { return this->multiply_shift_hash(key); };
        
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
        : capacity(other.capacity), element_count(other.element_count),
          hash_function(other.hash_function) {
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
            hash_function = other.hash_function;
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
    bool insert(const KeyType& key, const ValueType& value) {
        if (element_count >= capacity * MAX_LOAD_FACTOR) {
            rehash();
        }

        size_t hash = hash_function(key);
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
                return false;
            }
            
            attempt++;
        }
        
        return false;
    }

    // 8. Присвоение
    void assign(const KeyType& key, const ValueType& value) {
        size_t hash = hash_function(key);
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
        
        rehash();
        assign(key, value);
    }

    // 9. Проверка наличия элемента
    bool contains(const ValueType& value) const {
        for (size_t i = 0; i < capacity; ++i) {
            if (entries[i].is_occupied && !entries[i].is_deleted && 
                entries[i].value == value) {
                return true;
            }
            //else if (entries[i].)
        }
        return false;
    }

    // 10. Поиск элемента
    ValueType* search(const KeyType& key) {
        size_t hash = hash_function(key);
        
        for(size_t i = hash; i < capacity; ++i) {
            if(!entries[i].is_occupied && !entries[i].is_deleted) break;
            if(entries[i].is_occupied && !entries[i].is_deleted && 
               entries[i].key == key) {
                return &entries[i].value;
            }
        }
        
        for(size_t i = hash; i-- > 0; ) {
            if(!entries[i].is_occupied && !entries[i].is_deleted) break;
            if(entries[i].is_occupied && !entries[i].is_deleted && 
               entries[i].key == key) {
                return &entries[i].value;
            }
        }
        
        return nullptr;
    }

    // const-версия для поиска
    const ValueType* search(const KeyType& key) const {
        size_t hash = hash_function(key);
        
        for(size_t i = hash; i < capacity; ++i) {
            if(!entries[i].is_occupied && !entries[i].is_deleted) break;
            if(entries[i].is_occupied && !entries[i].is_deleted && 
               entries[i].key == key) {
                return &entries[i].value;
            }
        }
        
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
    bool erase(const KeyType& key) {
        size_t hash = hash_function(key);
        size_t attempt = 0;

        while (attempt < capacity) {
            size_t index = linear_probing(hash, attempt);
            
            if (!entries[index].is_occupied && !entries[index].is_deleted) {
                return false;
            }
            
            if (entries[index].is_occupied && !entries[index].is_deleted && 
                entries[index].key == key) {
                //entries[index].value = 0;
                entries[index].is_deleted = true;
                element_count--;
                return true;
            }
            
            attempt++;
        }

        return false;
    }

    // 12. Количество элементов с совпадающим хешем
    int count(const KeyType& key) const {
        size_t hash = hash_function(key);
        int result = 0;

        for (size_t i = 0; i < capacity; ++i) {
            if (entries[i].is_occupied && !entries[i].is_deleted) {
                size_t current_hash = hash_function(entries[i].key);
                if (current_hash == hash) {
                    result++;
                }
            }
        }
        
        return result;
    }

    // Специальный метод для перевода римских чисел (только для KeyType = std::string, ValueType = int)
    template <typename K = KeyType, typename V = ValueType>
    typename std::enable_if<std::is_same<K, std::string>::value && 
                           std::is_same<V, int>::value, int>::type
    roman_to_decimal(const std::string& roman) const {
        // Сначала попробуем найти в таблице
        const int* found = search(roman);
        if (found) {
            return *found;
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

template <typename KeyType, typename ValueType>
const double RomanNumeralHashTable<KeyType, ValueType>::MAX_LOAD_FACTOR = 0.7;

int main() {
    try {
        // 1. Демонстрация работы с римскими числами
        std::cout << "=== Roman numerals (original functionality) ===";
        RomanNumeralHashTable<std::string, int> romanTable(10, true);
        romanTable.print();
        
        std::string roman;
        std::cout << "Enter a Roman numeral: ";
        std::cin >> roman;
        std::cout << roman << " = " << romanTable.roman_to_decimal(roman);

        // 2. Демонстрация работы с другими типами
        std::cout << "\n=== Integer keys and string values ===";
        RomanNumeralHashTable<int, std::string> intStringTable;
        
        intStringTable.insert(1, "one");
        intStringTable.insert(2, "two");
        intStringTable.insert(3, "three");
        
        intStringTable.print();
        
        auto str = intStringTable.search(2);
        if (str) {
            std::cout << "Found string: " << *str;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what();
        return 1;
    }
    
    return 0;
}