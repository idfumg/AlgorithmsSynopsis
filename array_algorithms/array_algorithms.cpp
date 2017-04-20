#include <iostream>
#include <algorithm>
#include <map>
#include <stack>

template <typename T>
void print_array(T arr[], int size) {
    for (int i = 0; i < size; i++)
        std::cout << arr[i] << " ";
}

/* 1.
 Write a C program that, given an array A[] of n numbers and another number x,
 determines whether or not there exist two elements in S whose sum is exactly x.
*/

// common method
bool sum_of_two_in_array1(int arr[], int size, int sum) {
    int l = 0;
    int r = size - 1;

    std::sort(arr, arr + size);

    while (l < r) {
        int temp = arr[l] + arr[r];
        if (temp == sum)
            return true;
        else if (temp < sum)
            l++;
        else
            r--;
    }

    return false;
}

// if range of numbers is known
bool sum_of_two_in_array2(int arr[], int size, int sum, int min) {
    std::vector<bool> binMap(sum, false);

    for (int i = 0; i < size; i++) {
        int temp = sum - arr[i] - min;

        if (temp >= 0 and binMap[temp] == true)
            return true;

        if (arr[i] - min <= sum)
            binMap[arr[i] - min] = true;
    }

    return false;
}

/* 2.
  Maximum and minimum in an array using minimum number of comparisons.
*/

void maxmin(int arr[], int l, int r, int* min, int* max) {
    if (l == r) {
        *min = *max = arr[l];
    }
    else if (r - l == 1) {
        if (arr[l] > arr[r]) {
            *min = arr[r];
            *max = arr[l];
        }
        else {
            *min = arr[l];
            *max = arr[r];
        }
    }
    else {
        int mid = (l + r) / 2;
        int min_of_1 = 0;
        int max_of_1 = 0;
        int min_of_2 = 0;
        int max_of_2 = 0;

        maxmin(arr, l, mid, &min_of_1, &max_of_1);
        maxmin(arr, mid + 1, r, &min_of_2, &max_of_2);

        *min = std::min(min_of_1, min_of_2);
        *max = std::max(max_of_1, max_of_2);
    }
}

/* 3.
  Check the majority of an element in array.
*/

int find_candidate(int arr[], int size) {
    int major_index = 0;
    int count = 1;

    for (int i = 1;i < size; i++) {
        if (arr[major_index] == arr[i])
            count++;
        else
            count--;

        if (count == 0) {
            major_index = i;
            count = 1;
        }
    }

    return arr[major_index];
}

bool is_majority(int arr[], int size, int candidate) {
    int count = 0;

    for (int i = 0; i < size; i++)
        if (arr[i] == candidate)
            count++;

    return count > size / 2;
}

int find_majority(int arr[], int size) {
    int candidate = find_candidate(arr, size);
    if (is_majority(arr, size, candidate))
        return candidate;
    return -1;
}

/* 4.
  Find the number occuring odd number of times
 */

int find_odd_number_occurence(int arr[], int size) {
    int result = 0;

    for (int i = 0; i < size; i++)
        result = result ^ arr[i];

    return result;
}

/* 5.
  Largest Sum Contiguous Subarray
 */

int find_max_subarray_sum(int arr[], int size) {
    int result_max = arr[0];
    int temp_max = arr[0];

    for (int i = 1; i < size; i++) {
        temp_max = std::max(arr[i], temp_max + arr[i]);
        result_max = std::max(temp_max, result_max);
    }

    return result_max;
}

/* 6.
  Find the Missing Number.
  Example:
  I/P    [1, 2, 4, ,6, 3, 7, 8]
  O/P    5
 */

int find_missing_number(int arr[], int size) {
    int x1 = arr[0];
    for (int i = 1; i < size; i++)
        x1 ^= arr[i];

    int x2 = 1;
    for (int i = 2; i < size + 2; i++)
        x2 ^= i;

    return x1 ^ x2;
}

/* 7.
   Array rotation for time O(n) and space O(1).
 */

int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

void left_rotate(int arr[], int d, int n) {
    for (int i = 0; i < gcd(n, d); i++) {
        int temp = arr[i];

        int j = i;
        for (;;) {
            int k = j + d;
            if (k >= n)
                k = k - n;
            if (k == i)
                break;
            arr[j] = arr[k];
            j = k;
        }

        arr[j] = temp;
    }
}

