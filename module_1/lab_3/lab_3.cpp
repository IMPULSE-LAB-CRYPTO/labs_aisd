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
            T temp = arr[min_index];
            arr[min_index] = arr[i];
            arr[i] = temp;
            //std::swap(arr[i], arr[min_index]);
            s.copy_count += 3; // Замена подразумевает 3 копирования
        }
    }
    return s;
}


// Сортировка Шелла (от O(nlogn) до O(n^2) в худшем случае)
template <typename T>
stats shellSort(vector<T>& arr)
{
    stats s;
    size_t n = arr.size();
	for(size_t step = n/2; step > 0; step /= 2)
	{
		for (size_t i = step; i < n; i += 1)
        {       
			size_t j = i;
            s.comparison_count++;
			while(j >= step && arr[j - step] > arr[i])
			{   
                s.comparison_count++;
                //swap(list[j], list[j - step]);
                T temp = arr[i];
                arr[j] = arr[j - step];
                arr[j-step] = temp;
				j-=step;
                s.copy_count+=3;
			}
        }
	}
    return s;
}


// Пирамидальная сортировка ( O(n log n))
template<typename T>
void heapify(vector<T>& arr, size_t n, size_t i, stats& s) {
    size_t largest = i;
    T temp;
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
        //std::swap(arr[i], arr[largest]);
        temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        s.copy_count += 3;
        heapify(arr, n, largest, s);
    }
}


template<typename T>
stats heap_sort(vector<T>& arr) {
    
    stats s;
    size_t n = arr.size();
    T temp;
    for (size_t i = n / 2 - 1; i != SIZE_MAX; i--) {
        heapify(arr, n, i, s);
    }
    for (size_t i = n - 1; i != SIZE_MAX; i--) {
        //std::swap(arr[0], arr[i]);
        temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
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


// Функция для генерации отсортированного массива
vector<int> generate_sorted_vector(size_t size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = i + 1;
    }
    return arr;
}


// Функция для генерации обратно отсортированного массива
vector<int> generate_reverse_sorted_vector(size_t size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = size - i;
    }
    return arr;
}



