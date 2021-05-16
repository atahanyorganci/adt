#pragma once

#include <iostream>
#include <ostream>
#include <stddef.h>

#include "common/HashNode.h"

static const size_t DEFAULT_SIZE = 29;

template <typename T, typename Traits>
class HashTable {
  public:
    HashTable();
    HashTable(const size_t bucket_count);
    HashTable(const HashTable<T, Traits> &table);
    HashTable<T, Traits> &operator=(const HashTable<T, Traits> &table);
    ~HashTable();
    bool is_empty() const { return item_count == 0; };
    int size() const { return item_count; };
    bool insert(const T &item);
    T *find(const T &item) const;
    bool contains(const T &item) const;
    bool remove(const T &item);

  private:
    void copy(const HashTable<T, Traits> &table);
    void destroy();

    HashNode<T> *buckets;
    size_t bucket_count;
    int item_count;
};

template <typename T, typename Traits>
inline HashTable<T, Traits>::HashTable() {
    buckets = new HashNode<T>[DEFAULT_SIZE];
    bucket_count = DEFAULT_SIZE;
    item_count = 0;
}

template <typename T, typename Traits>
inline HashTable<T, Traits>::HashTable(const size_t bucket_count) {
    buckets = new HashNode<T>[bucket_count];
    bucket_count = bucket_count;
    item_count = 0;
}

template <typename T, typename Traits>
inline HashTable<T, Traits>::HashTable(const HashTable<T, Traits> &table) {
    copy(table);
}

template <typename T, typename Traits>
inline void HashTable<T, Traits>::copy(const HashTable<T, Traits> &table) {
    buckets = new HashNode<T>[table.bucket_count];
    bucket_count = table.bucket_count;
    item_count = table.item_count;
    for (size_t i = 0; i < table.bucket_count; i++) {
        buckets[i] = table.buckets[i];
    }
}

template <typename T, typename Traits>
inline void HashTable<T, Traits>::destroy() {
    for (size_t i = 0; i < bucket_count; i++) {
        if (buckets[i].is_occupied())
            buckets[i].destroy();
    }
    delete[] buckets;
}

template <typename T, typename Traits>
inline HashTable<T, Traits> &
HashTable<T, Traits>::operator=(const HashTable<T, Traits> &table) {
    destroy();
    copy(table);
    return *this;
}

template <typename T, typename Traits>
inline HashTable<T, Traits>::~HashTable() {
    destroy();
}

template <typename T, typename Traits>
inline bool HashTable<T, Traits>::insert(const T &item) {
    int hash = Traits::hash(item);
    size_t idx = static_cast<size_t>(hash) % bucket_count;

    if (!buckets[idx].is_occupied()) {
        buckets[idx].set(std::move(item));
        item_count++;
        return true;
    }

    if (Traits::equals(buckets[idx].data, item))
        return false;

    HashNode<T> *curr = &buckets[idx];
    while (curr->next != nullptr) {
        if (Traits::equals(curr->next->data, item))
            return false;
        curr = curr->next;
    }
    item_count++;
    curr->next = new HashNode<T>(item);
    return true;
}

template <typename T, typename Traits>
inline T *HashTable<T, Traits>::find(const T &item) const {
    int hash = Traits::hash(item);
    size_t idx = static_cast<size_t>(hash) % bucket_count;

    if (!buckets[idx].is_occupied())
        return nullptr;

    HashNode<T> *curr = &buckets[idx];
    while (curr) {
        if (Traits::equals(curr->data, item))
            return &curr->data;
        curr = curr->next;
    }
    return nullptr;
}

template <typename T, typename Traits>
inline bool HashTable<T, Traits>::contains(const T &item) const {
    return find(item) != nullptr;
}

template <typename T, typename Traits>
inline bool HashTable<T, Traits>::remove(const T &item) {
    int hash = Traits::hash(item);
    size_t idx = static_cast<size_t>(hash) % bucket_count;

    // If the bucket is unused item doesn't exist
    if (!buckets[idx].is_occupied())
        return false;

    // If the item belongs to bucket's head
    if (Traits::equals(buckets[idx].data, item)) {
        buckets[idx].unset();
        item_count--;
        return true;
    }

    // If the item belongs in the bucket
    HashNode<T> *prev = &buckets[idx];
    HashNode<T> *curr = buckets[idx].next;
    while (curr) {
        if (Traits::equals(curr->data, item)) {
            prev->next = curr->next;
            delete curr;
            item_count--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}