void right_rotate(int arr[], int d, int n) {
    for (int i = n - 1; i >= n - gcd(n, d); i--) {
        int temp = arr[i];

        int j = i;
        for (;;) {
            int k = j - d;
            if (k < 0)
                k = k + n;
            if (k == i)
                break;
            arr[j] = arr[k];
            j = k;
        }

        arr[j] = temp;
    }
}

/* 8.
   Floor and ceiling in a sorted array.
 */

void find_floor_and_ceiling(int arr[], int l, int r, int x) {
    std::cout << "x = " << x << ", ";

    if (x < arr[l]) {
        std::cout << "floor: -1, ceiling: " << arr[l] << std::endl;
        return;
    }

    if (x > arr[r]) {
        std::cout << "floor: " << arr[r] << ", ceiling: -1" << std::endl;
        return;
    }

    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (arr[mid] == x) {
            std::cout << "floor = ceiling = " << arr[mid] << std::endl;
            return;
        }
        else if (arr[mid] < x and arr[mid + 1] > x) {
            std::cout << "floor: " << arr[mid]
                      << ", ceiling: " << arr[mid + 1] << std::endl;
            return;
        }
        else if (arr[mid] > x and arr[mid - 1] < x) {
            std::cout << "floor: " << arr[mid - 1]
                      << ", ceiling: " << arr[mid] << std::endl;
            return;
        }
        else if (arr[mid] < x) {
            l = mid + 1;
        }
        else if (arr[mid] > x) {
            r = mid - 1;
        }
    }
}

/* 9.
   Find two repeating elements in an array.
 */

void print_repeating_elements(int arr[], int size, int max) {
    int count[max] = {0};
    std::cout << "Repeating elements: ";
    for (int i = 0; i < size; i++) {
        if (count[arr[i]] != 0)
            std::cout << arr[i] << " ";
        else
            count[arr[i]]++;
    }
    std::cout << std::endl;
}

/* 10.
   Segregate 0s and 1s in array.
*/

void sort_01(int arr[], int size) {
    int low = 0;
    int high = size - 1;

    while(low <= high) {
        switch(arr[low]) {
        case 0:
            low++;
            break;
        case 1:
            std::swap(arr[low], arr[high--]);
            break;
        }
    }
}

/* 11.
   Segregate 0s and 1s, and 2s in array.
*/

void sort_012(int arr[], int size) {
    int low = 0;
    int mid = 0;
    int high = size - 1;

    while (mid <= high) {
        switch(arr[mid]) {
        case 0:
            std::swap(arr[low++], arr[mid++]);
            break;
        case 1:
            mid++;
            break;
        case 2:
            std::swap(arr[mid], arr[high--]);
            break;
        }
    }
}

/* 12.
   Sort elements by frequency.
 */

void sort_by_freq(int arr[], int n) {
    std::map<int, int> m;
    std::multimap<int, int> multi_m;

    for (int i = 0; i < n; i++)
        m[arr[i]]++;

    for (const auto& elem : m)
        multi_m.insert(std::make_pair(elem.second, elem.first));

    int k = 0;
    for (auto it = multi_m.crbegin(); it != multi_m.crend(); it++)
        for (int temp = it->first; temp; temp--)
            arr[k++] = it->second;
}

/* 13.
   Find the minimum length of unsorted subarray,
   sorting which makes the complete array sorted.
 */

void print_unsorted(int arr[], int n) {
    int s {};
    for (s = 0; s < n - 1; s++)
        if (arr[s] > arr[s + 1])
            break;

    if (s == n - 1)
        return;

    int e {};
    for (e = n - 1; e > s; e--)
        if (arr[e] < arr[e - 1])
            break;

    int max = arr[s];
    int min = arr[s];
    for (int i = s + 1; i <= e; i++) {
        if (arr[i] > max)
            max = arr[i];
        if (arr[i] < min)
            min = arr[i];
    }

    for (int i = 0; i < s; i++)
        if (arr[i] > min) {
            s = i;
            break;
        }

    for (int i = n - 1; i >= e + 1; i--)
        if (arr[i] < max) {
            e = i;
            break;
        }

    print_array(arr + s, e - s + 1); std::cout << std::endl;
}

/* 14.
   Next greater element.
 */

