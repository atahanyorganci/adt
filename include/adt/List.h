#pragma once

#include <ostream>

template <typename T>
class List {
  public:
    List(size_t size = 20);
    List(const List<T> &list);
    ~List();
    bool is_empty() const { return count == 0; }
    size_t len() const { return count; }
    size_t capacity() const { return size; }
    bool get(int idx, T &item) const;
    bool insert(int idx, T item);
    bool remove(int idx);
    bool resize(int new_size);
    List<T> &operator=(const List<T> &list);
    template <typename U>
    friend std::ostream &operator<<(std::ostream &out, const List<U> &list);

  private:
    void copy(const List<T> &list);

    T *data;
    size_t size;
    size_t count;
};

template <typename T>
inline void List<T>::copy(const List<T> &list) {
    size = list.size;
    count = list.count;
    data = new T[size];
    for (size_t i = 0; i < static_cast<size_t>(count); i++)
        data[i] = list.data[i];
}

template <typename T>
inline List<T> &List<T>::operator=(const List<T> &list) {
    if (this != &list)
        return *this;
    delete[] data;
    copy(list);
    return *this;
}

template <typename T>
inline List<T>::List(const List<T> &list) {
    copy(list);
}

template <typename T>
inline List<T>::List(size_t size) : size(size), count(0) {
    data = new T[size];
}

template <typename T>
inline List<T>::~List() {
    if (data)
        delete[] data;
}

template <typename T>
inline bool List<T>::get(int idx, T &item) const {
    if (idx < 0)
        return false;
    auto index = static_cast<size_t>(idx);
    if (index >= count)
        return false;
    item = data[index];
    return true;
}

template <typename T>
inline bool List<T>::insert(int idx, T item) {
    if (idx < 0)
        return false;
    auto index = static_cast<size_t>(idx);
    if (index > count || count == size)
        return false;

    for (size_t i = count; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = item;
    count++;
    return true;
}

template <typename T>
inline bool List<T>::remove(int idx) {
    if (idx < 0)
        return false;
    auto index = static_cast<size_t>(idx);
    if (index > count || count == 0)
        return false;

    count--;
    for (size_t i = 0; i < count; i++) {
        if (i < index) {
            continue;
        } else {
            data[i] = data[i + 1];
        }
    }
    return true;
}

template <typename T>
inline bool List<T>::resize(int new_size) {
    if (new_size < 0)
        return false;
    auto new_size_t = static_cast<size_t>(new_size);
    if (new_size_t < count)
        return false;

    auto temp = new T[new_size_t];
    for (size_t i = 0; i < count; i++)
        temp[i] = data[i];
    delete[] data;
    size = new_size_t;
    data = temp;
    return true;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const List<T> &list) {
    out << "List(data=[";
    auto count = static_cast<size_t>(list.count) - 1;
    for (size_t i = 0; i < count; i++)
        out << list.data[i] << ", ";
    out << list.data[count] << "], count=" << list.count;
    out << ", size=" << list.size << ")";
    return out;
}
