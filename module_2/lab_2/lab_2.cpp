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

        // Метод умножения и сдвига для хеширования строк
        size_t compute_hash(const std::string& key) const {
            const int word_size = 32;
            const int shift_bits = log2(capacity);
            const uint64_t multiplier = 2654435761;
            
            uint64_t hash_value = 0;
            
            // Хеширование строки посимвольно
            for (char ch : key) {
                hash_value = (hash_value * multiplier) + static_cast<uint64_t>(ch);
            }
            
            uint32_t final_hash = static_cast<uint32_t>(hash_value >> (word_size - shift_bits));
            return final_hash % capacity;
        }


    public:
        // Конструктор пустой таблицы заданного размера
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

        // Конструктор с заполнением случайными значениями
        RomanNumeralHashTable(size_t initial_size, bool fill_random) : capacity(initial_size), element_count(0) {
            if (initial_size == 0) throw std::invalid_argument("Size cannot be zero");
            entries = new TableEntry[capacity];
            
            if (fill_random) {
                srand(time(nullptr)); // Инициализация генератора случайных чисел
                for (size_t i = 0; i < capacity; ++i) {
                    std::string roman = generate_random_roman();
                    int value = generate_random_value();
                    insert_element(roman, value);
                }
            }
        }

        // Генератор случайных римских чисел
        std::string generate_random_roman() const {
            static const std::string romans[] = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
            return romans[rand() % (sizeof(romans)/sizeof(romans[0]))];
        }

        // Генератор случайных чисел для значений
        int generate_random_value() const {
            static const int values[] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
            return values[rand() % (sizeof(values)/sizeof(values[0]))];
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