void find_nge(int arr[], int n) {
    std::stack<int> stack;
    stack.push(arr[0]);

    for (int i = 1; i < n; i++) {
        while (not stack.empty() and stack.top() < arr[i]) {
            std::cout << "\nthe nge of " << stack.top() << " is : " << arr[i] << std::endl;
            stack.pop();
        }
        stack.push(arr[i]);
    }

    while (not stack.empty()) {
        std::cout << "\nthe nge of " << stack.top() << " is : -1";
        stack.pop();
    }
}

/* 15.
   Check if array elements are consecutive.
 */

int get_min(int arr[], int n) {
    int min = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] < min)
            min = arr[i];
    return min;
}

int get_max(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}

bool array_elements_are_consecutive(int arr[], int n) {
    if (n < 1)
        return false;

    int min = get_min(arr, n);
    int max = get_max(arr, n);

    if (max - min + 1 != n)
        return false;

    bool visited[n] = {false};
    for (int i = 0; i < n; i++) {
        if (visited[arr[i] - min] != false)
            return false;
        visited[arr[i] - min] = true;
    }

    return true;
}

bool array_elements_are_consecutive2(int arr[], int n) {
    std::sort(arr, arr + n);
    for (int i = 1; i < n - 1; i++)
        if (arr[i] - arr[i - 1] != 1)
            return false;
    return true;
}

/* 16.
   Given an array arr[], find the maximum j – i such that arr[j] > arr[i]
 */

int max_index_diff(int arr[], int n) {
    int Lmin[n] = {};

    Lmin[0] = arr[0];
    for (int i = 1; i < n; i++)
        Lmin[i] = std::min(arr[i], Lmin[i - 1]);

    int Rmax[n] = {};

    Rmax[0] = arr[n - 1];
    for (int i = n - 2; i >= 0; i--)
        Rmax[i] = std::max(arr[i], Rmax[i + 1]);

    int i = 0; int j = 0; int max_diff = -1;

    while (i < n and j < n) {
        if (Lmin[i] < Rmax[j]) {
            max_diff = std::max(max_diff, j - i);
            j++;
        }
        else {
            i++;
        }
    }

    return max_diff;
}

