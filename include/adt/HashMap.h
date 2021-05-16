#pragma once

#include <iostream>
#include <ostream>
#include <stddef.h>

#include "adt/HashTable.h"

template <typename K, typename V, typename Traits>
class HashMap {
  private:
    struct Entry {
        K key;
        V value;
        Entry() = default;
        Entry(const K &key, const V &value) : key(key), value(value) {}
        Entry(const K &key) : key(key) {}
    };
    class EntryTraits {
      public:
        static int hash(const Entry &entry) { return Traits::hash(entry.key); };
        static bool equals(const Entry &e1, const Entry &e2) {
            return e1.key == e2.key;
        }
    };
    HashTable<Entry, EntryTraits> table;

  public:
    HashMap();
    HashMap(const size_t bucket_count);
    bool is_empty() const { return table.is_empty(); };
    int size() const { return table.size(); };
    bool insert(const K &key, const V &value);
    bool contains(const K &key) const;
    bool get(const K &key, V &value) const;
    bool remove(const K &key);
};

template <typename K, typename V, typename Traits>
inline HashMap<K, V, Traits>::HashMap() {}

template <typename K, typename V, typename Traits>
inline HashMap<K, V, Traits>::HashMap(const size_t bucket_count) {
    table = HashTable<Entry, Traits>(bucket_count);
}

template <typename K, typename V, typename Traits>
inline bool HashMap<K, V, Traits>::insert(const K &key, const V &value) {
    return table.insert(Entry(key, value));
}

template <typename K, typename V, typename Traits>
inline bool HashMap<K, V, Traits>::get(const K &key, V &value) const {
    Entry *entry = table.find(Entry(key));
    if (!entry)
        return false;
    value = entry->value;
    return true;
}

template <typename K, typename V, typename Traits>
inline bool HashMap<K, V, Traits>::remove(const K &key) {
    return table.remove(Entry(key));
}

template <typename K, typename V, typename Traits>
inline bool HashMap<K, V, Traits>::contains(const K &key) const {
    return table.contains(Entry(key));
}
