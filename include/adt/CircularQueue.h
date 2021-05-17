#pragma once

#include <stddef.h>

template <typename T>
class CircularQueue {
  public:
    CircularQueue();
    CircularQueue(const size_t size);
    CircularQueue(const CircularQueue<T> &queue);
    CircularQueue<T> &operator=(const CircularQueue<T> &queue);
    ~CircularQueue();
    bool isEmpty() const;
    bool isFull() const;
    bool enqueue(T data);
    bool dequeue();
    bool dequeue(T &data);
    bool getFront(T &data) const;

  private:
    void copy(const CircularQueue<T> &queue);
    void destroy();

    T *data;
    size_t size, start, end;
    int count;
};

template <typename T>
inline CircularQueue<T>::CircularQueue()
    : data(nullptr), size(20), start(0), end(0), count(0) {
    data = new T[20];
}

template <typename T>
inline CircularQueue<T>::CircularQueue(const size_t size)
    : data(nullptr), size(size), start(0), end(0), count(0) {
    data = new T[size];
}

template <typename T>
inline CircularQueue<T>::CircularQueue(const CircularQueue<T> &queue) {
    copy(queue);
}

template <typename T>
inline void CircularQueue<T>::copy(const CircularQueue<T> &queue) {
    size = queue.size;
    start = queue.start;
    end = queue.end;
    count = queue.count;
    data = new T[size];
    for (size_t i = 0; i < size; i++) {
        data[i] = queue.data[i];
    }
}

template <typename T>
inline void CircularQueue<T>::destroy() {
    delete[] data;
}

template <typename T>
inline CircularQueue<T> &
CircularQueue<T>::operator=(const CircularQueue<T> &queue) {
    if (this == &queue)
        return *this;
    destroy();
    copy(queue);
    return *this;
}

template <typename T>
inline CircularQueue<T>::~CircularQueue() {
    destroy();
}

template <typename T>
inline bool CircularQueue<T>::isEmpty() const {
    return count == 0;
}

template <typename T>
inline bool CircularQueue<T>::isFull() const {
    return count == static_cast<int>(size);
}

template <typename T>
inline bool CircularQueue<T>::enqueue(T data) {
    if (isFull())
        return false;
    this->data[end] = data;
    end = (end + 1) % size;
    count++;
    return true;
}

template <typename T>
inline bool CircularQueue<T>::dequeue() {
    if (isEmpty())
        return false;
    start = (start + 1) % size;
    count--;
    return true;
}

template <typename T>
inline bool CircularQueue<T>::dequeue(T &data) {
    getFront(data);
    return dequeue();
}

template <typename T>
inline bool CircularQueue<T>::getFront(T &data) const {
    if (isEmpty())
        return false;
    data = this->data[start];
    return true;
}
