#include <iostream>
#include <algorithm>
#include <map>
#include <multimap>

/*
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

/*
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

/*
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

/*
  Find the number occuring odd number of times
 */

int find_odd_number_occurence(int arr[], int size) {
    int result = 0;

    for (int i = 0; i < size; i++)
        result = result ^ arr[i];

    return result;
}

/*
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
}
