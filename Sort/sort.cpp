#include <iostream>
#include <algorithm>
#include <iterator>
#include <ostream>
#include <random>
#include <chrono>

// Function to create and shuffle a raw array of integers from 1 to size
int* random_array(std::size_t size) {
    int* arr = new int[size];
    
    // Initialize the array with values 1 to size
    for (auto i = 0; i < size; ++i) {
        arr[i] = i + 1;
    }
    
    // Shuffle the array
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(arr, arr + size, g);
    
    return arr;
}

void print(int* arr, int size) {
    for (std::size_t i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

void bubble_sort(int* arr, int size, bool verbose) {
    for(int i = 0 ; i < size ; i++) {
        for(int j = 0 ; j < size-i-1 ; j++) {
            if (verbose) {
                for (int v = 0 ; v < j ; v++) std::cout << "  " ;
                std::cout << arr[j] << " " << arr[j+1] << std::endl;
                print(arr, size);
            }
            if (arr[j] > arr[j+1]) {
                int temp = arr[j+1];
                arr[j+1] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void insert_sort(int *arr, int size, bool verbose) {
    size_t current_size = 1;
    while (current_size < size) {
        for (size_t i = 0 ; i < current_size ; ++i) {
            if (arr[current_size] < arr[i]) {
                int value = arr[current_size];
                for (size_t j = current_size ; j > i ; --j) {
                    if (verbose) {
                        for (int v = 0 ; v < j ; v++) std::cout << "  " ;
                        std::cout << value << std::endl;
                        print(arr, size);
                    }
                    arr[j] = arr[j-1];
                }
                arr[i] = value;
                break;
            }
        }
        current_size++;
    }
}

int pivot(int *arr, int low, int high) {
    int current_pivot = low - 1;
    for (int i = low ; i < high ; i++) {
        if (arr[i] <= arr[high]) {
            // step pivot forward
            current_pivot++;
            // put every lower value in front of pivot
            int tmp = arr[current_pivot];
            arr[current_pivot] = arr[i];
            arr[i] = tmp;
        }
    }
    // put pivot in its legitimate place
    current_pivot++;
    int tmp = arr[current_pivot];
    arr[current_pivot] = arr[high];
    arr[high] = tmp;
    return current_pivot;
}

void quick_sort_subarray(int *arr, int low, int high) {
    if (low < high) {
        int current_pivot = pivot(arr, low, high);
        quick_sort_subarray(arr, low, current_pivot-1);
        quick_sort_subarray(arr, current_pivot+1, high);
    }
}

void quick_sort(int *arr, int size, bool verbose) {
    int current_pivot = size-1;
    current_pivot = pivot(arr, 0, current_pivot);
    quick_sort_subarray(arr, 0, current_pivot -1);
    quick_sort_subarray(arr, current_pivot + 1, size-1);
}

int* conquer(int* first_split, int* second_split, int first_split_size, int second_split_size) {
    int result[first_split_size+second_split_size]; 
    int first_split_index = 0, second_split_index = 0;
    int current_index = 0;
    // put lowest value in each of the split until one of those is empty
    while (first_split_index < first_split_size && second_split_index < second_split_size) {
        if (first_split[first_split_index] < second_split[second_split_index]) {
            result[current_index] = first_split[first_split_index];
            first_split_index++;
            current_index++;
        } else {
            result[current_index] = second_split[second_split_index];
            second_split_index++;
            current_index++;
        }
    }
    // one of the splits is not empty yet, find out which 
    int *remaining_split;
    int split_index, split_size;
    if (first_split_index < first_split_size) {
        remaining_split = first_split;
        split_index = first_split_index;
        split_size = first_split_size;
    } else {
        remaining_split = second_split;
        split_index = second_split_index;
        split_size = second_split_size;
    }
    // fill the result with remaining split
    for (int i = split_index ; i < split_size ; i++) {
        result[current_index] = remaining_split[i];
        current_index++;
    }
    // split pointers are neighbors in mem, fill first split -> fill arr
    for (int i = 0 ; i < first_split_size+second_split_size; i++) {
        first_split[i] = result[i];
    }
    return first_split;
}

int* divide(int* arr, int size) {
    if (size <= 1) {
        return arr;
    }
    int split_index = int(size/2);
    int *first_split = divide(arr, split_index);
    int *second_split = divide(arr+split_index, size-split_index);
    // merge
    return conquer(first_split, second_split, split_index, size-split_index);
}

void merge_sort(int *arr, int size, bool verbose) {
    int *divide_and_conquer = divide(arr, size);
}

int binary_search(int value, int *arr, int size) {
    int border[2] = {0, size-1};
    int pivot = int((border[1]+border[0])/2);
    while (arr[pivot] != value) {
        if (arr[pivot] < value) {
            border[0] = pivot; 
        } else {
            border[1] = pivot;
        }
        // not in array
        if (border[0] == border[1]) return -1;
        pivot = int((border[1]+border[0])/2);
    }
    return pivot;
}

int main() {
    std::size_t size;
    std::cout << "Enter the size of the array: ";
    std::cin >> size;
    
    // Init random array
    int* arr = random_array(size);
    print(arr, size);

    // Sort Array
    auto chrono_start = std::chrono::high_resolution_clock::now();
    merge_sort(arr, size, false);
    auto chrono_end = std::chrono::high_resolution_clock::now();

    // print time and sorted arra
    print(arr, size);
    std::cout << "Sort time : " << std::chrono::duration_cast<std::chrono::microseconds>(chrono_end-chrono_start).count() << std::endl;

    //binary search
    chrono_start = std::chrono::high_resolution_clock::now();
    int index = binary_search(7, arr, size);
    chrono_end = std::chrono::high_resolution_clock::now();

    // print search result and time
    std::cout << "search time : " << std::chrono::duration_cast<std::chrono::microseconds>(chrono_end-chrono_start).count() << std::endl;
    std::cout << "search result : " << index << std::endl;

    // Don't forget to delete the allocated array to avoid memory leak
    delete[] arr;
    
    return 0;
}