int main() {
    /*
      Write a C program that, given an array A[] of n numbers and another number x,
      determines whether or not there exist two elements in S whose sum is exactly x.
    */
    {
        int arr[] = {1, 4, 45, 6, 10, -8};
        int size = 6;
        int sum = 16;
        int min = -8;

        std::cout << "sum_of_two_in_array1: " << sum_of_two_in_array1(arr, size, sum) << std::endl;
        std::cout << "sum_of_two_in_array2: " << sum_of_two_in_array2(arr, size, sum, min) << std::endl;
    }

    /*
      Maximum and minimum in an array using minimum number of comparisons.
    */
    {
        int arr[] = {1, 4, 45, 6, 10, -8, -4};

        int min = 0;
        int max = 0;

        maxmin(arr, 0, 5, &min, &max);
        std::cout << "maxmin: min = " << min << ", max = " << max << std::endl;
    }

    /*
      Check the majority of an element in array.
     */
    {
        int a[] = {1, 3, 3, 1, 2};
        std::cout << "majority element is: " << find_majority(a, 5) << std::endl;
    }

    /*
      Find the number occuring odd number of times
    */
    {
        int arr[] = {1, 2, 3, 2, 3, 1, 3};
        std::cout << "odd number: " << find_odd_number_occurence(arr, 7) << std::endl;
    }

    /*
      Largest Sum Contiguous Subarray
    */
    {
        int arr[] = {-2, -3, 4, -1, -2, 1, 5, -3};
        int size = 6;
        std::cout << "maximum contiguous sum is: " << find_max_subarray_sum(arr, size) << std::endl;
    }

    /*
      Find the Missing Number.
      Example:
      I/P    [1, 2, 4, ,6, 3, 7, 8]
      O/P    5
    */
    {
        int arr[] = {1, 2, 4, 5, 6};
        std::cout << "missing number in array: " << find_missing_number(arr, 5) << std::endl;
    }

    /* 7.
       Array rotation for time O(n) and space O(1).
    */
    {
        int arr[] = {1,2,3,4,5,6,7};
        std::cout << "left rotate '";
        print_array(arr, 7);
        std::cout << "' -> '";
        left_rotate(arr, 2, 7);
        print_array(arr, 7);
        std::cout << "'" << std::endl;
    }

    {
        int arr[] = {1,2,3,4,5,6,7};
        std::cout << "right rotate '";
        print_array(arr, 7);
        std::cout << "' -> '";
        right_rotate(arr, 2, 7);
        print_array(arr, 7);
        std::cout << "'" << std::endl;
    }

    /* 8.
       Floor and ceiling in a sorted array.
    */
    {
        int arr[] = {1,2,8,10,10,12,19};
        find_floor_and_ceiling(arr, 0, 6, 30);
        find_floor_and_ceiling(arr, 0, 6, 0);
        find_floor_and_ceiling(arr, 0, 6, 8);
        find_floor_and_ceiling(arr, 0, 6, 3);
    }

    /* 9.
       find two repeating elements in an array.
    */
    {
        int arr[] = {1,2,3,4,5,2,9};
        int size = sizeof(arr) / sizeof(arr[0]);
        print_repeating_elements(arr, size, 9);
    }

    /* 10.
       Segregate 0s and 1s in array.
    */
    {
        int arr[] = {0, 1, 0, 1, 0, 0, 1, 1, 1, 0};
        int size = sizeof(arr) / sizeof(arr[0]);
        sort_01(arr, size);
        std::cout << "sort_01: "; print_array(arr, size); std::cout << std::endl;
    }

    /* 11.
       Segregate 0s and 1s, and 2s in array.
    */
    {
        int arr[] = {0, 1, 1, 0, 1, 2, 1, 2, 0, 0, 0, 1};
        int size = sizeof(arr) / sizeof(arr[0]);
        sort_012(arr, size);
        std::cout << "sort_01: "; print_array(arr, size); std::cout << std::endl;
    }

    /* 12.
       Sort elements by frequency.
    */
    {
        int arr[] = {2, 1, 2, 3, 5, 4, 1, 2, 1, 3, 2};
        int n = sizeof(arr)/sizeof(arr[0]);

        print_array(arr, n); std::cout << " -> ";
        sort_by_freq(arr, n);
        print_array(arr, n);
        std::cout << std::endl;
    }

    /* 13.
       Find the minimum length of unsorted subarray,
       sorting which makes the complete array sorted.
    */
    {
        int arr[] = {10, 12, 20, 30, 25, 40, 32, 31, 35, 50, 60};
        int size = sizeof(arr)/sizeof(arr[0]);
        std::cout << "minimum unsorted array of '"; print_array(arr, size); std::cout << "' -> ";
        print_unsorted(arr, size);
    }

    /* 14.
       Next greater element.
    */
    {
        int arr[] = {10, 12, 20, 30, 25, 40, 32, 31, 35, 50, 60};
        int size = sizeof(arr)/sizeof(arr[0]);
        std::cout << "Find nge for array: "; print_array(arr, size);
        find_nge(arr, size);
        std::cout << std::endl << std::endl;
    }

    /* 15.
       Check if array elements are consecutive.
    */
    {
        int arr[] = {83, 78, 80, 81, 79, 82};
        int n = sizeof(arr)/sizeof(arr[0]);
        print_array(arr, n); std::cout << "elements are consecutive: ";
        std::cout << array_elements_are_consecutive(arr, n) << std::endl;
        print_array(arr, n); std::cout << "elements are consecutive2: ";
        std::cout << array_elements_are_consecutive2(arr, n) << std::endl;
    }

    {
        int arr[] = {34, 23, 52, 12, 3};
        int n = sizeof(arr)/sizeof(arr[0]);
        print_array(arr, n); std::cout << "elements are consecutive: ";
        std::cout << array_elements_are_consecutive(arr, n) << std::endl;
        print_array(arr, n); std::cout << "elements are consecutive2: ";
        std::cout << array_elements_are_consecutive2(arr, n) << std::endl;
    }

    /* 16.
       Given an array arr[], find the maximum j – i such that arr[j] > arr[i]
    */
    {
        int arr[] = {9, 2, 3, 4, 5, 6, 7, 8, 18, 0};
        int n = sizeof(arr)/sizeof(arr[0]);
        int maxDiff = max_index_diff(arr, n);
        std::cout << "max diff j - i of array: "; print_array(arr, n);
        std::cout << " is: " << maxDiff << std::endl;
    }
}