int main() {
    const size_t sizes_selectionsort[] = {1000, 2000, 3000, 4000, 5000};
    const size_t sizes[] = {1000, 2000, 3000, 4000, 5000, 25000, 100000};
    const int num_samples = 100;
    unsigned seed = 42;
/*
    cout << "\nFor Selection_sort:\n";
    for (size_t size : sizes_selectionsort) {
        size_t total_comparisons = 0;
        size_t total_copies = 0;
        
        vector<int> arr_sort = generate_sorted_vector(size);
        stats s_sorted = selection_sort(arr_sort);
        size_t total_comparisons_sort = s_sorted.comparison_count;
        size_t total_copies_sort = s_sorted.copy_count;

        vector<int> arr_reverse_sorted = generate_reverse_sorted_vector(size);
        stats s_reverse_sorted = selection_sort(arr_reverse_sorted);
        size_t total_comparisons_reverse_sort = s_reverse_sorted.comparison_count;
        size_t total_copies_reverse_sort = s_reverse_sorted.copy_count;

        for (int sample = 0; sample < num_samples; ++sample) {
            vector<int> arr = generate_random_vector(size, seed++);
            stats s = selection_sort(arr);
            total_comparisons += s.comparison_count;
            total_copies += s.copy_count;
        }

        cout << "Average for size " << size << ": "
                  << "Comparisons = " << total_comparisons / num_samples
                  << ", Copies = " << total_copies / num_samples << endl;

        cout << "Sorted_vector average for size " << size << ": "
             << "Comparisons = " << total_comparisons
             << ", Copies = " << total_copies<< endl;
        
        cout << "Reverse_sorted average for size " << size << ": "
                  << "Comparisons = " << total_comparisons
                  << ", Copies = " << total_copies<< endl;
    };
*/
    
    cout << "\nFor Selection_sort:\n";
    for (size_t size : sizes_selectionsort) {
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
        
    };


    cout << "\nFor Selection_sort (sorted_vector):\n";
    for (size_t size : sizes_selectionsort) {
        size_t total_comparisons = 0;
        size_t total_copies = 0;

        vector<int> arr = generate_sorted_vector(size);
        stats s = selection_sort(arr);
        total_comparisons += s.comparison_count;
        total_copies += s.copy_count;

        cout << "Average for size " << size << ": "
             << "Comparisons = " << total_comparisons
             << ", Copies = " << total_copies<< endl;
    };


    cout << "\nFor Selection_sort (reverse_sorted_vector):\n";
    for (size_t size : sizes_selectionsort) {
        size_t total_comparisons = 0;
        size_t total_copies = 0;

        vector<int> arr = generate_reverse_sorted_vector(size);
        stats s = selection_sort(arr);
        total_comparisons += s.comparison_count;
        total_copies += s.copy_count;

        cout << "Average for size " << size << ": "
                  << "Comparisons = " << total_comparisons
                  << ", Copies = " << total_copies<< endl;
    };

    cout << "\n______________________________________________________________________________________________\n";
    cout << "\nNext Shell_sort:\n";
    for (size_t size : sizes) {
        size_t total_comparisons = 0;
        size_t total_copies = 0;

        for (int sample = 0; sample < num_samples; ++sample) {
            vector<int> arr = generate_random_vector(size, seed++);
            stats s = shellSort(arr);
            total_comparisons += s.comparison_count;
            total_copies += s.copy_count;
        }

        cout << "Average for size " << size << ": "
                  << "Comparisons = " << total_comparisons / num_samples
                  << ", Copies = " << total_copies / num_samples << endl;
    }


    cout << "\nFor Shell_sort (sorted_vector):\n";
    for (size_t size : sizes) {
        size_t total_comparisons = 0;
        size_t total_copies = 0;

        vector<int> arr = generate_sorted_vector(size);
        stats s = shellSort(arr);
        total_comparisons += s.comparison_count;
        total_copies += s.copy_count;

        cout << "Average for size " << size << ": "
             << "Comparisons = " << total_comparisons
             << ", Copies = " << total_copies<< endl;
    };


    cout << "\nFor Shell_sort (reverse_sorted_vector):\n";
    for (size_t size : sizes) {
        size_t total_comparisons = 0;
        size_t total_copies = 0;

        vector<int> arr = generate_reverse_sorted_vector(size);
        stats s = shellSort(arr);
        total_comparisons += s.comparison_count;
        total_copies += s.copy_count;

        cout << "Average for size " << size << ": "
                  << "Comparisons = " << total_comparisons
                  << ", Copies = " << total_copies<< endl;
    };

    cout << "\n______________________________________________________________________________________________\n";
    cout << "\nNext Heap_sort:\n";

    for (size_t size : sizes) {
        size_t total_comparisons = 0;
        size_t total_copies = 0;

        for (int sample = 0; sample < num_samples; ++sample) {
            vector<int> arr = generate_random_vector(size, seed++);
            stats s = heap_sort(arr);
            total_comparisons += s.comparison_count;
            total_copies += s.copy_count;
        }

        cout << "Average for size " << size << ": "
                  << "Comparisons = " << total_comparisons / num_samples
                  << ", Copies = " << total_copies / num_samples << endl;
    }   

    cout << "\nFor heap_sort (sorted_vector):\n";
    for (size_t size : sizes) {
        size_t total_comparisons = 0;
        size_t total_copies = 0;

        vector<int> arr = generate_sorted_vector(size);
        stats s = heap_sort(arr);
        total_comparisons += s.comparison_count;
        total_copies += s.copy_count;

        cout << "Average for size " << size << ": "
             << "Comparisons = " << total_comparisons
             << ", Copies = " << total_copies<< endl;
    };


    cout << "\nFor heap_sort (reverse_sorted_vector):\n";
    for (size_t size : sizes) {
        size_t total_comparisons = 0;
        size_t total_copies = 0;

        vector<int> arr = generate_reverse_sorted_vector(size);
        stats s = heap_sort(arr);
        total_comparisons += s.comparison_count;
        total_copies += s.copy_count;

        cout << "Average for size " << size << ": "
                  << "Comparisons = " << total_comparisons
                  << ", Copies = " << total_copies<< endl;       
    };
     
    return 0;
}