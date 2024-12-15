#include <iostream>
#include <vector>
#include <random>

struct stats {
    size_t comparison_count = 0;
    size_t copy_count = 0;
};


// Сортировка выбором
stats selection_sort(std::vector<int>& arr) {
    stats s;
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        size_t min_index = i;
        for (size_t j = i + 1; j < n; ++j) {
            s.comparison_count++;
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            std::swap(arr[i], arr[min_index]);
            s.copy_count += 3; // Замена подразумевает 3 копирования
        }
    }
    return s;
}

// Сортировка Шелла
stats shell_sort(std::vector<int>& arr) {
    stats s;
    size_t n = arr.size();
    for (size_t gap = n / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < n; ++i) {
            int temp = arr[i];
            s.copy_count++; // Копирование во временную переменную
            size_t j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                s.comparison_count++;
                arr[j] = arr[j - gap];
                s.copy_count++;
            }
            arr[j] = temp;
            s.copy_count++; // Копирование обратно в массив
        }
    }
    return s;
}

// Пирамидальная сортировка