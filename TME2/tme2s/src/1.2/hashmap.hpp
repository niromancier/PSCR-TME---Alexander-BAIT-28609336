#include <cstddef>
#include <forward_list>
#include <iostream>
#include <vector>

namespace pr {
template <typename K, typename V>
class HashTable {
  class Entry {
   public:
    const K key;
    V value;

    Entry(K key, V value) : key(key), value(value) {}
  };
  typedef std::vector<std::forward_list<Entry>> bucket_t;
  bucket_t buckets;
  size_t sz;

 public:
  HashTable(size_t init = 100) {
    sz = 0;
    buckets.reserve(init);
    for (size_t i = 0; i < init; ++i)
      buckets.push_back(std::forward_list<Entry>());
  }

  V* get(const K& key) {
    size_t h = std::hash<K>()(key);
    std::forward_list<Entry>& l = buckets[h % buckets.size()];

    for (Entry& x : l)
      if (x.key == key) return &x.value;
    return nullptr;
  }

  bool put(const K& key, const V& value) {
    size_t size = sz;
    if ((((size == 0) ? 1 : size) * 100 / buckets.size()) >= 80) this->grow();

    size_t h = std::hash<K>()(key);
    std::forward_list<Entry>& l = buckets[h % buckets.size()];

    sz++;
    for (Entry& x : l) {
      if (x.key == key) {
        x.value = value;
        return true;
      }
    }
    l.push_front(Entry(key, value));
    return false;
  }

  size_t size() { return sz; }

  void grow() {
    size_t len = buckets.size();
    std::vector<Entry> vec;
    vec.reserve(len);
    sz = 0;

    for (std::forward_list<Entry>& l : buckets) {
      for (Entry& e : l) vec.push_back(e);
    }

    len *= 2;
    buckets.clear();
    buckets.reserve(len);
    for (size_t i = 0; i < len; ++i)
      buckets.push_back(std::forward_list<Entry>());

    for (Entry& e : vec) this->put(e.key, e.value);
  }

  std::vector<std::pair<std::string, int>> export_data() const {
    std::vector<std::pair<std::string, int>> vec;

    for (auto& l : buckets) {
      for (auto& e : l)
        vec.push_back(std::pair<std::string, int>(e.key, e.value));
    }
    return vec;
  }
};
}  // namespace pr
