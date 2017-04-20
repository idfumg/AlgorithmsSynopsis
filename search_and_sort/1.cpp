#include <assert.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

template <typename T>
void print(T arr[], int size) {
    for (int i = 0; i <= size; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

void merge(int arr[], int l, int mid, int r) {
    std::cout << __FUNCTION__ << ": l = " << l << ", r = " << r << std::endl;
    int n1 = mid - l + 1;
    int n2 = r - mid;

    int L[n1];
    int R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[i + l];
    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + i + 1];

    int i = 0; int j = 0; int k = l;

    std::cout << "L array: ";
    print(L, n1 - 1);
    std::cout << "R array: ";
    print(R, n2 - 1);

    while (i < n1 and j < n2) {
        if (L[i] <= R[j]) {
            std::cout << L[i] << " < " << R[j] << std::endl;
            arr[k++] = L[i++];
        }
        else {
            std::cout << L[i] << " > " << R[j] << std::endl;
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        std::cout << "L: " << L[i] << std::endl;
        arr[k++] = L[i++];
    }

    while (j < n2) {
        std::cout << "R: " << R[j] << std::endl;
        arr[k++] = R[j++];
    }

    std::cout << "temp result: ";
    print(arr, 4);

    std::set<int> result;
    i = 0; j = 0; k = l;
    while (i < n1) {
        if (arr[k] != L[i])
            result.insert(arr[k]);
        k++;
        i++;
    }
    while (j < n2) {
        if (arr[k] != R[j])
            result.insert(arr[k]);
        k++;
        j++;
    }

    std::cout << "count = "
              << (result.size() % 2 == 0 ? result.size() : result.size() - 1)
              << std::endl;
}

void sort_merge(int arr[], int l, int r) {
    if (l >= r)
        return;

    int mid = l + (r - l) / 2;
    sort_merge(arr, l, mid);
    sort_merge(arr, mid + 1, r);
    merge(arr, l, mid, r);
}

int main() {
    int arr[] = {2,3,2,9,9};
    int size = sizeof(arr) / sizeof(arr[0]);     
    sort_merge(arr, 0, size-1);                    
    print(arr, size-1);
        
    return 0;
}
