#include <iostream>
#include <vector>
#include <random>

using namespace std;


struct stats {
    size_t comparison_count = 0;
    size_t copy_count = 0;
};


// Сортировка выбором ( Сложность: O(n^2) )
template <typename T>
stats selection_sort(vector<T>& arr) {
    stats s;
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; i++) {
        size_t min_index = i;
        for (size_t j = i + 1; j < n; j++) {
            s.comparison_count++;
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            T temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
            //std::swap(arr[i], arr[min_index]);
            s.copy_count += 3; // Замена подразумевает 3 копирования
        }
    }
    return s;
}

// Сортировка Шелла
template <typename T>
stats shellSort(vector<T>& arr) {
    stats s;
    size_t n = arr.size();
    size_t gap = n / 2;
    while (gap > 0) {
        for (size_t i = gap; i < n; i++) {
            T temp = arr[i];
            size_t j = i;
            while (j >= gap && arr[j - gap] > temp) {
                s.comparison_count++;
                arr[j] = arr[j - gap];
                s.copy_count++;     
                j -= gap;
            }  
            arr[j] = temp;
            s.copy_count++;
        }
        gap /= 2;
    }
    return s;
}

// Пирамидальная сортировка
template<typename T>
void heapify(vector<T>& arr, size_t n, size_t i, stats& s) {
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n) {
        s.comparison_count++;
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }
    if (right < n) {
        s.comparison_count++;
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        s.copy_count += 3;
        heapify(arr, n, largest, s);
    }
}

template<typename T>
stats heap_sort(vector<T>& arr) {
    stats s;
    size_t n = arr.size();

    for (size_t i = n / 2 - 1; i != SIZE_MAX; --i) {
        heapify(arr, n, i, s);
    }
    for (size_t i = n - 1; i != SIZE_MAX; --i) {
        std::swap(arr[0], arr[i]);
        s.copy_count += 3;
        heapify(arr, i, 0, s);
    }
    return s;
}



// Функция генерации случайных массивов
vector<int> generate_random_vector(size_t size, unsigned seed) {
    vector<int> vec(size);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(0, 10000);
    for (size_t i = 0; i < size; ++i) {
        vec[i] = dist(gen);
    }
    return vec;
}



int main() {
    const size_t sizes[] = {1000, 2000, 3000, 4000, 5000, 25000, 50000, 100000};
    const int num_samples = 100;
    unsigned seed = 42;

    for (size_t size : sizes) {
        size_t total_comparisons = 0;
        size_t total_copies = 0;

        for (int sample = 0; sample < num_samples; ++sample) {
            vector<int> arr = generate_random_vector(size, seed++);
            stats s = selection_sort(arr);
            total_comparisons += s.comparison_count;
            total_copies += s.copy_count;
        }

        cout << "Average for size " << size << ": "
                  << "Comparisons = " << total_comparisons / num_samples
                  << ", Copies = " << total_copies / num_samples << endl;
    }

    return 0;
}