#include <assert.h>
#include <iostream>
#include <vector>
#include <algorithm>

/*******************************************************************************
 * UTILS
*******************************************************************************/

using sort_func_t = void(int[], int, int);
using sort_counting_func_t = void(int[], int, int, int, int);
using search_func_t = int(int[], int, int, int);

void swap(int& x, int& y) { int t = x; x = y; y = t; }

template <typename T>
void print(T arr[], int size) {
    for (int i = 0; i <= size; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

int max(int arr[], int l, int r) {
    int result = arr[0];
    for (int i = 1; i < r - l + 1; i++)
        if (arr[i] > result)
            result = arr[i];
    return result;
}

#define run_sort(sort_func)                             \
    do {                                                \
        int arr[] = {0,1,2,4,1,4,3,45,-3,44,3, 234};    \
        int size = sizeof(arr) / sizeof(arr[0]);        \
        sort_func(arr, 0, size-1);                      \
        print(arr, size-1);                             \
    } while (0);

#define run_count_sort(sort_func)                                       \
    do {                                                                \
        int arr[] = {1,2,3,6,3,10,3,1,5,-50,100, 200,101,102,11,6,-1, -100,234}; \
        int size = sizeof(arr) / sizeof(arr[0]);                        \
        sort_func(arr, 0, size-1, -200, 250);                           \
        print(arr, size-1);                                             \
    } while (0);

#define run_bucket_sort(sort_func)                      \
    do {                                                \
        double arr[] = {0.66,0,0.1,0.123,0.45,0.32,0.67,0.89,0.21,0.11,0.55,0.77,0.33,0.27,0.67}; \
        int size = sizeof(arr) / sizeof(arr[0]);        \
        sort_func(arr, 0, size-1);                      \
        print(arr, size-1);                             \
    } while (0);

#define run_search(search_func)                         \
    do {                                                \
        int arr[] = {0,1,2,4,5,6,7};                    \
        int size = sizeof(arr) / sizeof(arr[0]);        \
        assert(search_func(arr, 0, size-1, 7) == 6);    \
    } while(0);

/*******************************************************************************
 * SEARCH FUNCTIONS
*******************************************************************************/

int search_linear(int arr[], int l, int r, int key) {
    for (int i = l; i <= r; i++)
        if (arr[i] == key)
            return i;
    return -1;
}

int search_binary_recursive(int arr[], int l, int r, int key) {
    if (l > r)
        return -1;

    int mid = l + (r - l) / 2;

    if (arr[mid] == key)
        return mid;
    else if (arr[mid] < key)
        return search_binary_recursive(arr, mid + 1, r, key);
    else
        return search_binary_recursive(arr, l, mid - 1, key);
}

int search_binary_iterative(int arr[], int l, int r, int key) {
    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return -1;
}

int search_interpolation(int arr[], int l, int r, int key) {
    while (l <= r) {
        int mid = l + (r - l) * (key - arr[l]) / (arr[r] - arr[l]);
        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return -1;
}

int search_exponential(int arr[], int l, int r, int key) {
    if (l > r)
        return -1;

    int bound = 1;
    while (bound < r - l + 1 and arr[bound] < key)
        bound *= 2;

    return search_binary_iterative(arr, bound / 2, std::min(bound, r - l + 1), key);
}

/*******************************************************************************
 * SORT FUNCTIONS
*******************************************************************************/

void sort_bubble(int arr[], int l, int r) {
    for (int i = l; i < r; i++)
        for (int j = l; j < r - i; j++)
            if (arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
}

void sort_selection(int arr[], int l, int r) {
    for (int i = l; i < r; i++) {
        int min = i;
        for (int j = i; j <= r; j++)
            if (arr[j] < arr[min])
                min = j;
        swap(arr[min], arr[i]);
    }
}

void sort_insertion(int arr[], int l, int r) {
    for (int i = l; i <= r; i++)
        for (int j = i; j > 0; j--)
            if (arr[j - 1] > arr[j])
                swap(arr[j - 1], arr[j]);
            else
                break;
}

void merge(int arr[], int l, int mid, int r) {
    int n1 = mid - l + 1;
    int n2 = r - mid;

    int L[n1];
    int R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    int i = 0; int j = 0; int k = l;

    while (i < n1 and j < n2)
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

void sort_merge(int arr[], int l, int r) {
    if (l >= r)
        return;

    int mid = l + (r - l) / 2;
    sort_merge(arr, l, mid);
    sort_merge(arr, mid + 1, r);
    merge(arr, l, mid, r);
}

int partition(int arr[], int l, int r) {
    int key = arr[r];
    int i = l - 1;

    for (int j = l; j < r; j++)
        if (arr[j] <= key)
            swap(arr[j], arr[++i]);
    swap(arr[i+1], arr[r]);
    return i+1;
}

void sort_quick(int arr[], int l, int r) {
    if (l >= r)
        return;
    int p = partition(arr, l, r);
    sort_quick(arr, l, p - 1);
    sort_quick(arr, p + 1, r);
}

void sort_counting(int arr[], int l, int r, int min, int max) {
    int count[max - min + 1] = {0};
    int size = r - l + 1;

    for (int i = l; i < size; i++)
        count[arr[i] - min]++;
    for (int i = 1; i < max - min + 1; i++)
        count[i] += count[i-1];

    int output[size] = {0};

    for (int i = size - 1; i >= 0; i--)
        output[--count[arr[i] - min]] = arr[i];
    for (int i = l; i < size; i++)
        arr[i] = output[i];
}

void sort_radix_count(int arr[], int l, int r, int min, int max, int exp) {
    int count[max - min + 1] = {0};
    int size = r - l + 1;

    for (int i = 0; i < size; i++)
        count[(arr[i] / exp) % 10 - min]++;
    for (int i = 1; i < max - min + 1; i++)
        count[i] += count[i-1];

    int output[size] = {0};

    for (int i = size - 1; i >= 0; i--)
        output[--count[(arr[i] / exp) % 10 - min]] = arr[i];
    for (int i = 0; i < size; i++)
        arr[i] = output[i];
}

void sort_radix(int arr[], int l, int r) {
    int maximum = max(arr, l, r);
    for (int exp = 1; maximum / exp > 0; exp *= 10)
        sort_radix_count(arr, l, r, -200, 300, exp);
}

void sort_bucket(double arr[], int l, int r) {
    int size = r - l + 1;
    std::vector<double> buckets[size];

    for (int i = 0; i < size; i++) {
        int pos = size * arr[i];
        buckets[pos].push_back(arr[i]);
    }

    for (int i = 0; i < size; i++)
        std::sort(begin(buckets[i]), end(buckets[i]));;

    int k = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < buckets[i].size(); j++)
            arr[k++] = buckets[i][j];
}

int main() {
    run_search(search_linear);
    run_search(search_binary_recursive);
    run_search(search_binary_iterative);
    run_search(search_interpolation);
    run_search(search_exponential);

    run_sort(sort_bubble);
    run_sort(sort_selection);
    run_sort(sort_insertion);
    run_sort(sort_merge);
    run_sort(sort_quick);
    run_count_sort(sort_counting);
    run_sort(sort_radix);
    run_bucket_sort(sort_bucket);
}
