/*
  This algorithms uses O(log n) time complexity.
 */

#include <algorithm>
#include <assert.h>

int first(int arr[], int l, int r, int key) {
    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (arr[mid] == key and (arr[mid - 1] < key or mid == 0))
            return mid;

        if (arr[mid] < key)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return -1;
}

int last(int arr[], int l, int r, int key) {
    int size = r;
    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (arr[mid] == key and (arr[mid + 1] > key or mid == size - 1))
            return mid;

        if (arr[mid] < key)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return -1;
}

int count(int arr[], int size, int key) {
    int i = first(arr, 0, size - 1, key);
    if (i == -1)
        return i;
    
    int j = last(arr, 0, size - 1, key);
    
    return j - i + 1;
}

int main() {
    int arr[] {1, 2, 2, 3, 3, 3, 4, 5, 7, 10, 99};
    int size = sizeof (arr) / sizeof (arr[0]);
    std::sort(arr, arr + size);

    assert(count(arr, size, 3) == 3);
}
