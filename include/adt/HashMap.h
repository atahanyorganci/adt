#pragma once

#include <iostream>
#include <ostream>
#include <stddef.h>

#include "common/HashNode.h"

static const size_t DEFAULT_SIZE = 29;

template <typename K, typename V, typename Hasher>
class HashMap {
  private:
    struct Entry {
        K key;
        V value;
        Entry() = default;
        Entry(const K &key, const V &value) : key(key), value(value) {}
    };
    HashNode<Entry> *buckets;
    size_t count;

  public:
    HashMap();
    HashMap(const size_t bucket_count);
    HashMap(const HashMap<K, V, Hasher> &table);
    HashMap<K, V, Hasher> &operator=(const HashMap<K, V, Hasher> &table);
    ~HashMap();
    bool is_empty() const;
    bool insert(const K &key, const V &value);
    bool get(const K &key, V &value);
    bool remove(const K &key);

  private:
    void copy(const HashMap<K, V, Hasher> &table);
    void destroy();
};

template <typename K, typename V, typename Hasher>
inline HashMap<K, V, Hasher>::HashMap() {
    buckets = new HashNode<Entry>[DEFAULT_SIZE];
    count = DEFAULT_SIZE;
}

template <typename K, typename V, typename Hasher>
inline HashMap<K, V, Hasher>::HashMap(const size_t bucket_count) {
    buckets = new HashNode<Entry>[bucket_count];
    count = bucket_count;
}

template <typename K, typename V, typename Hasher>
inline HashMap<K, V, Hasher>::HashMap(const HashMap<K, V, Hasher> &table) {
    copy(table);
}

template <typename K, typename V, typename Hasher>
inline void HashMap<K, V, Hasher>::copy(const HashMap<K, V, Hasher> &table) {
    buckets = new HashNode<Entry>[table.count];
    count = table.count;
    for (size_t i = 0; i < table.count; i++) {
        buckets[i] = table.buckets[i];
        if (table.buckets[i].is_occupied()) {
            buckets[i].clone_children(&table.buckets[i]);
        }
    }
}

template <typename K, typename V, typename Hasher>
inline void HashMap<K, V, Hasher>::destroy() {
    for (size_t i = 0; i < count; i++) {
        if (buckets[i].is_occupied())
            buckets[i].destroy();
    }
    delete[] buckets;
}

template <typename K, typename V, typename Hasher>
inline HashMap<K, V, Hasher> &
HashMap<K, V, Hasher>::operator=(const HashMap<K, V, Hasher> &table) {
    destroy();
    copy(table);
    return *this;
}

template <typename K, typename V, typename Hasher>
inline HashMap<K, V, Hasher>::~HashMap() {
    destroy();
}

template <typename K, typename V, typename Hasher>
inline bool HashMap<K, V, Hasher>::insert(const K &key, const V &value) {
    int hash = Hasher::hash(key);
    size_t idx = static_cast<size_t>(hash) % count;

    if (!buckets[idx].is_occupied()) {
        buckets[idx].set(std::move(Entry(key, value)));
        return true;
    }

    HashNode<Entry> *curr = &buckets[idx];
    while (curr->next != nullptr) {
        if (curr->data.key == key)
            return false;
        curr = curr->next;
    }
    curr->next = new HashNode<Entry>(Entry(key, value));

    return true;
}

template <typename K, typename V, typename Hasher>
inline bool HashMap<K, V, Hasher>::get(const K &key, V &value) {
    int hash = Hasher::hash(key);
    size_t idx = static_cast<size_t>(hash) % count;

    if (!buckets[idx].is_occupied()) {
        return false;
    }

    HashNode<Entry> *curr = &buckets[idx];
    while (curr) {
        if (curr->data.key == key) {
            value = curr->data.value;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

template <typename K, typename V, typename Hasher>
inline bool HashMap<K, V, Hasher>::remove(const K &key) {
    int hash = Hasher::hash(key);
    size_t idx = static_cast<size_t>(hash) % count;

    // If the bucket is unused key doesn't exist
    if (!buckets[idx].is_occupied())
        return false;

    // If the key belongs to bucket's head
    if (buckets[idx].data.key == key) {
        buckets[idx].unset();
        return true;
    }

    // If the key belongs in the bucket
    HashNode<Entry> *prev = &buckets[idx];
    HashNode<Entry> *curr = buckets[idx].next;
    while (curr) {
        if (curr->data.key == key) {
            prev->next = curr->next;
            delete curr;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}
