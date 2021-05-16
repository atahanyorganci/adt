#pragma once

#include <utility>

template <typename T>
class HashNode {
  public:
    HashNode();
    HashNode(const T &data);
    HashNode(const HashNode<T> &node);
    HashNode<T> &operator=(const HashNode<T> &node);
    bool is_occupied() const;
    bool get(T &data);
    bool set(const T data);
    bool unset();
    void clone(const HashNode<T> &node);
    void destroy();
    void clone_children(const HashNode<T> *other);
    bool operator==(const T &other) const;
    bool operator!=(const T &other) const;

    HashNode<T> *next;
    T data;
};

template <typename T>
inline HashNode<T>::HashNode() : next(this) {}

template <typename T>
inline HashNode<T>::HashNode(const T &data) : next(nullptr), data(data) {}

template <typename T>
inline HashNode<T>::HashNode(const HashNode<T> &node) {
    clone(node);
}

template <typename T>
inline HashNode<T> &HashNode<T>::operator=(const HashNode<T> &node) {
    if (&node == this)
        return *this;
    destroy();
    clone(node);
    return *this;
}

template <typename T>
inline bool HashNode<T>::is_occupied() const {
    return this != next;
}

template <typename T>
inline bool HashNode<T>::get(T &item) {
    if (!is_occupied())
        return false;
    item = data;
    return true;
}

template <typename T>
inline bool HashNode<T>::set(const T item) {
    if (is_occupied())
        return false;
    data = item;
    next = nullptr;
    return true;
}

template <typename T>
inline bool HashNode<T>::unset() {
    if (!is_occupied())
        return false;
    auto to_delete = std::move(data);
    (void)to_delete;

    if (next) {
        auto temp = next;
        data = std::move(next->data);
        next = next->next;
        delete temp;
    } else {
        next = this;
    }
    return true;
}

template <typename T>
inline void HashNode<T>::clone(const HashNode<T> &node) {
    if (node.is_occupied()) {
        data = node.data;
        clone_children(&node);
    } else {
        next = this;
    }
}

template <typename T>
inline void HashNode<T>::destroy() {
    if (!is_occupied())
        return;
    if (next != nullptr) {
        next->destroy();
        delete next;
    }
}

template <typename T>
inline void HashNode<T>::clone_children(const HashNode<T> *other) {
    if (!is_occupied())
        return;

    HashNode<T> *curr = this;
    const HashNode<T> *other_curr = other;
    while (other_curr->next) {
        other_curr = other_curr->next;
        curr->next = new HashNode<T>(other_curr->data);
        curr = curr->next;
    }
}

template <typename T>
inline bool HashNode<T>::operator==(const T &other) const {
    if (!is_occupied())
        return false;
    return data == other;
}

template <typename T>
inline bool HashNode<T>::operator!=(const T &other) const {
    return !(data == other);
}
