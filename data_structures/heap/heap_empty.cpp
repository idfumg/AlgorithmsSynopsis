#include <iostream>

class MinHeap {
private:
    int m_size;
    int capacity;
    int* arr;

public:
    MinHeap(int capacity) : m_size(0), capacity(capacity), arr(new int[capacity]) {}
    ~MinHeap() { delete [] arr; arr = nullptr; }

public:
    int size() { return m_size; }
    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return (2*i + 1); }
    int right(int i) { return (2*i + 2); }

public:
    void minHeapify(int i) {
        int l;
        int r;
        int smallest;

        for (;;) {
            l = left(i);
            r = right(i);
            smallest = i;

            if (l < m_size and arr[l] < arr[smallest])
                smallest = l;

            if (r < m_size and arr[r] < arr[smallest])
                smallest = r;

            if (smallest == i)
                break;

            std::swap(arr[smallest], arr[i]);
            i = smallest;
        }
    }

    void insert(int item) {
        if (m_size == capacity - 1)
            throw std::runtime_error("stack overflow");

        arr[m_size] = item;
        m_size++;

        for (int i = m_size - 1; i > 0 and arr[parent(i)] > arr[i]; i = parent(i))
            std::swap(arr[parent(i)], arr[i]);
    }

    int extractMin() {
        if (m_size <= 0)
            throw std::runtime_error("stack underflow");

        int temp = arr[0];
        arr[0] = arr[m_size - 1];
        m_size--;
        minHeapify(0);
        return temp;
    }

    void buildHeap(int array[], int array_size) {
        delete [] arr;
        m_size = array_size;
        capacity = array_size;
        arr = new int[array_size];

        std::copy(array, array + array_size, arr);

        for (int i = m_size / 2; i >= 0; i--)
            minHeapify(i);
    }

    void heapSort(int array[], int array_size) {
        buildHeap(array, array_size);

        for (int i = 0; i < array_size; i++)
            array[i] = extractMin();
    }

    void heapSort_inplace(int array[], int array_size) {
        if (arr)
            delete [] arr;

        arr = array;
        m_size = array_size;
        capacity = array_size;

        for (int i = m_size / 2; i >= 0; i--)
            minHeapify(i);

        for (int i = 0; i < array_size; i++) {
            std::swap(arr[0], arr[m_size - 1]);
            m_size--;
            minHeapify(0);
        }

        arr = nullptr;
    }

public:
    void print() {
        for (int i = 0; i < m_size; i++)
            std::cout << arr[i] << " ";
        std::cout << std::endl;
    }
};

MinHeap* new_heap_for_test() {
    MinHeap* h = new MinHeap(11);
    h->insert(3);
    h->insert(2);
    h->insert(15);
    h->insert(5);
    h->insert(4);
    h->insert(45);
    return h;
}

int main() {
    MinHeap* h = new_heap_for_test();

    std::cout << "heap size = " << h->size() << std::endl;
    std::cout << "heap = "; h->print(); std::cout << std::endl;
    std::cout << "extract min = " << h->extractMin() << std::endl;

    {
        int array[8] = {5, 3, 1, 2, 4, -2, -40, 100};
        h->heapSort(array, 8);

        for (int i = 0; i < 8; i++)
            std::cout << array[i] << " ";
        std::cout << std::endl;
    }

    {
        int array[8] = {5, 3, 1, 2, 4, -2, -40, 100};
        h->heapSort_inplace(array, 8);

        for (int i = 0; i < 8; i++)
            std::cout << array[i] << " ";
        std::cout << std::endl;
    }
}
