#pragma once

#include "stddef.h"
#include <utility>

const size_t DEFAULT_HEAP_CAPACITY = 31;

template <typename T>
class Heap {
  public:
    Heap(size_t initial_capacity = DEFAULT_HEAP_CAPACITY);
    Heap(const Heap<T> &heap);
    Heap<T> &operator=(const Heap<T> &heap);
    ~Heap();
    bool insert(const T entry);
    bool max(T &out) const;
    bool extract_max(T &out);
    bool resize(size_t new_capacity);
    size_t get_size() const { return size; }
    size_t is_empty() const { return size == 0; }

    template <typename U>
    friend void heap_sort(U *arr, size_t len);

  private:
    Heap(T *arr, size_t len);
    T *steal();

    size_t left(size_t index) { return 2 * index; }
    size_t right(size_t index) { return 2 * index + 1; }
    size_t parent(size_t index) { return index / 2; }
    bool is_leaf(size_t index) { return 2 * index < size; }

    void max_heapify(const size_t index);
    void bubble_up(const size_t index);

    void copy_heap(const Heap<T> &heap);
    size_t size;
    size_t capacity;
    T *data;
};

template <typename T>
Heap<T>::Heap(size_t initial_capacity) : size(0), capacity(initial_capacity) {
    data = initial_capacity != 0 ? new T[initial_capacity] : nullptr;
}

template <typename T>
Heap<T>::Heap(T *arr, size_t len) : size(len), capacity(len), data(arr) {}

template <typename T>
Heap<T>::Heap(const Heap &heap) {
    copy_heap(heap);
}

template <typename T>
Heap<T> &Heap<T>::operator=(const Heap<T> &heap) {
    if (this == &heap)
        return *this;
    if (data)
        delete[] data;
    copy_heap(heap);
}

template <typename T>
Heap<T>::~Heap() {
    if (data)
        delete[] data;
}

template <typename T>
bool Heap<T>::insert(const T entry) {
    if (size >= capacity)
        return false;
    data[size] = entry;
    size++;
    bubble_up(size);
    return true;
}

template <typename T>
bool Heap<T>::max(T &out) const {
    if (size == 0)
        return false;
    out = data[0];
}

template <typename T>
bool Heap<T>::extract_max(T &out) {
    if (size == 0)
        return false;
    out = std::move(data[0]);
    data[0] = std::move(data[size - 1]);
    size--;
    max_heapify(1);
    return true;
}

template <typename T>
bool Heap<T>::resize(size_t new_capacity) {
    if (new_capacity < size)
        return false;
    T *temp = new T[new_capacity];
    if (data) {
        for (size_t i = 0; i < size; i++) {
            temp[i] = data[i];
        }
        delete[] data;
    }
    data = temp;
    return true;
}

template <typename T>
void Heap<T>::copy_heap(const Heap<T> &heap) {
    size = heap.size;
    capacity = heap.capacity;
    data = new T[capacity];
    for (size_t i = 0; i < size; i++) {
        data[i] = heap.data[i];
    }
}

template <typename T>
void Heap<T>::max_heapify(const size_t i) {
    if (i == 0)
        return;
    size_t l = left(i);
    size_t r = right(i);
    size_t largest = i;
    if (r <= size && data[r - 1] > data[largest - 1])
        largest = r;
    if (l <= size && data[l - 1] > data[largest - 1])
        largest = l;
    if (largest != i) {
        std::swap(data[largest - 1], data[i - 1]);
        max_heapify(largest);
    }
}

template <typename T>
void Heap<T>::bubble_up(const size_t i) {
    if (i == 1)
        return;
    size_t p = parent(i);
    if (data[i - 1] > data[p - 1]) {
        std::swap(data[i - 1], data[p - 1]);
        bubble_up(p);
    }
}

template <typename T>
void heap_sort(T *arr, size_t len) {
    Heap<T> h = Heap<T>(arr, len);
    for (size_t i = len / 2; i > 0; i--) {
        h.max_heapify(i);
    }
    for (size_t i = 0; i < len; i++) {
        h.size--;
        std::swap(h.data[0], h.data[h.size]);
        h.max_heapify(1);
    }
    arr = h.steal();
}

template <typename T>
T *Heap<T>::steal() {
    T *out = data;
    capacity = 0;
    size = 0;
    data = nullptr;
    return out;
}
